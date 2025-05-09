// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: track_publication.proto

#include "track_publication.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace livekit {
namespace proto {
PROTOBUF_CONSTEXPR EnableRemoteTrackPublicationRequest::EnableRemoteTrackPublicationRequest(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.track_publication_handle_)*/uint64_t{0u}
  , /*decltype(_impl_.enabled_)*/false} {}
struct EnableRemoteTrackPublicationRequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR EnableRemoteTrackPublicationRequestDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~EnableRemoteTrackPublicationRequestDefaultTypeInternal() {}
  union {
    EnableRemoteTrackPublicationRequest _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 EnableRemoteTrackPublicationRequestDefaultTypeInternal _EnableRemoteTrackPublicationRequest_default_instance_;
PROTOBUF_CONSTEXPR EnableRemoteTrackPublicationResponse::EnableRemoteTrackPublicationResponse(
    ::_pbi::ConstantInitialized) {}
struct EnableRemoteTrackPublicationResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR EnableRemoteTrackPublicationResponseDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~EnableRemoteTrackPublicationResponseDefaultTypeInternal() {}
  union {
    EnableRemoteTrackPublicationResponse _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 EnableRemoteTrackPublicationResponseDefaultTypeInternal _EnableRemoteTrackPublicationResponse_default_instance_;
PROTOBUF_CONSTEXPR UpdateRemoteTrackPublicationDimensionRequest::UpdateRemoteTrackPublicationDimensionRequest(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.track_publication_handle_)*/uint64_t{0u}
  , /*decltype(_impl_.width_)*/0u
  , /*decltype(_impl_.height_)*/0u} {}
struct UpdateRemoteTrackPublicationDimensionRequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR UpdateRemoteTrackPublicationDimensionRequestDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~UpdateRemoteTrackPublicationDimensionRequestDefaultTypeInternal() {}
  union {
    UpdateRemoteTrackPublicationDimensionRequest _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 UpdateRemoteTrackPublicationDimensionRequestDefaultTypeInternal _UpdateRemoteTrackPublicationDimensionRequest_default_instance_;
PROTOBUF_CONSTEXPR UpdateRemoteTrackPublicationDimensionResponse::UpdateRemoteTrackPublicationDimensionResponse(
    ::_pbi::ConstantInitialized) {}
struct UpdateRemoteTrackPublicationDimensionResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR UpdateRemoteTrackPublicationDimensionResponseDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~UpdateRemoteTrackPublicationDimensionResponseDefaultTypeInternal() {}
  union {
    UpdateRemoteTrackPublicationDimensionResponse _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 UpdateRemoteTrackPublicationDimensionResponseDefaultTypeInternal _UpdateRemoteTrackPublicationDimensionResponse_default_instance_;
}  // namespace proto
}  // namespace livekit
static ::_pb::Metadata file_level_metadata_track_5fpublication_2eproto[4];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_track_5fpublication_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_track_5fpublication_2eproto = nullptr;

const uint32_t TableStruct_track_5fpublication_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::livekit::proto::EnableRemoteTrackPublicationRequest, _impl_._has_bits_),
  PROTOBUF_FIELD_OFFSET(::livekit::proto::EnableRemoteTrackPublicationRequest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::livekit::proto::EnableRemoteTrackPublicationRequest, _impl_.track_publication_handle_),
  PROTOBUF_FIELD_OFFSET(::livekit::proto::EnableRemoteTrackPublicationRequest, _impl_.enabled_),
  0,
  1,
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::livekit::proto::EnableRemoteTrackPublicationResponse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest, _impl_._has_bits_),
  PROTOBUF_FIELD_OFFSET(::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest, _impl_.track_publication_handle_),
  PROTOBUF_FIELD_OFFSET(::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest, _impl_.width_),
  PROTOBUF_FIELD_OFFSET(::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest, _impl_.height_),
  0,
  1,
  2,
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::livekit::proto::UpdateRemoteTrackPublicationDimensionResponse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 8, -1, sizeof(::livekit::proto::EnableRemoteTrackPublicationRequest)},
  { 10, -1, -1, sizeof(::livekit::proto::EnableRemoteTrackPublicationResponse)},
  { 16, 25, -1, sizeof(::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest)},
  { 28, -1, -1, sizeof(::livekit::proto::UpdateRemoteTrackPublicationDimensionResponse)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::livekit::proto::_EnableRemoteTrackPublicationRequest_default_instance_._instance,
  &::livekit::proto::_EnableRemoteTrackPublicationResponse_default_instance_._instance,
  &::livekit::proto::_UpdateRemoteTrackPublicationDimensionRequest_default_instance_._instance,
  &::livekit::proto::_UpdateRemoteTrackPublicationDimensionResponse_default_instance_._instance,
};

