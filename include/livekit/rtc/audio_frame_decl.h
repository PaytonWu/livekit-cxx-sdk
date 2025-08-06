// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_FRAME_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_FRAME_DECL

#pragma once

#include "audio_frame_fwd_decl.h"

#include "livekit/ffi/proto/audio_frame.pb.h"

#include <abc/bytes_view.h>

#include <chrono>
#include <cstdint>
#include <vector>

namespace livekit::rtc
{

class AudioFrame
{
public:
    static constexpr std::size_t bytes_per_sample = sizeof(std::int16_t);

private:
    std::uint32_t sample_rate_{};
    std::uint32_t num_channels_{};
    std::uint32_t samples_per_channel_{};
    std::vector<std::int16_t> data_{};

    AudioFrame(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel) noexcept;

public:
    // sample_rate (int): The sample rate of the audio in Hz.
    // num_channels (int): The number of audio channels (e.g., 1 for mono, 2 for stereo).
    // samples_per_channel (int): The number of samples per channel.
    // data (bytes_view): The raw audio data, which must be at least
    // `num_channels * samples_per_channel * sizeof(int16)` bytes long.
    AudioFrame(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel, std::vector<std::int16_t> data) noexcept;
    AudioFrame(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel, abc::bytes_view_t const & data);

    static auto create(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel) noexcept -> AudioFrame;
    static auto from_proto(proto::OwnedAudioFrameBuffer const & proto_buffer) noexcept -> AudioFrame;

    auto sample_rate() const -> std::uint32_t;
    auto num_channels() const -> std::uint32_t;
    auto samples_per_channel() const -> std::uint32_t;
    auto data() const -> std::vector<std::int16_t> const &;
    auto duration() const -> std::chrono::milliseconds;
    auto proto_info() const -> proto::AudioFrameBufferInfo;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_AUDIO_FRAME_DECL
