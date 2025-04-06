// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/room.h>

#include <livekit/ffi/ffi_client.h>

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
    if (room_options.e2ee_options)
    {
        auto * e2ee = options->mutable_e2ee();
        e2ee->set_encryption_type(static_cast<proto::EncryptionType>(static_cast<int>(room_options.e2ee_options->encryption_type)));
        auto * key_provider = e2ee->mutable_key_provider_options();
        key_provider->set_shared_key(room_options.e2ee_options->key_provider_options.shared_key);
        key_provider->set_ratchet_window_size(room_options.e2ee_options->key_provider_options.ratchet_window_size);
        key_provider->set_ratchet_salt(room_options.e2ee_options->key_provider_options.ratchet_salt);
        key_provider->set_failure_tolerance(room_options.e2ee_options->key_provider_options.failure_tolerance);
    }

    // TODO: RTC config
    auto * rtc_config = options->mutable_rtc_config();
    for (const auto & server : room_options.rtc_config.ice_servers)
    {
        auto * ice_server = rtc_config->add_ice_servers();
        for (const auto & url : server.urls)
        {
            ice_server->add_urls(url);
        }
        if (server.username)
        {
            ice_server->set_username(*server.username);
        }
        if (server.password)
        {
            ice_server->set_password(*server.password);
        }
    }
    rtc_config->set_ice_transport_type(static_cast<proto::IceTransportType>(static_cast<int>(room_options.rtc_config.ice_transport_type)));
    rtc_config->set_continual_gathering_policy(static_cast<proto::ContinualGatheringPolicy>(static_cast<int>(room_options.rtc_config.continual_gathering_policy)));
    options->set_join_retries(room_options.join_retries);

    // livekit::ffi::FfiClient::instance()

    co_return;
}

}
