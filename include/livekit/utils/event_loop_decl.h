// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_EVENT_LOOP_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_EVENT_LOOP_DECL

#pragma once

#include "event_loop_fwd_decl.h"

#include <exec/timed_thread_scheduler.hpp>

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>


namespace livekit::utils
{

class [[nodiscard]] TimerHandle
{
public:
    // Cancel the scheduled callback
    auto cancel() noexcept -> void;

    // Check if the timer is cancelled
    auto is_cancelled() const noexcept -> bool;

private:
    std::shared_ptr<std::atomic<bool>> cancelled_{ std::make_shared<std::atomic<bool>>(false) };
};

class EventLoop
{
private:
    exec::timed_thread_context context_{};

public:
    using CallbackType = std::function<void()>;

    template <typename Callable, typename... Args>
    auto call_later(std::chrono::milliseconds delay, Callable && callback, Args &&... args) -> TimerHandle;
};

} // namespace livekit::utils

#endif
