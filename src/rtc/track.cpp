// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/rtc/track.h>

#include <livekit/error.h>
#include <livekit/ffi/ffi_client.h>
#include <livekit/ffi/proto/ffi.pb.h>
#include <livekit/rtc/audio_source.h>
#include <livekit/rtc/error.h>

#include <cassert>

namespace livekit::rtc
{

LocalAudioTrack::LocalAudioTrack(proto::OwnedTrack const & owned_track) : track_inner_{ std::make_shared<TrackInner>(owned_track) }, ffi_handle_{ owned_track.handle().id() }
{
}

auto LocalAudioTrack::create(std::string_view name, AudioSource const & source) -> LocalAudioTrack
{
    proto::FfiRequest req;
    auto * create_audio_track = req.mutable_create_audio_track();
    create_audio_track->set_name(name.data(), name.size());
    create_audio_track->set_source_handle(source.ffi_handle().id());

    auto resp = ffi::FfiClient::request(req);
    return LocalAudioTrack{ resp.create_audio_track().track() };
}

auto LocalAudioTrack::sid() const -> Sid
{
    assert(track_inner_ != nullptr);
    return track_inner_->sid();
}

auto LocalAudioTrack::sid(Sid const & sid) -> void
{
    assert(track_inner_ != nullptr);
    track_inner_->sid(sid);
}

auto LocalAudioTrack::name() const -> std::string const &
{
    assert(track_inner_ != nullptr);
    return track_inner_->name();
}

auto LocalAudioTrack::kind() const -> proto::TrackKind
{
    assert(track_inner_ != nullptr);
    return track_inner_->kind();
}

auto LocalAudioTrack::stream_state() const -> proto::StreamState
{
    assert(track_inner_ != nullptr);
    return track_inner_->stream_state();
}

auto LocalAudioTrack::muted() const -> bool
{
    assert(track_inner_ != nullptr);
    return track_inner_->muted();
}

auto LocalAudioTrack::get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>
{
    proto::FfiRequest req;
    auto * get_stats = req.mutable_get_stats();
    get_stats->set_track_handle(ffi_handle_.id());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);

    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_get_stats() && event.get_stats().has_async_id() && resp.has_get_stats() && resp.get_stats().has_async_id() &&
               event.get_stats().async_id() == resp.get_stats().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.get_stats().has_error())
    {
        co_return std::unexpected(make_error_code(ErrorCode::TrackGetStatsFailed));
    }

    auto const & stats = event.get_stats().stats();
    co_return std::vector<proto::RtcStats>{ stats.begin(), stats.end() };
}

auto LocalAudioTrack::mute() -> void
{
    assert(track_inner_ != nullptr);

    proto::FfiRequest req;
    auto * local_track_mute = req.mutable_local_track_mute();
    local_track_mute->set_track_handle(this->ffi_handle_.id());
    local_track_mute->set_mute(true);

    ffi::FfiClient::request(req);
    this->track_inner_->mute();
}

auto LocalAudioTrack::unmute() -> void
{
    assert(track_inner_ != nullptr);

    proto::FfiRequest req;
    auto * local_track_mute = req.mutable_local_track_mute();
    local_track_mute->set_track_handle(ffi_handle_.id());
    local_track_mute->set_mute(false);

    ffi::FfiClient::request(req);
    this->track_inner_->unmute();
}

auto LocalAudioTrack::is_remote() const -> bool
{
    assert(track_inner_ != nullptr);
    return track_inner_->remote();
}

RemoteAudioTrack::RemoteAudioTrack(proto::OwnedTrack const & owned_track) : track_inner_{ std::make_shared<TrackInner>(owned_track) }, ffi_handle_{ owned_track.handle().id() }
{
}

auto RemoteAudioTrack::sid() const -> Sid
{
    assert(track_inner_ != nullptr);
    return track_inner_->sid();
}

auto RemoteAudioTrack::name() const -> std::string const &
{
    assert(track_inner_ != nullptr);
    return track_inner_->name();
}

auto RemoteAudioTrack::kind() const -> proto::TrackKind
{
    assert(track_inner_ != nullptr);
    return track_inner_->kind();
}

auto RemoteAudioTrack::stream_state() const -> proto::StreamState
{
    assert(track_inner_ != nullptr);
    return track_inner_->stream_state();
}

auto RemoteAudioTrack::muted() const -> bool
{
    assert(track_inner_ != nullptr);
    return track_inner_->muted();
}

auto RemoteAudioTrack::get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>
{
    proto::FfiRequest req;
    auto * get_stats = req.mutable_get_stats();
    get_stats->set_track_handle(ffi_handle_.id());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_get_stats() && event.get_stats().has_async_id() && resp.has_get_stats() && resp.get_stats().has_async_id() &&
               event.get_stats().async_id() == resp.get_stats().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.get_stats().has_error())
    {
        co_return std::unexpected(make_error_code(ErrorCode::TrackGetStatsFailed));
    }

    auto const & stats = event.get_stats().stats();
    co_return std::vector<proto::RtcStats>{ stats.begin(), stats.end() };
}

auto RemoteAudioTrack::is_enabled() const -> bool
{
    assert(track_inner_ != nullptr);
    // Note: In Rust, this calls rtc_track.enabled(), but in C++ we don't have direct RTC track access.
    // The EnableRemoteTrackResponse returns the enabled state, but we can't easily query it here.
    // For now, we assume tracks are enabled by default. A proper implementation would track this state
    // or provide a way to query it through FFI.
    return true;
}

auto RemoteAudioTrack::enable() -> void
{
    assert(track_inner_ != nullptr);
    proto::FfiRequest req;
    auto * enable_remote_track = req.mutable_enable_remote_track();
    enable_remote_track->set_track_handle(ffi_handle_.id());
    enable_remote_track->set_enabled(true);

    ffi::FfiClient::request(req);
}

auto RemoteAudioTrack::disable() -> void
{
    assert(track_inner_ != nullptr);
    proto::FfiRequest req;
    auto * enable_remote_track = req.mutable_enable_remote_track();
    enable_remote_track->set_track_handle(ffi_handle_.id());
    enable_remote_track->set_enabled(false);

    ffi::FfiClient::request(req);
}

auto RemoteAudioTrack::is_remote() const -> bool
{
    assert(track_inner_ != nullptr);
    return track_inner_->remote();
}

AudioTrack::AudioTrack(proto::OwnedTrack const & owned_track)
    : track_{ [&owned_track]() -> std::variant<LocalAudioTrack, RemoteAudioTrack> {
        auto const & info = owned_track.info();
        bool const is_remote = info.remote();
        proto::TrackKind const kind = info.kind();

        if (is_remote)
        {
            return RemoteAudioTrack{ owned_track };
        }
        else
        {
            return LocalAudioTrack{ owned_track };
        }
    }() }
{
}

auto AudioTrack::sid() const -> Sid
{
    return std::visit([](auto const & track) { return track.sid(); }, track_);
}

auto AudioTrack::name() const -> std::string const &
{
    return std::visit([](auto const & track) -> std::string const & { return track.name(); }, track_);
}

auto AudioTrack::kind() const -> proto::TrackKind
{
    return std::visit([](auto const & track) { return track.kind(); }, track_);
}

auto AudioTrack::stream_state() const -> proto::StreamState
{
    return std::visit([](auto const & track) { return track.stream_state(); }, track_);
}

auto AudioTrack::muted() const -> bool
{
    return std::visit([](auto const & track) { return track.muted(); }, track_);
}

auto AudioTrack::get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>
{
    return std::visit([](auto const & track) -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>> { return track.get_stats(); }, track_);
}

auto AudioTrack::is_enabled() const -> bool
{
    return std::visit(
        [](auto const & track) -> bool {
            if constexpr (requires { track.is_enabled(); })
            {
                return track.is_enabled();
            }
            else
            {
                return true;
            }
        },
        track_);
}

auto AudioTrack::enable() -> void
{
    return std::visit(
        [](auto & track) -> void {
            if constexpr (requires { track.enable(); })
            {
                track.enable();
            }
        },
        track_);
}

auto AudioTrack::disable() -> void
{
    return std::visit([](auto & track) -> void {
            if constexpr (requires { track.disable(); })
            {
                track.disable();
            }
        },
        track_);
}

auto AudioTrack::mute() -> std::expected<void, std::error_code>
{
    return std::visit([](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.mute(); })
            {
                track.mute();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

auto AudioTrack::unmute() -> std::expected<void, std::error_code>
{
    return std::visit([](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.unmute(); })
            {
                track.unmute();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

LocalVideoTrack::LocalVideoTrack(proto::OwnedTrack const & owned_track) : track_inner_{ std::make_shared<TrackInner>(owned_track) }, ffi_handle_{ owned_track.handle().id() }
{
}

auto LocalVideoTrack::sid() const -> Sid
{
    assert(track_inner_ != nullptr);
    return track_inner_->sid();
}

auto LocalVideoTrack::sid(Sid const & sid) -> void
{
    assert(track_inner_ != nullptr);
    track_inner_->sid(sid);
}

// auto LocalVideoTrack::source() const -> VideoSource
// {
//     assert(track_inner_ != nullptr);
//     return track_inner_->source();
// }

auto LocalVideoTrack::name() const -> std::string const &
{
    assert(track_inner_ != nullptr);
    return track_inner_->name();
}

auto LocalVideoTrack::kind() const -> proto::TrackKind
{
    assert(track_inner_ != nullptr);
    return track_inner_->kind();
}

auto LocalVideoTrack::stream_state() const -> proto::StreamState
{
    assert(track_inner_ != nullptr);
    return track_inner_->stream_state();
}

auto LocalVideoTrack::muted() const -> bool
{
    assert(track_inner_ != nullptr);
    return track_inner_->muted();
}

auto LocalVideoTrack::get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>
{
    proto::FfiRequest req;
    auto * get_stats = req.mutable_get_stats();
    get_stats->set_track_handle(ffi_handle_.id());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_get_stats() && event.get_stats().has_async_id() && resp.has_get_stats() && resp.get_stats().has_async_id() &&
               event.get_stats().async_id() == resp.get_stats().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.get_stats().has_error())
    {
        co_return std::unexpected(make_error_code(ErrorCode::TrackGetStatsFailed));
    }

    auto const & stats = event.get_stats().stats();
    co_return std::vector<proto::RtcStats>{ stats.begin(), stats.end() };
}

auto LocalVideoTrack::mute() -> void
{
    assert(track_inner_ != nullptr);

    proto::FfiRequest req;
    auto * local_track_mute = req.mutable_local_track_mute();
    local_track_mute->set_track_handle(this->ffi_handle_.id());
    local_track_mute->set_mute(true);

    ffi::FfiClient::request(req);
    track_inner_->mute();
}

auto LocalVideoTrack::unmute() -> void
{
    assert(track_inner_ != nullptr);

    proto::FfiRequest req;
    auto * local_track_mute = req.mutable_local_track_mute();
    local_track_mute->set_track_handle(this->ffi_handle_.id());
    local_track_mute->set_mute(false);

    ffi::FfiClient::request(req);
    track_inner_->unmute();
}

auto LocalVideoTrack::is_remote() const -> bool
{
    assert(track_inner_ != nullptr);
    assert(!track_inner_->remote());

    return track_inner_->remote();
}

RemoteVideoTrack::RemoteVideoTrack(proto::OwnedTrack const & owned_track) : track_inner_{ std::make_shared<TrackInner>(owned_track) }, ffi_handle_{ owned_track.handle().id() }
{
}

auto RemoteVideoTrack::sid() const -> Sid
{
    assert(track_inner_ != nullptr);
    return track_inner_->sid();
}

auto RemoteVideoTrack::name() const -> std::string const &
{
    assert(track_inner_ != nullptr);
    return track_inner_->name();
}

auto RemoteVideoTrack::kind() const -> proto::TrackKind
{
    assert(track_inner_ != nullptr);
    return track_inner_->kind();
}

auto RemoteVideoTrack::stream_state() const -> proto::StreamState
{
    assert(track_inner_ != nullptr);
    return track_inner_->stream_state();
}

auto RemoteVideoTrack::muted() const -> bool
{
    assert(track_inner_ != nullptr);
    return track_inner_->muted();
}

auto RemoteVideoTrack::get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>
{
    proto::FfiRequest req;
    auto * get_stats = req.mutable_get_stats();
    get_stats->set_track_handle(ffi_handle_.id());

    auto queue = ffi::FfiClient::instance().subscribe();
    auto resp = ffi::FfiClient::request(req);
    proto::FfiEvent event = co_await queue->wait_for([&resp](proto::FfiEvent const & event) {
        return event.has_get_stats() && event.get_stats().has_async_id() && resp.has_get_stats() && resp.get_stats().has_async_id() &&
               event.get_stats().async_id() == resp.get_stats().async_id();
    });
    ffi::FfiClient::instance().unsubscribe(queue);

    if (event.get_stats().has_error())
    {
        co_return std::unexpected(make_error_code(ErrorCode::TrackGetStatsFailed));
    }

    auto const & stats = event.get_stats().stats();
    co_return std::vector<proto::RtcStats>{ stats.begin(), stats.end() };
}

auto RemoteVideoTrack::is_enabled() const -> bool
{
    assert(track_inner_ != nullptr);
    // Note: Same issue as RemoteAudioTrack - we can't easily query the enabled state.
    // For now, we assume tracks are enabled by default. A proper implementation would track this state.
    return true;
}

auto RemoteVideoTrack::enable() -> void
{
    assert(track_inner_ != nullptr);
    proto::FfiRequest req;
    auto * enable_remote_track = req.mutable_enable_remote_track();
    enable_remote_track->set_track_handle(ffi_handle_.id());
    enable_remote_track->set_enabled(true);

    ffi::FfiClient::request(req);
}

auto RemoteVideoTrack::disable() -> void
{
    assert(track_inner_ != nullptr);
    proto::FfiRequest req;
    auto * enable_remote_track = req.mutable_enable_remote_track();
    enable_remote_track->set_track_handle(ffi_handle_.id());
    enable_remote_track->set_enabled(false);

    ffi::FfiClient::request(req);
}

auto RemoteVideoTrack::is_remote() const -> bool
{
    assert(track_inner_ != nullptr);
    assert(track_inner_->remote());

    return track_inner_->remote();
}

VideoTrack::VideoTrack(proto::OwnedTrack const & owned_track)
    : track_{ [&owned_track]() -> std::variant<LocalVideoTrack, RemoteVideoTrack> {
        auto const & info = owned_track.info();
        bool const is_remote = info.remote();
        proto::TrackKind const kind = info.kind();

        if (is_remote)
        {
            return RemoteVideoTrack{ owned_track };
        }
        else
        {
            return LocalVideoTrack{ owned_track };
        }
    }() }
{
}

auto VideoTrack::sid() const -> Sid
{
    return std::visit([](auto const & track) { return track.sid(); }, track_);
}

auto VideoTrack::name() const -> std::string const &
{
    return std::visit([](auto const & track) -> std::string const & { return track.name(); }, track_);
}

auto VideoTrack::kind() const -> proto::TrackKind
{
    return std::visit([](auto const & track) { return track.kind(); }, track_);
}

auto VideoTrack::stream_state() const -> proto::StreamState
{
    return std::visit([](auto const & track) { return track.stream_state(); }, track_);
}

auto VideoTrack::muted() const -> bool
{
    return std::visit([](auto const & track) { return track.muted(); }, track_);
}

auto VideoTrack::get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>
{
    return std::visit([](auto const & track) -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>> { return track.get_stats(); }, track_);
}

auto VideoTrack::is_enabled() const -> bool
{
    return std::visit([](auto const & track) -> bool {
            if constexpr (requires { track.is_enabled(); })
            {
                return track.is_enabled();
            }
            else
            {
                return true;
            }
        },
        track_);
}

auto VideoTrack::enable() -> void
{
    return std::visit([](auto & track) -> void {
            if constexpr (requires { track.enable(); })
            {
                track.enable();
            }
        },
        track_);
}

auto VideoTrack::disable() -> void
{
    return std::visit([](auto & track) -> void {
            if constexpr (requires { track.disable(); })
            {
                track.disable();
            }
        },
        track_);
}

auto VideoTrack::is_remote() const -> bool
{
    return std::visit([](auto const & track) { return track.is_remote(); }, track_);
}

auto VideoTrack::mute() -> std::expected<void, std::error_code>
{
    return std::visit([](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.mute(); })
            {
                track.mute();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

auto VideoTrack::unmute() -> std::expected<void, std::error_code>
{
    return std::visit([](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.unmute(); })
            {
                track.unmute();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

Track::Track(proto::OwnedTrack const & owned_track)
    : track_{ [&owned_track]() -> std::variant<LocalAudioTrack, LocalVideoTrack, RemoteAudioTrack, RemoteVideoTrack> {
        auto const & info = owned_track.info();
        bool const is_remote = info.remote();
        proto::TrackKind const kind = info.kind();

        if (is_remote)
        {
            if (kind == proto::TrackKind::KIND_AUDIO)
            {
                return RemoteAudioTrack{ owned_track };
            }
            else // KIND_VIDEO
            {
                return RemoteVideoTrack{ owned_track };
            }
        }
        else
        {
            if (kind == proto::TrackKind::KIND_AUDIO)
            {
                return LocalAudioTrack{ owned_track };
            }
            else // KIND_VIDEO
            {
                return LocalVideoTrack{ owned_track };
            }
        }
    }() }
{
}

auto Track::sid() const -> Sid
{
    return std::visit([](auto const & track) { return track.sid(); }, track_);
}

auto Track::name() const -> std::string const &
{
    return std::visit([](auto const & track) -> std::string const & { return track.name(); }, track_);
}

auto Track::kind() const -> proto::TrackKind
{
    return std::visit([](auto const & track) { return track.kind(); }, track_);
}

auto Track::stream_state() const -> proto::StreamState
{
    return std::visit([](auto const & track) { return track.stream_state(); }, track_);
}

auto Track::muted() const -> bool
{
    return std::visit([](auto const & track) { return track.muted(); }, track_);
}

auto Track::get_stats() const -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>>
{
    return std::visit([](auto const & track) -> exec::task<std::expected<std::vector<proto::RtcStats>, std::error_code>> { return track.get_stats(); }, track_);
}

auto Track::is_enabled() const -> bool
{
    return std::visit(
        [](auto const & track) -> bool {
            if constexpr (requires { track.is_enabled(); })
            {
                return track.is_enabled();
            }
            else
            {
                // Local tracks are always enabled (they don't have enable/disable)
                return true;
            }
        },
        track_);
}

auto Track::enable() -> std::expected<void, std::error_code>
{
    return std::visit(
        [](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.enable(); })
            {
                track.enable();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

auto Track::disable() -> std::expected<void, std::error_code>
{
    return std::visit(
        [](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.disable(); })
            {
                track.disable();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

auto Track::is_remote() const -> bool
{
    return std::visit([](auto const & track) { return track.is_remote(); }, track_);
}

auto Track::mute() -> std::expected<void, std::error_code>
{
    return std::visit(
        [](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.mute(); })
            {
                track.mute();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

auto Track::unmute() -> std::expected<void, std::error_code>
{
    return std::visit(
        [](auto & track) -> std::expected<void, std::error_code> {
            if constexpr (requires { track.unmute(); })
            {
                track.unmute();
                return {};
            }
            else
            {
                return std::unexpected(make_error_code(ErrorCode::TrackOperationNotAvailable));
            }
        },
        track_);
}

} // namespace livekit::rtc
