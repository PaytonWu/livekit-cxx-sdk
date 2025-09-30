// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_FWD_DECL_H
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_FWD_DECL_H

#pragma once

#include <nlohmann/json.hpp>

namespace livekit::api
{

class [[nodiscard]] AccessToken;
class [[nodiscard]] TokenVerifier;
class [[nodiscard]] VideoGrants;
class [[nodiscard]] SIPGrants;
class [[nodiscard]] Claims;

} // namespace livekit::api

namespace nlohmann
{

template <>
struct adl_serializer<::livekit::api::VideoGrants>;

template <>
struct adl_serializer<::livekit::api::SIPGrants>;

template <>
struct adl_serializer<::livekit::api::Claims>;

} // namespace nlohmann

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_FWD_DECL_H
