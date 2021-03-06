// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.role_cd_cooldown.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "client.role_cd_cooldown.pb.h"

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

const ::google::protobuf::Descriptor* cd_info_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  cd_info_reflection_ = NULL;
const ::google::protobuf::Descriptor* cd_rsp_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  cd_rsp_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* CD_COOLDOWN_TYPE_ID_E_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* COOL_DOWN_FAGS_E_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* COOLDOWN_TYPE_E_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* CD_RESULT_TYPE_E_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_client_2erole_5fcd_5fcooldown_2eproto() {
  protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "client.role_cd_cooldown.proto");
  GOOGLE_CHECK(file != NULL);
  cd_info_descriptor_ = file->message_type(0);
  static const int cd_info_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_info, type_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_info, cooldown_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_info, life_time_),
  };
  cd_info_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      cd_info_descriptor_,
      cd_info::default_instance_,
      cd_info_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_info, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_info, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(cd_info));
  cd_rsp_descriptor_ = file->message_type(1);
  static const int cd_rsp_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_rsp, result_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_rsp, type_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_rsp, life_time_),
  };
  cd_rsp_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      cd_rsp_descriptor_,
      cd_rsp::default_instance_,
      cd_rsp_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_rsp, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(cd_rsp, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(cd_rsp));
  CD_COOLDOWN_TYPE_ID_E_descriptor_ = file->enum_type(0);
  COOL_DOWN_FAGS_E_descriptor_ = file->enum_type(1);
  COOLDOWN_TYPE_E_descriptor_ = file->enum_type(2);
  CD_RESULT_TYPE_E_descriptor_ = file->enum_type(3);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_client_2erole_5fcd_5fcooldown_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    cd_info_descriptor_, &cd_info::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    cd_rsp_descriptor_, &cd_rsp::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_client_2erole_5fcd_5fcooldown_2eproto() {
  delete cd_info::default_instance_;
  delete cd_info_reflection_;
  delete cd_rsp::default_instance_;
  delete cd_rsp_reflection_;
}

void protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::client::protobuf_AddDesc_client_2ecomm_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\035client.role_cd_cooldown.proto\022\006client\032"
    "\021client.comm.proto\"D\n\007cd_info\022\017\n\007type_id"
    "\030\001 \002(\r\022\025\n\rcooldown_type\030\002 \002(\r\022\021\n\tlife_ti"
    "me\030\003 \002(\r\"<\n\006cd_rsp\022\016\n\006result\030\001 \002(\r\022\017\n\007ty"
    "pe_id\030\002 \002(\r\022\021\n\tlife_time\030\003 \002(\r*`\n\025CD_COO"
    "LDOWN_TYPE_ID_E\022$\n CD_COOLDOW_TYPE_ID_TE"
    "LEPORTATION\020\001\022!\n\035CD_COOLDOW_TYPE_ID_BLAC"
    "K_SHOP\020\002*L\n\020COOL_DOWN_FAGS_E\022\014\n\010CDF_NONE"
    "\020\000\022\020\n\014CDF_NEEDSAVE\020\001\022\030\n\024CDF_NEEDSENDTOCL"
    "IENT\020\002*G\n\017COOLDOWN_TYPE_E\022\021\n\rCDT_COUNTDO"
    "WN\020\000\022\016\n\nCDT_REMAIN\020\001\022\021\n\rCDT_MAX_LIMIT\020\002*"
    "R\n\020CD_RESULT_TYPE_E\022\035\n\031CD_RESULT_TYPE_NO"
    "T_ENOUGH\020\000\022\037\n\033CD_RESULT_TYPE_ACCUMULATIO"
    "N\020\001", 523);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "client.role_cd_cooldown.proto", &protobuf_RegisterTypes);
  cd_info::default_instance_ = new cd_info();
  cd_rsp::default_instance_ = new cd_rsp();
  cd_info::default_instance_->InitAsDefaultInstance();
  cd_rsp::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_client_2erole_5fcd_5fcooldown_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_client_2erole_5fcd_5fcooldown_2eproto {
  StaticDescriptorInitializer_client_2erole_5fcd_5fcooldown_2eproto() {
    protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto();
  }
} static_descriptor_initializer_client_2erole_5fcd_5fcooldown_2eproto_;
const ::google::protobuf::EnumDescriptor* CD_COOLDOWN_TYPE_ID_E_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CD_COOLDOWN_TYPE_ID_E_descriptor_;
}
bool CD_COOLDOWN_TYPE_ID_E_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* COOL_DOWN_FAGS_E_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return COOL_DOWN_FAGS_E_descriptor_;
}
bool COOL_DOWN_FAGS_E_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* COOLDOWN_TYPE_E_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return COOLDOWN_TYPE_E_descriptor_;
}
bool COOLDOWN_TYPE_E_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* CD_RESULT_TYPE_E_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CD_RESULT_TYPE_E_descriptor_;
}
bool CD_RESULT_TYPE_E_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int cd_info::kTypeIdFieldNumber;
const int cd_info::kCooldownTypeFieldNumber;
const int cd_info::kLifeTimeFieldNumber;
#endif  // !_MSC_VER

cd_info::cd_info()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void cd_info::InitAsDefaultInstance() {
}

