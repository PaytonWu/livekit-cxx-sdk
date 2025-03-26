#!/bin/bash
# Copyright 2023 LiveKit, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


FFI_PROTOCOL=third-party/rust-sdks/livekit-ffi/protocol
LIVEKIT_PROTOCOL=third-party/rust-sdks/livekit-protocol/protocol/protobufs
OUT_CPP=include/livekit/ffi/proto/

# Create the output directory if it doesn't exist
mkdir -p $OUT_CPP

protoc \
    -I=$FFI_PROTOCOL \
    -I=$LIVEKIT_PROTOCOL \
    --cpp_out=$OUT_CPP \
    --experimental_allow_proto3_optional \
    $FFI_PROTOCOL/audio_frame.proto \
    $FFI_PROTOCOL/e2ee.proto \
    $FFI_PROTOCOL/ffi.proto \
    $FFI_PROTOCOL/handle.proto \
    $FFI_PROTOCOL/participant.proto \
    $FFI_PROTOCOL/room.proto \
    $FFI_PROTOCOL/rpc.proto \
    $FFI_PROTOCOL/stats.proto \
    $FFI_PROTOCOL/track.proto \
    $FFI_PROTOCOL/track_publication.proto \
    $FFI_PROTOCOL/video_frame.proto \
    $LIVEKIT_PROTOCOL/livekit_egress.proto \
    $LIVEKIT_PROTOCOL/livekit_rtc.proto \
    $LIVEKIT_PROTOCOL/livekit_room.proto \
    $LIVEKIT_PROTOCOL/livekit_webhook.proto \
    $LIVEKIT_PROTOCOL/livekit_sip.proto \
    $LIVEKIT_PROTOCOL/livekit_models.proto \
    $LIVEKIT_PROTOCOL/livekit_agent_dispatch.proto \
    $LIVEKIT_PROTOCOL/livekit_ingress.proto \
    $LIVEKIT_PROTOCOL/livekit_agent.proto \
    $LIVEKIT_PROTOCOL/livekit_metrics.proto