const char descriptor_table_protodef_track_5fpublication_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\027track_publication.proto\022\rlivekit.proto"
  "\"X\n#EnableRemoteTrackPublicationRequest\022"
  " \n\030track_publication_handle\030\001 \002(\004\022\017\n\007ena"
  "bled\030\002 \002(\010\"&\n$EnableRemoteTrackPublicati"
  "onResponse\"o\n,UpdateRemoteTrackPublicati"
  "onDimensionRequest\022 \n\030track_publication_"
  "handle\030\001 \002(\004\022\r\n\005width\030\002 \002(\r\022\016\n\006height\030\003 "
  "\002(\r\"/\n-UpdateRemoteTrackPublicationDimen"
  "sionResponseB\020\252\002\rLiveKit.Proto"
  ;
static ::_pbi::once_flag descriptor_table_track_5fpublication_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_track_5fpublication_2eproto = {
    false, false, 350, descriptor_table_protodef_track_5fpublication_2eproto,
    "track_publication.proto",
    &descriptor_table_track_5fpublication_2eproto_once, nullptr, 0, 4,
    schemas, file_default_instances, TableStruct_track_5fpublication_2eproto::offsets,
    file_level_metadata_track_5fpublication_2eproto, file_level_enum_descriptors_track_5fpublication_2eproto,
    file_level_service_descriptors_track_5fpublication_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_track_5fpublication_2eproto_getter() {
  return &descriptor_table_track_5fpublication_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_track_5fpublication_2eproto(&descriptor_table_track_5fpublication_2eproto);
namespace livekit {
namespace proto {

// ===================================================================

class EnableRemoteTrackPublicationRequest::_Internal {
 public:
  using HasBits = decltype(std::declval<EnableRemoteTrackPublicationRequest>()._impl_._has_bits_);
  static void set_has_track_publication_handle(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_enabled(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000003) ^ 0x00000003) != 0;
  }
};

EnableRemoteTrackPublicationRequest::EnableRemoteTrackPublicationRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:livekit.proto.EnableRemoteTrackPublicationRequest)
}
EnableRemoteTrackPublicationRequest::EnableRemoteTrackPublicationRequest(const EnableRemoteTrackPublicationRequest& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  EnableRemoteTrackPublicationRequest* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.track_publication_handle_){}
    , decltype(_impl_.enabled_){}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.track_publication_handle_, &from._impl_.track_publication_handle_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.enabled_) -
    reinterpret_cast<char*>(&_impl_.track_publication_handle_)) + sizeof(_impl_.enabled_));
  // @@protoc_insertion_point(copy_constructor:livekit.proto.EnableRemoteTrackPublicationRequest)
}

inline void EnableRemoteTrackPublicationRequest::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.track_publication_handle_){uint64_t{0u}}
    , decltype(_impl_.enabled_){false}
  };
}

EnableRemoteTrackPublicationRequest::~EnableRemoteTrackPublicationRequest() {
  // @@protoc_insertion_point(destructor:livekit.proto.EnableRemoteTrackPublicationRequest)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void EnableRemoteTrackPublicationRequest::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void EnableRemoteTrackPublicationRequest::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void EnableRemoteTrackPublicationRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:livekit.proto.EnableRemoteTrackPublicationRequest)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    ::memset(&_impl_.track_publication_handle_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&_impl_.enabled_) -
        reinterpret_cast<char*>(&_impl_.track_publication_handle_)) + sizeof(_impl_.enabled_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* EnableRemoteTrackPublicationRequest::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required uint64 track_publication_handle = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _Internal::set_has_track_publication_handle(&has_bits);
          _impl_.track_publication_handle_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // required bool enabled = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _Internal::set_has_enabled(&has_bits);
          _impl_.enabled_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _impl_._has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* EnableRemoteTrackPublicationRequest::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:livekit.proto.EnableRemoteTrackPublicationRequest)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  // required uint64 track_publication_handle = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(1, this->_internal_track_publication_handle(), target);
  }

  // required bool enabled = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteBoolToArray(2, this->_internal_enabled(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:livekit.proto.EnableRemoteTrackPublicationRequest)
  return target;
}

