// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/error.h>

#include <cassert>

namespace livekit::rtc
{

auto livekit_rtc_category() noexcept -> std::error_category const &
{
    static struct : std::error_category
    {
        [[nodiscard]] auto name() const noexcept -> char const * override
        {
            return "livekit::rtc";
        }

        [[nodiscard]] auto message(int const ec) const -> std::string override
        {
            switch (static_cast<ErrorCode>(ec))
            {
                case ErrorCode::Success:
                    return "success";
                case ErrorCode::RtcNotConnected:
                    return "rtc not connected";
                case ErrorCode::RtcGetStatsFailed:
                    return "rtc get stats failed";
                default:
                    assert(false);
                    return "unknown error";
            }
        }
    } category;

    return category;
}

auto make_error_code(ErrorCode const ec) noexcept -> std::error_code
{
    return std::error_code{ static_cast<int>(ec), livekit_rtc_category() };
}

} // namespace livekit::rtc
