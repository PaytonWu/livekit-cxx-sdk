// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_DECL

#pragma once

#include "access_token_fwd_decl.h"

#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <vector>

// Forward declarations for protobuf
namespace livekit
{
class RoomConfiguration;
}

namespace livekit::api
{

inline constexpr auto DEFAULT_TTL = std::chrono::hours(6);
inline constexpr auto DEFAULT_LEEWAY = std::chrono::minutes(1);

class VideoGrants
{
public:
    // actions on rooms
    std::optional<bool> room_create;
    std::optional<bool> room_list;
    std::optional<bool> room_record;

    // actions on a particular room
    std::optional<bool> room_admin;
    std::optional<bool> room_join;
    std::string room;

    // allows forwarding participant to room
    std::optional<std::string> destination_room;

    // permissions within a room
    bool can_publish = true;
    bool can_subscribe = true;
    bool can_publish_data = true;

    // TrackSource types that a participant may publish.
    // When set, it supersedes CanPublish. Only sources explicitly set here can be
    // published
    std::optional<std::vector<std::string>> can_publish_sources;

    // by default, a participant is not allowed to update its own metadata
    std::optional<bool> can_update_own_metadata;

    // actions on ingresses
    std::optional<bool> ingress_admin; // applies to all ingress

    // participant is not visible to other participants (useful when making bots)
    std::optional<bool> hidden;

    // [deprecated] indicates to the room that current participant is a recorder
    std::optional<bool> recorder;

    // indicates that the holder can register as an Agent framework worker
    std::optional<bool> agent;
};

class SIPGrants
{
public:
    // manage sip resources
    bool admin = false;
    // make outbound calls
    bool call = false;
};

class Claims
{
public:
    std::string identity;
    std::string name;
    std::string kind;
    std::string metadata;
    std::optional<VideoGrants> video;
    std::optional<SIPGrants> sip;
    std::optional<std::map<std::string, std::string>> attributes;
    std::optional<std::string> sha256;
    std::optional<std::string> room_preset;
    std::optional<livekit::RoomConfiguration const *> room_config;

    std::map<std::string, std::string> as_dict() const;
};

class AccessToken
{
public:
    using ParticipantKind = std::string; // "standard", "egress", "ingress", "sip", "agent"

    AccessToken(std::optional<std::string> api_key = std::nullopt, std::optional<std::string> api_secret = std::nullopt);

    auto with_ttl(std::chrono::duration<int64_t> ttl) -> AccessToken &;
    auto with_grants(VideoGrants const & grants) -> AccessToken &;
    auto with_sip_grants(SIPGrants const & grants) -> AccessToken &;
    auto with_identity(std::string const & identity) -> AccessToken &;
    auto with_kind(ParticipantKind const & kind) -> AccessToken &;
    auto with_name(std::string const & name) -> AccessToken &;
    auto with_metadata(std::string const & metadata) -> AccessToken &;
    auto with_attributes(std::map<std::string, std::string> const & attributes) -> AccessToken &;
    auto with_sha256(std::string const & sha256) -> AccessToken &;
    auto with_room_preset(std::string const & preset) -> AccessToken &;
    auto with_room_config(livekit::RoomConfiguration const * config) -> AccessToken &;

    std::string to_jwt() const;

private:
    std::string api_key_; // iss
    std::string api_secret_;
    Claims claims_;
    std::string identity_;                             // sub
    std::chrono::duration<int64_t> ttl_ = DEFAULT_TTL; // exp
};

class TokenVerifier
{
public:
    explicit TokenVerifier(std::optional<std::string> api_key = std::nullopt,
                           std::optional<std::string> api_secret = std::nullopt,
                           std::chrono::duration<int64_t> leeway = DEFAULT_LEEWAY);

    Claims verify(std::string const & token) const;

private:
    std::string api_key_;
    std::string api_secret_;
    std::chrono::duration<int64_t> leeway_;
};

// Utility functions
std::string camel_to_snake(std::string const & input);
std::string snake_to_lower_camel(std::string const & input);

} // namespace livekit::api

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN_DECL
