// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/audio_frame.h>

#include <livekit/error.h>

#include <cstdint>

namespace livekit::rtc
{

AudioFrame::AudioFrame(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel) noexcept
    : sample_rate_{ sample_rate }, num_channels_{ num_channels }, samples_per_channel_{ samples_per_channel }, data_(num_channels * samples_per_channel)
{
}

AudioFrame::AudioFrame(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel, std::vector<std::int16_t> data) noexcept
    : sample_rate_{ sample_rate }, num_channels_{ num_channels }, samples_per_channel_{ samples_per_channel }, data_{ std::move(data) }
{
    if (data.size() < num_channels * samples_per_channel)
    {
        throw_error(LivekitErrorCode::InvalidMediaData, "audio frame data length must be >= num_channels * samples_per_channel");
    }
}

AudioFrame::AudioFrame(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel, abc::bytes_view_t const & data)
    : sample_rate_{ sample_rate }, num_channels_{ num_channels }, samples_per_channel_{ samples_per_channel }
{
    if (data.size() < num_channels * samples_per_channel * sizeof(int16_t))
    {
        throw_error(LivekitErrorCode::InvalidMediaData, "audio frame data length must be >= num_channels * samples_per_channel * sizeof(int16)");
    }

    if (data.size() % sizeof(int16_t) != 0)
    {
        throw_error(LivekitErrorCode::InvalidMediaData, "audio frame data length must be divisible by sizeof(int16)");
    }

    data_.resize(data.size() / sizeof(int16_t));
    std::memcpy(data_.data(), data.data(), data.size());
}

auto AudioFrame::create(std::uint32_t sample_rate, std::uint32_t num_channels, std::uint32_t samples_per_channel) noexcept -> AudioFrame
{
    return AudioFrame{ sample_rate, num_channels, samples_per_channel };
}

auto AudioFrame::from_proto(proto::OwnedAudioFrameBuffer const & proto_buffer) noexcept -> AudioFrame
{
    auto const & info = proto_buffer.info();
    auto const sample_rate = info.sample_rate();
    auto const num_channels = info.num_channels();
    auto const samples_per_channel = info.samples_per_channel();

    auto const sample_count = samples_per_channel * num_channels;
    auto const frame_size_in_bytes = sample_count * AudioFrame::bytes_per_sample;
    auto const data = abc::bytes_view_t::from(reinterpret_cast<abc::byte const *>(info.data_ptr()), frame_size_in_bytes, abc::byte_numbering_none_t{});

    return AudioFrame{ sample_rate, num_channels, samples_per_channel, data };
}

auto AudioFrame::sample_rate() const -> std::uint32_t
{
    return sample_rate_;
}

auto AudioFrame::num_channels() const -> std::uint32_t
{
    return num_channels_;
}

auto AudioFrame::samples_per_channel() const -> std::uint32_t
{
    return samples_per_channel_;
}

auto AudioFrame::data() const -> std::vector<std::int16_t> const &
{
    return data_;
}

auto AudioFrame::duration() const -> std::chrono::milliseconds
{
    return std::chrono::milliseconds{ samples_per_channel_ * 1000 / sample_rate_ };
}

auto AudioFrame::proto_info() const -> proto::AudioFrameBufferInfo
{
    proto::AudioFrameBufferInfo info;
    info.set_sample_rate(sample_rate_);
    info.set_num_channels(num_channels_);
    info.set_samples_per_channel(samples_per_channel_);
    info.set_data_ptr(reinterpret_cast<std::uint64_t>(data_.data()));

    return info;
}

} // namespace livekit::rtc
