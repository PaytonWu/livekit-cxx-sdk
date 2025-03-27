// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/ffi/ffi_handle.h>

namespace livekit::ffi
{

FfiHandle::FfiHandle(FfiHandleId const id) noexcept : ref_counter_{}, ffi_handle_id_{ id }
{
}

FfiHandle::FfiHandle(FfiHandle const & other) noexcept : ref_counter_{ other.ref_counter_ }, ffi_handle_id_{ other.ffi_handle_id_ }
{
}

FfiHandle::FfiHandle(FfiHandle && other) noexcept : ref_counter_{ std::move(other.ref_counter_) }, ffi_handle_id_{ other.ffi_handle_id_ }
{
    other.ffi_handle_id_ = INVALID_HANDLE;
}

FfiHandle::~FfiHandle() noexcept
{
    if (ref_counter_.rel_ref())
    {
        dispose();
    }
}

auto FfiHandle::disposed() const noexcept -> bool
{
    return ref_counter_.use_count() == 0 || disposed_;
}

auto FfiHandle::dispose() noexcept -> void
{
    if (ffi_handle_id_ != INVALID_HANDLE && !disposed_)
    {
        disposed_ = true;
        auto const id = ffi_handle_id_;
        ffi_handle_id_ = INVALID_HANDLE;

        livekit_ffi_drop_handle(id);
    }
}

} // namespace livekit::ffi
