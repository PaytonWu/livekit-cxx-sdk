// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_FWD_DECL

#pragma once

#include <variant>

namespace livekit::rtc
{

class Track;
class LocalAudioTrack;
class LocalVideoTrack;
class RemoteAudioTrack;
class RemoteVideoTrack;

// using LocalTrack = std::variant<LocalAudioTrack, LocalVideoTrack>;
// using RemoteTrack = std::variant<RemoteAudioTrack, RemoteVideoTrack>;
// using AudioTrack = std::variant<LocalAudioTrack, RemoteAudioTrack>;
// using VideoTrack = std::variant<LocalVideoTrack, RemoteVideoTrack>;

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_FWD_DECL
