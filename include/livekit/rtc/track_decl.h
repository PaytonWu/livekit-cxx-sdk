// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_DECL

#pragma once

#include "track_fwd_decl.h"

#include "audio_source_decl.h"
#include "sid_decl.h"

#include "livekit/ffi/ffi_handle.h"
#include "livekit/ffi/proto/stats.pb.h"
#include "livekit/ffi/proto/track.pb.h"

#include <exec/task.hpp>

#include <concepts>
#include <vector>

namespace livekit::rtc
{

class Track
{
protected:
    proto::TrackInfo track_info_;
    ffi::FfiHandle ffi_handle_;

public:
    explicit Track(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;
    auto get_stats() const -> exec::task<std::vector<proto::RtcStats>>;
};

class LocalAudioTrack : public Track
{
public:
    explicit LocalAudioTrack(proto::OwnedTrack const & owned_track);

    auto create_audio_track(std::string_view name, AudioSource const & source) -> LocalAudioTrack;
    auto mute() -> void;
    auto unmute() -> void;
};

class LocalVideoTrack : public Track
{
public:
    explicit LocalVideoTrack(proto::OwnedTrack const & owned_track);
};

class RemoteAudioTrack : public Track
{
public:
    explicit RemoteAudioTrack(proto::OwnedTrack const & owned_track);
};

class RemoteVideoTrack : public Track
{
public:
    explicit RemoteVideoTrack(proto::OwnedTrack const & owned_track);
};

template <typename T>
concept LocalTrack = std::derived_from<T, Track> &&
    requires(T & t) {
        { t.mute() } -> std::same_as<void>;
        { t.unmute() } -> std::same_as<void>;
    };

}
#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_DECL
