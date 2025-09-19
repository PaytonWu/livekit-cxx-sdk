// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/participant.h>

#include <livekit/error.h>
#include <livekit/ffi/ffi_client.h>
#include <livekit/ffi/proto/ffi.pb.h>

namespace livekit::rtc
{

Participant::Participant(proto::OwnedParticipant const & owned_participant) : info_{ owned_participant.info() }, ffi_handle_{ owned_participant.handle().id() }
{
}

RemoteParticipant::RemoteParticipant(proto::OwnedParticipant const & owned_participant) : Participant{ owned_participant }
{
}

auto Participant::sid() -> Sid
{
    return Sid{ info_.sid() };
}

auto Participant::name() -> std::string const &
{
    return info_.name();
}

auto Participant::identity() -> std::string const &
{
    return info_.identity();
}

auto Participant::metadata() -> std::string const &
{
    return info_.metadata();
}

auto Participant::attributes() -> std::unordered_map<std::string, std::string>
{
    return { info_.attributes().begin(), info_.attributes().end() };
}

auto Participant::kind() -> proto::ParticipantKind
{
    return info_.kind();
}

auto Participant::disconnected_reason() -> std::optional<proto::DisconnectReason>
{
    return info_.has_disconnect_reason() ? std::make_optional(info_.disconnect_reason()) : std::nullopt;
}

LocalParticipant::LocalParticipant(proto::OwnedParticipant const & owned_participant, utils::BroadcastQueue<proto::FfiEvent> * room_event_queue)
    : Participant{ owned_participant }
    , room_event_queue_{ room_event_queue }
{
}

auto LocalParticipant::publish_data(std::vector<abc::byte_t> const & data, bool reliable, std::vector<std::string> const & destinations, std::optional<std::string> const & topic)
    -> exec::task<void>
{
    proto::FfiRequest req;

    auto * publish_data = req.mutable_publish_data();
    publish_data->set_local_participant_handle(ffi_handle_.id());
    publish_data->set_data_ptr(reinterpret_cast<std::uint64_t>(data.data()));
    publish_data->set_data_len(static_cast<std::uint64_t>(data.size()));
    publish_data->set_reliable(reliable);
    auto * destination_ids = publish_data->mutable_destination_identities();
    destination_ids->Add(destinations.begin(), destinations.end());
    if (topic)
    {
        publish_data->set_topic(topic.value());
    }

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_publish_data() && event.publish_data().has_async_id() && resp.has_publish_data() && resp.publish_data().has_async_id() &&
               event.publish_data().async_id() == resp.publish_data().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.publish_data().has_error())
    {
        throw_error(LivekitErrorCode::PublishDataFailed, event.publish_data().error());
    }

    co_return;
}

auto LocalParticipant::publish_dtmf(std::uint32_t const code, std::string const & digit) -> exec::task<void>
{
    proto::FfiRequest req;

    auto * publish_dtmf = req.mutable_publish_sip_dtmf();
    publish_dtmf->set_local_participant_handle(ffi_handle_.id());
    publish_dtmf->set_code(code);
    publish_dtmf->set_digit(digit);

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_publish_sip_dtmf() && event.publish_sip_dtmf().has_async_id() && resp.has_publish_sip_dtmf() && resp.publish_sip_dtmf().has_async_id() &&
               event.publish_sip_dtmf().async_id() == resp.publish_sip_dtmf().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.publish_sip_dtmf().has_error())
    {
        throw_error(LivekitErrorCode::PublishDtmfFailed, event.publish_sip_dtmf().error());
    }

    co_return;
}

auto LocalParticipant::track_publications() const -> std::unordered_map<Sid, std::shared_ptr<TrackPublication>>
{
    return track_publications_;
}

auto RemoteParticipant::add_track_publication(std::shared_ptr<RemoteTrackPublication> track_publication) -> void
{
    track_publications_.emplace(track_publication->sid(), std::static_pointer_cast<TrackPublication>(track_publication));
}

auto RemoteParticipant::track_publications() const -> std::unordered_map<Sid, std::shared_ptr<TrackPublication>>
{
    return track_publications_;
}

} // namespace livekit::rtc
