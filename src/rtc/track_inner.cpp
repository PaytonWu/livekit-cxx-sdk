#include <livekit/rtc/track_inner.h>

#include <livekit/ffi/proto/track.pb.h>

namespace livekit::rtc
{

TrackInner::TrackInner(proto::OwnedTrack const & owned_track) : track_info_{ owned_track.info() }
{
}

auto TrackInner::sid() const -> Sid
{
    return Sid{ track_info_.sid() };
}

auto TrackInner::sid(Sid const & value) -> void
{
    track_info_.set_sid(value.value());
}

auto TrackInner::name() const -> std::string const &
{
    return track_info_.name();
}

auto TrackInner::kind() const -> proto::TrackKind
{
    return track_info_.kind();
}

auto TrackInner::remote() const -> bool
{
    return track_info_.remote();
}

auto TrackInner::stream_state() const -> proto::StreamState
{
    return track_info_.stream_state();
}

auto TrackInner::muted() const -> bool
{
    return track_info_.muted();
}

auto TrackInner::mute() -> void
{
    track_info_.set_muted(true);
}

auto TrackInner::unmute() -> void
{
    track_info_.set_muted(false);
}

} // namespace livekit::rtc
