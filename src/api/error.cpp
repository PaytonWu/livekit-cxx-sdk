// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/api/error.h>

#include <cassert>

namespace livekit::api
{

auto make_error_code(ErrorCode const ec) noexcept -> std::error_code
{
    return std::error_code{ static_cast<int>(ec), livekit_api_category() };
}

auto livekit_api_category() noexcept -> std::error_category const &
{
    static struct : std::error_category
    {
        [[nodiscard]] auto name() const noexcept -> char const * override
        {
            return "livekit::api";
        }

        [[nodiscard]] auto message(int const ec) const -> std::string override
        {
            switch (static_cast<ErrorCode>(ec))
            {
                case ErrorCode::Success:
                    return "success";

                case ErrorCode::InvalidApiKeyOrSecret:
                    return "invalid api key or secret";

                default:
                    assert(false);
                    return "unknown error";
            }
        }
    } category;

    return category;
}

} // namespace livekit::api
