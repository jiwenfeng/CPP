// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: inner.server.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "inner.server.pb.h"

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

namespace inner {

namespace {

const ::google::protobuf::Descriptor* start_time_req_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  start_time_req_reflection_ = NULL;
const ::google::protobuf::Descriptor* start_time_rsp_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  start_time_rsp_reflection_ = NULL;
const ::google::protobuf::Descriptor* open_gm_req_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  open_gm_req_reflection_ = NULL;
const ::google::protobuf::Descriptor* open_gm_rsp_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  open_gm_rsp_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_inner_2eserver_2eproto() {
  protobuf_AddDesc_inner_2eserver_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "inner.server.proto");
  GOOGLE_CHECK(file != NULL);
  start_time_req_descriptor_ = file->message_type(0);
  static const int start_time_req_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_req, unixtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_req, head_),
  };
  start_time_req_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      start_time_req_descriptor_,
      start_time_req::default_instance_,
      start_time_req_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_req, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_req, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(start_time_req));
  start_time_rsp_descriptor_ = file->message_type(1);
  static const int start_time_rsp_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_rsp, unixtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_rsp, tail_),
  };
  start_time_rsp_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      start_time_rsp_descriptor_,
      start_time_rsp::default_instance_,
      start_time_rsp_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_rsp, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(start_time_rsp, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(start_time_rsp));
  open_gm_req_descriptor_ = file->message_type(2);
  static const int open_gm_req_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_req, unixtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_req, head_),
  };
  open_gm_req_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      open_gm_req_descriptor_,
      open_gm_req::default_instance_,
      open_gm_req_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_req, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_req, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(open_gm_req));
  open_gm_rsp_descriptor_ = file->message_type(3);
  static const int open_gm_rsp_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_rsp, unixtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_rsp, tail_),
  };
  open_gm_rsp_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      open_gm_rsp_descriptor_,
      open_gm_rsp::default_instance_,
      open_gm_rsp_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_rsp, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(open_gm_rsp, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(open_gm_rsp));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_inner_2eserver_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    start_time_req_descriptor_, &start_time_req::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    start_time_rsp_descriptor_, &start_time_rsp::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    open_gm_req_descriptor_, &open_gm_req::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    open_gm_rsp_descriptor_, &open_gm_rsp::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_inner_2eserver_2eproto() {
  delete start_time_req::default_instance_;
  delete start_time_req_reflection_;
  delete start_time_rsp::default_instance_;
  delete start_time_rsp_reflection_;
  delete open_gm_req::default_instance_;
  delete open_gm_req_reflection_;
  delete open_gm_rsp::default_instance_;
  delete open_gm_rsp_reflection_;
}

void protobuf_AddDesc_inner_2eserver_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::inner::protobuf_AddDesc_inner_2ecomm_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022inner.server.proto\022\005inner\032\020inner.comm."
    "proto\"D\n\016start_time_req\022\020\n\010unixtime\030\001 \002("
    "\r\022 \n\004head\030\350\007 \001(\0132\021.inner.inner_head\"D\n\016s"
    "tart_time_rsp\022\020\n\010unixtime\030\001 \002(\r\022 \n\004tail\030"
    "\350\007 \002(\0132\021.inner.inner_tail\"A\n\013open_gm_req"
    "\022\020\n\010unixtime\030\001 \002(\r\022 \n\004head\030\350\007 \001(\0132\021.inne"
    "r.inner_head\"A\n\013open_gm_rsp\022\020\n\010unixtime\030"
    "\001 \002(\r\022 \n\004tail\030\350\007 \002(\0132\021.inner.inner_tail", 319);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "inner.server.proto", &protobuf_RegisterTypes);
  start_time_req::default_instance_ = new start_time_req();
  start_time_rsp::default_instance_ = new start_time_rsp();
  open_gm_req::default_instance_ = new open_gm_req();
  open_gm_rsp::default_instance_ = new open_gm_rsp();
  start_time_req::default_instance_->InitAsDefaultInstance();
  start_time_rsp::default_instance_->InitAsDefaultInstance();
  open_gm_req::default_instance_->InitAsDefaultInstance();
  open_gm_rsp::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_inner_2eserver_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_inner_2eserver_2eproto {
  StaticDescriptorInitializer_inner_2eserver_2eproto() {
    protobuf_AddDesc_inner_2eserver_2eproto();
  }
} static_descriptor_initializer_inner_2eserver_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int start_time_req::kUnixtimeFieldNumber;
const int start_time_req::kHeadFieldNumber;
#endif  // !_MSC_VER

start_time_req::start_time_req()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void start_time_req::InitAsDefaultInstance() {
  head_ = const_cast< ::inner::inner_head*>(&::inner::inner_head::default_instance());
}

start_time_req::start_time_req(const start_time_req& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void start_time_req::SharedCtor() {
  _cached_size_ = 0;
  unixtime_ = 0u;
  head_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

start_time_req::~start_time_req() {
  SharedDtor();
}

void start_time_req::SharedDtor() {
  if (this != default_instance_) {
    delete head_;
  }
}

void start_time_req::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* start_time_req::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return start_time_req_descriptor_;
}

const start_time_req& start_time_req::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_inner_2eserver_2eproto();
  return *default_instance_;
}

start_time_req* start_time_req::default_instance_ = NULL;

start_time_req* start_time_req::New() const {
  return new start_time_req;
}

void start_time_req::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    unixtime_ = 0u;
    if (has_head()) {
      if (head_ != NULL) head_->::inner::inner_head::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool start_time_req::MergePartialFromCodedStream(
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
        if (input->ExpectTag(8002)) goto parse_head;
        break;
      }

      // optional .inner.inner_head head = 1000;
      case 1000: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_head:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_head()));
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

void start_time_req::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->unixtime(), output);
  }

  // optional .inner.inner_head head = 1000;
  if (has_head()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1000, this->head(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* start_time_req::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->unixtime(), target);
  }

  // optional .inner.inner_head head = 1000;
  if (has_head()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1000, this->head(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int start_time_req::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 unixtime = 1;
    if (has_unixtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->unixtime());
    }

    // optional .inner.inner_head head = 1000;
    if (has_head()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->head());
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

void start_time_req::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const start_time_req* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const start_time_req*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void start_time_req::MergeFrom(const start_time_req& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_unixtime()) {
      set_unixtime(from.unixtime());
    }
    if (from.has_head()) {
      mutable_head()->::inner::inner_head::MergeFrom(from.head());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void start_time_req::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void start_time_req::CopyFrom(const start_time_req& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool start_time_req::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void start_time_req::Swap(start_time_req* other) {
  if (other != this) {
    std::swap(unixtime_, other->unixtime_);
    std::swap(head_, other->head_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata start_time_req::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = start_time_req_descriptor_;
  metadata.reflection = start_time_req_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int start_time_rsp::kUnixtimeFieldNumber;
const int start_time_rsp::kTailFieldNumber;
#endif  // !_MSC_VER

start_time_rsp::start_time_rsp()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void start_time_rsp::InitAsDefaultInstance() {
  tail_ = const_cast< ::inner::inner_tail*>(&::inner::inner_tail::default_instance());
}

start_time_rsp::start_time_rsp(const start_time_rsp& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void start_time_rsp::SharedCtor() {
  _cached_size_ = 0;
  unixtime_ = 0u;
  tail_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

start_time_rsp::~start_time_rsp() {
  SharedDtor();
}

void start_time_rsp::SharedDtor() {
  if (this != default_instance_) {
    delete tail_;
  }
}

void start_time_rsp::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* start_time_rsp::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return start_time_rsp_descriptor_;
}

const start_time_rsp& start_time_rsp::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_inner_2eserver_2eproto();
  return *default_instance_;
}

start_time_rsp* start_time_rsp::default_instance_ = NULL;

start_time_rsp* start_time_rsp::New() const {
  return new start_time_rsp;
}

void start_time_rsp::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    unixtime_ = 0u;
    if (has_tail()) {
      if (tail_ != NULL) tail_->::inner::inner_tail::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool start_time_rsp::MergePartialFromCodedStream(
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
        if (input->ExpectTag(8002)) goto parse_tail;
        break;
      }

      // required .inner.inner_tail tail = 1000;
      case 1000: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_tail:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_tail()));
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

void start_time_rsp::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->unixtime(), output);
  }

  // required .inner.inner_tail tail = 1000;
  if (has_tail()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1000, this->tail(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* start_time_rsp::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->unixtime(), target);
  }

  // required .inner.inner_tail tail = 1000;
  if (has_tail()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1000, this->tail(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int start_time_rsp::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 unixtime = 1;
    if (has_unixtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->unixtime());
    }

    // required .inner.inner_tail tail = 1000;
    if (has_tail()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->tail());
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

void start_time_rsp::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const start_time_rsp* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const start_time_rsp*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void start_time_rsp::MergeFrom(const start_time_rsp& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_unixtime()) {
      set_unixtime(from.unixtime());
    }
    if (from.has_tail()) {
      mutable_tail()->::inner::inner_tail::MergeFrom(from.tail());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void start_time_rsp::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void start_time_rsp::CopyFrom(const start_time_rsp& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool start_time_rsp::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void start_time_rsp::Swap(start_time_rsp* other) {
  if (other != this) {
    std::swap(unixtime_, other->unixtime_);
    std::swap(tail_, other->tail_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata start_time_rsp::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = start_time_rsp_descriptor_;
  metadata.reflection = start_time_rsp_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int open_gm_req::kUnixtimeFieldNumber;
const int open_gm_req::kHeadFieldNumber;
#endif  // !_MSC_VER

open_gm_req::open_gm_req()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void open_gm_req::InitAsDefaultInstance() {
  head_ = const_cast< ::inner::inner_head*>(&::inner::inner_head::default_instance());
}

open_gm_req::open_gm_req(const open_gm_req& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void open_gm_req::SharedCtor() {
  _cached_size_ = 0;
  unixtime_ = 0u;
  head_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

open_gm_req::~open_gm_req() {
  SharedDtor();
}

void open_gm_req::SharedDtor() {
  if (this != default_instance_) {
    delete head_;
  }
}

void open_gm_req::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* open_gm_req::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return open_gm_req_descriptor_;
}

const open_gm_req& open_gm_req::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_inner_2eserver_2eproto();
  return *default_instance_;
}

open_gm_req* open_gm_req::default_instance_ = NULL;

open_gm_req* open_gm_req::New() const {
  return new open_gm_req;
}

void open_gm_req::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    unixtime_ = 0u;
    if (has_head()) {
      if (head_ != NULL) head_->::inner::inner_head::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool open_gm_req::MergePartialFromCodedStream(
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
        if (input->ExpectTag(8002)) goto parse_head;
        break;
      }

      // optional .inner.inner_head head = 1000;
      case 1000: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_head:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_head()));
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

void open_gm_req::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->unixtime(), output);
  }

  // optional .inner.inner_head head = 1000;
  if (has_head()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1000, this->head(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* open_gm_req::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->unixtime(), target);
  }

  // optional .inner.inner_head head = 1000;
  if (has_head()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1000, this->head(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int open_gm_req::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 unixtime = 1;
    if (has_unixtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->unixtime());
    }

    // optional .inner.inner_head head = 1000;
    if (has_head()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->head());
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

void open_gm_req::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const open_gm_req* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const open_gm_req*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void open_gm_req::MergeFrom(const open_gm_req& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_unixtime()) {
      set_unixtime(from.unixtime());
    }
    if (from.has_head()) {
      mutable_head()->::inner::inner_head::MergeFrom(from.head());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void open_gm_req::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void open_gm_req::CopyFrom(const open_gm_req& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool open_gm_req::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void open_gm_req::Swap(open_gm_req* other) {
  if (other != this) {
    std::swap(unixtime_, other->unixtime_);
    std::swap(head_, other->head_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata open_gm_req::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = open_gm_req_descriptor_;
  metadata.reflection = open_gm_req_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int open_gm_rsp::kUnixtimeFieldNumber;
const int open_gm_rsp::kTailFieldNumber;
#endif  // !_MSC_VER

open_gm_rsp::open_gm_rsp()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void open_gm_rsp::InitAsDefaultInstance() {
  tail_ = const_cast< ::inner::inner_tail*>(&::inner::inner_tail::default_instance());
}

open_gm_rsp::open_gm_rsp(const open_gm_rsp& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void open_gm_rsp::SharedCtor() {
  _cached_size_ = 0;
  unixtime_ = 0u;
  tail_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

open_gm_rsp::~open_gm_rsp() {
  SharedDtor();
}

void open_gm_rsp::SharedDtor() {
  if (this != default_instance_) {
    delete tail_;
  }
}

void open_gm_rsp::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* open_gm_rsp::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return open_gm_rsp_descriptor_;
}

const open_gm_rsp& open_gm_rsp::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_inner_2eserver_2eproto();
  return *default_instance_;
}

open_gm_rsp* open_gm_rsp::default_instance_ = NULL;

open_gm_rsp* open_gm_rsp::New() const {
  return new open_gm_rsp;
}

void open_gm_rsp::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    unixtime_ = 0u;
    if (has_tail()) {
      if (tail_ != NULL) tail_->::inner::inner_tail::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool open_gm_rsp::MergePartialFromCodedStream(
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
        if (input->ExpectTag(8002)) goto parse_tail;
        break;
      }

      // required .inner.inner_tail tail = 1000;
      case 1000: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_tail:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_tail()));
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

void open_gm_rsp::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->unixtime(), output);
  }

  // required .inner.inner_tail tail = 1000;
  if (has_tail()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1000, this->tail(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* open_gm_rsp::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 unixtime = 1;
  if (has_unixtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->unixtime(), target);
  }

  // required .inner.inner_tail tail = 1000;
  if (has_tail()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1000, this->tail(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int open_gm_rsp::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 unixtime = 1;
    if (has_unixtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->unixtime());
    }

    // required .inner.inner_tail tail = 1000;
    if (has_tail()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->tail());
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

void open_gm_rsp::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const open_gm_rsp* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const open_gm_rsp*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void open_gm_rsp::MergeFrom(const open_gm_rsp& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_unixtime()) {
      set_unixtime(from.unixtime());
    }
    if (from.has_tail()) {
      mutable_tail()->::inner::inner_tail::MergeFrom(from.tail());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void open_gm_rsp::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void open_gm_rsp::CopyFrom(const open_gm_rsp& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool open_gm_rsp::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void open_gm_rsp::Swap(open_gm_rsp* other) {
  if (other != this) {
    std::swap(unixtime_, other->unixtime_);
    std::swap(tail_, other->tail_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata open_gm_rsp::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = open_gm_rsp_descriptor_;
  metadata.reflection = open_gm_rsp_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace inner

// @@protoc_insertion_point(global_scope)
