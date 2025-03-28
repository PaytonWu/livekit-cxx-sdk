// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: handle.proto

#include "handle.pb.h"

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
PROTOBUF_CONSTEXPR FfiOwnedHandle::FfiOwnedHandle(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.id_)*/uint64_t{0u}} {}
struct FfiOwnedHandleDefaultTypeInternal {
  PROTOBUF_CONSTEXPR FfiOwnedHandleDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~FfiOwnedHandleDefaultTypeInternal() {}
  union {
    FfiOwnedHandle _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 FfiOwnedHandleDefaultTypeInternal _FfiOwnedHandle_default_instance_;
}  // namespace proto
}  // namespace livekit
static ::_pb::Metadata file_level_metadata_handle_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_handle_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_handle_2eproto = nullptr;

const uint32_t TableStruct_handle_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::livekit::proto::FfiOwnedHandle, _impl_._has_bits_),
  PROTOBUF_FIELD_OFFSET(::livekit::proto::FfiOwnedHandle, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::livekit::proto::FfiOwnedHandle, _impl_.id_),
  0,
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 7, -1, sizeof(::livekit::proto::FfiOwnedHandle)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::livekit::proto::_FfiOwnedHandle_default_instance_._instance,
};

const char descriptor_table_protodef_handle_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014handle.proto\022\rlivekit.proto\"\034\n\016FfiOwne"
  "dHandle\022\n\n\002id\030\001 \002(\004B\020\252\002\rLiveKit.Proto"
  ;
static ::_pbi::once_flag descriptor_table_handle_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_handle_2eproto = {
    false, false, 77, descriptor_table_protodef_handle_2eproto,
    "handle.proto",
    &descriptor_table_handle_2eproto_once, nullptr, 0, 1,
    schemas, file_default_instances, TableStruct_handle_2eproto::offsets,
    file_level_metadata_handle_2eproto, file_level_enum_descriptors_handle_2eproto,
    file_level_service_descriptors_handle_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_handle_2eproto_getter() {
  return &descriptor_table_handle_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_handle_2eproto(&descriptor_table_handle_2eproto);
namespace livekit {
namespace proto {

// ===================================================================

class FfiOwnedHandle::_Internal {
 public:
  using HasBits = decltype(std::declval<FfiOwnedHandle>()._impl_._has_bits_);
  static void set_has_id(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000001) ^ 0x00000001) != 0;
  }
};

FfiOwnedHandle::FfiOwnedHandle(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:livekit.proto.FfiOwnedHandle)
}
FfiOwnedHandle::FfiOwnedHandle(const FfiOwnedHandle& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  FfiOwnedHandle* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.id_){}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.id_ = from._impl_.id_;
  // @@protoc_insertion_point(copy_constructor:livekit.proto.FfiOwnedHandle)
}

inline void FfiOwnedHandle::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.id_){uint64_t{0u}}
  };
}

FfiOwnedHandle::~FfiOwnedHandle() {
  // @@protoc_insertion_point(destructor:livekit.proto.FfiOwnedHandle)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void FfiOwnedHandle::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void FfiOwnedHandle::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void FfiOwnedHandle::Clear() {
// @@protoc_insertion_point(message_clear_start:livekit.proto.FfiOwnedHandle)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.id_ = uint64_t{0u};
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* FfiOwnedHandle::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required uint64 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _Internal::set_has_id(&has_bits);
          _impl_.id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
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

uint8_t* FfiOwnedHandle::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:livekit.proto.FfiOwnedHandle)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  // required uint64 id = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(1, this->_internal_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:livekit.proto.FfiOwnedHandle)
  return target;
}

size_t FfiOwnedHandle::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:livekit.proto.FfiOwnedHandle)
  size_t total_size = 0;

  // required uint64 id = 1;
  if (_internal_has_id()) {
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_id());
  }
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData FfiOwnedHandle::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    FfiOwnedHandle::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*FfiOwnedHandle::GetClassData() const { return &_class_data_; }


void FfiOwnedHandle::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<FfiOwnedHandle*>(&to_msg);
  auto& from = static_cast<const FfiOwnedHandle&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:livekit.proto.FfiOwnedHandle)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_id()) {
    _this->_internal_set_id(from._internal_id());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void FfiOwnedHandle::CopyFrom(const FfiOwnedHandle& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:livekit.proto.FfiOwnedHandle)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FfiOwnedHandle::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_impl_._has_bits_)) return false;
  return true;
}

void FfiOwnedHandle::InternalSwap(FfiOwnedHandle* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  swap(_impl_.id_, other->_impl_.id_);
}

::PROTOBUF_NAMESPACE_ID::Metadata FfiOwnedHandle::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_handle_2eproto_getter, &descriptor_table_handle_2eproto_once,
      file_level_metadata_handle_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
}  // namespace livekit
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::livekit::proto::FfiOwnedHandle*
Arena::CreateMaybeMessage< ::livekit::proto::FfiOwnedHandle >(Arena* arena) {
  return Arena::CreateMessageInternal< ::livekit::proto::FfiOwnedHandle >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
