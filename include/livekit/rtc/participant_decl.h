// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT_DECL

#pragma once

#include "participant_fwd_decl.h"


#include "sid_decl.h"
#include "track_decl.h"
#include "track_publication_decl.h"

#include "livekit/ffi/ffi_handle.h"
#include "livekit/ffi/proto/ffi.pb.h"
#include "livekit/ffi/proto/participant.pb.h"
#include "livekit/ffi/proto/room.pb.h"
#include "livekit/utils/broadcast_queue_decl.h"

#include <abc/byte.h>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace livekit::rtc
{

class Participant
{
protected:
    proto::ParticipantInfo info_{};
    ffi::FfiHandle ffi_handle_{};

protected:
    Participant() = default;

    explicit Participant(proto::OwnedParticipant const & owned_participant);

public:
    auto sid() const -> Sid;
    auto name() const noexcept -> std::string const &;
    auto identity() const noexcept -> std::string const &;
    auto metadata() const noexcept -> std::string const &;
    auto attributes() const -> std::unordered_map<std::string, std::string>;
    auto kind() const noexcept -> proto::ParticipantKind;
    auto disconnected_reason() const -> std::optional<proto::DisconnectReason>;
};

class LocalParticipant : public Participant
{
private:
    utils::BroadcastQueue<proto::FfiEvent> * room_event_queue_{ nullptr };
    std::unordered_map<Sid, LocalTrackPublication> track_publications_{};

public:
    explicit LocalParticipant(proto::OwnedParticipant const & owned_participant, utils::BroadcastQueue<proto::FfiEvent> * room_event_queue);

    auto publish_data(std::vector<abc::byte_t> const & data, bool reliable, std::vector<std::string> const & destinations, std::optional<std::string> const & topic)
        -> exec::task<void>;

    auto publish_dtmf(std::uint32_t code, std::string const & digit) -> exec::task<void>;

    auto publish_track(LocalTrack auto & track, proto::TrackPublishOptions const & options = {}) -> exec::task<LocalTrackPublication>;

    auto track_publications() const -> std::unordered_map<Sid, LocalTrackPublication> const &;
    // auto track_publication(Sid const & sid) const -> std::expected<LocalTrackPublication const, std::error_code>;
};

class RemoteParticipant : public Participant
{
private:
    std::unordered_map<Sid, RemoteTrackPublication> track_publications_{};

public:
    RemoteParticipant() = default;

    explicit RemoteParticipant(proto::OwnedParticipant const & owned_participant);

    auto add_track_publication(RemoteTrackPublication track_publication) -> void;
    auto remove_track_publication(Sid const & sid) -> void;

    auto track_publication(Sid const & sid) const -> std::expected<std::reference_wrapper<RemoteTrackPublication const>, std::error_code>;
    auto track_publication(Sid const & sid) -> std::expected<std::reference_wrapper<RemoteTrackPublication>, std::error_code>;

    auto track_publications() const -> std::unordered_map<Sid, RemoteTrackPublication> const &;
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT_DECL