size_t EnableRemoteTrackPublicationRequest::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:livekit.proto.EnableRemoteTrackPublicationRequest)
  size_t total_size = 0;

  if (_internal_has_track_publication_handle()) {
    // required uint64 track_publication_handle = 1;
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_track_publication_handle());
  }

  if (_internal_has_enabled()) {
    // required bool enabled = 2;
    total_size += 1 + 1;
  }

  return total_size;
}
size_t EnableRemoteTrackPublicationRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:livekit.proto.EnableRemoteTrackPublicationRequest)
  size_t total_size = 0;

  if (((_impl_._has_bits_[0] & 0x00000003) ^ 0x00000003) == 0) {  // All required fields are present.
    // required uint64 track_publication_handle = 1;
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_track_publication_handle());

    // required bool enabled = 2;
    total_size += 1 + 1;

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData EnableRemoteTrackPublicationRequest::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    EnableRemoteTrackPublicationRequest::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*EnableRemoteTrackPublicationRequest::GetClassData() const { return &_class_data_; }


void EnableRemoteTrackPublicationRequest::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<EnableRemoteTrackPublicationRequest*>(&to_msg);
  auto& from = static_cast<const EnableRemoteTrackPublicationRequest&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:livekit.proto.EnableRemoteTrackPublicationRequest)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _this->_impl_.track_publication_handle_ = from._impl_.track_publication_handle_;
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_impl_.enabled_ = from._impl_.enabled_;
    }
    _this->_impl_._has_bits_[0] |= cached_has_bits;
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void EnableRemoteTrackPublicationRequest::CopyFrom(const EnableRemoteTrackPublicationRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:livekit.proto.EnableRemoteTrackPublicationRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool EnableRemoteTrackPublicationRequest::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_impl_._has_bits_)) return false;
  return true;
}

void EnableRemoteTrackPublicationRequest::InternalSwap(EnableRemoteTrackPublicationRequest* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(EnableRemoteTrackPublicationRequest, _impl_.enabled_)
      + sizeof(EnableRemoteTrackPublicationRequest::_impl_.enabled_)
      - PROTOBUF_FIELD_OFFSET(EnableRemoteTrackPublicationRequest, _impl_.track_publication_handle_)>(
          reinterpret_cast<char*>(&_impl_.track_publication_handle_),
          reinterpret_cast<char*>(&other->_impl_.track_publication_handle_));
}

::PROTOBUF_NAMESPACE_ID::Metadata EnableRemoteTrackPublicationRequest::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_track_5fpublication_2eproto_getter, &descriptor_table_track_5fpublication_2eproto_once,
      file_level_metadata_track_5fpublication_2eproto[0]);
}

// ===================================================================

class EnableRemoteTrackPublicationResponse::_Internal {
 public:
};

EnableRemoteTrackPublicationResponse::EnableRemoteTrackPublicationResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase(arena, is_message_owned) {
  // @@protoc_insertion_point(arena_constructor:livekit.proto.EnableRemoteTrackPublicationResponse)
}
EnableRemoteTrackPublicationResponse::EnableRemoteTrackPublicationResponse(const EnableRemoteTrackPublicationResponse& from)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase() {
  EnableRemoteTrackPublicationResponse* const _this = this; (void)_this;
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:livekit.proto.EnableRemoteTrackPublicationResponse)
}





const ::PROTOBUF_NAMESPACE_ID::Message::ClassData EnableRemoteTrackPublicationResponse::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::CopyImpl,
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::MergeImpl,
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*EnableRemoteTrackPublicationResponse::GetClassData() const { return &_class_data_; }







