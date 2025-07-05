// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_STRONG_TYPED_VALUE
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_STRONG_TYPED_VALUE

#pragma once

#include "strong_typed_value_decl.h"

#include <utility>

namespace livekit::utils
{

template <typename T, typename Tag>
StrongTypedValue<T, Tag>::StrongTypedValue(T const & value) : value_{value}
{
}

template <typename T, typename Tag>
StrongTypedValue<T, Tag>::StrongTypedValue(T && value) : value_{std::move(value)}
{
}

template <typename T, typename Tag>
StrongTypedValue<T, Tag>::operator T &&() &&
{
    return std::move(value_);
}

template <typename T, typename Tag>
StrongTypedValue<T, Tag>::operator T const &() const &
{
    return value_;
}

template <typename T, typename Tag>
auto StrongTypedValue<T, Tag>::value() const & -> const T &
{
    return value_;
}

template <typename T, typename Tag>
auto StrongTypedValue<T, Tag>::value() && -> T &&
{
    return std::move(value_);
}

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_STRONG_TYPED_VALUE
