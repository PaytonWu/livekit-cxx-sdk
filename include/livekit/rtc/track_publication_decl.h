// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_PUBLICATION_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_PUBLICATION_DECL

#pragma once

#include "track_publication_fwd_decl.h"

#include "sid_decl.h"
#include "track_decl.h"

#include "livekit/e2ee_decl.h"
#include "livekit/ffi/ffi_handle_decl.h"
#include "livekit/ffi/proto/livekit_models.pb.h"
#include "livekit/ffi/proto/track.pb.h"

#include <cstdint>
#include <variant>

namespace livekit::rtc
{

class TrackPublication
{
protected:
    proto::TrackPublicationInfo info_{};
    ffi::FfiHandle ffi_handle_{};
    std::variant<std::monostate, LocalAudioTrack, LocalVideoTrack, RemoteAudioTrack, RemoteVideoTrack> track_{};

public:
    explicit TrackPublication(proto::OwnedTrackPublication const & owned_track_publication);
    TrackPublication(TrackPublication &&) = default;
    auto operator=(TrackPublication &&) -> TrackPublication & = default;
    virtual ~TrackPublication() = default;

    template <proto::TrackKind TrackKindValue>
    auto track() const -> std::enable_if_t<TrackKindValue == proto::TrackKind::KIND_AUDIO, LocalAudioTrack> const &;
    template <proto::TrackKind TrackKindValue>
    auto track() const -> std::enable_if_t<TrackKindValue == proto::TrackKind::KIND_VIDEO, LocalVideoTrack> const &;

    auto sid() const -> Sid;
    auto name() const noexcept -> std::string const &;
    auto kind() const noexcept -> proto::TrackKind;
    auto source() const noexcept -> proto::TrackSource;
    auto simulcasted() const noexcept -> bool;
    auto width() const noexcept -> int32_t;
    auto height() const noexcept -> int32_t;
    auto mime_type() const noexcept -> std::string const &;
    auto muted() const noexcept -> bool;
    auto encryption_type() const noexcept -> livekit::EncryptionType;
    auto audio_features() const noexcept -> std::vector<livekit::AudioTrackFeature>;
};

class LocalTrackPublication : public TrackPublication
{
public:
    explicit LocalTrackPublication(proto::OwnedTrackPublication const & owned_track_publication);
    auto set_track(LocalTrack auto const & track) -> void;
};

class RemoteTrackPublication : public TrackPublication
{
public:
    explicit RemoteTrackPublication(proto::OwnedTrackPublication const & owned_track_publication);
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_PUBLICATION_DECL
