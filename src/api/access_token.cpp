// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/api/access_token.h>
#include <livekit/ffi/proto/livekit_room.pb.h>

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

    // For now, return a placeholder JWT implementation
    // In a real implementation, you would use a JWT library like jwt-cpp or similar
    std::stringstream jwt;
    jwt << "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.";

    // Create a simple payload (this is a simplified version)
    std::map<std::string, std::string> payload;
    payload["iss"] = api_key_;
    payload["sub"] = identity_;
    payload["exp"] = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch() + ttl_).count());

    // Add claims
    auto claims_dict = claims_.as_dict();
    for (auto const & [key, value] : claims_dict)
    {
        payload[key] = value;
    }

    // Convert payload to a simple string representation
    std::string payload_str;
    for (auto const & [key, value] : payload)
    {
        if (!payload_str.empty())
        {
            payload_str += ",";
        }
        payload_str += key + ":" + value;
    }

    jwt << payload_str << ".";
    jwt << "signature_placeholder"; // In real implementation, this would be the actual signature

    return jwt.str();
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
