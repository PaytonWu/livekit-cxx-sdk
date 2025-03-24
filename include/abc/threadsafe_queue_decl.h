// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_ABC_THREADSAFE_QUEUE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_ABC_THREADSAFE_QUEUE_DECL

#pragma once

#include "threadsafe_queue_fwd_decl.h"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <type_traits>

namespace abc
{

template <typename T>
class ThreadSafeQueue
{
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

public:
    auto put(T const & value) -> void;

    auto get() const -> T;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_ABC_THREADSAFE_QUEUE_DECL
