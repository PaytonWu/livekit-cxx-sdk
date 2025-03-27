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

class [[nodiscard]] FfiHandle
{
private:
    abc::RefCounter ref_counter_;
    FfiHandleId ffi_handle_id_{ INVALID_HANDLE };
    bool disposed_{ false };

public:
    explicit FfiHandle(FfiHandleId id) noexcept;
    FfiHandle(FfiHandle const & other) noexcept;
    FfiHandle(FfiHandle && other) noexcept;
    ~FfiHandle() noexcept;

    auto operator=(FfiHandle const &) noexcept -> FfiHandle &;
    auto operator=(FfiHandle && other) noexcept -> FfiHandle &;

    [[nodiscard]] auto disposed() const noexcept -> bool;
    auto dispose() noexcept -> void;
};

} // namespace livekit::ffi

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_HANDLE_DECL
