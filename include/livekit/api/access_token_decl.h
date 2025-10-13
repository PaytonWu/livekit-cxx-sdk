// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_DECL

#pragma once

#include "access_token_fwd_decl.h"

#include <livekit/ffi/proto/livekit_room.pb.h>

#include <abc/type_traits.h>
#include <nlohmann/json.hpp>

#include <any>
#include <chrono>
#include <expected>
#include <map>
#include <optional>
#include <string>
#include <system_error>
#include <variant>
#include <vector>

namespace livekit::api
{

inline constexpr auto DEFAULT_TTL = std::chrono::hours(6);
inline constexpr auto DEFAULT_LEEWAY = std::chrono::minutes(1);

template <typename T>
auto from_json(nlohmann::json const & j) -> std::expected<T, std::error_code>;

class VideoGrants
{
public:
    // actions on rooms
    bool room_create{ false };
    bool room_list{ false };
    bool room_record{ false };

    // actions on a particular room
    bool room_admin{ false };
    bool room_join{ false };
    std::string room{};

    // allows forwarding participant to room
    std::string destination_room{};

    // permissions within a room
    bool can_publish = true;
    bool can_subscribe = true;
    bool can_publish_data = true;

    // TrackSource types that a participant may publish.
    // When set, it supersedes CanPublish. Only sources explicitly set here can be
    // published
    std::vector<std::string> can_publish_sources{};

    // by default, a participant is not allowed to update its own metadata
    bool can_update_own_metadata{ false };

    // actions on ingresses
    bool ingress_admin{ false }; // applies to all ingress

    // participant is not visible to other participants (useful when making bots)
    bool hidden{ false };

    // [deprecated] indicates to the room that current participant is a recorder
    bool recorder{ false };

    // indicates that the holder can register as an Agent framework worker
    // bool agent{false};
};

auto to_json(nlohmann::json & j, VideoGrants const & v) -> void;
auto from_json(nlohmann::json const & j, VideoGrants & v) -> void;

template <>
auto from_json<VideoGrants>(nlohmann::json const & j) -> std::expected<VideoGrants, std::error_code>;

class SIPGrants
{
public:
    // manage sip resources
    bool admin = false;
    // make outbound calls
    bool call = false;
};

auto to_json(nlohmann::json & j, SIPGrants const & v) -> void;
auto from_json(nlohmann::json const & j, SIPGrants & v) -> void;

template <>
auto from_json<SIPGrants>(nlohmann::json const & j) -> std::expected<SIPGrants, std::error_code>;

class Claims
{
public:
    std::size_t expires_at{ static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) +
                            static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::seconds>(DEFAULT_TTL).count()) };
    std::string issuer;
    std::size_t not_before{ static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
    std::string identity_subject; // identity

    std::string name;
    std::string metadata;
    std::optional<VideoGrants> video;
    std::optional<SIPGrants> sip;
    std::optional<std::map<std::string, std::string>> attributes;
    std::optional<std::string> sha256;
    std::optional<std::string> room_preset;
    std::optional<livekit::RoomConfiguration> room_config;
};

auto to_json(nlohmann::json & j, Claims const & v) -> void;
auto from_json(nlohmann::json const & j, Claims & v) -> void;

template <>
auto from_json<Claims>(nlohmann::json const & j) -> std::expected<Claims, std::error_code>;

class AccessToken
{
public:
    using ParticipantKind = std::string; // "standard", "egress", "ingress", "sip", "agent"

    AccessToken(std::optional<std::string> api_key = std::nullopt, std::optional<std::string> api_secret = std::nullopt);

    auto with_ttl(std::chrono::duration<int64_t> ttl) -> AccessToken &;
    auto with_grants(VideoGrants const & grants) -> AccessToken &;
    auto with_sip_grants(SIPGrants const & grants) -> AccessToken &;
    auto with_identity(std::string const & identity) -> AccessToken &;
    auto with_name(std::string const & name) -> AccessToken &;
    auto with_metadata(std::string const & metadata) -> AccessToken &;
    auto with_attributes(std::map<std::string, std::string> const & attributes) -> AccessToken &;
    auto with_sha256(std::string const & sha256) -> AccessToken &;
    auto with_room_preset(std::string const & preset) -> AccessToken &;
    auto with_room_config(livekit::RoomConfiguration const & config) -> AccessToken &;

    auto to_jwt() const -> std::expected<std::string, std::error_code>;

private:
    std::string api_key_{}; // iss
    std::string api_secret_{};
    Claims claims_{};
};

class TokenVerifier
{
public:
    explicit TokenVerifier(std::optional<std::string> api_key = std::nullopt,
                           std::optional<std::string> api_secret = std::nullopt,
                           std::chrono::seconds leeway = DEFAULT_LEEWAY);

    auto verify(std::string const & token) const -> std::expected<Claims, std::error_code>;

private:
    std::string api_key_{};
    std::string api_secret_{};
    std::chrono::seconds leeway_;
};

} // namespace livekit::api

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_DECL
