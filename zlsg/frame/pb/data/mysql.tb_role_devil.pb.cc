// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_role_devil.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_role_devil.pb.h"

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

const ::google::protobuf::Descriptor* tb_role_devil_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_role_devil_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5frole_5fdevil_2eproto() {
  protobuf_AddDesc_mysql_2etb_5frole_5fdevil_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_role_devil.proto");
  GOOGLE_CHECK(file != NULL);
  tb_role_devil_descriptor_ = file->message_type(0);
  static const int tb_role_devil_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_devil, role_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_devil, stage_info_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_devil, raid_info_),
  };
  tb_role_devil_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_role_devil_descriptor_,
      tb_role_devil::default_instance_,
      tb_role_devil_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_devil, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_devil, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_role_devil));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5frole_5fdevil_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_role_devil_descriptor_, &tb_role_devil::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5frole_5fdevil_2eproto() {
  delete tb_role_devil::default_instance_;
  delete tb_role_devil_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5frole_5fdevil_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\031mysql.tb_role_devil.proto\022\005mysql\"G\n\rtb"
    "_role_devil\022\017\n\007role_id\030\001 \002(\r\022\022\n\nstage_in"
    "fo\030\002 \002(\014\022\021\n\traid_info\030\003 \002(\014", 107);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_role_devil.proto", &protobuf_RegisterTypes);
  tb_role_devil::default_instance_ = new tb_role_devil();
  tb_role_devil::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5frole_5fdevil_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5frole_5fdevil_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5frole_5fdevil_2eproto() {
    protobuf_AddDesc_mysql_2etb_5frole_5fdevil_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5frole_5fdevil_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_role_devil::kRoleIdFieldNumber;
const int tb_role_devil::kStageInfoFieldNumber;
const int tb_role_devil::kRaidInfoFieldNumber;
#endif  // !_MSC_VER

tb_role_devil::tb_role_devil()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_role_devil::InitAsDefaultInstance() {
}

tb_role_devil::tb_role_devil(const tb_role_devil& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_role_devil::SharedCtor() {
  _cached_size_ = 0;
  role_id_ = 0u;
  stage_info_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  raid_info_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_role_devil::~tb_role_devil() {
  SharedDtor();
}

void tb_role_devil::SharedDtor() {
  if (stage_info_ != &::google::protobuf::internal::kEmptyString) {
    delete stage_info_;
  }
  if (raid_info_ != &::google::protobuf::internal::kEmptyString) {
    delete raid_info_;
  }
  if (this != default_instance_) {
  }
}

void tb_role_devil::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_role_devil::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_role_devil_descriptor_;
}

const tb_role_devil& tb_role_devil::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5frole_5fdevil_2eproto();
  return *default_instance_;
}

tb_role_devil* tb_role_devil::default_instance_ = NULL;

tb_role_devil* tb_role_devil::New() const {
  return new tb_role_devil;
}

void tb_role_devil::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    role_id_ = 0u;
    if (has_stage_info()) {
      if (stage_info_ != &::google::protobuf::internal::kEmptyString) {
        stage_info_->clear();
      }
    }
    if (has_raid_info()) {
      if (raid_info_ != &::google::protobuf::internal::kEmptyString) {
        raid_info_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_role_devil::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 role_id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &role_id_)));
          set_has_role_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_stage_info;
        break;
      }

      // required bytes stage_info = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_stage_info:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_stage_info()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_raid_info;
        break;
      }

      // required bytes raid_info = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_raid_info:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_raid_info()));
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

void tb_role_devil::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->role_id(), output);
  }

  // required bytes stage_info = 2;
  if (has_stage_info()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->stage_info(), output);
  }

  // required bytes raid_info = 3;
  if (has_raid_info()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      3, this->raid_info(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_role_devil::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->role_id(), target);
  }

  // required bytes stage_info = 2;
  if (has_stage_info()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->stage_info(), target);
  }

  // required bytes raid_info = 3;
  if (has_raid_info()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        3, this->raid_info(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_role_devil::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 role_id = 1;
    if (has_role_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->role_id());
    }

    // required bytes stage_info = 2;
    if (has_stage_info()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->stage_info());
    }

    // required bytes raid_info = 3;
    if (has_raid_info()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->raid_info());
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

void tb_role_devil::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_role_devil* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_role_devil*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_role_devil::MergeFrom(const tb_role_devil& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_role_id()) {
      set_role_id(from.role_id());
    }
    if (from.has_stage_info()) {
      set_stage_info(from.stage_info());
    }
    if (from.has_raid_info()) {
      set_raid_info(from.raid_info());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_role_devil::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_role_devil::CopyFrom(const tb_role_devil& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_role_devil::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void tb_role_devil::Swap(tb_role_devil* other) {
  if (other != this) {
    std::swap(role_id_, other->role_id_);
    std::swap(stage_info_, other->stage_info_);
    std::swap(raid_info_, other->raid_info_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_role_devil::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_role_devil_descriptor_;
  metadata.reflection = tb_role_devil_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
