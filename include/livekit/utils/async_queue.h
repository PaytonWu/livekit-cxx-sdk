// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE

#pragma once

#include "async_queue_decl.h"

namespace livekit::utils
{

template <typename T, stdexec::scheduler Scheduler>
AsyncQueue<T, Scheduler>::AsyncQueue(Scheduler scheduler) : queue_{ scheduler }
{
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::async_enqueue(T value) -> exec::task<void>
{
    return queue_.async_enqueue(std::move(value));
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::async_dequeue() -> exec::task<T>
{
    return queue_.async_dequeue();
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::enqueue(T const & value) -> bool
{
    return queue_.enqueue(value);
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::enqueue(T && value) -> bool
{
    return queue_.enqueue(std::move(value));
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::dequeue() -> std::optional<T>
{
    return queue_.try_dequeue();
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::empty() const noexcept -> bool
{
    return queue_.empty();
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::full() const noexcept -> bool
{
    return queue_.full();
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::size() const noexcept -> std::size_t
{
    return queue_.size();
}

template <typename T, stdexec::scheduler Scheduler>
constexpr auto AsyncQueue<T, Scheduler>::capacity() const noexcept -> std::size_t
{
    return queue_.capacity();
}

template <typename T, stdexec::scheduler Scheduler>
auto AsyncQueue<T, Scheduler>::wait_for(auto pred) -> exec::task<T>
{
    return queue_.wait_for(pred);
}

} // namespace abc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE
