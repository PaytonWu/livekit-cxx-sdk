// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_EVENT_LOOP
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_EVENT_LOOP

#pragma once

#include "event_loop_decl.h"

#include <stdexec/execution.hpp>

#include <tuple>

namespace livekit::utils
{

template <typename Callable, typename... Args>
auto EventLoop::call_later(std::chrono::milliseconds delay, Callable && callback, Args &&... args) -> TimerHandle
{
    TimerHandle handle{};

    // Create a sender that delays and then executes the callback
    auto delayed_work = exec::schedule_after(this->context_.get_scheduler(), delay) |
                        stdexec::then([handle, callback = std::forward<Callable>(callback), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                            if (!handle.is_cancelled())
                            {
                                std::apply(callback, std::move(args));
                            }
                        });

    // Start the work
    stdexec::start_detached(std::move(delayed_work));

    return handle;
}

} // namespace livekit::utils

#endif
