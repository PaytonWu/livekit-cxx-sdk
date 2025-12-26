// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_DECL

#pragma once

#include "track_fwd_decl.h"

#include "audio_source_decl.h"
#include "sid_decl.h"
#include "track_inner_decl.h"

#include "livekit/ffi/ffi_handle.h"
#include "livekit/ffi/proto/stats.pb.h"
#include "livekit/ffi/proto/track.pb.h"

#include <exec/task.hpp>

#include <concepts>
#include <expected>
#include <system_error>
#include <variant>
#include <vector>

namespace livekit::rtc
{

class LocalAudioTrack
{
private:
    std::shared_ptr<TrackInner> track_inner_;
    ffi::FfiHandle ffi_handle_;

public:
    explicit LocalAudioTrack(proto::OwnedTrack const & owned_track);

    static auto create(std::string_view name, AudioSource const & source) -> LocalAudioTrack;

    auto sid() const -> Sid;
    auto sid(Sid const & sid) -> void;

    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;

    auto get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>;

    auto mute() -> void;
    auto unmute() -> void;

    auto is_remote() const -> bool;
};

class RemoteAudioTrack
{
private:
    std::shared_ptr<TrackInner> track_inner_;
    ffi::FfiHandle ffi_handle_;

public:
    explicit RemoteAudioTrack(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;

    auto get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>;

    auto is_enabled() const -> bool;
    auto enable() -> void;
    auto disable() -> void;
    auto is_remote() const -> bool;
};

class AudioTrack
{
private:
    std::variant<LocalAudioTrack, RemoteAudioTrack> track_;

public:
    explicit AudioTrack(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;

    auto get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>;

    auto is_enabled() const -> bool;
    auto enable() -> void;
    auto disable() -> void;
    auto is_remote() const -> bool;
    auto mute() -> std::expected<void, std::error_code>;
    auto unmute() -> std::expected<void, std::error_code>;
};

class LocalVideoTrack
{
private:
    std::shared_ptr<TrackInner> track_inner_;
    ffi::FfiHandle ffi_handle_;

public:
    explicit LocalVideoTrack(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto sid(Sid const & sid) -> void;
    // auto source() const -> VideoSource;
    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;

    auto get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>;

    auto mute() -> void;
    auto unmute() -> void;

    auto is_remote() const -> bool;
};

class RemoteVideoTrack
{
private:
    std::shared_ptr<TrackInner> track_inner_;
    ffi::FfiHandle ffi_handle_;

public:
    explicit RemoteVideoTrack(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;

    auto get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>;

    auto is_enabled() const -> bool;
    auto enable() -> void;
    auto disable() -> void;
    auto is_remote() const -> bool;
};

class VideoTrack
{
private:
    std::variant<LocalVideoTrack, RemoteVideoTrack> track_;

public:
    explicit VideoTrack(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;

    auto get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>;

    auto is_enabled() const -> bool;
    auto enable() -> void;
    auto disable() -> void;
    auto is_remote() const -> bool;
    auto mute() -> std::expected<void, std::error_code>;
    auto unmute() -> std::expected<void, std::error_code>;
};

/*
#[derive(Clone, Debug)]
pub enum Track {
    LocalAudio(LocalAudioTrack),
    LocalVideo(LocalVideoTrack),
    RemoteAudio(RemoteAudioTrack),
    RemoteVideo(RemoteVideoTrack),
}
*/
class Track
{
private:
    std::variant<LocalAudioTrack, LocalVideoTrack, RemoteAudioTrack, RemoteVideoTrack> track_;

public:
    explicit Track(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;
    auto get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>;

    auto is_enabled() const -> bool;
    auto enable() -> std::expected<void, std::error_code>;
    auto disable() -> std::expected<void, std::error_code>;
    auto is_remote() const -> bool;
    auto mute() -> std::expected<void, std::error_code>;
    auto unmute() -> std::expected<void, std::error_code>;
};

template <typename T>
concept LocalTrack = std::derived_from<T, Track> && requires(T & t) {
    { t.mute() } -> std::same_as<void>;
    { t.unmute() } -> std::same_as<void>;
    { t.sid(std::declval<Sid>()) } -> std::same_as<void>;
};

template <typename T>
concept RemoteTrack = std::is_same_v<T, RemoteAudioTrack> || std::is_same_v<T, RemoteVideoTrack>;

} // namespace livekit::rtc
#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_DECL
