// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "livekit/rtc/track_publication.h"

namespace livekit::rtc
{

TrackPublication::TrackPublication(proto::OwnedTrackPublication const & owned_track_publication)
    : info_{ owned_track_publication.info() }
    , ffi_handle_{ owned_track_publication.handle().id() }
{
}

auto TrackPublication::sid() const -> Sid
{
    return Sid{ info_.sid() };
}

auto TrackPublication::reset_track() noexcept -> void
{
    track_ = std::monostate{};
}

LocalTrackPublication::LocalTrackPublication(proto::OwnedTrackPublication const & owned_track_publication) : TrackPublication{ owned_track_publication }
{
}

RemoteTrackPublication::RemoteTrackPublication(proto::OwnedTrackPublication const & owned_track_publication) : TrackPublication{ owned_track_publication }
{
}

auto RemoteTrackPublication::set_subscribed(bool subscribed) noexcept -> bool
{
    bool old_subscribed = subscribed_;
    subscribed_ = subscribed;
    return old_subscribed;
}

auto RemoteTrackPublication::subscribed() const noexcept -> bool
{
    return subscribed_;
}

} // namespace livekit::rtc
