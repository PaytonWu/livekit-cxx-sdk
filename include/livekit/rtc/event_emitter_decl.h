// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER_DECL

#pragma once

#include "event_emitter_fwd_decl.h"

#include "livekit/utils/callable.h"

#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace livekit::rtc
{

template <typename EventT>
class EventEmitter
{
public:
    using HandlerId = std::size_t;

private:
    std::atomic<HandlerId> next_handler_id_{ 0 };
    std::unordered_map<EventT, std::unordered_map<HandlerId, std::unique_ptr<utils::Callable>>> handlers_;
    std::mutex handlers_mutex_;

public:
    auto on(EventT event, std::unique_ptr<utils::Callable> callable) -> HandlerId;
    auto off(EventT event, HandlerId id) -> void;
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER_DECL
