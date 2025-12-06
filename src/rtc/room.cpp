// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/room.h>

#include <livekit/ffi/ffi_client.h>
#include <livekit/rtc/error.h>

#include <fmt/format.h>

namespace livekit::rtc
{

Room::Room(exec::static_thread_pool::scheduler scheduler) : scheduler_{ scheduler }
{
}

Room::~Room() noexcept
{
    // Destructor implementation
    async_scope_.request_stop();
}

auto Room::sid() const -> exec::task<std::string>
{
    // Implementation of sid() method
    co_return std::string{};
}

auto Room::connect(std::string_view const url, std::string_view const token, RoomOptions const & room_options) -> exec::task<void>
{
    proto::FfiRequest req;

    auto * connection = req.mutable_connect();
    connection->set_url(url.data());
    connection->set_token(token.data());

    auto * options = connection->mutable_options();
    options->set_auto_subscribe(room_options.auto_subscribe);
    options->set_dynacast(room_options.dynacast);

    // TODO: e2ee options
    if (room_options.e2ee_options)
    {
        auto * e2ee = options->mutable_e2ee();
        e2ee->set_encryption_type(static_cast<proto::EncryptionType>(static_cast<int>(room_options.e2ee_options->encryption_type)));
        auto * key_provider = e2ee->mutable_key_provider_options();
        if (room_options.e2ee_options->key_provider_options.shared_key)
        {
            key_provider->set_shared_key(room_options.e2ee_options->key_provider_options.shared_key.value().data(),
                                         room_options.e2ee_options->key_provider_options.shared_key.value().size());
        }
        key_provider->set_ratchet_window_size(room_options.e2ee_options->key_provider_options.ratchet_window_size);
        key_provider->set_ratchet_salt(room_options.e2ee_options->key_provider_options.ratchet_salt.data(), room_options.e2ee_options->key_provider_options.ratchet_salt.size());
        key_provider->set_failure_tolerance(room_options.e2ee_options->key_provider_options.failure_tolerance);
    }

    // TODO: RTC config
    auto * rtc_config = options->mutable_rtc_config();
    for (auto const & server : room_options.rtc_config.ice_servers)
    {
        auto * ice_server = rtc_config->add_ice_servers();
        for (auto const & server_url : server.urls)
        {
            ice_server->add_urls(server_url);
        }
        if (server.username)
        {
            ice_server->set_username(*server.username);
        }
        if (server.password)
        {
            ice_server->set_password(*server.password);
        }
    }
    rtc_config->set_ice_transport_type(static_cast<proto::IceTransportType>(static_cast<int>(room_options.rtc_config.ice_transport_type)));
    rtc_config->set_continual_gathering_policy(static_cast<proto::ContinualGatheringPolicy>(static_cast<int>(room_options.rtc_config.continual_gathering_policy)));
    options->set_join_retries(room_options.join_retries);

    event_queue_ = livekit::ffi::FfiClient::instance().subscribe(scheduler_);

    auto queue = ffi::FfiClient::instance().subscribe(scheduler_);
    auto response = livekit::ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&response](auto const & ev) { return ev.has_connect() && ev.connect().async_id() == response.connect().async_id(); });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.connect().has_error())
    {
        ffi::FfiClient::instance().unsubscribe(event_queue_);
        throw std::runtime_error{ event.connect().error() };
    }

    ffi_handle_ = std::make_optional<ffi::FfiHandle>(event.connect().result().room().handle().id());
    room_info_ = event.connect().result().room().info();
    connection_state_ = proto::ConnectionState::CONN_CONNECTED;

    local_participant_ = std::make_unique<LocalParticipant>(event.connect().result().local_participant(), std::addressof(room_event_queue_));

    for (auto const & pt : event.connect().result().participants())
    {
        if (remote_participants_.contains(pt.participant().info().identity()))
        {
            abc::throw_error(make_error_code(ErrorCode::ParticipantAlreadyExist), fmt::format("Remote particiaptn {} already exist", pt.participant().info().identity()));
        }

        remote_participants_.emplace(pt.participant().info().identity(), RemoteParticipant{ pt.participant() });

        for (auto const & tp : pt.publications())
        {
            auto track_publication = std::make_shared<RemoteTrackPublication>(tp);
            remote_participants_.at(pt.participant().info().identity()).add_track_publication(std::move(track_publication));
        }
    }

    async_scope_.spawn(stdexec::starts_on(scheduler_, listen_room_events()));

    co_return;
}

