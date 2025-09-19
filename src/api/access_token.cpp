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

template <>
auto to_dict<VideoGrants>(VideoGrants const & value) -> std::map<std::string, std::any>
{
    std::map<std::string, std::any> result;
    if (value.room_create.has_value())
    {
        result["roomCreate"] = value.room_create.value();
    }
    if (value.room_list.has_value())
    {
        result["roomList"] = value.room_list.value();
    }
    if (value.room_record.has_value())
    {
        result["roomRecord"] = value.room_record.value();
    }
    if (value.room_admin.has_value())
    {
        result["roomAdmin"] = value.room_admin.value();
    }
    if (value.room_join.has_value())
    {
        result["roomJoin"] = value.room_join.value();
    }
    if (!value.room.empty())
    {
        result["room"] = value.room;
    }
    if (value.destination_room.has_value() && !value.destination_room->empty())
    {
        result["destinationRoom"] = value.destination_room.value();
    }
    result["canPublish"] = value.can_publish;
    result["canSubscribe"] = value.can_subscribe;
    result["canPublishData"] = value.can_publish_data;
    if (value.can_publish_sources.has_value() && !value.can_publish_sources->empty())
    {
        result["canPublishSources"] = value.can_publish_sources.value();
    }
    if (value.can_update_own_metadata.has_value())
    {
        result["canUpdateOwnMetadata"] = value.can_update_own_metadata.value();
    }
    if (value.ingress_admin.has_value())
    {
        result["ingressAdmin"] = value.ingress_admin.value();
    }
    if (value.hidden.has_value())
    {
        result["hidden"] = value.hidden.value();
    }
    if (value.recorder.has_value())
    {
        result["recorder"] = value.recorder.value();
    }
    if (value.agent.has_value())
    {
        result["agent"] = value.agent.value();
    }

    return result;
}

template <>
auto to_dict<SIPGrants>(SIPGrants const & value) -> std::map<std::string, std::any>
{
    std::map<std::string, std::any> result;
    result["admin"] = value.admin;
    result["call"] = value.call;
    return result;
}

// Claims implementation
auto Claims::as_dict() const -> std::map<std::string, std::any>
{
    std::map<std::string, std::any> claims;

    // Add claims only if they have values (matching Python behavior)
    if (!identity.empty())
    {
        claims["identity"] = identity;
    }
    if (!name.empty())
    {
        claims["name"] = name;
    }
    if (!kind.empty())
    {
        claims["kind"] = kind;
    }
    if (!metadata.empty())
    {
        claims["metadata"] = metadata;
    }
    if (sha256.has_value() && !sha256->empty())
    {
        claims["sha256"] = sha256.value();
    }
    if (room_preset.has_value() && !room_preset->empty())
    {
        claims["roomPreset"] = room_preset.value();
    }

    // Handle video grants
    if (video.has_value())
    {
        claims["video"] = to_dict(video.value());
    }

    // Handle SIP grants
    if (sip.has_value())
    {
        claims["sip"] = to_dict(sip.value());
    }

    // Handle attributes
    if (attributes.has_value() && !attributes->empty())
    {
        claims["attributes"] = attributes.value();
    }

    // Handle room config
    if (room_config.has_value())
    {
        std::string json_string;
        auto status = google::protobuf::util::MessageToJsonString(room_config.value(), &json_string);
        if (status.ok())
        {
            claims["roomConfig"] = json_string;
        }
    }

    return claims;
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
        throw_error(ErrorCode::InvalidApiKeyOrSecret);
    }
}

auto AccessToken::with_ttl(std::chrono::duration<int64_t> ttl) -> AccessToken &
{
    ttl_ = ttl;
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
    identity_ = identity;
    claims_.identity = identity;
    return *this;
}

