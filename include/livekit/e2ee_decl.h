// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_E2EE_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_E2EE_DECL

#pragma once

#include "e2ee_fwd_decl.h"

#include <cstdint>
#include <string>

namespace livekit
{

// enum EncryptionType {
//    NONE = 0;
//    GCM = 1;
//    CUSTOM = 2;
//}
//
// message KeyProviderOptions {
//    // Only specify if you want to use a shared_key
//    optional bytes shared_key = 1;
//    required int32 ratchet_window_size = 2;
//    required bytes ratchet_salt = 3;
//    required int32 failure_tolerance = 4; // -1 = no tolerance
//}
//
// message E2eeOptions {
//    required EncryptionType encryption_type = 1;
//    required KeyProviderOptions key_provider_options = 2;
//}

enum class EncryptionType
{
    None,
    Gcm,
    Custom,
};

struct KeyProviderOptions
{
    std::string shared_key;
    int32_t ratchet_window_size{ 0 };
    std::string ratchet_salt;
    int32_t failure_tolerance{ -1 }; // -1 = no tolerance
};

struct E2eeOptions
{
    EncryptionType encryption_type{ EncryptionType::None };
    KeyProviderOptions key_provider_options{};
};

} // namespace livekit

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_E2EE_DECL
