// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/ffi/ffi_client.h>

#include <livekit/ffi/details/livekit_ffi.h>

#include <cstdlib>

namespace livekit::ffi
{

FfiClient::FfiClient()
{
    livekit_ffi_initialize(ffi_event_callback, true);
}

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

auto ffi_event_callback(std::uint8_t const * data, std::size_t size) -> void
{
    proto::FfiEvent event;
    if (!event.ParseFromArray(data, static_cast<int>(size)))
    {
        // log error
        return;
    }

    switch (event.message_case())
    {
        case proto::FfiEvent::kLogs:
        {
            /*
            for record in event.logs.records:
                level = to_python_level(record.level)
                debug_env = os.environ.get("LIVEKIT_RTC_DEBUG", "").strip().lower()
                rtc_debug = debug_env in ("true", "1")

                if level == logging.DEBUG and not rtc_debug:
                    # ignore the rtc debug logs by default
                    if record.target == "libwebrtc" or record.target.startswith("livekit"):
                        continue

                if level is not None:
                    logger.log(
                        level,
                        "%s:%s:%s - %s",
                        record.target,
                        record.line,
                        record.module_path,
                        record.message,
                    )
            */
            std::string const debug_env = std::getenv("LIVEKIT_RTC_DEBUG");
            auto const rtc_debug = debug_env == "true" || debug_env == "1";

            for (auto const & record : event.logs().records())
            {
                auto const level = record.level();
                if (level == proto::LogLevel::LOG_DEBUG && !rtc_debug)
                {
                    if (record.target() == "libwebrtc" || record.target().starts_with("livekit"))
                    {
                        continue;
                    }
                }

                // logger.log(level, "%s:%s:%s - %s", record.target, record.line, record.module_path, record.message);
            }
            break;
        }

        case proto::FfiEvent::kPanic:
        {
            /*
            logger.critical("Panic: %s", event.panic.message)
            # We are in a unrecoverable state, terminate the process
            os.kill(os.getpid(), signal.SIGTERM)
             */
            break;
        }

        default:
        {
            FfiClient::instance().queue_.put(event);
            break;
        }
    }
}

} // namespace livekit::ffi
