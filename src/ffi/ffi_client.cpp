// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/ffi/ffi_client.h>

#include <livekit/ffi/ffi.h>
#include <livekit/version.h>

#include <cstdlib>

namespace livekit::ffi
{

FfiClient::FfiClient()
{
    livekit_ffi_initialize(ffi_event_callback, false, "livekit-cxx-sdk", LIVEKIT_CXX_SDK_VERSION);
}

auto FfiClient::next_thread_index() -> std::size_t
{
    return thread_pool_index_.fetch_add(1, std::memory_order_relaxed) % thread_pool_size_;
}

auto FfiClient::instance() -> FfiClient &
{
    static FfiClient instance;
    return instance;
}

auto FfiClient::match_ffi_event(proto::FfiResponse const & response, proto::FfiEvent const & event) -> bool
{
    switch (response.message_case())
    {
        case proto::FfiResponse::MessageCase::kConnect:
            return event.has_connect() && event.connect().has_async_id() && response.connect().has_async_id() &&
                   event.connect().async_id() == response.connect().async_id();
        case proto::FfiResponse::MessageCase::kDisconnect:
            return event.has_disconnect() && event.disconnect().has_async_id() && response.disconnect().has_async_id() &&
                   event.disconnect().async_id() == response.disconnect().async_id();
        case proto::FfiResponse::MessageCase::kPublishTrack:
            return event.has_publish_track() && event.publish_track().has_async_id() && response.publish_track().has_async_id() &&
                   event.publish_track().async_id() == response.publish_track().async_id();
        case proto::FfiResponse::MessageCase::kPublishData:
            return event.has_publish_data() && event.publish_data().has_async_id() && response.publish_data().has_async_id() &&
                   event.publish_data().async_id() == response.publish_data().async_id();
        case proto::FfiResponse::MessageCase::kPublishSipDtmf:
            return event.has_publish_sip_dtmf() && event.publish_sip_dtmf().has_async_id() && response.publish_sip_dtmf().has_async_id() &&
                   event.publish_sip_dtmf().async_id() == response.publish_sip_dtmf().async_id();
        case proto::FfiResponse::MessageCase::kCaptureAudioFrame:
            return event.has_capture_audio_frame() && event.capture_audio_frame().has_async_id() && response.capture_audio_frame().has_async_id() &&
                   event.capture_audio_frame().async_id() == response.capture_audio_frame().async_id();
        case proto::FfiResponse::MessageCase::kGetStats:
            return event.has_get_stats() && event.get_stats().has_async_id() && response.get_stats().has_async_id() &&
                   event.get_stats().async_id() == response.get_stats().async_id();
        case proto::FfiResponse::MessageCase::kGetSessionStats:
            return event.has_get_session_stats() && event.get_session_stats().has_async_id() && response.get_session_stats().has_async_id() &&
                   event.get_session_stats().async_id() == response.get_session_stats().async_id();
        default:
            break;
    }

    return false;
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

auto FfiClient::async_request(proto::FfiRequest const & request) -> exec::task<proto::FfiEvent>
{
    auto queue = subscribe();
    auto response = FfiClient::request(request);

    proto::FfiEvent event = co_await queue->wait_for([&response](proto::FfiEvent const & ev) {
        return match_ffi_event(response, ev);
    });

    unsubscribe(queue);

    co_return event;
}

auto FfiClient::subscribe() -> std::shared_ptr<utils::AsyncQueue<proto::FfiEvent>>
{
    auto scheduler = thread_pool_.get_scheduler_on_thread(next_thread_index());
    return queue_.subscribe(scheduler);
}

auto FfiClient::subscribe(exec::static_thread_pool::scheduler scheduler) -> std::shared_ptr<utils::AsyncQueue<proto::FfiEvent>>
{
    return queue_.subscribe(scheduler);
}

auto FfiClient::unsubscribe(std::shared_ptr<utils::AsyncQueue<proto::FfiEvent>> const & queue) -> void
{
    queue_.unsubscribe(queue);
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
