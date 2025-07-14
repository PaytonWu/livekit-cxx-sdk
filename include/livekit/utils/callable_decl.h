// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_CALLABLE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_CALLABLE_DECL

#pragma once

#include "callable_fwd_decl.h"

namespace livekit::utils
{

class Callable
{
public:
    Callable() = default;
    virtual ~Callable() = default;

    virtual auto operator()() -> void = 0;
};

} // namespace livekit::utils

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_CALLABLE_DECL