::PROTOBUF_NAMESPACE_ID::Metadata EnableRemoteTrackPublicationResponse::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_track_5fpublication_2eproto_getter, &descriptor_table_track_5fpublication_2eproto_once,
      file_level_metadata_track_5fpublication_2eproto[1]);
}

// ===================================================================

class UpdateRemoteTrackPublicationDimensionRequest::_Internal {
 public:
  using HasBits = decltype(std::declval<UpdateRemoteTrackPublicationDimensionRequest>()._impl_._has_bits_);
  static void set_has_track_publication_handle(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_width(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_height(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000007) ^ 0x00000007) != 0;
  }
};

UpdateRemoteTrackPublicationDimensionRequest::UpdateRemoteTrackPublicationDimensionRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
}
UpdateRemoteTrackPublicationDimensionRequest::UpdateRemoteTrackPublicationDimensionRequest(const UpdateRemoteTrackPublicationDimensionRequest& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  UpdateRemoteTrackPublicationDimensionRequest* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.track_publication_handle_){}
    , decltype(_impl_.width_){}
    , decltype(_impl_.height_){}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.track_publication_handle_, &from._impl_.track_publication_handle_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.height_) -
    reinterpret_cast<char*>(&_impl_.track_publication_handle_)) + sizeof(_impl_.height_));
  // @@protoc_insertion_point(copy_constructor:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
}

inline void UpdateRemoteTrackPublicationDimensionRequest::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.track_publication_handle_){uint64_t{0u}}
    , decltype(_impl_.width_){0u}
    , decltype(_impl_.height_){0u}
  };
}

UpdateRemoteTrackPublicationDimensionRequest::~UpdateRemoteTrackPublicationDimensionRequest() {
  // @@protoc_insertion_point(destructor:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void UpdateRemoteTrackPublicationDimensionRequest::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void UpdateRemoteTrackPublicationDimensionRequest::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void UpdateRemoteTrackPublicationDimensionRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    ::memset(&_impl_.track_publication_handle_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&_impl_.height_) -
        reinterpret_cast<char*>(&_impl_.track_publication_handle_)) + sizeof(_impl_.height_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* UpdateRemoteTrackPublicationDimensionRequest::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required uint64 track_publication_handle = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _Internal::set_has_track_publication_handle(&has_bits);
          _impl_.track_publication_handle_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // required uint32 width = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _Internal::set_has_width(&has_bits);
          _impl_.width_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // required uint32 height = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _Internal::set_has_height(&has_bits);
          _impl_.height_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _impl_._has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* UpdateRemoteTrackPublicationDimensionRequest::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  // required uint64 track_publication_handle = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(1, this->_internal_track_publication_handle(), target);
  }

  // required uint32 width = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(2, this->_internal_width(), target);
  }

  // required uint32 height = 3;
  if (cached_has_bits & 0x00000004u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(3, this->_internal_height(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  return target;
}

size_t UpdateRemoteTrackPublicationDimensionRequest::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  size_t total_size = 0;

  if (_internal_has_track_publication_handle()) {
    // required uint64 track_publication_handle = 1;
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_track_publication_handle());
  }

  if (_internal_has_width()) {
    // required uint32 width = 2;
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_width());
  }

  if (_internal_has_height()) {
    // required uint32 height = 3;
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_height());
  }

  return total_size;
}
size_t UpdateRemoteTrackPublicationDimensionRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  size_t total_size = 0;

  if (((_impl_._has_bits_[0] & 0x00000007) ^ 0x00000007) == 0) {  // All required fields are present.
    // required uint64 track_publication_handle = 1;
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_track_publication_handle());

    // required uint32 width = 2;
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_width());

    // required uint32 height = 3;
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_height());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData UpdateRemoteTrackPublicationDimensionRequest::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    UpdateRemoteTrackPublicationDimensionRequest::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*UpdateRemoteTrackPublicationDimensionRequest::GetClassData() const { return &_class_data_; }


