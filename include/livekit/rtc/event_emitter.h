// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER

#pragma once

#include "event_emitter_decl.h"

namespace livekit::rtc
{

template <typename EventT>
auto EventEmitter<EventT>::on(EventT const event, std::shared_ptr<utils::Callable> callable) -> HandlerId
{
    std::lock_guard lock{ handlers_mutex_ };
    auto id = next_handler_id_.fetch_add(1, std::memory_order_relaxed);
    x[event][id] = std::move(callable);
    return id;
}

template <typename EventT>
auto EventEmitter<EventT>::off(EventT const event, HandlerId id) -> void
{
    std::lock_guard lock{ handlers_mutex_ };
    handlers_[event].erase(id);
}

template <typename EventT>
auto EventEmitter<EventT>::emit(EventT const event, auto &&... args) -> void
{
    std::unordered_map<HandlerId, std::shared_ptr<utils::Callable>> handlers;
    {
        std::lock_guard lock{ handlers_mutex_ };
        if (auto it = handlers_.find(event); it != handlers_.end())
        {
            handlers = it->second;
        }
    }
    for (auto & [_, callable] : handlers)
    {
        // TODO: need try ... catch ...?
        std::invoke(*callable, std::forward<decltype(args)>(args)...);
    }
}

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_EVENT_EMITTER
