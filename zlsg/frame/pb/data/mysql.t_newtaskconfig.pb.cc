// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.t_newtaskconfig.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.t_newtaskconfig.pb.h"

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

namespace mysql {

namespace {

const ::google::protobuf::Descriptor* t_newtaskconfig_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  t_newtaskconfig_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2et_5fnewtaskconfig_2eproto() {
  protobuf_AddDesc_mysql_2et_5fnewtaskconfig_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.t_newtaskconfig.proto");
  GOOGLE_CHECK(file != NULL);
  t_newtaskconfig_descriptor_ = file->message_type(0);
  static const int t_newtaskconfig_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtaskconfig, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtaskconfig, taskid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtaskconfig, taskname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtaskconfig, taskorder_),
  };
  t_newtaskconfig_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      t_newtaskconfig_descriptor_,
      t_newtaskconfig::default_instance_,
      t_newtaskconfig_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtaskconfig, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtaskconfig, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(t_newtaskconfig));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2et_5fnewtaskconfig_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    t_newtaskconfig_descriptor_, &t_newtaskconfig::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2et_5fnewtaskconfig_2eproto() {
  delete t_newtaskconfig::default_instance_;
  delete t_newtaskconfig_reflection_;
}

void protobuf_AddDesc_mysql_2et_5fnewtaskconfig_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\033mysql.t_newtaskconfig.proto\022\005mysql\"R\n\017"
    "t_newtaskconfig\022\n\n\002id\030\001 \002(\r\022\016\n\006taskid\030\002 "
    "\002(\r\022\020\n\010taskname\030\003 \002(\t\022\021\n\ttaskOrder\030\004 \002(\r", 120);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.t_newtaskconfig.proto", &protobuf_RegisterTypes);
  t_newtaskconfig::default_instance_ = new t_newtaskconfig();
  t_newtaskconfig::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2et_5fnewtaskconfig_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2et_5fnewtaskconfig_2eproto {
  StaticDescriptorInitializer_mysql_2et_5fnewtaskconfig_2eproto() {
    protobuf_AddDesc_mysql_2et_5fnewtaskconfig_2eproto();
  }
} static_descriptor_initializer_mysql_2et_5fnewtaskconfig_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int t_newtaskconfig::kIdFieldNumber;
const int t_newtaskconfig::kTaskidFieldNumber;
const int t_newtaskconfig::kTasknameFieldNumber;
const int t_newtaskconfig::kTaskOrderFieldNumber;
#endif  // !_MSC_VER

t_newtaskconfig::t_newtaskconfig()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void t_newtaskconfig::InitAsDefaultInstance() {
}

t_newtaskconfig::t_newtaskconfig(const t_newtaskconfig& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void t_newtaskconfig::SharedCtor() {
  _cached_size_ = 0;
  id_ = 0u;
  taskid_ = 0u;
  taskname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  taskorder_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

t_newtaskconfig::~t_newtaskconfig() {
  SharedDtor();
}

void t_newtaskconfig::SharedDtor() {
  if (taskname_ != &::google::protobuf::internal::kEmptyString) {
    delete taskname_;
  }
  if (this != default_instance_) {
  }
}

void t_newtaskconfig::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* t_newtaskconfig::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return t_newtaskconfig_descriptor_;
}

const t_newtaskconfig& t_newtaskconfig::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2et_5fnewtaskconfig_2eproto();
  return *default_instance_;
}

t_newtaskconfig* t_newtaskconfig::default_instance_ = NULL;

t_newtaskconfig* t_newtaskconfig::New() const {
  return new t_newtaskconfig;
}

void t_newtaskconfig::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    id_ = 0u;
    taskid_ = 0u;
    if (has_taskname()) {
      if (taskname_ != &::google::protobuf::internal::kEmptyString) {
        taskname_->clear();
      }
    }
    taskorder_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool t_newtaskconfig::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &id_)));
          set_has_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_taskid;
        break;
      }

      // required uint32 taskid = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_taskid:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &taskid_)));
          set_has_taskid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_taskname;
        break;
      }

      // required string taskname = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_taskname:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_taskname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->taskname().data(), this->taskname().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_taskOrder;
        break;
      }

      // required uint32 taskOrder = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_taskOrder:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &taskorder_)));
          set_has_taskorder();
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

void t_newtaskconfig::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 id = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->id(), output);
  }

  // required uint32 taskid = 2;
  if (has_taskid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->taskid(), output);
  }

  // required string taskname = 3;
  if (has_taskname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->taskname().data(), this->taskname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->taskname(), output);
  }

  // required uint32 taskOrder = 4;
  if (has_taskorder()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->taskorder(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* t_newtaskconfig::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 id = 1;
  if (has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->id(), target);
  }

  // required uint32 taskid = 2;
  if (has_taskid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->taskid(), target);
  }

  // required string taskname = 3;
  if (has_taskname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->taskname().data(), this->taskname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->taskname(), target);
  }

  // required uint32 taskOrder = 4;
  if (has_taskorder()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->taskorder(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int t_newtaskconfig::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->id());
    }

    // required uint32 taskid = 2;
    if (has_taskid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->taskid());
    }

    // required string taskname = 3;
    if (has_taskname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->taskname());
    }

    // required uint32 taskOrder = 4;
    if (has_taskorder()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->taskorder());
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

void t_newtaskconfig::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const t_newtaskconfig* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const t_newtaskconfig*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void t_newtaskconfig::MergeFrom(const t_newtaskconfig& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_taskid()) {
      set_taskid(from.taskid());
    }
    if (from.has_taskname()) {
      set_taskname(from.taskname());
    }
    if (from.has_taskorder()) {
      set_taskorder(from.taskorder());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void t_newtaskconfig::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void t_newtaskconfig::CopyFrom(const t_newtaskconfig& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool t_newtaskconfig::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;

  return true;
}

void t_newtaskconfig::Swap(t_newtaskconfig* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(taskid_, other->taskid_);
    std::swap(taskname_, other->taskname_);
    std::swap(taskorder_, other->taskorder_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata t_newtaskconfig::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = t_newtaskconfig_descriptor_;
  metadata.reflection = t_newtaskconfig_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
