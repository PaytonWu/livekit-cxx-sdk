// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ERROR_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ERROR_FWD_DECL

#pragma once

#include <system_error>

namespace livekit::rtc
{

enum class [[nodiscard]] ErrorCode;

} // namespace livekit::rtc

namespace std
{

template <>
struct is_error_code_enum<livekit::rtc::ErrorCode>;

} // namespace std

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ERROR_FWD_DECL
