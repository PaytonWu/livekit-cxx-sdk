// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_PUBLICATION
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_PUBLICATION

#pragma once

#include "track_publication_decl.h"

#include "sid.h"
#include "track.h"

#include "livekit/ffi/ffi_handle.h"

namespace livekit::rtc
{

template <proto::TrackKind TrackKindValue>
auto TrackPublication::track() const -> std::enable_if_t<TrackKindValue == proto::TrackKind::KIND_AUDIO, LocalAudioTrack> const &
{
    return std::get<LocalAudioTrack>(track_);
}

template <proto::TrackKind TrackKindValue>
auto TrackPublication::track() const -> std::enable_if_t<TrackKindValue == proto::TrackKind::KIND_VIDEO, LocalVideoTrack> const &
{
    return std::get<LocalVideoTrack>(track_);
}

auto LocalTrackPublication::set_track(LocalTrack auto const & track) -> void
{
    track_ = track;
}

auto RemoteTrackPublication::set_track(RemoteTrack auto const & track) -> void
{
    track_ = track;
}

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_PUBLICATION
