// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/utils/event_loop.h>


namespace livekit::utils
{

TimerHandle::TimerHandle(bool cancelled)
    : cancelled_{ std::make_shared<std::atomic<bool>>(cancelled) }
{
}

auto TimerHandle::cancel() noexcept -> void
{
    if (cancelled_ == nullptr)
    {
        return;
    }

    cancelled_->store(true, std::memory_order_release);
}

auto TimerHandle::is_cancelled() const noexcept -> bool
{
    return cancelled_ == nullptr || cancelled_->load(std::memory_order_acquire);
}

} // namespace livekit::utils
