// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE_DECL

#pragma once

#include "broadcast_queue_fwd_decl.h"

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace livekit::utils
{

template <typename T>
class BroadcastQueue
{
private:
    std::deque<T> queue_;
    mutable std::mutex mutex_;

public:
    auto enqueue(T && item) -> void;
    auto enqueue(const T & item) -> void;

    auto dequeue() -> std::optional<T>;

    auto size() const -> size_t;
    auto empty() const -> bool;

    auto clear() -> void;

};

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_BROADCAST_QUEUE_DECL
