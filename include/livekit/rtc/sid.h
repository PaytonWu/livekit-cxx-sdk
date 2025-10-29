// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID

#pragma once

#include "sid_decl.h"

#include "livekit/utils/strong_typed_value.h"

namespace fmt
{

constexpr auto formatter<livekit::rtc::Sid>::parse(format_parse_context & ctx) -> fmt::format_parse_context::iterator
{
    return ctx.begin(); // simple parser for now
}

template <typename FormatContext>
auto formatter<livekit::rtc::Sid>::format(livekit::rtc::Sid const & sid, FormatContext & ctx) const -> typename FormatContext::iterator
{
    return fmt::format_to(ctx.out(), "{}", sid.value());
}

} // namespace fmt


#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID
