// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_family_diplomacy.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_family_diplomacy.pb.h"

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

const ::google::protobuf::Descriptor* tb_family_diplomacy_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_family_diplomacy_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5ffamily_5fdiplomacy_2eproto() {
  protobuf_AddDesc_mysql_2etb_5ffamily_5fdiplomacy_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_family_diplomacy.proto");
  GOOGLE_CHECK(file != NULL);
  tb_family_diplomacy_descriptor_ = file->message_type(0);
  static const int tb_family_diplomacy_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_family_diplomacy, family_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_family_diplomacy, target_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_family_diplomacy, relation_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_family_diplomacy, apply_time_),
  };
  tb_family_diplomacy_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_family_diplomacy_descriptor_,
      tb_family_diplomacy::default_instance_,
      tb_family_diplomacy_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_family_diplomacy, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_family_diplomacy, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_family_diplomacy));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5ffamily_5fdiplomacy_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_family_diplomacy_descriptor_, &tb_family_diplomacy::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5ffamily_5fdiplomacy_2eproto() {
  delete tb_family_diplomacy::default_instance_;
  delete tb_family_diplomacy_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5ffamily_5fdiplomacy_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\037mysql.tb_family_diplomacy.proto\022\005mysql"
    "\"f\n\023tb_family_diplomacy\022\021\n\tfamily_id\030\001 \002"
    "(\r\022\021\n\ttarget_id\030\002 \002(\r\022\025\n\rrelation_type\030\003"
    " \002(\r\022\022\n\napply_time\030\004 \002(\r", 144);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_family_diplomacy.proto", &protobuf_RegisterTypes);
  tb_family_diplomacy::default_instance_ = new tb_family_diplomacy();
  tb_family_diplomacy::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5ffamily_5fdiplomacy_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5ffamily_5fdiplomacy_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5ffamily_5fdiplomacy_2eproto() {
    protobuf_AddDesc_mysql_2etb_5ffamily_5fdiplomacy_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5ffamily_5fdiplomacy_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_family_diplomacy::kFamilyIdFieldNumber;
const int tb_family_diplomacy::kTargetIdFieldNumber;
const int tb_family_diplomacy::kRelationTypeFieldNumber;
const int tb_family_diplomacy::kApplyTimeFieldNumber;
#endif  // !_MSC_VER

tb_family_diplomacy::tb_family_diplomacy()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_family_diplomacy::InitAsDefaultInstance() {
}

tb_family_diplomacy::tb_family_diplomacy(const tb_family_diplomacy& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_family_diplomacy::SharedCtor() {
  _cached_size_ = 0;
  family_id_ = 0u;
  target_id_ = 0u;
  relation_type_ = 0u;
  apply_time_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_family_diplomacy::~tb_family_diplomacy() {
  SharedDtor();
}

void tb_family_diplomacy::SharedDtor() {
  if (this != default_instance_) {
  }
}

void tb_family_diplomacy::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_family_diplomacy::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_family_diplomacy_descriptor_;
}

const tb_family_diplomacy& tb_family_diplomacy::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5ffamily_5fdiplomacy_2eproto();
  return *default_instance_;
}

tb_family_diplomacy* tb_family_diplomacy::default_instance_ = NULL;

tb_family_diplomacy* tb_family_diplomacy::New() const {
  return new tb_family_diplomacy;
}

void tb_family_diplomacy::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    family_id_ = 0u;
    target_id_ = 0u;
    relation_type_ = 0u;
    apply_time_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_family_diplomacy::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 family_id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &family_id_)));
          set_has_family_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_target_id;
        break;
      }

      // required uint32 target_id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_target_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &target_id_)));
          set_has_target_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_relation_type;
        break;
      }

      // required uint32 relation_type = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_relation_type:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &relation_type_)));
          set_has_relation_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_apply_time;
        break;
      }

      // required uint32 apply_time = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_apply_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &apply_time_)));
          set_has_apply_time();
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

void tb_family_diplomacy::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 family_id = 1;
  if (has_family_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->family_id(), output);
  }

  // required uint32 target_id = 2;
  if (has_target_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->target_id(), output);
  }

  // required uint32 relation_type = 3;
  if (has_relation_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->relation_type(), output);
  }

  // required uint32 apply_time = 4;
  if (has_apply_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->apply_time(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_family_diplomacy::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 family_id = 1;
  if (has_family_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->family_id(), target);
  }

  // required uint32 target_id = 2;
  if (has_target_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->target_id(), target);
  }

  // required uint32 relation_type = 3;
  if (has_relation_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->relation_type(), target);
  }

  // required uint32 apply_time = 4;
  if (has_apply_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->apply_time(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_family_diplomacy::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 family_id = 1;
    if (has_family_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->family_id());
    }

    // required uint32 target_id = 2;
    if (has_target_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->target_id());
    }

    // required uint32 relation_type = 3;
    if (has_relation_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->relation_type());
    }

    // required uint32 apply_time = 4;
    if (has_apply_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->apply_time());
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

void tb_family_diplomacy::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_family_diplomacy* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_family_diplomacy*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_family_diplomacy::MergeFrom(const tb_family_diplomacy& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_family_id()) {
      set_family_id(from.family_id());
    }
    if (from.has_target_id()) {
      set_target_id(from.target_id());
    }
    if (from.has_relation_type()) {
      set_relation_type(from.relation_type());
    }
    if (from.has_apply_time()) {
      set_apply_time(from.apply_time());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_family_diplomacy::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_family_diplomacy::CopyFrom(const tb_family_diplomacy& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_family_diplomacy::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;

  return true;
}

void tb_family_diplomacy::Swap(tb_family_diplomacy* other) {
  if (other != this) {
    std::swap(family_id_, other->family_id_);
    std::swap(target_id_, other->target_id_);
    std::swap(relation_type_, other->relation_type_);
    std::swap(apply_time_, other->apply_time_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_family_diplomacy::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_family_diplomacy_descriptor_;
  metadata.reflection = tb_family_diplomacy_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
