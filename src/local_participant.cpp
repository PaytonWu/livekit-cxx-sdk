// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/local_participant.h>

#include <livekit/error.h>
#include <livekit/ffi/ffi_client.h>
#include <livekit/ffi/proto/ffi.pb.h>

namespace livekit::rtc
{

LocalParticipant::LocalParticipant(proto::OwnedParticipant const & owned_participant,
                                   utils::BroadcastQueue<proto::FfiEvent> * room_event_queue,
                                   exec::static_thread_pool::scheduler scheduler)
    : Participant{ owned_participant }
    , room_event_queue_{ room_event_queue }
    , scheduler_{ scheduler }
{
}

auto LocalParticipant::publish_data(std::vector<abc::byte> const & data, bool reliable, std::vector<std::string> const & destinations, std::optional<std::string> const & topic)
    -> exec::task<void>
{
    proto::FfiRequest req;

    auto * publish_data = req.mutable_publish_data();
    publish_data->set_local_participant_handle(ffi_handle_.id());
    publish_data->set_data_ptr(reinterpret_cast<std::uint64_t>(data.data()));
    publish_data->set_data_len(static_cast<std::uint64_t>(data.size()));
    publish_data->set_reliable(reliable);
    auto * destination_ids = publish_data->mutable_destination_identities();
    destination_ids->Add(destinations.begin(), destinations.end());
    if (topic)
    {
        publish_data->set_topic(topic.value());
    }

    auto queue = ffi::FfiClient::instance().subscribe(scheduler_);
    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_publish_data() && event.publish_data().has_async_id() && event.publish_data().async_id() == resp.has_publish_data() &&
               resp.publish_data().has_async_id() && resp.publish_data().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.publish_data().has_error())
    {
        throw std::runtime_error(event.publish_data().error());
    }

    co_return;
}

} // namespace livekit::rtc
