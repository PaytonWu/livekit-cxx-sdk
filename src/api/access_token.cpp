// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/api/access_token.h>
#include <livekit/api/error.h>

#include <livekit/error.h>
#include <livekit/ffi/proto/livekit_room.pb.h>

#include <google/protobuf/util/json_util.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

#include <any>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <typeindex>
#include <typeinfo>
#include <variant>

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
        // j["video"] = to_json<::livekit::api::VideoGrants>(v.video.value());
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
            nlohmann::json room_config_json_obj = nlohmann::json::parse(room_config_json);
            j["roomConfig"] = room_config_json_obj;
        }
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

auto AccessToken::with_ttl(std::chrono::duration<int64_t> ttl) -> AccessToken &
{
    auto now = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    auto exp_seconds = now_seconds + std::chrono::duration_cast<std::chrono::seconds>(ttl).count();
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
TokenVerifier::TokenVerifier(std::optional<std::string> api_key, std::optional<std::string> api_secret, std::chrono::duration<int64_t> leeway)
{
    // Match Python implementation exactly: api_key = api_key or os.getenv("LIVEKIT_API_KEY")
    if (api_key.has_value() && !api_key->empty())
    {
        api_key_ = api_key.value();
    }
    else
    {
        char const * env_key = std::getenv("LIVEKIT_API_KEY");
        api_key_ = env_key ? env_key : "";
    }

    // Match Python implementation exactly: api_secret = api_secret or os.getenv("LIVEKIT_API_SECRET")
    if (api_secret.has_value() && !api_secret->empty())
    {
        api_secret_ = api_secret.value();
    }
    else
    {
        char const * env_secret = std::getenv("LIVEKIT_API_SECRET");
        api_secret_ = env_secret ? env_secret : "";
    }

    leeway_ = leeway;

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
    jwt::verify<jwt::traits::nlohmann_json>().allow_algorithm(jwt::algorithm::hs256{ api_secret_ }).with_issuer(api_key_).verify(decoded, ec);
    if (ec)
    {
        return std::unexpected(ec);
    }

    Claims claims;

    claims.identity_subject = decoded.get_subject();

    if (decoded.has_payload_claim("name"))
    {
        claims.name = decoded.get_payload_claim("name").to_json().get<std::string>();
    }
    if (decoded.has_payload_claim("metadata"))
    {
        claims.metadata = decoded.get_payload_claim("metadata").to_json().get<std::string>();
    }

    if (decoded.has_payload_claim("sha256"))
    {
        claims.sha256 = decoded.get_payload_claim("sha256").to_json().get<std::string>();
    }

    if (decoded.has_payload_claim("roomPreset"))
    {
        claims.room_preset = decoded.get_payload_claim("roomPreset").to_json().get<std::string>();
    }

    // Extract video grants
    if (decoded.has_payload_claim("video"))
    {
        auto video_claim = decoded.get_payload_claim("video");
        auto video_json = video_claim.to_json();
        if (video_json.is_object())
        {
            auto video_obj = video_json.get<nlohmann::json::object_t>();
            VideoGrants video_grants;

            // Convert camelCase keys to snake_case and extract values
            if (video_obj.find("roomCreate") != video_obj.end())
            {
                video_grants.room_create = video_obj["roomCreate"].get<bool>();
            }
            if (video_obj.find("roomList") != video_obj.end())
            {
                video_grants.room_list = video_obj["roomList"].get<bool>();
            }
            if (video_obj.find("roomRecord") != video_obj.end())
            {
                video_grants.room_record = video_obj["roomRecord"].get<bool>();
            }
            if (video_obj.find("roomAdmin") != video_obj.end())
            {
                video_grants.room_admin = video_obj["roomAdmin"].get<bool>();
            }
            if (video_obj.find("roomJoin") != video_obj.end())
            {
                video_grants.room_join = video_obj["roomJoin"].get<bool>();
            }
            if (video_obj.find("room") != video_obj.end())
            {
                video_grants.room = video_obj["room"].get<std::string>();
            }
            if (video_obj.find("destinationRoom") != video_obj.end())
            {
                video_grants.destination_room = video_obj["destinationRoom"].get<std::string>();
            }
            if (video_obj.find("canPublish") != video_obj.end())
            {
                video_grants.can_publish = video_obj["canPublish"].get<bool>();
            }
            if (video_obj.find("canSubscribe") != video_obj.end())
            {
                video_grants.can_subscribe = video_obj["canSubscribe"].get<bool>();
            }
            if (video_obj.find("canPublishData") != video_obj.end())
            {
                video_grants.can_publish_data = video_obj["canPublishData"].get<bool>();
            }
            if (video_obj.find("canPublishSources") != video_obj.end())
            {
                auto sources_array = video_obj["canPublishSources"].get<nlohmann::json::array_t>();
                std::vector<std::string> sources;
                for (auto const & source : sources_array)
                {
                    sources.push_back(source.get<std::string>());
                }
                video_grants.can_publish_sources = sources;
            }
            if (video_obj.find("canUpdateOwnMetadata") != video_obj.end())
            {
                video_grants.can_update_own_metadata = video_obj["canUpdateOwnMetadata"].get<bool>();
            }
            if (video_obj.find("ingressAdmin") != video_obj.end())
            {
                video_grants.ingress_admin = video_obj["ingressAdmin"].get<bool>();
            }
            if (video_obj.find("hidden") != video_obj.end())
            {
                video_grants.hidden = video_obj["hidden"].get<bool>();
            }
            if (video_obj.find("recorder") != video_obj.end())
            {
                video_grants.recorder = video_obj["recorder"].get<bool>();
            }
            if (video_obj.find("agent") != video_obj.end())
            {
                // video_grants.agent = video_obj["agent"].get<bool>();
            }

            claims.video = video_grants;
        }
    }

    // Extract SIP grants
    if (decoded.has_payload_claim("sip"))
    {
        auto sip_claim = decoded.get_payload_claim("sip");
        auto sip_json = sip_claim.to_json();
        if (sip_json.is_object())
        {
            auto sip_obj = sip_json.get<nlohmann::json::object_t>();
            SIPGrants sip_grants;

            if (sip_obj.find("admin") != sip_obj.end())
            {
                sip_grants.admin = sip_obj["admin"].get<bool>();
            }
            if (sip_obj.find("call") != sip_obj.end())
            {
                sip_grants.call = sip_obj["call"].get<bool>();
            }

            claims.sip = sip_grants;
        }
    }

    // Extract attributes
    if (decoded.has_payload_claim("attributes"))
    {
        auto attr_claim = decoded.get_payload_claim("attributes");
        auto attr_json = attr_claim.to_json();
        if (attr_json.is_object())
        {
            auto attr_obj = attr_json.get<nlohmann::json::object_t>();
            std::map<std::string, std::string> attributes;

            for (auto const & [key, value] : attr_obj)
            {
                if (value.is_string())
                {
                    attributes[key] = value.get<std::string>();
                }
            }

            claims.attributes = attributes;
        }
    }

    return claims;
}

} // namespace livekit::api
