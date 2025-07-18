// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/room.h>

#include <livekit/error.h>
#include <livekit/ffi/ffi_client.h>

#include <fmt/format.h>

namespace livekit::rtc
{

Room::Room(exec::static_thread_pool::scheduler scheduler) : scheduler_{ scheduler }
{
}

Room::~Room() noexcept
{
    // Destructor implementation
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
        key_provider->set_shared_key(room_options.e2ee_options->key_provider_options.shared_key);
        key_provider->set_ratchet_window_size(room_options.e2ee_options->key_provider_options.ratchet_window_size);
        key_provider->set_ratchet_salt(room_options.e2ee_options->key_provider_options.ratchet_salt);
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

    ffi_handle_ = ffi::FfiHandle{ event.connect().result().room().handle().id() };
    room_info_ = event.connect().result().room().info();
    connection_state_ = proto::ConnectionState::CONN_CONNECTED;

    local_participant_ = std::make_unique<LocalParticipant>(event.connect().result().local_participant(), std::addressof(room_event_queue_));

    for (auto const & pt : event.connect().result().participants())
    {
        if (remote_participants_.contains(pt.participant().info().identity()))
        {
            throw_error(LivekitErrorCode::RemoteParticipantAlreadyExist, fmt::format("Remote particiaptn {} already exist", pt.participant().info().identity()));
        }

        remote_participants_.emplace(pt.participant().info().identity(), RemoteParticipant{ pt.participant() });

        for (auto const & tp : pt.publications())
        {
            auto track_publication = std::make_shared<RemoteTrackPublication>(tp);
            remote_participants_.at(pt.participant().info().identity()).add_track_publication(std::move(track_publication));
        }
    }

    co_return;
}

auto Room::create_remote_participant(proto::OwnedParticipant const & owned_participant) -> std::unique_ptr<RemoteParticipant>
{
    return std::make_unique<RemoteParticipant>(owned_participant);
}

auto Room::listen_room_events_task() -> exec::task<void>
{
    bool quit = false;
    while (!quit)
    {
        auto event = co_await event_queue_->async_dequeue();
        if (event.has_room_event() && event.room_event().room_handle() == ffi_handle_.id())
        {
            if (event.room_event().has_eos())
            {
                quit = true;
            }
        }
    }
}

auto Room::on_room_event(proto::RoomEvent const & event) -> void
{
    switch (event.message_case())
    {
    }
}

} // namespace livekit::rtc
