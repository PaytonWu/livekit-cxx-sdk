// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_ABC_ASYNC_QUEUE_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_ABC_ASYNC_QUEUE_FWD_DECL

#pragma once

#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>

namespace abc
{

template <typename T, stdexec::scheduler Scheduler = exec::static_thread_pool::scheduler>
class AsyncQueue;

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_ABC_ASYNC_QUEUE_FWD_DECL
