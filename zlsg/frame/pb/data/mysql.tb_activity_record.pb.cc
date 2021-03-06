// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_activity_record.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_activity_record.pb.h"

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

const ::google::protobuf::Descriptor* tb_activity_record_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_activity_record_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5factivity_5frecord_2eproto() {
  protobuf_AddDesc_mysql_2etb_5factivity_5frecord_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_activity_record.proto");
  GOOGLE_CHECK(file != NULL);
  tb_activity_record_descriptor_ = file->message_type(0);
  static const int tb_activity_record_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_activity_record, activity_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_activity_record, act_record_),
  };
  tb_activity_record_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_activity_record_descriptor_,
      tb_activity_record::default_instance_,
      tb_activity_record_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_activity_record, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_activity_record, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_activity_record));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5factivity_5frecord_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_activity_record_descriptor_, &tb_activity_record::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5factivity_5frecord_2eproto() {
  delete tb_activity_record::default_instance_;
  delete tb_activity_record_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5factivity_5frecord_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\036mysql.tb_activity_record.proto\022\005mysql\""
    "=\n\022tb_activity_record\022\023\n\013activity_id\030\001 \002"
    "(\r\022\022\n\nact_record\030\002 \002(\014", 102);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_activity_record.proto", &protobuf_RegisterTypes);
  tb_activity_record::default_instance_ = new tb_activity_record();
  tb_activity_record::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5factivity_5frecord_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5factivity_5frecord_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5factivity_5frecord_2eproto() {
    protobuf_AddDesc_mysql_2etb_5factivity_5frecord_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5factivity_5frecord_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_activity_record::kActivityIdFieldNumber;
const int tb_activity_record::kActRecordFieldNumber;
#endif  // !_MSC_VER

tb_activity_record::tb_activity_record()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_activity_record::InitAsDefaultInstance() {
}

tb_activity_record::tb_activity_record(const tb_activity_record& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_activity_record::SharedCtor() {
  _cached_size_ = 0;
  activity_id_ = 0u;
  act_record_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_activity_record::~tb_activity_record() {
  SharedDtor();
}

void tb_activity_record::SharedDtor() {
  if (act_record_ != &::google::protobuf::internal::kEmptyString) {
    delete act_record_;
  }
  if (this != default_instance_) {
  }
}

void tb_activity_record::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_activity_record::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_activity_record_descriptor_;
}

const tb_activity_record& tb_activity_record::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5factivity_5frecord_2eproto();
  return *default_instance_;
}

tb_activity_record* tb_activity_record::default_instance_ = NULL;

tb_activity_record* tb_activity_record::New() const {
  return new tb_activity_record;
}

void tb_activity_record::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    activity_id_ = 0u;
    if (has_act_record()) {
      if (act_record_ != &::google::protobuf::internal::kEmptyString) {
        act_record_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_activity_record::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 activity_id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &activity_id_)));
          set_has_activity_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_act_record;
        break;
      }

      // required bytes act_record = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_act_record:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_act_record()));
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

void tb_activity_record::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 activity_id = 1;
  if (has_activity_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->activity_id(), output);
  }

  // required bytes act_record = 2;
  if (has_act_record()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->act_record(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_activity_record::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 activity_id = 1;
  if (has_activity_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->activity_id(), target);
  }

  // required bytes act_record = 2;
  if (has_act_record()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->act_record(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_activity_record::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 activity_id = 1;
    if (has_activity_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->activity_id());
    }

    // required bytes act_record = 2;
    if (has_act_record()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->act_record());
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

void tb_activity_record::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_activity_record* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_activity_record*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_activity_record::MergeFrom(const tb_activity_record& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_activity_id()) {
      set_activity_id(from.activity_id());
    }
    if (from.has_act_record()) {
      set_act_record(from.act_record());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_activity_record::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_activity_record::CopyFrom(const tb_activity_record& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_activity_record::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void tb_activity_record::Swap(tb_activity_record* other) {
  if (other != this) {
    std::swap(activity_id_, other->activity_id_);
    std::swap(act_record_, other->act_record_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_activity_record::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_activity_record_descriptor_;
  metadata.reflection = tb_activity_record_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
