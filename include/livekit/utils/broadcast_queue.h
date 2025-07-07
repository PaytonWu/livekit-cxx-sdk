// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE

#pragma once

#include "broadcast_queue_decl.h"

namespace livekit::utils
{

template <typename T>
auto BroadcastQueue<T>::enqueue(T && item) -> void
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.emplace_back(std::move(item));
}

template <typename T>
auto BroadcastQueue<T>::enqueue(const T & item) -> void
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.emplace_back(item);
}

template <typename T>
auto BroadcastQueue<T>::dequeue() -> std::optional<T>
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty())
    {
        return std::nullopt;
    }
    auto item = std::move(queue_.front());
    queue_.pop_front();
    return std::move(item);
}

template <typename T>
auto BroadcastQueue<T>::size() const -> size_t
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

template <typename T>
auto BroadcastQueue<T>::empty() const -> bool
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

template <typename T>
auto BroadcastQueue<T>::clear() -> void
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.clear();
}

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE
