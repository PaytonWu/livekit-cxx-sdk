// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER

#pragma once

#include "event_emitter_decl.h"

namespace livekit::rtc
{

template <typename EventT, typename EventDataT>
auto EventEmitter<EventT, EventDataT>::on(EventT const event, std::function<void(EventDataT const &)> callable) -> HandlerId
{
    std::lock_guard lock{ handlers_mutex_ };
    auto id = next_handler_id_.fetch_add(1, std::memory_order_relaxed);
    handlers_[event][id] = std::move(callable);
    return id;
}

template <typename EventT, typename EventDataT>
auto EventEmitter<EventT, EventDataT>::off(EventT const event, HandlerId id) -> void
{
    std::lock_guard lock{ handlers_mutex_ };
    handlers_[event].erase(id);
}

template <typename EventT, typename EventDataT>
auto EventEmitter<EventT, EventDataT>::emit(EventT const event, EventDataT const & data) -> void
{
    std::unordered_map<HandlerId, std::function<void(EventDataT const &)>> handlers;
    {
        std::lock_guard lock{ handlers_mutex_ };
        if (auto it = handlers_.find(event); it != handlers_.end())
        {
            handlers = it->second;
        }
    }
    for (auto const & [_, callable] : handlers)
    {
        // TODO: need try ... catch ...?
        std::invoke(callable, data);
    }
}

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER
