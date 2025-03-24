// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_HANDLE_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_HANDLE_FWD_DECL

#pragma once

#include "details/livekit_ffi_fwd_decl.h"

namespace livekit::ffi
{

class FfiHandle;
using FfiHandleId = ::FfiHandleId;
static inline constexpr const FfiHandleId INVALID_HANDLE = ::INVALID_HANDLE;

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_HANDLE_FWD_DECL
