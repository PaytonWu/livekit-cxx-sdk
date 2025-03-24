// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_HANDLE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_HANDLE_DECL

#pragma once

#include "ffi_handle_fwd_decl.h"

#include "details/livekit_ffi_decl.h"

#include <abc/reference_counter.h>

namespace livekit::ffi
{

class FfiHandle
{
private:
    abc::RefCounter ref_counter_;
    FfiHandleId id_{INVALID_HANDLE};
    bool disposed_{false};

    explicit FfiHandle(FfiHandleId id) noexcept;

    auto disposed() const noexcept -> bool;
    auto dispose() noexcept -> void;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_HANDLE_DECL
