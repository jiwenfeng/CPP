// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.miniserver.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "client.miniserver.pb.h"

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

namespace client {

namespace {

const ::google::protobuf::Descriptor* exit_miniserver_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  exit_miniserver_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_client_2eminiserver_2eproto() {
  protobuf_AddDesc_client_2eminiserver_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "client.miniserver.proto");
  GOOGLE_CHECK(file != NULL);
  exit_miniserver_descriptor_ = file->message_type(0);
  static const int exit_miniserver_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(exit_miniserver, unixtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(exit_miniserver, svr_head_),
  };
  exit_miniserver_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      exit_miniserver_descriptor_,
      exit_miniserver::default_instance_,
      exit_miniserver_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(exit_miniserver, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(exit_miniserver, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(exit_miniserver));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_client_2eminiserver_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    exit_miniserver_descriptor_, &exit_miniserver::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_client_2eminiserver_2eproto() {
  delete exit_miniserver::default_instance_;
  delete exit_miniserver_reflection_;
}

void protobuf_AddDesc_client_2eminiserver_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::client::protobuf_AddDesc_client_2ecomm_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\027client.miniserver.proto\022\006client\032\021clien"
    "t.comm.proto\"K\n\017exit_miniserver\022\020\n\010unixt"
    "ime\030\001 \002(\r\022&\n\010svr_head\030\350\007 \001(\0132\023.client.se"
    "rver_head", 129);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "client.miniserver.proto", &protobuf_RegisterTypes);
  exit_miniserver::default_instance_ = new exit_miniserver();
  exit_miniserver::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_client_2eminiserver_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_client_2eminiserver_2eproto {
  StaticDescriptorInitializer_client_2eminiserver_2eproto() {
    protobuf_AddDesc_client_2eminiserver_2eproto();
  }
} static_descriptor_initializer_client_2eminiserver_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int exit_miniserver::kUnixtimeFieldNumber;
const int exit_miniserver::kSvrHeadFieldNumber;
#endif  // !_MSC_VER

exit_miniserver::exit_miniserver()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void exit_miniserver::InitAsDefaultInstance() {
  svr_head_ = const_cast< ::client::server_head*>(&::client::server_head::default_instance());
}

exit_miniserver::exit_miniserver(const exit_miniserver& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void exit_miniserver::SharedCtor() {
  _cached_size_ = 0;
  unixtime_ = 0u;
  svr_head_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

exit_miniserver::~exit_miniserver() {
  SharedDtor();
}

void exit_miniserver::SharedDtor() {
  if (this != default_instance_) {
    delete svr_head_;
  }
}

void exit_miniserver::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* exit_miniserver::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return exit_miniserver_descriptor_;
}

const exit_miniserver& exit_miniserver::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2eminiserver_2eproto();
  return *default_instance_;
}

exit_miniserver* exit_miniserver::default_instance_ = NULL;

exit_miniserver* exit_miniserver::New() const {
  return new exit_miniserver;
}

void exit_miniserver::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    unixtime_ = 0u;
    if (has_svr_head()) {
      if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool exit_miniserver::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 unixtime = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &unixtime_)));
          set_has_unixtime();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(8002)) goto parse_svr_head;
        break;
      }

      // optional .client.server_head svr_head = 1000;
      case 1000: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_svr_head:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_svr_head()));
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

void exit_miniserver::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->unixtime(), output);
  }

  // optional .client.server_head svr_head = 1000;
  if (has_svr_head()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1000, this->svr_head(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* exit_miniserver::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->unixtime(), target);
  }

  // optional .client.server_head svr_head = 1000;
  if (has_svr_head()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1000, this->svr_head(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int exit_miniserver::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 unixtime = 1;
    if (has_unixtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->unixtime());
    }

    // optional .client.server_head svr_head = 1000;
    if (has_svr_head()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->svr_head());
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

void exit_miniserver::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const exit_miniserver* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const exit_miniserver*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void exit_miniserver::MergeFrom(const exit_miniserver& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_unixtime()) {
      set_unixtime(from.unixtime());
    }
    if (from.has_svr_head()) {
      mutable_svr_head()->::client::server_head::MergeFrom(from.svr_head());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void exit_miniserver::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void exit_miniserver::CopyFrom(const exit_miniserver& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool exit_miniserver::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void exit_miniserver::Swap(exit_miniserver* other) {
  if (other != this) {
    std::swap(unixtime_, other->unixtime_);
    std::swap(svr_head_, other->svr_head_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata exit_miniserver::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = exit_miniserver_descriptor_;
  metadata.reflection = exit_miniserver_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace client

// @@protoc_insertion_point(global_scope)
