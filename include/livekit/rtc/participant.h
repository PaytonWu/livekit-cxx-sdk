// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT

#pragma once

#include "participant_decl.h"

#include "livekit/error.h"
#include "livekit/ffi/ffi_client.h"
#include "sid.h"
#include "track.h"
#include "track_publication.h"

#include <abc/scope_guard.h>

namespace livekit::rtc
{

auto LocalParticipant::publish_track(LocalTrack auto const & track, proto::TrackPublishOptions const & options) -> exec::task<LocalTrackPublication>
{
    proto::FfiRequest req;

    auto * publish_track = req.mutable_publish_track();
    publish_track->set_track_handle(track.handle().id());
    publish_track->set_local_participant_handle(ffi_handle_.id());
    auto * op = publish_track->mutable_options();
    op->CopyFrom(options);

    auto queue = ffi::FfiClient::instance().subscribe();
    auto unsub = abc::make_scope_guard([&queue]() {
        ffi::FfiClient::instance().unsubscribe(queue);
    });

    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_publish_track() && event.publish_track().has_async_id() && resp.has_publish_track() && resp.publish_track().has_async_id() &&
               event.publish_track().async_id() == resp.publish_track().async_id();
    });

    if (event.publish_track().has_error())
    {
        throw_error(LivekitErrorCode::PublishTrackFailed, event.publish_track().error());
    }

    auto track_publication = std::make_shared<LocalTrackPublication>(event.publish_track().publication());
    track_publication->set_track(track);
    // TODO: set track sid
    // track.info().set_track_sid(track_publication.sid());

    track_publications_[track_publication->sid()] = track_publication;
    queue->task_done();

    co_return LocalTrackPublication{ event.publish_track().publication() };
}

}

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT
