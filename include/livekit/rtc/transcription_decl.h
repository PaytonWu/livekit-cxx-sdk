// Copyright(c) 2026 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRANSCRIPTION_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRANSCRIPTION_DECL

#pragma once

#include "transcription_fwd_decl.h"

#include "sid_decl.h"

#include <chrono>
#include <string>
#include <vector>

namespace livekit::rtc
{

struct Transcription
{
    std::string participant_identity;
    Sid track_sid;
    std::vector<TranscriptionSegment> segments;
};

struct TranscriptionSegment
{
    std::string id;
    std::string text;
    std::string language;
    std::chrono::utc_time<std::chrono::milliseconds> start_time;
    std::chrono::utc_time<std::chrono::milliseconds> end_time;
    bool final;
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_TRANSCRIPTION_DECL
