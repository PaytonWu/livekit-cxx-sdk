// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/sid.h>

namespace livekit::rtc
{

} // namespace livekit::rtc

namespace std
{

auto hash<livekit::rtc::Sid>::operator()(livekit::rtc::Sid const & sid) const -> std::size_t
{
    return std::hash<std::string>{}(sid.value());
}


} // namespace std
