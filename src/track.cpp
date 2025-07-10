// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/track.h>

#include <livekit/error.h>
#include <livekit/ffi/ffi_client.h>
#include <livekit/ffi/proto/ffi.pb.h>

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

LocalVideoTrack::LocalVideoTrack(proto::OwnedTrack const & owned_track) : Track{ owned_track }
{
}

} // namespace livekit::rtc