void UpdateRemoteTrackPublicationDimensionRequest::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<UpdateRemoteTrackPublicationDimensionRequest*>(&to_msg);
  auto& from = static_cast<const UpdateRemoteTrackPublicationDimensionRequest&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      _this->_impl_.track_publication_handle_ = from._impl_.track_publication_handle_;
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_impl_.width_ = from._impl_.width_;
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.height_ = from._impl_.height_;
    }
    _this->_impl_._has_bits_[0] |= cached_has_bits;
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void UpdateRemoteTrackPublicationDimensionRequest::CopyFrom(const UpdateRemoteTrackPublicationDimensionRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:livekit.proto.UpdateRemoteTrackPublicationDimensionRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool UpdateRemoteTrackPublicationDimensionRequest::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_impl_._has_bits_)) return false;
  return true;
}

void UpdateRemoteTrackPublicationDimensionRequest::InternalSwap(UpdateRemoteTrackPublicationDimensionRequest* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(UpdateRemoteTrackPublicationDimensionRequest, _impl_.height_)
      + sizeof(UpdateRemoteTrackPublicationDimensionRequest::_impl_.height_)
      - PROTOBUF_FIELD_OFFSET(UpdateRemoteTrackPublicationDimensionRequest, _impl_.track_publication_handle_)>(
          reinterpret_cast<char*>(&_impl_.track_publication_handle_),
          reinterpret_cast<char*>(&other->_impl_.track_publication_handle_));
}

::PROTOBUF_NAMESPACE_ID::Metadata UpdateRemoteTrackPublicationDimensionRequest::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_track_5fpublication_2eproto_getter, &descriptor_table_track_5fpublication_2eproto_once,
      file_level_metadata_track_5fpublication_2eproto[2]);
}

// ===================================================================

class UpdateRemoteTrackPublicationDimensionResponse::_Internal {
 public:
};

UpdateRemoteTrackPublicationDimensionResponse::UpdateRemoteTrackPublicationDimensionResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase(arena, is_message_owned) {
  // @@protoc_insertion_point(arena_constructor:livekit.proto.UpdateRemoteTrackPublicationDimensionResponse)
}
UpdateRemoteTrackPublicationDimensionResponse::UpdateRemoteTrackPublicationDimensionResponse(const UpdateRemoteTrackPublicationDimensionResponse& from)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase() {
  UpdateRemoteTrackPublicationDimensionResponse* const _this = this; (void)_this;
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:livekit.proto.UpdateRemoteTrackPublicationDimensionResponse)
}





const ::PROTOBUF_NAMESPACE_ID::Message::ClassData UpdateRemoteTrackPublicationDimensionResponse::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::CopyImpl,
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::MergeImpl,
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*UpdateRemoteTrackPublicationDimensionResponse::GetClassData() const { return &_class_data_; }







::PROTOBUF_NAMESPACE_ID::Metadata UpdateRemoteTrackPublicationDimensionResponse::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_track_5fpublication_2eproto_getter, &descriptor_table_track_5fpublication_2eproto_once,
      file_level_metadata_track_5fpublication_2eproto[3]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
}  // namespace livekit
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::livekit::proto::EnableRemoteTrackPublicationRequest*
Arena::CreateMaybeMessage< ::livekit::proto::EnableRemoteTrackPublicationRequest >(Arena* arena) {
  return Arena::CreateMessageInternal< ::livekit::proto::EnableRemoteTrackPublicationRequest >(arena);
}
template<> PROTOBUF_NOINLINE ::livekit::proto::EnableRemoteTrackPublicationResponse*
Arena::CreateMaybeMessage< ::livekit::proto::EnableRemoteTrackPublicationResponse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::livekit::proto::EnableRemoteTrackPublicationResponse >(arena);
}
template<> PROTOBUF_NOINLINE ::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest*
Arena::CreateMaybeMessage< ::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest >(Arena* arena) {
  return Arena::CreateMessageInternal< ::livekit::proto::UpdateRemoteTrackPublicationDimensionRequest >(arena);
}
template<> PROTOBUF_NOINLINE ::livekit::proto::UpdateRemoteTrackPublicationDimensionResponse*
Arena::CreateMaybeMessage< ::livekit::proto::UpdateRemoteTrackPublicationDimensionResponse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::livekit::proto::UpdateRemoteTrackPublicationDimensionResponse >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
