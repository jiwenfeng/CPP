// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_role_label.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_role_label.pb.h"

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

const ::google::protobuf::Descriptor* tb_role_label_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_role_label_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5frole_5flabel_2eproto() {
  protobuf_AddDesc_mysql_2etb_5frole_5flabel_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_role_label.proto");
  GOOGLE_CHECK(file != NULL);
  tb_role_label_descriptor_ = file->message_type(0);
  static const int tb_role_label_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_label, role_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_label, label_code_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_label, use_state_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_label, effective_state_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_label, end_date_),
  };
  tb_role_label_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_role_label_descriptor_,
      tb_role_label::default_instance_,
      tb_role_label_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_label, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_label, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_role_label));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5frole_5flabel_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_role_label_descriptor_, &tb_role_label::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5frole_5flabel_2eproto() {
  delete tb_role_label::default_instance_;
  delete tb_role_label_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5frole_5flabel_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\031mysql.tb_role_label.proto\022\005mysql\"r\n\rtb"
    "_role_label\022\017\n\007role_id\030\001 \002(\r\022\022\n\nlabel_co"
    "de\030\002 \002(\r\022\021\n\tuse_state\030\003 \002(\r\022\027\n\017effective"
    "_state\030\004 \002(\r\022\020\n\010end_date\030\005 \002(\r", 150);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_role_label.proto", &protobuf_RegisterTypes);
  tb_role_label::default_instance_ = new tb_role_label();
  tb_role_label::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5frole_5flabel_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5frole_5flabel_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5frole_5flabel_2eproto() {
    protobuf_AddDesc_mysql_2etb_5frole_5flabel_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5frole_5flabel_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_role_label::kRoleIdFieldNumber;
const int tb_role_label::kLabelCodeFieldNumber;
const int tb_role_label::kUseStateFieldNumber;
const int tb_role_label::kEffectiveStateFieldNumber;
const int tb_role_label::kEndDateFieldNumber;
#endif  // !_MSC_VER

tb_role_label::tb_role_label()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_role_label::InitAsDefaultInstance() {
}

tb_role_label::tb_role_label(const tb_role_label& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_role_label::SharedCtor() {
  _cached_size_ = 0;
  role_id_ = 0u;
  label_code_ = 0u;
  use_state_ = 0u;
  effective_state_ = 0u;
  end_date_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_role_label::~tb_role_label() {
  SharedDtor();
}

void tb_role_label::SharedDtor() {
  if (this != default_instance_) {
  }
}

void tb_role_label::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_role_label::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_role_label_descriptor_;
}

const tb_role_label& tb_role_label::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5frole_5flabel_2eproto();
  return *default_instance_;
}

tb_role_label* tb_role_label::default_instance_ = NULL;

tb_role_label* tb_role_label::New() const {
  return new tb_role_label;
}

void tb_role_label::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    role_id_ = 0u;
    label_code_ = 0u;
    use_state_ = 0u;
    effective_state_ = 0u;
    end_date_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_role_label::MergePartialFromCodedStream(
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
        if (input->ExpectTag(16)) goto parse_label_code;
        break;
      }

      // required uint32 label_code = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_label_code:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &label_code_)));
          set_has_label_code();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_use_state;
        break;
      }

      // required uint32 use_state = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_use_state:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &use_state_)));
          set_has_use_state();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_effective_state;
        break;
      }

      // required uint32 effective_state = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_effective_state:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &effective_state_)));
          set_has_effective_state();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_end_date;
        break;
      }

      // required uint32 end_date = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_end_date:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &end_date_)));
          set_has_end_date();
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

void tb_role_label::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->role_id(), output);
  }

  // required uint32 label_code = 2;
  if (has_label_code()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->label_code(), output);
  }

  // required uint32 use_state = 3;
  if (has_use_state()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->use_state(), output);
  }

  // required uint32 effective_state = 4;
  if (has_effective_state()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->effective_state(), output);
  }

  // required uint32 end_date = 5;
  if (has_end_date()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->end_date(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_role_label::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->role_id(), target);
  }

  // required uint32 label_code = 2;
  if (has_label_code()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->label_code(), target);
  }

  // required uint32 use_state = 3;
  if (has_use_state()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->use_state(), target);
  }

  // required uint32 effective_state = 4;
  if (has_effective_state()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->effective_state(), target);
  }

  // required uint32 end_date = 5;
  if (has_end_date()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(5, this->end_date(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_role_label::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 role_id = 1;
    if (has_role_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->role_id());
    }

    // required uint32 label_code = 2;
    if (has_label_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->label_code());
    }

    // required uint32 use_state = 3;
    if (has_use_state()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->use_state());
    }

    // required uint32 effective_state = 4;
    if (has_effective_state()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->effective_state());
    }

    // required uint32 end_date = 5;
    if (has_end_date()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->end_date());
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

void tb_role_label::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_role_label* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_role_label*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_role_label::MergeFrom(const tb_role_label& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_role_id()) {
      set_role_id(from.role_id());
    }
    if (from.has_label_code()) {
      set_label_code(from.label_code());
    }
    if (from.has_use_state()) {
      set_use_state(from.use_state());
    }
    if (from.has_effective_state()) {
      set_effective_state(from.effective_state());
    }
    if (from.has_end_date()) {
      set_end_date(from.end_date());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_role_label::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_role_label::CopyFrom(const tb_role_label& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_role_label::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;

  return true;
}

void tb_role_label::Swap(tb_role_label* other) {
  if (other != this) {
    std::swap(role_id_, other->role_id_);
    std::swap(label_code_, other->label_code_);
    std::swap(use_state_, other->use_state_);
    std::swap(effective_state_, other->effective_state_);
    std::swap(end_date_, other->end_date_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_role_label::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_role_label_descriptor_;
  metadata.reflection = tb_role_label_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
