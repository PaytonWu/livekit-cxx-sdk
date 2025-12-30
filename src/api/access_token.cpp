// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/api/access_token.h>

#include <livekit/api/error.h>
#include <livekit/error.h>
#include <livekit/ffi/proto/livekit_room.pb.h>

#include <google/protobuf/util/json_util.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

#include <cstdlib>

namespace livekit::api
{

auto to_json(nlohmann::json & j, VideoGrants const & v) -> void
{
    j = nlohmann::json::object();

    // camelCase to match existing JWT claims
    if (v.room_create)
    {
        j["roomCreate"] = v.room_create;
    }
    if (v.room_list)
    {
        j["roomList"] = v.room_list;
    }
    if (v.room_record)
    {
        j["roomRecord"] = v.room_record;
    }
    if (v.room_admin)
    {
        j["roomAdmin"] = v.room_admin;
    }
    if (v.room_join)
    {
        j["roomJoin"] = v.room_join;
    }
    if (!v.room.empty())
    {
        j["room"] = v.room;
    }
    if (!v.destination_room.empty())
    {
        j["destinationRoom"] = v.destination_room;
    }
    if (!v.can_publish)
    {
        j["canPublish"] = v.can_publish;
    }
    if (!v.can_subscribe)
    {
        j["canSubscribe"] = v.can_subscribe;
    }
    if (!v.can_publish_data)
    {
        j["canPublishData"] = v.can_publish_data;
    }
    if (!v.can_publish_sources.empty())
    {
        j["canPublishSources"] = v.can_publish_sources;
    }
    if (v.can_update_own_metadata)
    {
        j["canUpdateOwnMetadata"] = v.can_update_own_metadata;
    }
    if (v.ingress_admin)
    {
        j["ingressAdmin"] = v.ingress_admin;
    }
    if (v.hidden)
    {
        j["hidden"] = v.hidden;
    }
    if (v.recorder)
    {
        j["recorder"] = v.recorder;
    }
}

template <>
auto from_json<VideoGrants>(nlohmann::json const & j) -> std::expected<VideoGrants, std::error_code>
{
    VideoGrants v;

    auto it = j.find("roomCreate");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.room_create = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("roomList");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.room_list = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("roomRecord");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.room_record = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("roomAdmin");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.room_admin = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("roomJoin");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.room_join = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("room");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.room = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("destinationRoom");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.destination_room = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("canPublish");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.can_publish = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("canSubscribe");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.can_subscribe = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("canPublishData");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.can_publish_data = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("canPublishSources");
    if (it != j.end())
    {
        if (it.value().is_array())
        {
            v.can_publish_sources = it.value().get<std::vector<std::string>>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("canUpdateOwnMetadata");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.can_update_own_metadata = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("ingressAdmin");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.ingress_admin = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("hidden");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.hidden = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    it = j.find("recorder");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.recorder = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    return v;
}

auto from_json(nlohmann::json const & j, VideoGrants & v) -> void
{
    auto r = from_json<VideoGrants>(j);
    if (r.has_value())
    {
        v = r.value();
    }
    else
    {
        throw_error(r.error());
    }
}

auto to_json(nlohmann::json & j, SIPGrants const & v) -> void
{
    j = nlohmann::json::object();
    if (v.admin)
    {
        j["admin"] = v.admin;
    }
    if (v.call)
    {
        j["call"] = v.call;
    }
}

template <>
auto from_json<SIPGrants>(nlohmann::json const & j) -> std::expected<SIPGrants, std::error_code>
{
    SIPGrants v;
    auto it = j.find("admin");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.admin = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("call");
    if (it != j.end())
    {
        if (it.value().is_boolean())
        {
            v.call = it.value().get<bool>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    return v;
}

auto from_json(nlohmann::json const & j, SIPGrants & v) -> void
{
    auto r = from_json<SIPGrants>(j);
    if (r.has_value())
    {
        v = r.value();
    }
    else
    {
        throw_error(r.error());
    }
}

auto to_json(nlohmann::json & j, Claims const & v) -> void
{
    j = nlohmann::json::object({
        { "exp", v.expires_at },
        { "iss", v.issuer },
        { "nbf", v.not_before },
        { "sub", v.identity_subject },
    });

    if (!v.name.empty())
    {
        j["name"] = v.name;
    }

    if (!v.metadata.empty())
    {
        j["metadata"] = v.metadata;
    }

    if (v.video.has_value())
    {
        j["video"] = v.video.value();
    }
    if (v.sip.has_value())
    {
        j["sip"] = v.sip.value();
    }
    if (v.attributes.has_value())
    {
        j["attributes"] = v.attributes.value();
    }
    if (v.sha256.has_value())
    {
        j["sha256"] = v.sha256.value();
    }
    if (v.room_preset.has_value())
    {
        j["roomPreset"] = v.room_preset.value();
    }
    if (v.room_config.has_value())
    {
        std::string room_config_json;
        if (google::protobuf::util::MessageToJsonString(v.room_config.value(), &room_config_json).ok())
        {
            // nlohmann::json room_config_json_obj = nlohmann::json::parse(room_config_json);
            j["roomConfig"] = room_config_json;
        }
    }
}

template <>
auto from_json<Claims>(nlohmann::json const & j) -> std::expected<Claims, std::error_code>
{
    Claims v;
    auto it = j.find("exp");
    if (it != j.end())
    {
        if (it.value().is_number_unsigned())
        {
            v.expires_at = it.value().get<std::size_t>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("iss");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.issuer = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("nbf");
    if (it != j.end())
    {
        if (it.value().is_number_unsigned())
        {
            v.not_before = it.value().get<std::size_t>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("sub");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.identity_subject = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("name");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.name = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("metadata");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.metadata = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("video");
    if (it != j.end())
    {
        if (it.value().is_object())
        {
            auto r = from_json<VideoGrants>(it.value());
            if (r.has_value())
            {
                v.video = r.value();
            }
            else
            {
                return std::unexpected(r.error());
            }
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("sip");
    if (it != j.end())
    {
        if (it.value().is_object())
        {
            auto r = from_json<SIPGrants>(it.value());
            if (r.has_value())
            {
                v.sip = r.value();
            }
            else
            {
                return std::unexpected(r.error());
            }
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("attributes");
    if (it != j.end())
    {
        if (it.value().is_object())
        {
            v.attributes = it.value().get<std::map<std::string, std::string>>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("sha256");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.sha256 = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("roomPreset");
    if (it != j.end())
    {
        if (it.value().is_string())
        {
            v.room_preset = it.value().get<std::string>();
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }
    it = j.find("roomConfig");
    if (it != j.end())
    {
        livekit::RoomConfiguration room_config;
        if (google::protobuf::util::JsonStringToMessage(it.value().get<std::string>(), &room_config).ok())
        {
            v.room_config = room_config;
        }
        else
        {
            return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
        }
    }

    return v;
}

auto from_json(nlohmann::json const & j, Claims & v) -> void
{
    auto r = from_json<Claims>(j);
    if (r.has_value())
    {
        v = r.value();
    }
    else
    {
        throw_error(r.error());
    }
}

// AccessToken implementation
AccessToken::AccessToken(std::optional<std::string> api_key, std::optional<std::string> api_secret)
{
    // Match Python implementation exactly: api_key = api_key or os.getenv("LIVEKIT_API_KEY")
    if (api_key.has_value() && !api_key->empty())
    {
        api_key_ = api_key.value();
    }
    else
    {
        char const * env_key = std::getenv("LIVEKIT_API_KEY");
        if (env_key != nullptr)
        {
            api_key_ = env_key;
        }
    }

    // Match Python implementation exactly: api_secret = api_secret or os.getenv("LIVEKIT_API_SECRET")
    if (api_secret.has_value() && !api_secret->empty())
    {
        api_secret_ = api_secret.value();
    }
    else
    {
        char const * env_secret = std::getenv("LIVEKIT_API_SECRET");
        if (env_secret != nullptr)
        {
            api_secret_ = env_secret;
        }
    }

    if (api_key_.empty() || api_secret_.empty())
    {
        throw_error(ErrorCode::AccessTokenInvalidKeys);
    }

    claims_.issuer = api_key_;
}

auto AccessToken::with_ttl(std::chrono::seconds ttl) -> AccessToken &
{
    auto now = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    auto exp_seconds = now_seconds + ttl.count();
    claims_.expires_at = static_cast<std::size_t>(exp_seconds);

    return *this;
}

auto AccessToken::with_grants(VideoGrants const & grants) -> AccessToken &
{
    claims_.video = grants;
    return *this;
}

auto AccessToken::with_sip_grants(SIPGrants const & grants) -> AccessToken &
{
    claims_.sip = grants;
    return *this;
}

auto AccessToken::with_identity(std::string const & identity) -> AccessToken &
{
    claims_.identity_subject = identity;
    return *this;
}

auto AccessToken::with_name(std::string const & name) -> AccessToken &
{
    claims_.name = name;
    return *this;
}

auto AccessToken::with_metadata(std::string const & metadata) -> AccessToken &
{
    claims_.metadata = metadata;
    return *this;
}

auto AccessToken::with_attributes(std::map<std::string, std::string> const & attributes) -> AccessToken &
{
    claims_.attributes = attributes;
    return *this;
}

auto AccessToken::with_sha256(std::string const & sha256) -> AccessToken &
{
    claims_.sha256 = sha256;
    return *this;
}

auto AccessToken::with_room_preset(std::string const & preset) -> AccessToken &
{
    claims_.room_preset = preset;
    return *this;
}

auto AccessToken::with_room_config(livekit::RoomConfiguration const & config) -> AccessToken &
{
    claims_.room_config = config;
    return *this;
}

auto AccessToken::to_jwt() const -> std::expected<std::string, std::error_code>
{
    if (api_key_.empty() || api_secret_.empty())
    {
        return std::unexpected(ErrorCode::AccessTokenInvalidKeys);
    }

    if (claims_.video && claims_.video->room_join && (claims_.identity_subject.empty() || claims_.video->room.empty()))
    {
        return std::unexpected(ErrorCode::AccessTokenInvalidClaims);
    }

    nlohmann::json j = claims_;
    auto token = jwt::create<jwt::traits::nlohmann_json>();

    for (auto const & [key, value] : j.items())
    {
        token.set_payload_claim(key, value);
    }

    // Sign the token with HS256 algorithm using the API secret
    return token.sign(jwt::algorithm::hs256{ api_secret_ });
}

// TokenVerifier implementation
TokenVerifier::TokenVerifier(std::optional<std::string> api_key, std::optional<std::string> api_secret, std::chrono::seconds leeway) : leeway_{ leeway }
{
    if (api_key.has_value() && !api_key->empty())
    {
        api_key_ = api_key.value();
    }
    else
    {
        char const * env_key = std::getenv("LIVEKIT_API_KEY");
        if (env_key != nullptr)
        {
            api_key_ = env_key;
        }
    }

    if (api_secret.has_value() && !api_secret->empty())
    {
        api_secret_ = api_secret.value();
    }
    else
    {
        char const * env_secret = std::getenv("LIVEKIT_API_SECRET");
        if (env_secret != nullptr)
        {
            api_secret_ = env_secret;
        }
    }

    if (api_key_.empty() || api_secret_.empty())
    {
        throw_error(ErrorCode::AccessTokenInvalidKeys);
    }
}

auto TokenVerifier::verify(std::string const & token) const -> std::expected<Claims, std::error_code>
{
    std::error_code ec;
    // Decode and verify the JWT token using jwt-cpp with nlohmann_json traits
    auto decoded = jwt::decode<jwt::traits::nlohmann_json>(token);

    // Verify the token signature and issuer
    jwt::verify<jwt::traits::nlohmann_json>().allow_algorithm(jwt::algorithm::hs256{ api_secret_ }).with_issuer(api_key_).leeway(leeway_.count()).verify(decoded, ec);
    if (ec)
    {
        return std::unexpected(ec);
    }

    return from_json<Claims>(nlohmann::json::parse(decoded.get_payload()));
}

} // namespace livekit::api
