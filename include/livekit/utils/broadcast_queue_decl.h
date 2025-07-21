// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE_DECL

#pragma once

#include "broadcast_queue_fwd_decl.h"

#include "async_queue_decl.h"

#include <mutex>
#include <vector>

namespace livekit::utils
{

template <typename T, stdexec::scheduler Scheduler>
class BroadcastQueue
{
private:
    Scheduler scheduler_;
    std::vector<std::shared_ptr<AsyncQueue<T, Scheduler>>> subscribers_;
    mutable std::mutex mutex_;

public:
    explicit BroadcastQueue(Scheduler scheduler);

    auto enqueue(T item) -> void;

    auto subscribe() -> std::shared_ptr<AsyncQueue<T, Scheduler>>;
    auto unsubscribe(std::shared_ptr<AsyncQueue<T, Scheduler>> subscriber) -> void;

    auto join() -> exec::task<void>;
};

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE_DECL
