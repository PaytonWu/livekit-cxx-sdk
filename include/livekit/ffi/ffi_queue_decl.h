// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_INCLUDE_LIVEKIT_FFI_FFI_QUEUE_DECL
#define LIVEKIT_INCLUDE_LIVEKIT_FFI_FFI_QUEUE_DECL

#pragma once

#include "ffi_queue_fwd_decl.h"

#include <abc/threadsafe_queue.h>

#include <memory>
#include <mutex>
#include <vector>

namespace livekit::ffi
{

template <typename T>
class FfiQueue
{
private:
    std::vector<std::shared_ptr<abc::ThreadSafeQueue<T>>> subscribers_;
    std::mutex mutex_;

public:
    auto put(T const & item) -> void;

    auto subscribe() -> std::shared_ptr<abc::ThreadSafeQueue<T>>;

    auto unsubscribe(std::shared_ptr<abc::ThreadSafeQueue<T>> const & queue) -> void;
};

}

#endif
