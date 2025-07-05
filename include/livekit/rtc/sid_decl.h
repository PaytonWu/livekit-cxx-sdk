// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID_DECL

#pragma once

#include "sid_fwd_decl.h"

#include "livekit/utils/strong_typed_value_decl.h"

#include <string>

namespace livekit::rtc
{

class Sid : public utils::StrongTypedValue<std::string, Sid>
{
public:
    using utils::StrongTypedValue<std::string, Sid>::StrongTypedValue;
};

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_SID_DECL
