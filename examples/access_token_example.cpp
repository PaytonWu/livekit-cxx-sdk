// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/api/access_token.h>

#include <iostream>

auto main() -> int
{
    try
    {
        // Set environment variables for testing
        setenv("LIVEKIT_API_KEY", "test_api_key", 1);
        setenv("LIVEKIT_API_SECRET", "test_api_secret", 1);

        // Create an access token
        livekit::api::AccessToken token{};

        // Configure video grants
        livekit::api::VideoGrants video_grants;
        video_grants.room_join = true;
        video_grants.room = "test_room";
        video_grants.can_publish = true;
        video_grants.can_subscribe = true;

        // Configure the token
        token.with_identity("test_user").with_name("Test User").with_kind("standard").with_grants(video_grants).with_ttl(std::chrono::hours(1));

        // Generate JWT
        std::string jwt = token.to_jwt();
        std::cout << "Generated JWT: " << jwt << std::endl;

        // Verify the token
        livekit::api::TokenVerifier verifier;
        livekit::api::Claims claims = verifier.verify(jwt);

        std::cout << "Verified claims:" << std::endl;
        std::cout << "  Identity: " << claims.identity << std::endl;
        std::cout << "  Name: " << claims.name << std::endl;
        std::cout << "  Kind: " << claims.kind << std::endl;

        std::cout << "Access token example completed successfully!" << std::endl;
    }
    catch (std::exception const & e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
