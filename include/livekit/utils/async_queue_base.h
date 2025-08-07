// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE

#pragma once

#include "async_queue_base_decl.h"

namespace livekit::utils
{

template <typename T, stdexec::scheduler Scheduler>
AsyncQueueBase<T, Scheduler>::AsyncQueueBase(Scheduler scheduler) : scheduler_{ scheduler }, queue_{ scheduler }
{
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueueBase<T, Scheduler>::wait_for(auto pred) -> exec::task<T>
{
    while (true)
    {
        auto value = co_await queue_.async_dequeue();
        if (static_cast<bool>(pred(value)))
        {
            // task_done must be manually called for the returned item
            co_return value;
        }

        queue_.task_done();
    }
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueueBase<T, Scheduler>::task_done() -> void
{
    queue_.task_done();
}

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE
