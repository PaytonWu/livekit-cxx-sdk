// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/participant.h>

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

auto Participant::sid() const -> Sid
{
    return Sid{ info_.sid() };
}

auto Participant::name() const noexcept -> std::string const &
{
    return info_.name();
}

auto Participant::identity() const noexcept -> std::string const &
{
    return info_.identity();
}

auto Participant::metadata() const noexcept -> std::string const &
{
    return info_.metadata();
}

auto Participant::attributes() const -> std::unordered_map<std::string, std::string>
{
    return { info_.attributes().begin(), info_.attributes().end() };
}

auto Participant::kind() const noexcept -> proto::ParticipantKind
{
    return info_.kind();
}

auto Participant::disconnected_reason() const -> std::optional<proto::DisconnectReason>
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

    proto::FfiEvent event = co_await ffi::FfiClient::instance().async_request(req);

    if (event.publish_data().has_error())
    {
        abc::throw_error(make_error_code(ErrorCode::PublishDataFailed), event.publish_data().error());
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

    proto::FfiEvent event = co_await ffi::FfiClient::instance().async_request(req);

    if (event.publish_sip_dtmf().has_error())
    {
        abc::throw_error(make_error_code(ErrorCode::PublishDtmfFailed), event.publish_sip_dtmf().error());
    }

    co_return;
}

auto LocalParticipant::track_publications() const -> std::unordered_map<Sid, LocalTrackPublication> const &
{
    return track_publications_;
}

auto RemoteParticipant::add_track_publication(RemoteTrackPublication track_publication) -> void
{
    track_publications_.emplace(track_publication.sid(), std::move(track_publication));
}

auto RemoteParticipant::remove_track_publication(Sid const & sid) -> void
{
    track_publications_.erase(sid);
}

auto RemoteParticipant::track_publication(Sid const & sid) const -> std::expected<std::reference_wrapper<RemoteTrackPublication const>, std::error_code>
{
    if (auto it = track_publications_.find(sid); it != track_publications_.end())
    {
        return std::cref(it->second);
    }

    return std::unexpected{ make_error_code(rtc::ErrorCode::TrackPublicationNotFound) };
}

auto RemoteParticipant::track_publication(Sid const & sid) -> std::expected<std::reference_wrapper<RemoteTrackPublication>, std::error_code>
{
    if (auto it = track_publications_.find(sid); it != track_publications_.end())
    {
        return std::ref(it->second);
    }

    return std::unexpected{ make_error_code(rtc::ErrorCode::TrackPublicationNotFound) };
}

auto RemoteParticipant::track_publications() const -> std::unordered_map<Sid, RemoteTrackPublication> const &
{
    return track_publications_;
}
} // namespace livekit::rtc
