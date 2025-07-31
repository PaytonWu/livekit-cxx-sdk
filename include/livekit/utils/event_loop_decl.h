// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_EVENT_LOOP_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_EVENT_LOOP_DECL

#pragma once

#include "event_loop_fwd_decl.h"

#include <exec/timed_thread_scheduler.hpp>

#include <atomic>
#include <functional>
#include <memory>


namespace livekit::utils
{

class EventLoop
{
private:
    exec::timed_thread_context context_{};

public:
    using CallbackType = std::function<void()>;

    struct TimerHandle
    {
        std::atomic<bool> cancelled{ false };
        auto cancel() noexcept -> void;
        auto is_cancelled() const noexcept -> bool;
    };

    template <typename Callable, typename... Args>
    auto call_later(double delay_seconds, Callable && callback, Args &&... args) -> std::shared_ptr<TimerHandle>;
};

} // namespace livekit::utils

#endif
