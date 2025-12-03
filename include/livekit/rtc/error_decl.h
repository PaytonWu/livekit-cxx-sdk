// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ERROR_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ERROR_DECL

#pragma once

#include "error_fwd_decl.h"

namespace livekit::rtc
{

enum class ErrorCode
{
    Success = 0,
    RtcNotConnected,
    RtcGetStatsFailed,
    TrackPublicationNotFound,
    ParticipantNotFound,
    PublishTrackFailed,
    PublishDataFailed,
    PublishDtmfFailed,
    ParticipantAlreadyExist,
};

auto livekit_rtc_category() noexcept -> std::error_category const &;
auto make_error_code(ErrorCode ec) noexcept -> std::error_code;

} // namespace livekit::rtc

namespace std
{

template <>
struct is_error_code_enum<livekit::rtc::ErrorCode> : true_type
{
};

} // namespace std

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ERROR_DECL
