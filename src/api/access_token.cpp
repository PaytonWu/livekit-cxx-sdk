// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)


#include <livekit/api/access_token.h>
#include <livekit/ffi/proto/livekit_room.pb.h>

#include <jwt-cpp/jwt.h>

#include <cstdlib>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace livekit::api
{

// Utility functions
std::string camel_to_snake(std::string const & input)
{
    std::string result = input;
    std::regex camel_case_regex("(?<!^)(?=[A-Z])");
    result = std::regex_replace(result, camel_case_regex, "_");

    // Convert to lowercase
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string snake_to_lower_camel(std::string const & input)
{
    std::string result;
    std::istringstream iss(input);
    std::string word;
    bool first = true;

    while (std::getline(iss, word, '_'))
    {
        if (first)
        {
            result += word;
            first = false;
        }
        else
        {
            if (!word.empty())
            {
                word[0] = std::toupper(word[0]);
                result += word;
            }
        }
    }
    return result;
}

// Claims implementation
std::map<std::string, std::string> Claims::as_dict() const
{
    std::map<std::string, std::string> claims;

    if (!identity.empty())
    {
        claims["sub"] = identity;
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
    if (!sha256.value_or("").empty())
    {
        claims["sha256"] = sha256.value();
    }
    if (!room_preset.value_or("").empty())
    {
        claims["roomPreset"] = room_preset.value();
    }

    // Handle video grants
    if (video)
    {
        std::map<std::string, std::string> video_claims;
        if (video->room_create.has_value())
        {
            video_claims["roomCreate"] = video->room_create.value() ? "true" : "false";
        }
        if (video->room_list.has_value())
        {
            video_claims["roomList"] = video->room_list.value() ? "true" : "false";
        }
        if (video->room_record.has_value())
        {
            video_claims["roomRecord"] = video->room_record.value() ? "true" : "false";
        }
        if (video->room_admin.has_value())
        {
            video_claims["roomAdmin"] = video->room_admin.value() ? "true" : "false";
        }
        if (video->room_join.has_value())
        {
            video_claims["roomJoin"] = video->room_join.value() ? "true" : "false";
        }
        if (!video->room.empty())
        {
            video_claims["room"] = video->room;
        }
        if (video->destination_room.has_value())
        {
            video_claims["destinationRoom"] = video->destination_room.value();
        }
        if (video->can_publish_sources.has_value())
        {
            // Convert vector to JSON-like string for now
            std::string sources;
            for (auto const & source : video->can_publish_sources.value())
            {
                if (!sources.empty())
                {
                    sources += ",";
                }
                sources += source;
            }
            video_claims["canPublishSources"] = sources;
        }
        if (video->can_update_own_metadata.has_value())
        {
            video_claims["canUpdateOwnMetadata"] = video->can_update_own_metadata.value() ? "true" : "false";
        }
        if (video->ingress_admin.has_value())
        {
            video_claims["ingressAdmin"] = video->ingress_admin.value() ? "true" : "false";
        }
        if (video->hidden.has_value())
        {
            video_claims["hidden"] = video->hidden.value() ? "true" : "false";
        }
        if (video->recorder.has_value())
        {
            video_claims["recorder"] = video->recorder.value() ? "true" : "false";
        }
        if (video->agent.has_value())
        {
            video_claims["agent"] = video->agent.value() ? "true" : "false";
        }

        // Convert to JSON-like string for now
        std::string video_str;
        for (auto const & [key, value] : video_claims)
        {
            if (!video_str.empty())
            {
                video_str += ",";
            }
            video_str += key + ":" + value;
        }
        claims["video"] = video_str;
    }

    // Handle SIP grants
    if (sip)
    {
        std::map<std::string, std::string> sip_claims;
        sip_claims["admin"] = sip->admin ? "true" : "false";
        sip_claims["call"] = sip->call ? "true" : "false";

        // Convert to JSON-like string for now
        std::string sip_str;
        for (auto const & [key, value] : sip_claims)
        {
            if (!sip_str.empty())
            {
                sip_str += ",";
            }
            sip_str += key + ":" + value;
        }
        claims["sip"] = sip_str;
    }

    // Handle attributes
    if (attributes)
    {
        std::string attr_str;
        for (auto const & [key, value] : attributes.value())
        {
            if (!attr_str.empty())
            {
                attr_str += ",";
            }
            attr_str += key + ":" + value;
        }
        claims["attributes"] = attr_str;
    }

    return claims;
}

// AccessToken implementation
AccessToken::AccessToken(std::optional<std::string> api_key, std::optional<std::string> api_secret)
{
    api_key_ = api_key.value_or(std::getenv("LIVEKIT_API_KEY") ? std::getenv("LIVEKIT_API_KEY") : "");
    api_secret_ = api_secret.value_or(std::getenv("LIVEKIT_API_SECRET") ? std::getenv("LIVEKIT_API_SECRET") : "");

    if (api_key_.empty() || api_secret_.empty())
    {
        throw std::invalid_argument("api_key and api_secret must be set");
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

auto AccessToken::with_room_config(livekit::RoomConfiguration const * config) -> AccessToken &
{
    claims_.room_config = config;
    return *this;
}

std::string AccessToken::to_jwt() const
{
    // Validate required fields
    if (claims_.video && claims_.video->room_join && (identity_.empty() || claims_.video->room.empty()))
    {
        throw std::invalid_argument("identity and room must be set when joining a room");
    }

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    auto exp_seconds = now_seconds + std::chrono::duration_cast<std::chrono::seconds>(ttl_).count();

    // Create JWT token using jwt-cpp
    auto token = jwt::create().set_issuer(api_key_).set_subject(identity_).set_not_before(now).set_expires_at(std::chrono::system_clock::from_time_t(exp_seconds));

    // Add claims from the claims_ object
    if (!claims_.name.empty())
    {
        token.set_payload_claim("name", jwt::claim(claims_.name));
    }
    if (!claims_.kind.empty())
    {
        token.set_payload_claim("kind", jwt::claim(claims_.kind));
    }
    if (!claims_.metadata.empty())
    {
        token.set_payload_claim("metadata", jwt::claim(claims_.metadata));
    }
    if (claims_.sha256.has_value() && !claims_.sha256->empty())
    {
        token.set_payload_claim("sha256", jwt::claim(claims_.sha256.value()));
    }
    if (claims_.room_preset.has_value() && !claims_.room_preset->empty())
    {
        token.set_payload_claim("roomPreset", jwt::claim(claims_.room_preset.value()));
    }

    // Add video grants if present
    if (claims_.video.has_value())
    {
        auto const & video = claims_.video.value();
        picojson::object video_obj;

        if (video.room_create.has_value())
        {
            bool val = video.room_create.value();
            video_obj["roomCreate"] = picojson::value(val);
        }
        if (video.room_list.has_value())
        {
            bool val = video.room_list.value();
            video_obj["roomList"] = picojson::value(val);
        }
        if (video.room_record.has_value())
        {
            bool val = video.room_record.value();
            video_obj["roomRecord"] = picojson::value(val);
        }
        if (video.room_admin.has_value())
        {
            bool val = video.room_admin.value();
            video_obj["roomAdmin"] = picojson::value(val);
        }
        if (video.room_join.has_value())
        {
            bool val = video.room_join.value();
            video_obj["roomJoin"] = picojson::value(val);
        }
        if (!video.room.empty())
        {
            video_obj["room"] = picojson::value(video.room);
        }
        video_obj["canPublish"] = picojson::value(video.can_publish);
        video_obj["canSubscribe"] = picojson::value(video.can_subscribe);
        video_obj["canPublishData"] = picojson::value(video.can_publish_data);
        if (video.can_update_own_metadata.has_value())
        {
            bool val = video.can_update_own_metadata.value();
            video_obj["canUpdateOwnMetadata"] = picojson::value(val);
        }
        if (video.ingress_admin.has_value())
        {
            bool val = video.ingress_admin.value();
            video_obj["ingressAdmin"] = picojson::value(val);
        }
        if (video.hidden.has_value())
        {
            bool val = video.hidden.value();
            video_obj["hidden"] = picojson::value(val);
        }
        if (video.recorder.has_value())
        {
            bool val = video.recorder.value();
            video_obj["recorder"] = picojson::value(val);
        }
        if (video.agent.has_value())
        {
            bool val = video.agent.value();
            video_obj["agent"] = picojson::value(val);
        }

        // Handle can_publish_sources array
        if (video.can_publish_sources.has_value() && !video.can_publish_sources->empty())
        {
            picojson::array sources_array;
            for (auto const & source : video.can_publish_sources.value())
            {
                sources_array.push_back(picojson::value(source));
            }
            video_obj["canPublishSources"] = picojson::value(sources_array);
        }

        token.set_payload_claim("video", jwt::claim(picojson::value(video_obj)));
    }

    // Add SIP grants if present
    if (claims_.sip.has_value())
    {
        auto const & sip = claims_.sip.value();
        picojson::object sip_obj;
        sip_obj["admin"] = picojson::value(sip.admin);
        sip_obj["call"] = picojson::value(sip.call);
        token.set_payload_claim("sip", jwt::claim(picojson::value(sip_obj)));
    }

    // Add attributes if present
    if (claims_.attributes.has_value() && !claims_.attributes->empty())
    {
        picojson::object attr_obj;
        for (auto const & [key, value] : claims_.attributes.value())
        {
            attr_obj[key] = picojson::value(value);
        }
        token.set_payload_claim("attributes", jwt::claim(picojson::value(attr_obj)));
    }

    // Add room configuration if present
    if (claims_.room_config.has_value())
    {
        // For now, we'll skip room_config as it requires protobuf serialization
        // This can be implemented later if needed
    }

    // Sign the token with HS256 algorithm using the API secret
    return token.sign(jwt::algorithm::hs256{ api_secret_ });
}

// TokenVerifier implementation
TokenVerifier::TokenVerifier(std::optional<std::string> api_key, std::optional<std::string> api_secret, std::chrono::duration<int64_t> leeway)
{
    api_key_ = api_key.value_or(std::getenv("LIVEKIT_API_KEY") ? std::getenv("LIVEKIT_API_KEY") : "");
    api_secret_ = api_secret.value_or(std::getenv("LIVEKIT_API_SECRET") ? std::getenv("LIVEKIT_API_SECRET") : "");
    leeway_ = leeway;

    if (api_key_.empty() || api_secret_.empty())
    {
        throw std::invalid_argument("api_key and api_secret must be set");
    }
}

Claims TokenVerifier::verify(std::string const & token) const
{
    // For now, return a placeholder implementation
    // In a real implementation, you would decode and verify the JWT token
    Claims claims;

    // Parse the token (simplified - in reality you'd use a JWT library)
    if (token.find("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.") == 0)
    {
        // Extract payload part
        size_t first_dot = token.find('.');
        size_t second_dot = token.find('.', first_dot + 1);

        if (first_dot != std::string::npos && second_dot != std::string::npos)
        {
            std::string payload_str = token.substr(first_dot + 1, second_dot - first_dot - 1);

            // Parse payload (simplified)
            std::istringstream iss(payload_str);
            std::string item;
            while (std::getline(iss, item, ','))
            {
                size_t colon_pos = item.find(':');
                if (colon_pos != std::string::npos)
                {
                    std::string key = item.substr(0, colon_pos);
                    std::string value = item.substr(colon_pos + 1);

                    if (key == "sub")
                    {
                        claims.identity = value;
                    }
                    else if (key == "name")
                    {
                        claims.name = value;
                    }
                    else if (key == "kind")
                    {
                        claims.kind = value;
                    }
                    else if (key == "metadata")
                    {
                        claims.metadata = value;
                    }
                    else if (key == "sha256")
                    {
                        claims.sha256 = value;
                    }
                    else if (key == "roomPreset")
                    {
                        claims.room_preset = value;
                    }
                }
            }
        }
    }

    return claims;
}

} // namespace livekit::api
