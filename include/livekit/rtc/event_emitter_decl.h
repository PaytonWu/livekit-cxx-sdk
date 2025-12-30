// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER_DECL

#pragma once

#include "event_emitter_fwd_decl.h" // IWYU pragma: export

#include <atomic>
#include <cstddef>
#include <functional>
#include <mutex>
#include <unordered_map>

namespace livekit::rtc
{

template <typename EventT, typename EventDataT>
class EventEmitter
{
public:
    using HandlerId = std::size_t;

private:
    std::atomic<HandlerId> next_handler_id_{ 0 };
    std::unordered_map<EventT, std::unordered_map<HandlerId, std::function<void(EventDataT const &)>>> handlers_;
    std::mutex handlers_mutex_;

public:
    auto on(EventT event, std::function<void(EventDataT const &)> callable) -> HandlerId;
    auto off(EventT event, HandlerId id) -> void;

    auto emit(EventT event, EventDataT const & data) -> void;
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER_DECL
