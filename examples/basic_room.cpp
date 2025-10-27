// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/room.h>

#include <livekit/rtc/participant.h>

#include <livekit/ffi/proto/room.pb.h>

#include <chrono>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>
#include <fmt/format.h>
#include <iostream>
#include <thread>

auto async_main() -> exec::task<void>
{
    exec::static_thread_pool pool{ 4 };

    auto room = livekit::rtc::Room{ pool.get_scheduler() };
    room.on(livekit::proto::RoomEvent::MessageCase::kDisconnected, [](livekit::proto::RoomEvent const &) { std::cout << "Disconnected from room" << std::endl; });

    room.on(livekit::proto::RoomEvent::MessageCase::kParticipantConnected, [](livekit::proto::RoomEvent const &) { std::cout << "Participant connected to room" << std::endl; });
    room.on(livekit::proto::RoomEvent::MessageCase::kParticipantDisconnected, [](auto const & event) { std::cout << "Participant disconnected from room" << std::endl; });

    room.on(livekit::proto::RoomEvent::MessageCase::kTrackSubscribed, [](auto const & event) { std::cout << "Track subscribed to room" << std::endl; });
    room.on(livekit::proto::RoomEvent::MessageCase::kTrackUnsubscribed, [](auto const & event) { std::cout << "Track unsubscribed from room" << std::endl; });

    auto const url = std::getenv("LIVEKIT_URL");
    auto const token = std::getenv("LIVEKIT_TOKEN");
    if (!url || !token)
    {
        std::cerr << "Error: LIVEKIT_URL and LIVEKIT_TOKEN environment variables must be set" << std::endl;
        co_return;
    }
    co_await room.connect(url, token);

    for (auto const & [_, rparticipant] : room.remote_participants())
    {
        fmt::print("remote participant: {}\n", rparticipant.sid().value());
        fmt::print("identity: {}\n", rparticipant.identity());
        fmt::print("name: {}\n", rparticipant.name());
        fmt::print("metadata: {}\n", rparticipant.metadata());
        // fmt::print("attributes: {}\n", rparticipant.attributes());
        fmt::print("kind: {}\n", rparticipant.kind());
    }

    co_return;
}

auto main() -> int
{
    stdexec::sync_wait(async_main());
    std::this_thread::sleep_for(std::chrono::minutes(10));
    return 0;
}
