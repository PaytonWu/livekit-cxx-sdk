// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE_FWD_DECL

#pragma once

#include <exec/static_thread_pool.hpp>

namespace livekit::utils
{

template <typename T, stdexec::scheduler Scheduler = exec::static_thread_pool::scheduler>
class AsyncQueueBase;

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_ASYNC_QUEUE_BASE_FWD_DECL
