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

LocalTrackPublication::LocalTrackPublication(proto::OwnedTrackPublication const & owned_track_publication) : TrackPublication{ owned_track_publication }
{
}

RemoteTrackPublication::RemoteTrackPublication(proto::OwnedTrackPublication const & owned_track_publication) : TrackPublication{ owned_track_publication }
{
}

} // namespace livekit::rtc
