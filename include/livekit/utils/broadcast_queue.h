// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE

#pragma once

#include "broadcast_queue_decl.h"

#include <algorithm>
#include <range/v3/algorithm/remove.hpp>

namespace livekit::utils
{

template <typename T, stdexec::scheduler Scheduler>
BroadcastQueue<T, Scheduler>::BroadcastQueue(Scheduler scheduler) : scheduler_{ scheduler }
{
}

template <typename T, stdexec::scheduler Scheduler>
auto BroadcastQueue<T, Scheduler>::enqueue(T item) -> void
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto & subscriber : subscribers_)
    {
        subscriber->enqueue(item);
    }
}

template <typename T, stdexec::scheduler Scheduler>
auto BroadcastQueue<T, Scheduler>::subscribe() -> std::shared_ptr<AsyncQueue<T, Scheduler>>
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto subscriber = std::make_shared<AsyncQueue<T, Scheduler>>(scheduler_);
    subscribers_.push_back(subscriber);
    return subscriber;
}

template <typename T, stdexec::scheduler Scheduler>
auto BroadcastQueue<T, Scheduler>::unsubscribe(std::shared_ptr<AsyncQueue<T, Scheduler>> subscriber) -> void
{
    std::lock_guard<std::mutex> lock(mutex_);
    subscribers_.erase(ranges::remove(subscribers_, subscriber), subscribers_.end());
}

template <typename T, stdexec::scheduler Scheduler>
auto BroadcastQueue<T, Scheduler>::join() -> exec::task<void>
{
    std::vector<std::shared_ptr<AsyncQueue<T, Scheduler>>> subscribers;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        subscribers = subscribers_;
    }

    for (auto & subscriber : subscribers)
    {
        co_await subscriber->join();
    }
}

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE
