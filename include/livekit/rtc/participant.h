// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT
#define LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT

#pragma once

#include "participant_decl.h"

#include "error.h"
#include "sid.h"   // IWYU pragma: export
#include "track.h" // IWYU pragma: export
#include "track_publication.h"
#include "transcription.h" // IWYU pragma: export

#include "livekit/ffi/ffi_client.h"

namespace livekit::rtc
{

auto LocalParticipant::publish_track(LocalTrack auto & track, proto::TrackPublishOptions const & options) -> exec::task<LocalTrackPublication>
{
    proto::FfiRequest req;

    auto * publish_track = req.mutable_publish_track();
    publish_track->set_track_handle(track.handle().id());
    publish_track->set_local_participant_handle(ffi_handle_.id());
    auto * op = publish_track->mutable_options();
    op->CopyFrom(options);

    auto event = co_await ffi::FfiClient::instance().async_request(req);
    if (event.publish_track().has_error())
    {
        abc::throw_error(make_error_code(rtc::ErrorCode::PublishTrackFailed), event.publish_track().error());
    }

    auto track_publication = LocalTrackPublication{ event.publish_track().publication() };
    track_publication.set_track(track);
    track.sid(track_publication.sid());

    track_publications_[track_publication.sid()] = track_publication;

    co_return track_publication;
}

} // namespace livekit::rtc

#endif // LIVEKIT_CXX_SDK_INCLUDE_LIVEKIT_RTC_PARTICIPANT
