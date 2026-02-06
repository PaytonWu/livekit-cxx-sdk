// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_STRONG_TYPED_VALUE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_STRONG_TYPED_VALUE_DECL

#pragma once

#include "strong_typed_value_fwd_decl.h"

#include <compare>
#include <concepts>
#include <format>

namespace livekit::utils
{

template <typename T, typename Tag>
class StrongTypedValue
{
private:
    T value_{};

public:
    StrongTypedValue() noexcept(std::is_nothrow_constructible_v<T>)
        requires std::is_default_constructible_v<T>
    = default;

    explicit StrongTypedValue(T const & value);
    explicit StrongTypedValue(T && value);

    auto operator==(StrongTypedValue<T, Tag> const &) const -> bool = default;

    auto operator<=>(StrongTypedValue<T, Tag> const &) const -> std::strong_ordering
        requires std::three_way_comparable<T> && std::convertible_to<std::compare_three_way_result_t<T>, std::strong_ordering>
    = default;

    auto operator<=>(StrongTypedValue<T, Tag> const &) const -> std::weak_ordering
        requires std::three_way_comparable<T> && std::convertible_to<std::compare_three_way_result_t<T>, std::weak_ordering> &&
                     (!std::convertible_to<std::compare_three_way_result_t<T>, std::strong_ordering>)
    = default;

    auto operator<=>(StrongTypedValue<T, Tag> const &) const -> std::partial_ordering
        requires std::three_way_comparable<T> && (!std::convertible_to<std::compare_three_way_result_t<T>, std::weak_ordering>)
    = default;

    operator T const &() const &;
    operator T &() &;
    operator T &&() &&;

    auto value() const & -> T const &;
    auto value() & -> T &;
    auto value() && -> T &&;
};

} // namespace livekit::utils

namespace std
{

template <typename T, typename Tag>
struct formatter<livekit::utils::StrongTypedValue<T, Tag>>
{
    template <typename FormatContext>
    auto format(livekit::utils::StrongTypedValue<T, Tag> const & value, FormatContext & ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}", value.value());
    }
};

} // namespace std

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_UTILS_STRONG_TYPED_VALUE_DECL
