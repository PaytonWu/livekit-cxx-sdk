// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID_DECL

#pragma once

#include "sid_fwd_decl.h"

#include "livekit/utils/strong_typed_value_decl.h"

#include <cstddef>
#include <string>

namespace livekit::rtc
{

class Sid : public utils::StrongTypedValue<std::string, Sid>
{
public:
    using utils::StrongTypedValue<std::string, Sid>::StrongTypedValue;
};

} // namespace livekit::rtc

namespace std
{

template <>
struct hash<livekit::rtc::Sid>
{
    auto operator()(livekit::rtc::Sid const & sid) const -> std::size_t;
};

} // namespace std

namespace fmt
{

template <>
struct formatter<livekit::rtc::Sid>
{
    constexpr auto parse(format_parse_context & ctx) -> fmt::format_parse_context::iterator;

    template <typename FormatContext>
    auto format(livekit::rtc::Sid const & sid, FormatContext & ctx) const -> typename FormatContext::iterator;
};

} // namespace fmt

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID_DECL