auto Room::disconnect() -> exec::task<void>
{
    if (!connected())
    {
        co_return;
    }

    // TODO: drain rpc invocation tasks and data stream tasks
    // await self._drain_rpc_invocation_tasks()
    // await self._drain_data_stream_tasks()

    proto::FfiRequest req;
    auto * disconnect = req.mutable_disconnect();
    disconnect->set_room_handle(ffi_handle_->id());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);
    co_await queue->wait_for([&resp](auto const & ev) { return ev.has_disconnect() && ev.disconnect().async_id() == resp.disconnect().async_id(); });
    ffi::FfiClient::instance().unsubscribe(queue);

    co_await async_scope_.on_empty();

    connection_state_ = proto::ConnectionState::CONN_DISCONNECTED;
    ffi_handle_.reset();
    ffi::FfiClient::instance().unsubscribe(event_queue_);

    co_return;
}

auto Room::local_participant() const noexcept -> std::expected<std::reference_wrapper<LocalParticipant const>, std::error_code>
{
    if (!local_participant_)
    {
        return std::unexpected{ make_error_code(ErrorCode::RtcNotConnected) };
    }
    return std::ref(*local_participant_);
}

auto Room::remote_participants() const noexcept -> std::unordered_map<std::string, RemoteParticipant> const &
{
    return remote_participants_;
}

auto Room::remote_participant(std::string const & participant_identity) const noexcept -> std::expected<std::reference_wrapper<RemoteParticipant const>, std::error_code>
{
    if (auto it = remote_participants_.find(participant_identity); it != remote_participants_.end())
    {
        return std::ref(it->second);
    }
    return std::unexpected{ make_error_code(ErrorCode::ParticipantNotFound) };
}

auto Room::remote_participant(std::string const & participant_identity) noexcept -> std::expected<std::reference_wrapper<RemoteParticipant>, std::error_code>
{
    if (auto it = remote_participants_.find(participant_identity); it != remote_participants_.end())
    {
        return std::ref(it->second);
    }
    return std::unexpected{ make_error_code(ErrorCode::ParticipantNotFound) };
}

auto Room::connection_state() const noexcept -> proto::ConnectionState
{
    return connection_state_;
}

auto Room::name() const noexcept -> std::string const &
{
    return room_info_.name();
}

auto Room::metadata() const noexcept -> std::string const &
{
    return room_info_.metadata();
}

auto Room::num_participants() const noexcept -> std::size_t
{
    return room_info_.num_participants();
}

auto Room::num_publishers() const noexcept -> std::size_t
{
    return room_info_.num_publishers();
}

auto Room::creation_time() const noexcept -> std::time_t
{
    return static_cast<std::time_t>(room_info_.creation_time());
}

auto Room::is_recording() const noexcept -> bool
{
    return room_info_.active_recording();
}

auto Room::departure_timeout() const noexcept -> std::chrono::seconds
{
    return std::chrono::seconds{ room_info_.departure_timeout() };
}

auto Room::empty_timeout() const noexcept -> std::chrono::seconds
{
    return std::chrono::seconds{ room_info_.empty_timeout() };
}

auto Room::connected() const noexcept -> bool
{
    return ffi_handle_.has_value() && connection_state_ != proto::ConnectionState::CONN_DISCONNECTED;
}

auto Room::rtc_stats() const noexcept -> exec::task<std::expected<RtcStats, std::error_code>>
{
    if (!connected())
    {
        co_return std::unexpected{ make_error_code(ErrorCode::RtcNotConnected) };
    }

    proto::FfiRequest req;
    auto * get_session_stats = req.mutable_get_session_stats();
    get_session_stats->set_room_handle(ffi_handle_->id());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);

    auto event = co_await queue->wait_for([&resp](auto const & ev) {
        return ev.has_get_session_stats() && ev.get_session_stats().has_async_id() && resp.has_get_session_stats() && resp.get_session_stats().has_async_id() &&
               ev.get_session_stats().async_id() == resp.get_session_stats().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    auto const & publisher_stats = event.get_session_stats().result().publisher_stats();
    auto const & subscriber_stats = event.get_session_stats().result().subscriber_stats();
    co_return RtcStats{ std::vector<proto::RtcStats>{ publisher_stats.begin(), publisher_stats.end() },
                        std::vector<proto::RtcStats>{ subscriber_stats.begin(), subscriber_stats.end() } };
}

auto Room::create_remote_participant(proto::OwnedParticipant const & owned_participant) -> RemoteParticipant
{
    auto remote_participant = RemoteParticipant{ owned_participant };
    remote_participants_.emplace(owned_participant.info().identity(), remote_participant);
    return remote_participant;
}

auto Room::listen_room_events() -> exec::task<void>
{
    while (true)
    {
        auto event = co_await event_queue_->async_dequeue();
        if (event.has_rpc_method_invocation())
        {
            // auto const & rpc = event.rpc_method_invocation();
        }
        else if (event.has_room_event() && ffi_handle_.has_value() && event.room_event().room_handle() == ffi_handle_->id())
        {
            if (event.room_event().has_eos())
            {
                break;
            }
            try
            {
                on_room_event(event.room_event());
            }
            catch (std::exception const &)
            {
            }
        }

        // wait for the subscribers to process the event
        // before processing the next one
        room_event_queue_.enqueue(event);
        co_await room_event_queue_.join();
    }

    // TODO: drain rpc invocation tasks and data stream tasks
    // Clean up any pending RPC invocation tasks
    // await self._drain_rpc_invocation_tasks()
    // await self._drain_data_stream_tasks()

    co_return;
}

auto Room::on_room_event(proto::RoomEvent const & room_event) -> void
{
    bool emit_event = true;
    switch (room_event.message_case())
    {
        case proto::RoomEvent::MessageCase::kParticipantConnected:
        {
            create_remote_participant(room_event.participant_connected().info());

            emit(room_event.message_case(), room_event);
            emit_event = false;

            break;
        }

        case proto::RoomEvent::MessageCase::kParticipantDisconnected:
        {
            emit(room_event.message_case(), room_event);
            emit_event = false;

            auto remote_participant_id = room_event.participant_disconnected().participant_identity();
            RemoteParticipant remote_participant;
            if (auto it = remote_participants_.find(remote_participant_id); it != remote_participants_.end())
            {
                remote_participant = std::move(it->second);
                remote_participants_.erase(it);
            }

            break;
        }

        case proto::RoomEvent::MessageCase::kTrackPublished:
        {
            remote_participant(room_event.track_published().participant_identity())
                .transform([](auto && rparticipant) { return rparticipant.get(); })
                .transform([&room_event](auto && rparticipant) {
                    rparticipant.add_track_publication(std::make_shared<RemoteTrackPublication>(room_event.track_published().publication()));
                    return rparticipant;
                });
        }

        case proto::RoomEvent::MessageCase::kTrackUnpublished:
        {
            emit(room_event.message_case(), room_event);
            emit_event = false;

            remote_participant(room_event.track_unpublished().participant_identity())
                .transform([](auto && rparticipant) { return rparticipant.get(); })
                .transform([&room_event](auto && rparticipant) {
                    rparticipant.remove_track_publication(Sid{ room_event.track_unpublished().publication_sid() });
                    return rparticipant;
                });

            break;
        }

        case proto::RoomEvent::MessageCase::kTrackSubscribed:
        {
            // remote_participant(room_event.track_subscribed().participant_identity()).transform([&room_event](auto && rparticipant) {
            //     rparticipant.get().add_track_subscription(std::make_shared<RemoteTrackSubscription>(room_event.track_subscribed().track().info().sid()));
            //     return rparticipant;
            // });
            break;
        }

        case proto::RoomEvent::MessageCase::kTrackUnsubscribed:
        {
            // remove_remote_track_subscription(room_event.track_unsubscribed().track_sid());
            break;
        }

        case proto::RoomEvent::MessageCase::kTrackSubscriptionFailed:
        {
            // create_remote_track_subscription_failed(room_event.track_subscription_failed().error(), room_event.track_subscription_failed().track_sid());
            break;
        }

        case proto::RoomEvent::MessageCase::kTrackMuted:
        {
            // create_remote_track_muted(room_event.track_muted().publication_sid());
            break;
        }

        case proto::RoomEvent::MessageCase::kTrackUnmuted:
        {
            // create_remote_track_unmuted(room_event.track_unmuted().publication_sid());
            break;
        }
    }

    if (emit_event)
    {
        emit(room_event.message_case(), room_event);
    }
}

} // namespace livekit::rtc
