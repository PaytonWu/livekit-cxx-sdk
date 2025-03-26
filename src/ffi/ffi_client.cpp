// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/ffi/ffi_client.h>

namespace livekit::ffi
{

auto FfiClient::instance() -> FfiClient &
{
    static FfiClient instance;
    return instance;
}

} // namespace livekit::ffi
