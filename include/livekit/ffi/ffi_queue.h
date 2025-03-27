// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_INCLUDE_LIVEKIT_FFI_FFI_QUEUE
#define LIVEKIT_INCLUDE_LIVEKIT_FFI_FFI_QUEUE

#pragma once

#include "ffi_queue_decl.h"

#include <exception>

namespace livekit::ffi
{

template <typename T>
auto FfiQueue<T>::put(T const & item) -> void
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto & queue : subscribers_) {
        try {
            queue->put(item);
        } catch (std::exception const & e) {
            std::cerr << "error putting to queue: " << e.what() << std::endl;   // todo: log to file.
        }
    }
}

template <typename T>
auto FfiQueue<T>::subscribe() -> std::shared_ptr<abc::ThreadSafeQueue<T>>
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto queue = std::make_shared<abc::ThreadSafeQueue<T>>();
    subscribers_.push_back(queue);
    return queue;
}

template <typename T>
auto FfiQueue<T>::unsubscribe(std::shared_ptr<abc::ThreadSafeQueue<T>> const & queue) -> void
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto it = subscribers_.begin(); it != subscribers_.end(); ++it) {
        if (*it == queue) {
            subscribers_.erase(it);
            break;
        }
    }
}

}

#endif
