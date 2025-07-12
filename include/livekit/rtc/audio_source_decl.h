// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_SOURCE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_SOURCE_DECL

#pragma once

#include "audio_source_fwd_decl.h"

#include "audio_frame_decl.h"

#include "livekit/ffi/ffi_handle_decl.h"
#include "livekit/ffi/proto/ffi.pb.h"

#include <exec/task.hpp>

#include <cstdint>
#include <chrono>

namespace livekit::rtc
{

class AudioSource
{
private:
    int sample_rate_{};
    int num_of_channels_{};
    std::chrono::milliseconds queue_size_{};
    proto::OwnedAudioSource owned_audio_source_{};
    ffi::FfiHandle handle_{};

public:
    AudioSource(int sample_rate, int num_of_channels, std::chrono::milliseconds queue_size);

    auto capture_frame(AudioFrame const & frame) -> exec::task<void>;

    auto sample_rate() const noexcept -> int;
    auto num_channels() const -> int;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_SOURCE_DECL
