// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_FWD_DECL

#pragma once

#include <fmt/format.h>

namespace livekit::rtc
{

class LocalAudioTrack;
class LocalVideoTrack;

class RemoteAudioTrack;
class RemoteVideoTrack;

class AudioTrack;
class VideoTrack;

class Track;

} // namespace livekit::rtc

namespace fmt
{

template <>
struct formatter<livekit::rtc::LocalAudioTrack>;

template <>
struct formatter<livekit::rtc::LocalVideoTrack>;

template <>
struct formatter<livekit::rtc::RemoteAudioTrack>;

template <>
struct formatter<livekit::rtc::RemoteVideoTrack>;

template <>
struct formatter<livekit::rtc::AudioTrack>;

template <>
struct formatter<livekit::rtc::VideoTrack>;

template <>
struct formatter<livekit::rtc::Track>;

} // namespace fmt

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_FWD_DECL
