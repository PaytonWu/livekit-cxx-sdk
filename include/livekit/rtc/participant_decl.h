// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT_DECL

#pragma once

#include "participant_fwd_decl.h"

#include "livekit/ffi/proto/participant.pb.h"
#include "livekit/ffi/ffi_handle_decl.h"
#include "sid_decl.h"

#include <string>
#include <unordered_map>
#include <optional>

namespace livekit::rtc
{

class Participant
{
protected:
    proto::ParticipantInfo info_{};
    ffi::FfiHandle ffi_handle_{};

public:
    explicit Participant(proto::OwnedParticipant const & owned_participant);

    Participant(Participant const &) = delete;
    auto operator=(Participant const &) -> Participant & = delete;
    Participant(Participant &&) = default;
    auto operator=(Participant &&) -> Participant & = default;

    auto sid() -> Sid;
    auto name() -> std::string const &;
    auto identity() -> std::string const &;
    auto metadata() -> std::string const &;
    auto attributes() -> std::unordered_map<std::string, std::string>;
    auto kind() -> proto::ParticipantKind;
    auto disconnected_reason() -> std::optional<proto::DisconnectReason>;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT_DECL
