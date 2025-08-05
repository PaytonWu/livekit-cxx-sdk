// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/utils/event_loop.h>

#include <cassert>

namespace livekit::utils
{

auto TimerHandle::cancel() noexcept -> void
{
    assert(cancelled_ != nullptr);
    cancelled_->store(true, std::memory_order_release);
}

auto TimerHandle::is_cancelled() const noexcept -> bool
{
    assert(cancelled_ != nullptr);
    return cancelled_->load(std::memory_order_acquire);
}

auto EventLoop::get_scheduler() const noexcept -> exec::timed_thread_scheduler
{
    return const_cast<exec::timed_thread_context &>(context_).get_scheduler();
}

} // namespace livekit::utils
