// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/ffi/ffi_handle.h>

namespace livekit::ffi
{

FfiHandle::FfiHandle(FfiHandleId const id) noexcept : ref_counter_{}, id_{ id }
{
}

auto FfiHandle::disposed() const noexcept -> bool
{
    return ref_counter_.use_count() == 0 || disposed_;
}

auto FfiHandle::dispose() noexcept -> void
{
    if (id_ != INVALID_HANDLE && !disposed_)
    {
        disposed_ = true;
        auto const id = id_;
        id_ = INVALID_HANDLE;

        livekit_ffi_drop_handle(id);
    }
}

}