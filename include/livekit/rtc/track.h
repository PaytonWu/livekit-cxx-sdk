// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK

#pragma once

#include "track_decl.h" // IWYU pragma: export

#include "sid.h"         // IWYU pragma: export
#include "track_inner.h" // IWYU pragma: export

#include "livekit/ffi/ffi_handle.h" // IWYU pragma: export

namespace std
{

template <typename FormatContext>
auto formatter<livekit::rtc::LocalAudioTrack>::format(livekit::rtc::LocalAudioTrack const & track, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return std::format_to(ctx.out(), "rtc::LocalAudioTrack(sid={}, name={})", track.sid(), track.name());
}

template <typename FormatContext>
auto formatter<livekit::rtc::RemoteAudioTrack>::format(livekit::rtc::RemoteAudioTrack const & track, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return std::format_to(ctx.out(), "rtc::RemoteAudioTrack(sid={}, name={})", track.sid(), track.name());
}

template <typename FormatContext>
auto formatter<livekit::rtc::LocalVideoTrack>::format(livekit::rtc::LocalVideoTrack const & track, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return std::format_to(ctx.out(), "rtc::LocalVideoTrack(sid={}, name={})", track.sid(), track.name());
}

template <typename FormatContext>
auto formatter<livekit::rtc::RemoteVideoTrack>::format(livekit::rtc::RemoteVideoTrack const & track, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return std::format_to(ctx.out(), "rtc::RemoteVideoTrack(sid={}, name={})", track.sid(), track.name());
}

template <typename FormatContext>
auto formatter<livekit::rtc::AudioTrack>::format(livekit::rtc::AudioTrack const & track, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return std::format_to(ctx.out(), "rtc::AudioTrack(sid={}, name={})", track.sid(), track.name());
}

template <typename FormatContext>
auto formatter<livekit::rtc::VideoTrack>::format(livekit::rtc::VideoTrack const & track, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return std::format_to(ctx.out(), "rtc::VideoTrack(sid={}, name={})", track.sid(), track.name());
}

template <typename FormatContext>
auto formatter<livekit::rtc::Track>::format(livekit::rtc::Track const & track, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return std::format_to(ctx.out(), "rtc::Track(sid={}, name={})", track.sid(), track.name());
}
} // namespace std

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK
