// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_SOURCE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_SOURCE_DECL

#pragma once

#include "audio_source_fwd_decl.h"

#include <cstdint>
#include <chrono>

namespace livekit::rtc
{

class AudioSource
{
public:
    AudioSource(int sample_rate, int channels, std::chrono::milliseconds queue_size);
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_SOURCE_DECL
