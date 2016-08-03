// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_role_center_set.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_role_center_set.pb.h"

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

const ::google::protobuf::Descriptor* tb_role_center_set_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_role_center_set_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5frole_5fcenter_5fset_2eproto() {
  protobuf_AddDesc_mysql_2etb_5frole_5fcenter_5fset_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_role_center_set.proto");
  GOOGLE_CHECK(file != NULL);
  tb_role_center_set_descriptor_ = file->message_type(0);
  static const int tb_role_center_set_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_center_set, role_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_center_set, team_auto_agree_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_center_set, team_auto_join_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_center_set, team_min_lvl_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_center_set, team_max_lvl_),
  };
  tb_role_center_set_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_role_center_set_descriptor_,
      tb_role_center_set::default_instance_,
      tb_role_center_set_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_center_set, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_center_set, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_role_center_set));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5frole_5fcenter_5fset_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_role_center_set_descriptor_, &tb_role_center_set::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5frole_5fcenter_5fset_2eproto() {
  delete tb_role_center_set::default_instance_;
  delete tb_role_center_set_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5frole_5fcenter_5fset_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\036mysql.tb_role_center_set.proto\022\005mysql\""
    "\202\001\n\022tb_role_center_set\022\017\n\007role_id\030\001 \002(\r\022"
    "\027\n\017team_auto_agree\030\002 \002(\r\022\026\n\016team_auto_jo"
    "in\030\003 \002(\r\022\024\n\014team_min_lvl\030\004 \002(\r\022\024\n\014team_m"
    "ax_lvl\030\005 \002(\r", 172);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_role_center_set.proto", &protobuf_RegisterTypes);
  tb_role_center_set::default_instance_ = new tb_role_center_set();
  tb_role_center_set::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5frole_5fcenter_5fset_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5frole_5fcenter_5fset_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5frole_5fcenter_5fset_2eproto() {
    protobuf_AddDesc_mysql_2etb_5frole_5fcenter_5fset_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5frole_5fcenter_5fset_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_role_center_set::kRoleIdFieldNumber;
const int tb_role_center_set::kTeamAutoAgreeFieldNumber;
const int tb_role_center_set::kTeamAutoJoinFieldNumber;
const int tb_role_center_set::kTeamMinLvlFieldNumber;
const int tb_role_center_set::kTeamMaxLvlFieldNumber;
#endif  // !_MSC_VER

tb_role_center_set::tb_role_center_set()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_role_center_set::InitAsDefaultInstance() {
}

tb_role_center_set::tb_role_center_set(const tb_role_center_set& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_role_center_set::SharedCtor() {
  _cached_size_ = 0;
  role_id_ = 0u;
  team_auto_agree_ = 0u;
  team_auto_join_ = 0u;
  team_min_lvl_ = 0u;
  team_max_lvl_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_role_center_set::~tb_role_center_set() {
  SharedDtor();
}

void tb_role_center_set::SharedDtor() {
  if (this != default_instance_) {
  }
}

void tb_role_center_set::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_role_center_set::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_role_center_set_descriptor_;
}

const tb_role_center_set& tb_role_center_set::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5frole_5fcenter_5fset_2eproto();
  return *default_instance_;
}

tb_role_center_set* tb_role_center_set::default_instance_ = NULL;

tb_role_center_set* tb_role_center_set::New() const {
  return new tb_role_center_set;
}

void tb_role_center_set::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    role_id_ = 0u;
    team_auto_agree_ = 0u;
    team_auto_join_ = 0u;
    team_min_lvl_ = 0u;
    team_max_lvl_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_role_center_set::MergePartialFromCodedStream(
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
        if (input->ExpectTag(16)) goto parse_team_auto_agree;
        break;
      }

      // required uint32 team_auto_agree = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_team_auto_agree:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &team_auto_agree_)));
          set_has_team_auto_agree();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_team_auto_join;
        break;
      }

      // required uint32 team_auto_join = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_team_auto_join:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &team_auto_join_)));
          set_has_team_auto_join();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_team_min_lvl;
        break;
      }

      // required uint32 team_min_lvl = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_team_min_lvl:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &team_min_lvl_)));
          set_has_team_min_lvl();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_team_max_lvl;
        break;
      }

      // required uint32 team_max_lvl = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_team_max_lvl:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &team_max_lvl_)));
          set_has_team_max_lvl();
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

void tb_role_center_set::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->role_id(), output);
  }

  // required uint32 team_auto_agree = 2;
  if (has_team_auto_agree()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->team_auto_agree(), output);
  }

  // required uint32 team_auto_join = 3;
  if (has_team_auto_join()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->team_auto_join(), output);
  }

  // required uint32 team_min_lvl = 4;
  if (has_team_min_lvl()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->team_min_lvl(), output);
  }

  // required uint32 team_max_lvl = 5;
  if (has_team_max_lvl()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->team_max_lvl(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_role_center_set::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->role_id(), target);
  }

  // required uint32 team_auto_agree = 2;
  if (has_team_auto_agree()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->team_auto_agree(), target);
  }

  // required uint32 team_auto_join = 3;
  if (has_team_auto_join()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->team_auto_join(), target);
  }

  // required uint32 team_min_lvl = 4;
  if (has_team_min_lvl()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->team_min_lvl(), target);
  }

  // required uint32 team_max_lvl = 5;
  if (has_team_max_lvl()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(5, this->team_max_lvl(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_role_center_set::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 role_id = 1;
    if (has_role_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->role_id());
    }

    // required uint32 team_auto_agree = 2;
    if (has_team_auto_agree()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->team_auto_agree());
    }

    // required uint32 team_auto_join = 3;
    if (has_team_auto_join()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->team_auto_join());
    }

    // required uint32 team_min_lvl = 4;
    if (has_team_min_lvl()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->team_min_lvl());
    }

    // required uint32 team_max_lvl = 5;
    if (has_team_max_lvl()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->team_max_lvl());
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

void tb_role_center_set::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_role_center_set* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_role_center_set*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_role_center_set::MergeFrom(const tb_role_center_set& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_role_id()) {
      set_role_id(from.role_id());
    }
    if (from.has_team_auto_agree()) {
      set_team_auto_agree(from.team_auto_agree());
    }
    if (from.has_team_auto_join()) {
      set_team_auto_join(from.team_auto_join());
    }
    if (from.has_team_min_lvl()) {
      set_team_min_lvl(from.team_min_lvl());
    }
    if (from.has_team_max_lvl()) {
      set_team_max_lvl(from.team_max_lvl());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_role_center_set::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_role_center_set::CopyFrom(const tb_role_center_set& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_role_center_set::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;

  return true;
}

void tb_role_center_set::Swap(tb_role_center_set* other) {
  if (other != this) {
    std::swap(role_id_, other->role_id_);
    std::swap(team_auto_agree_, other->team_auto_agree_);
    std::swap(team_auto_join_, other->team_auto_join_);
    std::swap(team_min_lvl_, other->team_min_lvl_);
    std::swap(team_max_lvl_, other->team_max_lvl_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_role_center_set::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_role_center_set_descriptor_;
  metadata.reflection = tb_role_center_set_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
