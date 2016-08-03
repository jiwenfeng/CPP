// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_role_levels.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_role_levels.pb.h"

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

const ::google::protobuf::Descriptor* tb_role_levels_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_role_levels_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5frole_5flevels_2eproto() {
  protobuf_AddDesc_mysql_2etb_5frole_5flevels_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_role_levels.proto");
  GOOGLE_CHECK(file != NULL);
  tb_role_levels_descriptor_ = file->message_type(0);
  static const int tb_role_levels_offsets_[8] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, role_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, new_level_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, old_level_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, new_power_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, old_power_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, cur_level_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, levels_),
  };
  tb_role_levels_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_role_levels_descriptor_,
      tb_role_levels::default_instance_,
      tb_role_levels_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_levels, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_role_levels));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5frole_5flevels_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_role_levels_descriptor_, &tb_role_levels::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5frole_5flevels_2eproto() {
  delete tb_role_levels::default_instance_;
  delete tb_role_levels_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5frole_5flevels_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\032mysql.tb_role_levels.proto\022\005mysql\"\236\001\n\016"
    "tb_role_levels\022\017\n\007role_id\030\001 \002(\r\022\014\n\004time\030"
    "\002 \002(\r\022\021\n\tnew_level\030\003 \002(\r\022\021\n\told_level\030\004 "
    "\002(\r\022\021\n\tnew_power\030\005 \002(\r\022\021\n\told_power\030\006 \002("
    "\r\022\021\n\tcur_level\030\007 \002(\r\022\016\n\006levels\030\010 \002(\014", 196);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_role_levels.proto", &protobuf_RegisterTypes);
  tb_role_levels::default_instance_ = new tb_role_levels();
  tb_role_levels::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5frole_5flevels_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5frole_5flevels_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5frole_5flevels_2eproto() {
    protobuf_AddDesc_mysql_2etb_5frole_5flevels_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5frole_5flevels_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_role_levels::kRoleIdFieldNumber;
const int tb_role_levels::kTimeFieldNumber;
const int tb_role_levels::kNewLevelFieldNumber;
const int tb_role_levels::kOldLevelFieldNumber;
const int tb_role_levels::kNewPowerFieldNumber;
const int tb_role_levels::kOldPowerFieldNumber;
const int tb_role_levels::kCurLevelFieldNumber;
const int tb_role_levels::kLevelsFieldNumber;
#endif  // !_MSC_VER

tb_role_levels::tb_role_levels()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_role_levels::InitAsDefaultInstance() {
}

tb_role_levels::tb_role_levels(const tb_role_levels& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_role_levels::SharedCtor() {
  _cached_size_ = 0;
  role_id_ = 0u;
  time_ = 0u;
  new_level_ = 0u;
  old_level_ = 0u;
  new_power_ = 0u;
  old_power_ = 0u;
  cur_level_ = 0u;
  levels_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_role_levels::~tb_role_levels() {
  SharedDtor();
}

void tb_role_levels::SharedDtor() {
  if (levels_ != &::google::protobuf::internal::kEmptyString) {
    delete levels_;
  }
  if (this != default_instance_) {
  }
}

void tb_role_levels::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_role_levels::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_role_levels_descriptor_;
}

const tb_role_levels& tb_role_levels::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5frole_5flevels_2eproto();
  return *default_instance_;
}

tb_role_levels* tb_role_levels::default_instance_ = NULL;

tb_role_levels* tb_role_levels::New() const {
  return new tb_role_levels;
}

void tb_role_levels::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    role_id_ = 0u;
    time_ = 0u;
    new_level_ = 0u;
    old_level_ = 0u;
    new_power_ = 0u;
    old_power_ = 0u;
    cur_level_ = 0u;
    if (has_levels()) {
      if (levels_ != &::google::protobuf::internal::kEmptyString) {
        levels_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_role_levels::MergePartialFromCodedStream(
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
        if (input->ExpectTag(16)) goto parse_time;
        break;
      }

      // required uint32 time = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &time_)));
          set_has_time();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_new_level;
        break;
      }

      // required uint32 new_level = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_new_level:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &new_level_)));
          set_has_new_level();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_old_level;
        break;
      }

      // required uint32 old_level = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_old_level:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &old_level_)));
          set_has_old_level();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_new_power;
        break;
      }

      // required uint32 new_power = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_new_power:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &new_power_)));
          set_has_new_power();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_old_power;
        break;
      }

      // required uint32 old_power = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_old_power:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &old_power_)));
          set_has_old_power();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_cur_level;
        break;
      }

      // required uint32 cur_level = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_cur_level:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &cur_level_)));
          set_has_cur_level();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_levels;
        break;
      }

      // required bytes levels = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_levels:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_levels()));
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

