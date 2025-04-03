// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ROOM_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ROOM_DECL

#pragma once

#include "room_fwd_decl.h"

#include "livekit/e2ee_decl.h"
#include "livekit/ffi/ffi_handle_decl.h"
#include "livekit/ffi/proto/ffi.pb.h"

#include <abc/threadsafe_queue.h>

#include <exec/task.hpp>

#include <optional>
#include <string>
#include <string_view>

namespace livekit::rtc
{

struct RoomOptions
{
    bool auto_subscribe{ true };
    bool adaptive_stream{ false };
    bool dynacast{ false };
    std::optional<E2eeOptions> e2ee_options{};
};

class Room
{
private:
    ffi::FfiHandle ffi_handle_{};
    abc::ThreadSafeQueue<proto::FfiEvent> event_queue_{};

public:
    Room();
    ~Room() noexcept;

    [[nodiscard]] auto sid() const -> exec::task<std::string>;

    auto connect(std::string_view url, std::string_view token, RoomOptions const & room_options = RoomOptions{}) -> exec::task<void>;
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ROOM_DECL
