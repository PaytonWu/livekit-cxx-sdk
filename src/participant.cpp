// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/participant.h>

namespace livekit::rtc
{

Participant::Participant(proto::OwnedParticipant const & owned_participant) : info_{ owned_participant.info() }, ffi_handle_{ owned_participant.handle().id() }
{
}

auto Participant::sid() -> Sid
{
    return Sid{ info_.sid() };
}

auto Participant::name() -> std::string const &
{
    return info_.name();
}

auto Participant::identity() -> std::string const &
{
    return info_.identity();
}

auto Participant::metadata() -> std::string const &
{
    return info_.metadata();
}

auto Participant::attributes() -> std::unordered_map<std::string, std::string>
{
    return { info_.attributes().begin(), info_.attributes().end()};
}

auto Participant::kind() -> proto::ParticipantKind
{
    return info_.kind();
}

auto Participant::disconnected_reason() -> std::optional<proto::DisconnectReason>
{
    return info_.has_disconnect_reason() ? std::make_optional(info_.disconnect_reason()) : std::nullopt;
}

} // namespace livekit::rtc
