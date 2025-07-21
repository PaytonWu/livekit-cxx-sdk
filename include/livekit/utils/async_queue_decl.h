// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_DECL

#pragma once

#include "async_queue_fwd_decl.h"

#include "async_queue_base_decl.h"

namespace livekit::utils
{

template <typename T, stdexec::scheduler Scheduler>
class AsyncQueue : public AsyncQueueBase<T, Scheduler>
{
public:
    AsyncQueue(AsyncQueue const &) = delete;
    AsyncQueue(AsyncQueue &&) = delete;
    auto operator=(AsyncQueue const &) -> AsyncQueue & = delete;
    auto operator=(AsyncQueue &&) -> AsyncQueue & = delete;

    explicit AsyncQueue(Scheduler scheduler);

    auto async_enqueue(T value) -> exec::task<void>;
    auto async_dequeue() -> exec::task<T>;

    auto enqueue(T const & value) -> bool;
    auto enqueue(T && value) -> bool;
    auto dequeue() -> std::optional<T>;

    auto empty() const noexcept -> bool;
    auto full() const noexcept -> bool;
    auto size() const noexcept -> std::size_t;
    constexpr auto capacity() const noexcept -> std::size_t;

    auto join() -> exec::task<void>;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_DECL
