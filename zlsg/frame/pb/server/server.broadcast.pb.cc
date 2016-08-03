// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: server.broadcast.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "server.broadcast.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace server {

namespace {

const ::google::protobuf::Descriptor* broadcast_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  broadcast_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* broadcast_TYPE_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_server_2ebroadcast_2eproto() {
  protobuf_AddDesc_server_2ebroadcast_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "server.broadcast.proto");
  GOOGLE_CHECK(file != NULL);
  broadcast_descriptor_ = file->message_type(0);
  static const int broadcast_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(broadcast, type_),
  };
  broadcast_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      broadcast_descriptor_,
      broadcast::default_instance_,
      broadcast_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(broadcast, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(broadcast, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(broadcast));
  broadcast_TYPE_descriptor_ = broadcast_descriptor_->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_server_2ebroadcast_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    broadcast_descriptor_, &broadcast::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_server_2ebroadcast_2eproto() {
  delete broadcast::default_instance_;
  delete broadcast_reflection_;
}

void protobuf_AddDesc_server_2ebroadcast_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026server.broadcast.proto\022\006server\"_\n\tbroa"
    "dcast\022$\n\004type\030\001 \002(\0162\026.server.broadcast.T"
    "YPE\",\n\004TYPE\022\021\n\rbroadcast_all\020\000\022\021\n\rbroadc"
    "ast_mut\020\001", 129);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "server.broadcast.proto", &protobuf_RegisterTypes);
  broadcast::default_instance_ = new broadcast();
  broadcast::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_server_2ebroadcast_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_server_2ebroadcast_2eproto {
  StaticDescriptorInitializer_server_2ebroadcast_2eproto() {
    protobuf_AddDesc_server_2ebroadcast_2eproto();
  }
} static_descriptor_initializer_server_2ebroadcast_2eproto_;

// ===================================================================

const ::google::protobuf::EnumDescriptor* broadcast_TYPE_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return broadcast_TYPE_descriptor_;
}
bool broadcast_TYPE_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const broadcast_TYPE broadcast::broadcast_all;
const broadcast_TYPE broadcast::broadcast_mut;
const broadcast_TYPE broadcast::TYPE_MIN;
const broadcast_TYPE broadcast::TYPE_MAX;
const int broadcast::TYPE_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int broadcast::kTypeFieldNumber;
#endif  // !_MSC_VER

broadcast::broadcast()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void broadcast::InitAsDefaultInstance() {
}

broadcast::broadcast(const broadcast& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void broadcast::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

broadcast::~broadcast() {
  SharedDtor();
}

void broadcast::SharedDtor() {
  if (this != default_instance_) {
  }
}

void broadcast::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* broadcast::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return broadcast_descriptor_;
}

const broadcast& broadcast::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_server_2ebroadcast_2eproto();
  return *default_instance_;
}

broadcast* broadcast::default_instance_ = NULL;

broadcast* broadcast::New() const {
  return new broadcast;
}

void broadcast::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool broadcast::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .server.broadcast.TYPE type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::server::broadcast_TYPE_IsValid(value)) {
            set_type(static_cast< ::server::broadcast_TYPE >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void broadcast::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .server.broadcast.TYPE type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* broadcast::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .server.broadcast.TYPE type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int broadcast::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .server.broadcast.TYPE type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void broadcast::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const broadcast* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const broadcast*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void broadcast::MergeFrom(const broadcast& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void broadcast::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void broadcast::CopyFrom(const broadcast& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool broadcast::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void broadcast::Swap(broadcast* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata broadcast::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = broadcast_descriptor_;
  metadata.reflection = broadcast_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace server

// @@protoc_insertion_point(global_scope)
