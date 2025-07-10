// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_LOCAL_PARTICIPANT_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_LOCAL_PARTICIPANT_DECL

#pragma once

#include "local_participant_fwd_decl.h"

#include "livekit/ffi/proto/ffi.pb.h"
#include "livekit/utils/broadcast_queue_decl.h"
#include "participant_decl.h"

#include <abc/byte.h>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>

#include <cstdint>
#include <optional>
#include <vector>

namespace livekit::rtc
{

class LocalParticipant : public Participant
{
private:
    utils::BroadcastQueue<proto::FfiEvent> * room_event_queue_{ nullptr };
    exec::static_thread_pool::scheduler scheduler_;

public:
    explicit LocalParticipant(proto::OwnedParticipant const & owned_participant,
                              utils::BroadcastQueue<proto::FfiEvent> * room_event_queue,
                              exec::static_thread_pool::scheduler scheduler);

    auto publish_data(std::vector<abc::byte> const & data, bool reliable, std::vector<std::string> const & destinations, std::optional<std::string> const & topic)
        -> exec::task<void>;

    auto publish_dtmf(std::uint32_t code, std::string const & digit) -> exec::task<void>;

    // auto publish_track
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_LOCAL_PARTICIPANT_DECL