cd_info::cd_info(const cd_info& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void cd_info::SharedCtor() {
  _cached_size_ = 0;
  type_id_ = 0u;
  cooldown_type_ = 0u;
  life_time_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

cd_info::~cd_info() {
  SharedDtor();
}

void cd_info::SharedDtor() {
  if (this != default_instance_) {
  }
}

void cd_info::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* cd_info::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return cd_info_descriptor_;
}

const cd_info& cd_info::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto();
  return *default_instance_;
}

cd_info* cd_info::default_instance_ = NULL;

cd_info* cd_info::New() const {
  return new cd_info;
}

void cd_info::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_id_ = 0u;
    cooldown_type_ = 0u;
    life_time_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool cd_info::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 type_id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &type_id_)));
          set_has_type_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_cooldown_type;
        break;
      }

      // required uint32 cooldown_type = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_cooldown_type:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &cooldown_type_)));
          set_has_cooldown_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_life_time;
        break;
      }

      // required uint32 life_time = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_life_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &life_time_)));
          set_has_life_time();
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

void cd_info::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 type_id = 1;
  if (has_type_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->type_id(), output);
  }

  // required uint32 cooldown_type = 2;
  if (has_cooldown_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->cooldown_type(), output);
  }

  // required uint32 life_time = 3;
  if (has_life_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->life_time(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* cd_info::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 type_id = 1;
  if (has_type_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->type_id(), target);
  }

  // required uint32 cooldown_type = 2;
  if (has_cooldown_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->cooldown_type(), target);
  }

  // required uint32 life_time = 3;
  if (has_life_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->life_time(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int cd_info::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 type_id = 1;
    if (has_type_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->type_id());
    }

    // required uint32 cooldown_type = 2;
    if (has_cooldown_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->cooldown_type());
    }

    // required uint32 life_time = 3;
    if (has_life_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->life_time());
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

void cd_info::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const cd_info* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const cd_info*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void cd_info::MergeFrom(const cd_info& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type_id()) {
      set_type_id(from.type_id());
    }
    if (from.has_cooldown_type()) {
      set_cooldown_type(from.cooldown_type());
    }
    if (from.has_life_time()) {
      set_life_time(from.life_time());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void cd_info::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void cd_info::CopyFrom(const cd_info& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool cd_info::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void cd_info::Swap(cd_info* other) {
  if (other != this) {
    std::swap(type_id_, other->type_id_);
    std::swap(cooldown_type_, other->cooldown_type_);
    std::swap(life_time_, other->life_time_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata cd_info::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = cd_info_descriptor_;
  metadata.reflection = cd_info_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int cd_rsp::kResultFieldNumber;
const int cd_rsp::kTypeIdFieldNumber;
const int cd_rsp::kLifeTimeFieldNumber;
#endif  // !_MSC_VER

cd_rsp::cd_rsp()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void cd_rsp::InitAsDefaultInstance() {
}

cd_rsp::cd_rsp(const cd_rsp& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void cd_rsp::SharedCtor() {
  _cached_size_ = 0;
  result_ = 0u;
  type_id_ = 0u;
  life_time_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

cd_rsp::~cd_rsp() {
  SharedDtor();
}

void cd_rsp::SharedDtor() {
  if (this != default_instance_) {
  }
}

void cd_rsp::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* cd_rsp::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return cd_rsp_descriptor_;
}

const cd_rsp& cd_rsp::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto();
  return *default_instance_;
}

cd_rsp* cd_rsp::default_instance_ = NULL;

cd_rsp* cd_rsp::New() const {
  return new cd_rsp;
}

void cd_rsp::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    result_ = 0u;
    type_id_ = 0u;
    life_time_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool cd_rsp::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 result = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &result_)));
          set_has_result();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_type_id;
        break;
      }

      // required uint32 type_id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_type_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &type_id_)));
          set_has_type_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_life_time;
        break;
      }

      // required uint32 life_time = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_life_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &life_time_)));
          set_has_life_time();
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

void cd_rsp::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 result = 1;
  if (has_result()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->result(), output);
  }

  // required uint32 type_id = 2;
  if (has_type_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->type_id(), output);
  }

  // required uint32 life_time = 3;
  if (has_life_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->life_time(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* cd_rsp::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 result = 1;
  if (has_result()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->result(), target);
  }

  // required uint32 type_id = 2;
  if (has_type_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->type_id(), target);
  }

  // required uint32 life_time = 3;
  if (has_life_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->life_time(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int cd_rsp::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 result = 1;
    if (has_result()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->result());
    }

    // required uint32 type_id = 2;
    if (has_type_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->type_id());
    }

    // required uint32 life_time = 3;
    if (has_life_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->life_time());
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

void cd_rsp::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const cd_rsp* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const cd_rsp*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void cd_rsp::MergeFrom(const cd_rsp& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_result()) {
      set_result(from.result());
    }
    if (from.has_type_id()) {
      set_type_id(from.type_id());
    }
    if (from.has_life_time()) {
      set_life_time(from.life_time());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void cd_rsp::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void cd_rsp::CopyFrom(const cd_rsp& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool cd_rsp::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void cd_rsp::Swap(cd_rsp* other) {
  if (other != this) {
    std::swap(result_, other->result_);
    std::swap(type_id_, other->type_id_);
    std::swap(life_time_, other->life_time_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata cd_rsp::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = cd_rsp_descriptor_;
  metadata.reflection = cd_rsp_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace client

// @@protoc_insertion_point(global_scope)
