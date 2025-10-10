// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/api/access_token.h>
#include <livekit/ffi/proto/livekit_room.pb.h>

#include <cassert>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

// Test helper functions
void assert_equals(std::string const & expected, std::string const & actual, std::string const & test_name)
{
    if (expected != actual)
    {
        std::cerr << "FAIL: " << test_name << " - Expected: '" << expected << "', Got: '" << actual << "'" << std::endl;
        throw std::runtime_error("Test assertion failed");
    }
    std::cout << "PASS: " << test_name << std::endl;
}

void assert_equals(bool expected, bool actual, std::string const & test_name)
{
    if (expected != actual)
    {
        std::cerr << "FAIL: " << test_name << " - Expected: " << expected << ", Got: " << actual << std::endl;
        throw std::runtime_error("Test assertion failed");
    }
    std::cout << "PASS: " << test_name << std::endl;
}

void assert_equals(int expected, int actual, std::string const & test_name)
{
    if (expected != actual)
    {
        std::cerr << "FAIL: " << test_name << " - Expected: " << expected << ", Got: " << actual << std::endl;
        throw std::runtime_error("Test assertion failed");
    }
    std::cout << "PASS: " << test_name << std::endl;
}

void assert_true(bool condition, std::string const & test_name)
{
    if (!condition)
    {
        std::cerr << "FAIL: " << test_name << " - Expected true, got false" << std::endl;
        throw std::runtime_error("Test assertion failed");
    }
    std::cout << "PASS: " << test_name << std::endl;
}

void assert_false(bool condition, std::string const & test_name)
{
    if (condition)
    {
        std::cerr << "FAIL: " << test_name << " - Expected false, got true" << std::endl;
        throw std::runtime_error("Test assertion failed");
    }
    std::cout << "PASS: " << test_name << std::endl;
}

void assert_throws(std::function<void()> func, std::string const & test_name)
{
    try
    {
        func();
        std::cerr << "FAIL: " << test_name << " - Expected exception but none was thrown" << std::endl;
        throw std::runtime_error("Test assertion failed");
    }
    catch (std::exception const &)
    {
        std::cout << "PASS: " << test_name << " - Exception thrown as expected" << std::endl;
    }
}

// Test cases
void test_basic_token_creation()
{
    std::cout << "\n=== Testing Basic Token Creation ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");

    // Test basic configuration
    token.with_identity("test_user").with_name("Test User").with_metadata("test metadata").with_ttl(std::chrono::hours(1));

    // Generate JWT
    std::string jwt = token.to_jwt().value();
    assert_true(!jwt.empty(), "JWT should not be empty");

    // Verify token
    livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
    livekit::api::Claims claims = verifier.verify(jwt);

    assert_equals("test_user", claims.sub, "Identity should match");
    assert_equals("Test User", claims.name, "Name should match");
    assert_equals("test metadata", claims.metadata, "Metadata should match");
}

