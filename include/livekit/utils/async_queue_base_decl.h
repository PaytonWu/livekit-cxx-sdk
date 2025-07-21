// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE_DECL

#pragma once

#include "async_queue_base_fwd_decl.h"

#include <exec/task.hpp>

#include <abc/async/queue.h>

namespace livekit::utils
{

template <typename T, stdexec::scheduler Scheduler>
class AsyncQueueBase
{
protected:
    Scheduler scheduler_;
    abc::async::Queue<T, 1024, Scheduler> queue_;

protected:
    AsyncQueueBase(AsyncQueueBase const &) = delete;
    AsyncQueueBase(AsyncQueueBase &&) = delete;
    auto operator=(AsyncQueueBase const &) -> AsyncQueueBase & = delete;
    auto operator=(AsyncQueueBase &&) -> AsyncQueueBase & = delete;

    explicit AsyncQueueBase(Scheduler scheduler);
    virtual ~AsyncQueueBase() = default;

public:
    auto wait_for(auto pred) -> exec::task<T>;
};

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE_DECL
