// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/error.h>

#include <cassert>

namespace livekit
{

LivekitError::LivekitError(std::error_code const & ec) : std::runtime_error{ ec.message() }, ec_{ ec }
{
}

LivekitError::LivekitError(std::error_code const & ec, std::string_view msg) : std::runtime_error{ msg.data() }, ec_{ ec }
{
}

LivekitError::LivekitError(int const ec, std::error_category const & category) : std::runtime_error{ std::error_code{ ec, category }.message() }, ec_{ ec, category }
{
}

LivekitError::LivekitError(int const ec, std::error_category const & category, std::string_view msg) : std::runtime_error{ msg.data() }, ec_{ ec, category }
{
}

auto LivekitError::code() const noexcept -> std::error_code const &
{
    return ec_;
}

auto LivekitError::what() const noexcept -> char const *
{
    return std::runtime_error::what();
}

auto make_error_code(LivekitErrorCode const ec) noexcept -> std::error_code
{
    return std::error_code{ static_cast<int>(ec), livekit_category() };
}

auto livekit_category() noexcept -> std::error_category const &
{
    static struct : std::error_category
    {
        [[nodiscard]] auto name() const noexcept -> char const * override
        {
            return "livekit";
        }

        [[nodiscard]] auto message(int const ec) const -> std::string override
        {
            switch (static_cast<LivekitErrorCode>(ec))
            {
                case LivekitErrorCode::Success:
                    return "success";
                case LivekitErrorCode::PublishDataFailed:
                    return "publish data failed";
                case LivekitErrorCode::PublishDtmfFailed:
                    return "publish dtmf failed";
                case LivekitErrorCode::GetStatsFailed:
                    return "get stats failed";
                case LivekitErrorCode::InvalidMediaData:
                    return "invalid media data";
                case LivekitErrorCode::CaptureAudioFrameFailed:
                    return "capture audio frame failed";
                case LivekitErrorCode::PublishTrackFailed:
                    return "publish track failed";
                case LivekitErrorCode::RemoteParticipantAlreadyExist:
                    return "remote participant already exist";
                default:
                    assert(false);
                    return "unknown error";
            }
        }
    } category;

    return category;
}

void throw_error(std::error_code const & ec)
{
    if (ec)
    {
        throw LivekitError{ ec };
    }
}

void throw_error(std::error_code const & ec, std::string_view error_msg)
{
    if (ec)
    {
        throw LivekitError{ ec, error_msg };
    }
}

} // namespace livekit
