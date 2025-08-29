// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN

#pragma once

#include "access_token_decl.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace livekit::api
{

// Inline utility functions
inline std::string snake_to_lower_camel(std::string const & input)
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

// Helper function to check if a value should be included in JWT claims
template <typename T>
inline bool should_include_in_claims(T const & value)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return !value.empty();
    }
    else if constexpr (std::is_same_v<T, std::vector<std::string>>)
    {
        return !value.empty();
    }
    else if constexpr (std::is_same_v<T, std::map<std::string, std::string>>)
    {
        return !value.empty();
    }
    else
    {
        return true; // For boolean and other types, always include
    }
}

} // namespace livekit::api

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_API_ACCESS_TOKEN
