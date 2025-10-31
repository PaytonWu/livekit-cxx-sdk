// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ROOM_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ROOM_DECL

#pragma once

#include "room_fwd_decl.h"

#include "event_emitter_decl.h"
#include "participant_decl.h"
#include "rtc_stats_decl.h"

#include "livekit/e2ee_decl.h"
#include "livekit/ffi/ffi_handle_decl.h"
#include "livekit/ffi/proto/ffi.pb.h"
#include "livekit/ffi/proto/room.pb.h"
#include "livekit/utils/async_queue_decl.h"
#include "livekit/utils/broadcast_queue_decl.h"

#include <exec/async_scope.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>

#include <chrono>
#include <cstdint>
#include <ctime>
#include <expected>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <unordered_map>
#include <vector>

namespace livekit::rtc
{

enum class IceTransportType
{
    Relay,
    NoHost,
    All,
};
enum class ContinualGatheringPolicy
{
    GatherOnce,
    GatherContinually,
};

struct IceServer
{
    std::vector<std::string> urls;
    std::optional<std::string> username;
    std::optional<std::string> password;
};

struct RtcConfiguration
{
    std::vector<IceServer> ice_servers{};
    IceTransportType ice_transport_type{ IceTransportType::All };
    ContinualGatheringPolicy continual_gathering_policy{ ContinualGatheringPolicy::GatherContinually };
};

struct RoomOptions
{
    bool auto_subscribe{ true };
    bool adaptive_stream{ false };
    bool dynacast{ false };
    std::optional<E2EEOptions> e2ee_options{};
    RtcConfiguration rtc_config{};
    std::uint32_t join_retries{ 3 };
};

class Room : public EventEmitter<proto::RoomEvent::MessageCase, proto::RoomEvent>
{
private:
    exec::static_thread_pool::scheduler scheduler_;
    exec::async_scope async_scope_{};

    std::optional<ffi::FfiHandle> ffi_handle_{};
    std::shared_ptr<utils::AsyncQueue<proto::FfiEvent>> event_queue_{};
    utils::BroadcastQueue<proto::FfiEvent> room_event_queue_{ scheduler_ };
    // E2EEManager e2ee_mgr_;
    proto::RoomInfo room_info_;
    proto::ConnectionState connection_state_{ proto::ConnectionState::CONN_DISCONNECTED };
    std::unique_ptr<LocalParticipant> local_participant_{};
    std::unordered_map<std::string, RemoteParticipant> remote_participants_{};

public:
    explicit Room(exec::static_thread_pool::scheduler scheduler);
    ~Room() noexcept;

    [[nodiscard]] auto sid() const -> exec::task<std::string>;

    auto connect(std::string_view url, std::string_view token, RoomOptions const & room_options = RoomOptions{}) -> exec::task<void>;
    auto disconnect() -> exec::task<void>;
    auto connected() const noexcept -> bool;

    auto local_participant() const noexcept -> std::expected<std::reference_wrapper<LocalParticipant const>, std::error_code>;
    auto remote_participants() const noexcept -> std::unordered_map<std::string, RemoteParticipant> const &;
    auto connection_state() const noexcept -> proto::ConnectionState;
    auto name() const noexcept -> std::string const &;
    auto metadata() const noexcept -> std::string const &;
    auto num_participants() const noexcept -> std::size_t;
    auto num_publishers() const noexcept -> std::size_t;
    auto creation_time() const noexcept -> std::time_t;
    auto is_recording() const noexcept -> bool;
    auto departure_timeout() const noexcept -> std::chrono::seconds;
    auto empty_timeout() const noexcept -> std::chrono::seconds;
    auto rtc_stats() const noexcept -> exec::task<std::expected<RtcStats, std::error_code>>;

private:
    auto create_remote_participant(proto::OwnedParticipant const & owned_participant) -> RemoteParticipant;
    auto listen_room_events() -> exec::task<void>;
    auto on_room_event(proto::RoomEvent const & event) -> void;
};

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_ROOM_DECL
