// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_ERROR_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_ERROR_DECL

#pragma once

#include "error_fwd_decl.h"

#include <stdexcept>
#include <string>
#include <type_traits>

namespace livekit
{

class LivekitError : public std::runtime_error
{
private:
    std::error_code ec_{};

public:
    explicit LivekitError(std::error_code const & ec);
    explicit LivekitError(std::error_code const & ec, std::string_view msg);
    explicit LivekitError(int ec, std::error_category const & category);
    explicit LivekitError(int ec, std::error_category const & category, std::string_view msg);

    [[nodiscard]] auto code() const noexcept -> std::error_code const &;

    [[nodiscard]] auto what() const noexcept -> char const * override;
};

enum class LivekitErrorCode
{
    Success = 0,
    PublishDataFailed = 1,
    PublishDtmfFailed = 2,
    GetStatsFailed = 3,
    InvalidMediaData = 4,
};

} // namespace livekit

namespace std
{

template <>
struct is_error_code_enum<livekit::LivekitErrorCode> : true_type
{
};

} // namespace std

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_ERROR_DECL