auto AccessToken::with_kind(ParticipantKind const & kind) -> AccessToken &
{
    claims_.kind = kind;
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

std::string AccessToken::to_jwt() const
{
    // Validate required fields (matching Python implementation)
    if (claims_.video && claims_.video->room_join && (identity_.empty() || claims_.video->room.empty()))
    {
        throw std::invalid_argument("identity and room must be set when joining a room");
    }

    // Get current time in UTC (matching Python's calendar.timegm)
    auto now = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    auto exp_seconds = now_seconds + std::chrono::duration_cast<std::chrono::seconds>(ttl_).count();

    // Create JWT token using jwt-cpp with nlohmann_json traits
    auto token = jwt::create<jwt::traits::nlohmann_json>().set_issuer(api_key_).set_subject(identity_).set_not_before(now).set_expires_at(
        std::chrono::system_clock::from_time_t(exp_seconds));

    // Get claims from the claims_ object using as_dict() (matching Python implementation)
    auto jwt_claims = claims_.as_dict();

    // Convert claims map to nlohmann::json for JWT encoding
    nlohmann::json payload;
    for (auto const & [key, value] : jwt_claims)
    {
        try
        {
            if (value.type() == typeid(std::string))
            {
                payload[key] = std::any_cast<std::string>(value);
            }
            else if (value.type() == typeid(bool))
            {
                payload[key] = std::any_cast<bool>(value);
            }
            else if (value.type() == typeid(std::int64_t))
            {
                payload[key] = std::any_cast<std::int64_t>(value);
            }
            else if (value.type() == typeid(double))
            {
                payload[key] = std::any_cast<double>(value);
            }
            else if (value.type() == typeid(std::vector<std::string>))
            {
                auto const & vec = std::any_cast<std::vector<std::string> const &>(value);
                payload[key] = vec;
            }
            else if (value.type() == typeid(std::map<std::string, std::string>))
            {
                auto const & map = std::any_cast<std::map<std::string, std::string> const &>(value);
                payload[key] = map;
            }
            else if (value.type() == typeid(std::map<std::string, std::any>))
            {
                // Handle nested objects like video and sip grants using to_dict templates
                auto const & nested_map = std::any_cast<std::map<std::string, std::any> const &>(value);
                nlohmann::json nested_json = nlohmann::json::object();

                for (auto const & [k, v] : nested_map)
                {
                    try
                    {
                        if (v.type() == typeid(std::string))
                        {
                            nested_json[k] = std::any_cast<std::string>(v);
                        }
                        else if (v.type() == typeid(bool))
                        {
                            nested_json[k] = std::any_cast<bool>(v);
                        }
                        else if (v.type() == typeid(std::int64_t))
                        {
                            nested_json[k] = std::any_cast<std::int64_t>(v);
                        }
                        else if (v.type() == typeid(double))
                        {
                            nested_json[k] = std::any_cast<double>(v);
                        }
                        else if (v.type() == typeid(std::vector<std::string>))
                        {
                            auto const & vec = std::any_cast<std::vector<std::string> const &>(v);
                            nested_json[k] = vec;
                        }
                    }
                    catch (...)
                    {
                        // Skip invalid values
                        continue;
                    }
                }

                payload[key] = nested_json;
            }
        }
        catch (...)
        {
            // Skip invalid values
            continue;
        }
    }

    // Add each claim individually
    for (auto const & [key, value] : payload.items())
    {
        token.set_payload_claim(key, jwt::basic_claim<jwt::traits::nlohmann_json>(value));
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
        throw_error(ErrorCode::InvalidApiKeyOrSecret);
    }
}

auto TokenVerifier::verify(std::string const & token) const -> Claims
{
    try
    {
        // Decode and verify the JWT token using jwt-cpp with nlohmann_json traits
        auto decoded = jwt::decode<jwt::traits::nlohmann_json>(token);

        // Verify the token signature and issuer
        jwt::verify<jwt::traits::nlohmann_json>().allow_algorithm(jwt::algorithm::hs256{ api_secret_ }).with_issuer(api_key_).verify(decoded);

        Claims claims;

        // Extract claims from the decoded token (matching Python implementation)
        claims.identity = decoded.get_subject();

        if (decoded.has_payload_claim("name"))
        {
            claims.name = decoded.get_payload_claim("name").to_json().get<std::string>();
        }
        if (decoded.has_payload_claim("kind"))
        {
            claims.kind = decoded.get_payload_claim("kind").to_json().get<std::string>();
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
                    video_grants.agent = video_obj["agent"].get<bool>();
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
    catch (std::exception const & e)
    {
        throw std::runtime_error("Failed to verify JWT token: " + std::string(e.what()));
    }
}

} // namespace livekit::api
