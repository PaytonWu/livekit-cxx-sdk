// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_INNER_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_INNER_DECL

#pragma once

#include "track_inner_fwd_decl.h"

#include "sid_decl.h"

#include "livekit/ffi/proto/track.pb.h"

namespace livekit::rtc
{

class TrackInner
{
private:
    proto::TrackInfo track_info_;

public:
    explicit TrackInner(proto::OwnedTrack const & owned_track);

    auto sid() const -> Sid;
    auto sid(Sid const & value) -> void;

    auto name() const -> std::string const &;
    auto kind() const -> proto::TrackKind;
    auto remote() const -> bool;
    auto stream_state() const -> proto::StreamState;
    auto muted() const -> bool;
    auto muted(bool value) -> void;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRACK_INNER_DECL