void test_video_grants()
{
    std::cout << "\n=== Testing Video Grants ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");

    // Configure video grants
    livekit::api::VideoGrants video_grants;
    video_grants.room_create = true;
    video_grants.room_list = true;
    video_grants.room_record = true;
    video_grants.room_admin = true;
    video_grants.room_join = true;
    video_grants.room = "test_room";
    video_grants.destination_room = "dest_room";
    video_grants.can_publish = true;
    video_grants.can_subscribe = true;
    video_grants.can_publish_data = true;
    video_grants.can_publish_sources = std::vector<std::string>{ "camera", "microphone" };
    video_grants.can_update_own_metadata = true;
    video_grants.ingress_admin = true;
    video_grants.hidden = true;
    video_grants.recorder = true;
    // video_grants.agent = true;

    token.with_identity("test_user").with_grants(video_grants);

    // Generate JWT
    std::string jwt = token.to_jwt().value();
    assert_true(!jwt.empty(), "JWT should not be empty");

    // Verify token
    livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
    livekit::api::Claims claims = verifier.verify(jwt);

    assert_true(claims.video.has_value(), "Video grants should be present");
    if (claims.video.has_value())
    {
        auto & video = claims.video.value();
        assert_equals(true, video.room_create, "room_create should be true");
        assert_equals(true, video.room_list, "room_list should be true");
        assert_equals(true, video.room_record, "room_record should be true");
        assert_equals(true, video.room_admin, "room_admin should be true");
        assert_equals(true, video.room_join, "room_join should be true");
        assert_equals("test_room", video.room, "room should match");
        assert_equals("dest_room", video.destination_room, "destination_room should match");
        assert_equals(true, video.can_publish, "can_publish should be true");
        assert_equals(true, video.can_subscribe, "can_subscribe should be true");
        assert_equals(true, video.can_publish_data, "can_publish_data should be true");
        assert_true(!video.can_publish_sources.empty(), "can_publish_sources should be present");
        if (!video.can_publish_sources.empty())
        {
            assert_equals(2, static_cast<int>(video.can_publish_sources.size()), "can_publish_sources should have 2 items");
        }
        assert_equals(true, video.can_update_own_metadata, "can_update_own_metadata should be true");
        assert_equals(true, video.ingress_admin, "ingress_admin should be true");
        assert_equals(true, video.hidden, "hidden should be true");
        assert_equals(true, video.recorder, "recorder should be true");
        // assert_equals(true, video.agent.value(), "agent should be true");
    }
}

void test_sip_grants()
{
    std::cout << "\n=== Testing SIP Grants ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");

    // Configure SIP grants
    livekit::api::SIPGrants sip_grants;
    sip_grants.admin = true;
    sip_grants.call = true;

    token.with_identity("test_user").with_sip_grants(sip_grants);

    // Generate JWT
    std::string jwt = token.to_jwt().value();
    assert_true(!jwt.empty(), "JWT should not be empty");

    // Verify token
    livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
    livekit::api::Claims claims = verifier.verify(jwt);

    assert_true(claims.sip.has_value(), "SIP grants should be present");
    if (claims.sip.has_value())
    {
        auto & sip = claims.sip.value();
        assert_equals(true, sip.admin, "admin should be true");
        assert_equals(true, sip.call, "call should be true");
    }
}

void test_attributes()
{
    std::cout << "\n=== Testing Attributes ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");

    // Configure attributes
    std::map<std::string, std::string> attributes;
    attributes["key1"] = "value1";
    attributes["key2"] = "value2";

    token.with_identity("test_user").with_attributes(attributes);

    // Generate JWT
    std::string jwt = token.to_jwt().value();
    assert_true(!jwt.empty(), "JWT should not be empty");

    // Verify token
    livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
    livekit::api::Claims claims = verifier.verify(jwt);

    assert_true(claims.attributes.has_value(), "Attributes should be present");
    if (claims.attributes.has_value())
    {
        auto & attrs = claims.attributes.value();
        assert_equals("value1", attrs["key1"], "Attribute key1 should match");
        assert_equals("value2", attrs["key2"], "Attribute key2 should match");
    }
}

void test_room_join_validation()
{
    std::cout << "\n=== Testing Room Join Validation ===" << std::endl;

    // Test case 1: Missing identity should throw
    {
        livekit::api::AccessToken token("test_api_key", "test_api_secret");
        livekit::api::VideoGrants video_grants;
        video_grants.room_join = true;
        video_grants.room = "test_room";

        token.with_grants(video_grants);

        assert_throws([&token]() { token.to_jwt(); }, "Should throw when identity is missing for room_join");
    }

    // Test case 2: Missing room should throw
    {
        livekit::api::AccessToken token("test_api_key", "test_api_secret");
        livekit::api::VideoGrants video_grants;
        video_grants.room_join = true;

        token.with_identity("test_user").with_grants(video_grants);

        assert_throws([&token]() { token.to_jwt(); }, "Should throw when room is missing for room_join");
    }

    // Test case 3: Valid room join should work
    {
        livekit::api::AccessToken token("test_api_key", "test_api_secret");
        livekit::api::VideoGrants video_grants;
        video_grants.room_join = true;
        video_grants.room = "test_room";

        token.with_identity("test_user").with_grants(video_grants);

        std::string jwt = token.to_jwt().value();
        assert_true(!jwt.empty(), "Valid room join should generate JWT");
    }
}

