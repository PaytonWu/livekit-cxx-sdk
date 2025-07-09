// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_ERROR_FWD_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_ERROR_FWD_DECL

#pragma once

#include <system_error>

namespace livekit
{

class LivekitError;
enum class LivekitErrorCode;

auto make_error_code(LivekitErrorCode ec) noexcept -> std::error_code;
auto livekit_category() noexcept -> std::error_category const &;

} // namespace livekit

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_ERROR_FWD_DECL
