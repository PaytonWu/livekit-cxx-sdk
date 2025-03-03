// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_DETAILS_LIVEKIT_FFI_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_DETAILS_LIVEKIT_FFI_FWD_DECL

#pragma once

#include <cstdint>

using FfiHandleId = std::uint64_t;

inline constexpr const FfiHandleId INVALID_HANDLE = 0;

extern "C"
{
    // /// # SAFTEY: The "C" callback must be threadsafe and not block
    // pub type FfiCallbackFn = unsafe extern "C" fn(*const u8, usize);
    //
    // /// # Safety
    // ///
    // /// The foreign language must only provide valid pointers
    // #[no_mangle]
    // pub unsafe extern "C" fn livekit_ffi_initialize(cb: FfiCallbackFn, capture_logs: bool)

    using FfiCallback = auto (*)(std::uint8_t const *, std::size_t) -> void;

    // livekit_ffi_initialize(cb: FfiCallbackFn, capture_logs: bool)
    auto
    livekit_ffi_initialize(FfiCallback cb, bool capture_logs) -> void;

    auto
    livekit_ffi_request(const std::uint8_t * data, std::size_t len, const std::uint8_t ** res_ptr, std::size_t * res_len) -> FfiHandleId;

    auto
    livekit_ffi_drop_handle(FfiHandleId handle_id) -> bool;
};

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_DETAILS_LIVEKIT_FFI_FWD_DECL