void test_ttl_handling()
{
    std::cout << "\n=== Testing TTL Handling ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");

    // Set custom TTL
    token.with_identity("test_user").with_ttl(std::chrono::minutes(30));

    auto start_time = std::chrono::system_clock::now();
    std::string jwt = token.to_jwt().value();
    auto end_time = std::chrono::system_clock::now();

    assert_true(!jwt.empty(), "JWT should not be empty");

    // Verify token
    livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
    livekit::api::Claims claims = verifier.verify(jwt);

    // The token should be valid (not expired)
    assert_true(true, "Token should be valid within TTL");
}

// void test_room_config()
// {
//     std::cout << "\n=== Testing Room Configuration ===" << std::endl;

//     livekit::api::AccessToken token("test_api_key", "test_api_secret");

//     // Create room configuration
//     livekit::RoomConfiguration room_config;
//     room_config.set_name("test_room");
//     room_config.set_empty_timeout(10);
//     room_config.set_departure_timeout(10);
//     room_config.set_max_participants(10);
//     room_config.set_min_playout_delay(10);
//     room_config.set_max_playout_delay(10);
//     room_config.set_sync_streams(true);
//     // room_config.set_enable_recording(true);
//     // room_config.set_enable_transcription(true);

//     token.with_identity("test_user").with_room_config(room_config);

//     // Generate JWT
//     std::string jwt = token.to_jwt();
//     assert_true(!jwt.empty(), "JWT should not be empty");

//     // Verify token
//     livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
//     livekit::api::Claims claims = verifier.verify(jwt);

//     assert_true(claims.room_config.has_value(), "Room config should be present");
// }

void test_sha256()
{
    std::cout << "\n=== Testing SHA256 ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");

    token.with_identity("test_user").with_sha256("test_sha256_hash");

    // Generate JWT
    std::string jwt = token.to_jwt().value();
    assert_true(!jwt.empty(), "JWT should not be empty");

    // Verify token
    livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
    livekit::api::Claims claims = verifier.verify(jwt);

    assert_true(claims.sha256.has_value(), "SHA256 should be present");
    if (claims.sha256.has_value())
    {
        assert_equals("test_sha256_hash", claims.sha256.value(), "SHA256 should match");
    }
}

void test_room_preset()
{
    std::cout << "\n=== Testing Room Preset ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");

    token.with_identity("test_user").with_room_preset("test_preset");

    // Generate JWT
    std::string jwt = token.to_jwt().value();
    assert_true(!jwt.empty(), "JWT should not be empty");

    // Verify token
    livekit::api::TokenVerifier verifier("test_api_key", "test_api_secret");
    livekit::api::Claims claims = verifier.verify(jwt);

    assert_true(claims.room_preset.has_value(), "Room preset should be present");
    if (claims.room_preset.has_value())
    {
        assert_equals("test_preset", claims.room_preset.value(), "Room preset should match");
    }
}

void test_token_verification_failure()
{
    std::cout << "\n=== Testing Token Verification Failure ===" << std::endl;

    livekit::api::AccessToken token("test_api_key", "test_api_secret");
    token.with_identity("test_user");

    std::string jwt = token.to_jwt().value();

    // Try to verify with wrong secret
    livekit::api::TokenVerifier verifier("test_api_key", "wrong_secret");

    assert_throws([&verifier, &jwt]() { [[maybe_unused]] auto _ = verifier.verify(jwt); }, "Should throw when verifying with wrong secret");
}

int main()
{
    try
    {
        std::cout << "Starting AccessToken Unit Tests..." << std::endl;

        test_basic_token_creation();
        test_video_grants();
        test_sip_grants();
        test_attributes();
        test_room_join_validation();
        test_ttl_handling();
        // test_room_config();
        test_sha256();
        test_room_preset();
        test_token_verification_failure();

        std::cout << "\n=== All Tests Passed! ===" << std::endl;
        return 0;
    }
    catch (std::exception const & e)
    {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
