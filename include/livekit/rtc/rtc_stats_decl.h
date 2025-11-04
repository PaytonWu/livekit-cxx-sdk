// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_RTC_STATS_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_RTC_STATS_DECL

#pragma once

#include "rtc_stats_fwd_decl.h"

#include "livekit/ffi/proto/stats.pb.h"

#include <vector>

namespace livekit::rtc
{

struct RtcStats
{
    std::vector<proto::RtcStats> publisher_stats{};
    std::vector<proto::RtcStats> subscriber_stats{};
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_RTC_STATS_DECL
