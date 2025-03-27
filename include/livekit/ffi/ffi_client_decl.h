// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_CLIENT_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_CLIENT_DECL

#pragma once

#include "ffi_client_fwd_decl.h"

#include "ffi_queue_decl.h"
#include "proto/ffi.pb.h"

namespace livekit::ffi
{

class FfiClient final
{
private:
    FfiClient();
    friend auto ffi_event_callback(std::uint8_t const *, std::size_t) -> void;

private:
    FfiQueue<proto::FfiEvent> queue_{}; // queue for events

public:
    static auto instance() -> FfiClient &;

    static auto request(proto::FfiRequest const & request) -> proto::FfiResponse;
};

} // namespace livekit::ffi

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_CLIENT_DECL
