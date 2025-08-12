// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/track.h>

#include <livekit/error.h>
#include <livekit/ffi/ffi_client.h>
#include <livekit/ffi/proto/ffi.pb.h>
#include <livekit/rtc/audio_source.h>
#include <livekit/rtc/sid.h>

namespace livekit::rtc
{

Track::Track(proto::OwnedTrack const & owned_track) : track_info_{ owned_track.info() }, ffi_handle_{ owned_track.handle().id() }
{
}

auto Track::sid() const -> Sid
{
    return Sid{ track_info_.sid() };
}

auto Track::name() const -> std::string const &
{
    return track_info_.name();
}

auto Track::kind() const -> proto::TrackKind
{
    return track_info_.kind();
}

auto Track::stream_state() const -> proto::StreamState
{
    return track_info_.stream_state();
}

auto Track::muted() const -> bool
{
    return track_info_.muted();
}

auto Track::get_stats() const -> exec::task<std::vector<proto::RtcStats>>
{
    proto::FfiRequest req;
    auto * get_stats = req.mutable_get_stats();
    get_stats->set_track_handle(ffi_handle_.id());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_get_stats() && event.get_stats().has_async_id() && resp.has_get_stats() && resp.get_stats().has_async_id() &&
               event.get_stats().async_id() == resp.get_stats().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.get_stats().has_error())
    {
        throw_error(LivekitErrorCode::GetStatsFailed, event.get_stats().error());
    }

    auto const & stats = event.get_stats().stats();
    co_return std::vector<proto::RtcStats>{ stats.begin(), stats.end() };
}

LocalAudioTrack::LocalAudioTrack(proto::OwnedTrack const & owned_track) : Track{ owned_track }
{
}

auto LocalAudioTrack::create(std::string_view name, AudioSource const & source) -> LocalAudioTrack
{
    proto::FfiRequest req;
    auto * create_audio_track = req.mutable_create_audio_track();
    create_audio_track->set_name(name.data(), name.size());
    create_audio_track->set_source_handle(source.ffi_handle().id());

    auto resp = ffi::FfiClient::request(req);
    return LocalAudioTrack{ resp.create_audio_track().track() };
}

auto LocalAudioTrack::mute() -> void
{
    proto::FfiRequest req;
    auto * local_track_mute = req.mutable_local_track_mute();
    local_track_mute->set_track_handle(this->ffi_handle_.id());
    local_track_mute->set_mute(true);

    ffi::FfiClient::request(req);
    this->track_info_.set_muted(true);
}

auto LocalAudioTrack::unmute() -> void
{
    proto::FfiRequest req;
    auto * local_track_mute = req.mutable_local_track_mute();
    local_track_mute->set_track_handle(this->ffi_handle_.id());
    local_track_mute->set_mute(false);

    ffi::FfiClient::request(req);
    this->track_info_.set_muted(false);
}

auto LocalAudioTrack::sid(Sid sid) -> void
{
    this->track_info_.set_sid(sid.value());
}

LocalVideoTrack::LocalVideoTrack(proto::OwnedTrack const & owned_track) : Track{ owned_track }
{
}

RemoteAudioTrack::RemoteAudioTrack(proto::OwnedTrack const & owned_track) : Track{ owned_track }
{
}

RemoteVideoTrack::RemoteVideoTrack(proto::OwnedTrack const & owned_track) : Track{ owned_track }
{
}

} // namespace livekit::rtc