void tb_role_levels::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->role_id(), output);
  }

  // required uint32 time = 2;
  if (has_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->time(), output);
  }

  // required uint32 new_level = 3;
  if (has_new_level()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->new_level(), output);
  }

  // required uint32 old_level = 4;
  if (has_old_level()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->old_level(), output);
  }

  // required uint32 new_power = 5;
  if (has_new_power()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->new_power(), output);
  }

  // required uint32 old_power = 6;
  if (has_old_power()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(6, this->old_power(), output);
  }

  // required uint32 cur_level = 7;
  if (has_cur_level()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(7, this->cur_level(), output);
  }

  // required bytes levels = 8;
  if (has_levels()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      8, this->levels(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_role_levels::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->role_id(), target);
  }

  // required uint32 time = 2;
  if (has_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->time(), target);
  }

  // required uint32 new_level = 3;
  if (has_new_level()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->new_level(), target);
  }

  // required uint32 old_level = 4;
  if (has_old_level()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->old_level(), target);
  }

  // required uint32 new_power = 5;
  if (has_new_power()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(5, this->new_power(), target);
  }

  // required uint32 old_power = 6;
  if (has_old_power()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(6, this->old_power(), target);
  }

  // required uint32 cur_level = 7;
  if (has_cur_level()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(7, this->cur_level(), target);
  }

  // required bytes levels = 8;
  if (has_levels()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        8, this->levels(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_role_levels::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 role_id = 1;
    if (has_role_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->role_id());
    }

    // required uint32 time = 2;
    if (has_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->time());
    }

    // required uint32 new_level = 3;
    if (has_new_level()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->new_level());
    }

    // required uint32 old_level = 4;
    if (has_old_level()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->old_level());
    }

    // required uint32 new_power = 5;
    if (has_new_power()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->new_power());
    }

    // required uint32 old_power = 6;
    if (has_old_power()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->old_power());
    }

    // required uint32 cur_level = 7;
    if (has_cur_level()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->cur_level());
    }

    // required bytes levels = 8;
    if (has_levels()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->levels());
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

void tb_role_levels::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_role_levels* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_role_levels*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_role_levels::MergeFrom(const tb_role_levels& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_role_id()) {
      set_role_id(from.role_id());
    }
    if (from.has_time()) {
      set_time(from.time());
    }
    if (from.has_new_level()) {
      set_new_level(from.new_level());
    }
    if (from.has_old_level()) {
      set_old_level(from.old_level());
    }
    if (from.has_new_power()) {
      set_new_power(from.new_power());
    }
    if (from.has_old_power()) {
      set_old_power(from.old_power());
    }
    if (from.has_cur_level()) {
      set_cur_level(from.cur_level());
    }
    if (from.has_levels()) {
      set_levels(from.levels());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_role_levels::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_role_levels::CopyFrom(const tb_role_levels& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_role_levels::IsInitialized() const {
  if ((_has_bits_[0] & 0x000000ff) != 0x000000ff) return false;

  return true;
}

void tb_role_levels::Swap(tb_role_levels* other) {
  if (other != this) {
    std::swap(role_id_, other->role_id_);
    std::swap(time_, other->time_);
    std::swap(new_level_, other->new_level_);
    std::swap(old_level_, other->old_level_);
    std::swap(new_power_, other->new_power_);
    std::swap(old_power_, other->old_power_);
    std::swap(cur_level_, other->cur_level_);
    std::swap(levels_, other->levels_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_role_levels::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_role_levels_descriptor_;
  metadata.reflection = tb_role_levels_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
