// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_CLIENT_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_CLIENT_FWD_DECL

#pragma once

#include <cstdint>

namespace livekit::ffi
{
auto ffi_event_callback(std::uint8_t const *, std::size_t) -> void;
class FfiClient;
} // namespace livekit::ffi

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_FFI_FFI_CLIENT_FWD_DECL
