// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/ffi/ffi_client.h>

#include <livekit/ffi/details/livekit_ffi.h>

namespace livekit::ffi
{
auto FfiClient::instance() -> FfiClient &
{
    static FfiClient instance;
    return instance;
}

auto FfiClient::request(proto::FfiRequest const & request) -> proto::FfiResponse
{
    auto const bytes = request.SerializeAsString();

    std::uint8_t const * response_data_ptr{ nullptr };
    std::size_t response_data_size{ 0 };

    proto::FfiResponse response;
    auto handle = livekit_ffi_request(reinterpret_cast<std::uint8_t const *>(bytes.data()), bytes.length(), std::addressof(response_data_ptr), std::addressof(response_data_size));
    if (handle == INVALID_HANDLE)
    {
        return response;
    }

    response.ParseFromArray(response_data_ptr, static_cast<int>(response_data_size));
    livekit_ffi_drop_handle(handle);

    return response;
}

} // namespace livekit::ffi
