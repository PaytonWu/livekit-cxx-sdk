// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_LIVEKIT_FFI
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_LIVEKIT_FFI

#pragma once

#include <livekit_ffi.h>

#include <cstdint>

extern "C"
{
    /// # SAFTEY: The "C" callback must be threadsafe and not block
    // pub type FfiCallbackFn = unsafe extern "C" fn(*const u8, usize);
    //
    /// # Safety
    ///
    /// The foreign language must only provide valid pointers
    // #[no_mangle]
    // pub unsafe extern "C" fn livekit_ffi_initialize(cb: FfiCallbackFn, capture_logs: bool)
    // livekit_ffi_initialize(cb: FfiCallbackFn, capture_logs: bool)

    using FfiCallback = auto (*)(std::uint8_t const *, std::size_t) -> void;

    auto livekit_ffi_initialize(FfiCallback cb, bool capture_logs) -> void;
    auto livekit_ffi_dispose() -> void;
};

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_LIVEKIT_FFI
