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
        auto result = queue_.dequeue();
        if (result)
        {
            if (auto value = std::move(*result); static_cast<bool>(pred(value)))
            {
                co_return value;
            }
        }

        co_await stdexec::schedule(scheduler_);
    }
}

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE
