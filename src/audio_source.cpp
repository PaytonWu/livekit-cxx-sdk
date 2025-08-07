// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/audio_source.h>

#include <livekit/error.h>
#include <livekit/ffi/ffi_client.h>
#include <livekit/ffi/proto/audio_frame.pb.h>
#include <livekit/ffi/proto/ffi.pb.h>

#include <exec/just_from.hpp>
#include <stdexec/execution.hpp>

namespace livekit::rtc
{

AudioSource::AudioSource(int sample_rate, int num_of_channels, std::chrono::milliseconds queue_size)
    : sample_rate_{ sample_rate }
    , num_of_channels_{ num_of_channels }
    , queue_size_{ queue_size }
{
    proto::FfiRequest request;
    auto * new_audio_source = request.mutable_new_audio_source();
    new_audio_source->set_type(proto::AudioSourceType::AUDIO_SOURCE_NATIVE);
    new_audio_source->set_sample_rate(sample_rate);
    new_audio_source->set_num_channels(num_of_channels);
    new_audio_source->set_queue_size_ms(queue_size.count());

    auto response = ffi::FfiClient::request(request);
    owned_audio_source_ = response.new_audio_source().source();
    handle_ = ffi::FfiHandle{ owned_audio_source_.handle().id() };
}

auto AudioSource::capture_frame(AudioFrame const & frame) -> exec::task<void>
{
    if (frame.sample_rate() == 0 || ffi_handle().disposed())
    {
        co_return;
    }

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
    auto elapsed = last_capture_time_.count() > 0 ? now - last_capture_time_ : std::chrono::milliseconds::zero();
    q_size_ += std::chrono::milliseconds{ frame.samples_per_channel() / sample_rate_ * 1000 } - elapsed;
    last_capture_time_ = now;

    if (join_handle_.has_value())
    {
        join_handle_->cancel();
    }

    if (!capture_frame_future_.has_value())
    {
        capture_frame_future_ = std::make_optional(capture_frame_promise_.get_future());
    }

    join_handle_ = event_loop_.call_later(q_size_, [this]() { release_waiter(); });

    last_capture_time_ = now;
    proto::FfiRequest request;
    auto * capture_frame = request.mutable_capture_audio_frame();
    capture_frame->set_source_handle(handle_.id());
    auto * buffer = capture_frame->mutable_buffer();
    buffer->CopyFrom(frame.into_proto());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto response = ffi::FfiClient::request(request);
    proto::FfiEvent event = co_await queue->wait_for([&response](proto::FfiEvent const & event) {
        return event.has_capture_audio_frame() && event.capture_audio_frame().has_async_id() && response.has_capture_audio_frame() &&
               response.capture_audio_frame().has_async_id() && event.capture_audio_frame().async_id() == response.capture_audio_frame().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.capture_audio_frame().has_error())
    {
        throw_error(LivekitErrorCode::CaptureAudioFrameFailed, event.capture_audio_frame().error());
    }

    co_return;
}

auto AudioSource::wait_for_playout() -> exec::task<void>
{
    if (!capture_frame_future_.has_value())
    {
        co_return;
    }

    // Execute the future.get() on the scheduler and ignore the result
    co_await stdexec::starts_on(event_loop_.get_scheduler(), exec::just_from([future = std::move(*capture_frame_future_)](auto /*sink*/) mutable {
                                    // sink();
                                    future.get();
                                    return stdexec::completion_signatures<stdexec::set_value_t()>{};
                                }));

    co_return;
}

auto AudioSource::sample_rate() const noexcept -> int
{
    return sample_rate_;
}

auto AudioSource::num_channels() const -> int
{
    return num_of_channels_;
}

auto AudioSource::ffi_handle() const -> ffi::FfiHandle const &
{
    return handle_;
}

auto AudioSource::release_waiter() -> void
{
    if (!capture_frame_future_.has_value())
    {
        return;
    }

    capture_frame_promise_.set_value();

    last_capture_time_ = std::chrono::milliseconds::zero();
    q_size_ = std::chrono::milliseconds::zero();
    // join_handle_ = std::nullopt;
    capture_frame_future_ = std::nullopt;
}

} // namespace livekit::rtc
