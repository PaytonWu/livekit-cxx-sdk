// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_INCLUDE_LIVEKIT_FFI_FFI_QUEUE_DECL
#define LIVEKIT_INCLUDE_LIVEKIT_FFI_FFI_QUEUE_DECL

#pragma once

#include "ffi_queue_fwd_decl.h" // IWYU pragma: export

#include "livekit/utils/async_queue_decl.h"

#include <memory>
#include <mutex>
#include <vector>

namespace livekit::ffi
{

template <typename T>
class FfiQueue
{
private:
    constexpr static std::size_t Capacity = 1024;

    std::vector<std::shared_ptr<utils::AsyncQueue<T>>> subscribers_;
    std::mutex mutex_;

public:
    auto put(T const & item) -> void;

    auto subscribe(exec::static_thread_pool::scheduler scheduler) -> std::shared_ptr<utils::AsyncQueue<T>>;

    auto unsubscribe(std::shared_ptr<utils::AsyncQueue<T>> const & queue) -> void;
};

}

#endif
