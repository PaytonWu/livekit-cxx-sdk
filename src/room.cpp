// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/room.h>

namespace livekit::rtc
{

Room::Room()
{

}

Room::~Room() noexcept
{
    // Destructor implementation
}

auto Room::sid() const -> exec::task<std::string>
{
    // Implementation of sid() method
    co_return std::string{};
}

auto Room::connect(std::string_view const url, std::string_view const token, RoomOptions const & room_options) -> exec::task<void>
{
    proto::FfiRequest req;

    auto * connection = req.mutable_connect();
    connection->set_url(url.data());
    connection->set_token(token.data());

    auto * options = connection->mutable_options();
    options->set_auto_subscribe(room_options.auto_subscribe);
    options->set_dynacast(room_options.dynacast);

    // TODO: e2ee options
    // TODO: RTC config

    co_return;
}

}
