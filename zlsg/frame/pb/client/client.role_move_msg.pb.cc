// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.role_move_msg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "client.role_move_msg.pb.h"

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

const ::google::protobuf::Descriptor* move_info_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  move_info_reflection_ = NULL;
const ::google::protobuf::Descriptor* role_move_msg_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  role_move_msg_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* role_move_msg_MOVE_TYPE_descriptor_ = NULL;
const ::google::protobuf::Descriptor* role_postion_msg_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  role_postion_msg_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_client_2erole_5fmove_5fmsg_2eproto() {
  protobuf_AddDesc_client_2erole_5fmove_5fmsg_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "client.role_move_msg.proto");
  GOOGLE_CHECK(file != NULL);
  move_info_descriptor_ = file->message_type(0);
  static const int move_info_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(move_info, rds_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(move_info, mps_),
  };
  move_info_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      move_info_descriptor_,
      move_info::default_instance_,
      move_info_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(move_info, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(move_info, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(move_info));
  role_move_msg_descriptor_ = file->message_type(1);
  static const int role_move_msg_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_move_msg, move_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_move_msg, mi_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_move_msg, timestamp_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_move_msg, svr_head_),
  };
  role_move_msg_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      role_move_msg_descriptor_,
      role_move_msg::default_instance_,
      role_move_msg_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_move_msg, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_move_msg, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(role_move_msg));
  role_move_msg_MOVE_TYPE_descriptor_ = role_move_msg_descriptor_->enum_type(0);
  role_postion_msg_descriptor_ = file->message_type(2);
  static const int role_postion_msg_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, x_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, y_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, z_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, timestamp_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, rd_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, svr_head_),
  };
  role_postion_msg_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      role_postion_msg_descriptor_,
      role_postion_msg::default_instance_,
      role_postion_msg_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(role_postion_msg, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(role_postion_msg));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_client_2erole_5fmove_5fmsg_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    move_info_descriptor_, &move_info::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    role_move_msg_descriptor_, &role_move_msg::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    role_postion_msg_descriptor_, &role_postion_msg::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_client_2erole_5fmove_5fmsg_2eproto() {
  delete move_info::default_instance_;
  delete move_info_reflection_;
  delete role_move_msg::default_instance_;
  delete role_move_msg_reflection_;
  delete role_postion_msg::default_instance_;
  delete role_postion_msg_reflection_;
}

void protobuf_AddDesc_client_2erole_5fmove_5fmsg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::client::protobuf_AddDesc_client_2ecomm_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\032client.role_move_msg.proto\022\006client\032\021cl"
    "ient.comm.proto\"P\n\tmove_info\022\036\n\003rds\030\001 \002("
    "\0132\021.client.role_data\022#\n\003mps\030\002 \003(\0132\026.clie"
    "nt.client_map_pos\"\317\001\n\rrole_move_msg\0222\n\tm"
    "ove_type\030\001 \002(\0162\037.client.role_move_msg.MO"
    "VE_TYPE\022\035\n\002mi\030\002 \003(\0132\021.client.move_info\022\021"
    "\n\ttimestamp\030\003 \001(\r\022&\n\010svr_head\030\350\007 \001(\0132\023.c"
    "lient.server_head\"0\n\tMOVE_TYPE\022\021\n\rCLIENT"
    "_NOTIFY\020\000\022\020\n\014MOVE_CONFIRM\020\001\"\220\001\n\020role_pos"
    "tion_msg\022\t\n\001x\030\001 \002(\005\022\t\n\001y\030\002 \002(\005\022\t\n\001z\030\003 \002("
    "\005\022\024\n\ttimestamp\030\004 \002(\r:\0010\022\035\n\002rd\030\005 \001(\0132\021.cl"
    "ient.role_data\022&\n\010svr_head\030\350\007 \001(\0132\023.clie"
    "nt.server_head", 494);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "client.role_move_msg.proto", &protobuf_RegisterTypes);
  move_info::default_instance_ = new move_info();
  role_move_msg::default_instance_ = new role_move_msg();
  role_postion_msg::default_instance_ = new role_postion_msg();
  move_info::default_instance_->InitAsDefaultInstance();
  role_move_msg::default_instance_->InitAsDefaultInstance();
  role_postion_msg::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_client_2erole_5fmove_5fmsg_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_client_2erole_5fmove_5fmsg_2eproto {
  StaticDescriptorInitializer_client_2erole_5fmove_5fmsg_2eproto() {
    protobuf_AddDesc_client_2erole_5fmove_5fmsg_2eproto();
  }
} static_descriptor_initializer_client_2erole_5fmove_5fmsg_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int move_info::kRdsFieldNumber;
const int move_info::kMpsFieldNumber;
#endif  // !_MSC_VER

move_info::move_info()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void move_info::InitAsDefaultInstance() {
  rds_ = const_cast< ::client::role_data*>(&::client::role_data::default_instance());
}

move_info::move_info(const move_info& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void move_info::SharedCtor() {
  _cached_size_ = 0;
  rds_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

move_info::~move_info() {
  SharedDtor();
}

void move_info::SharedDtor() {
  if (this != default_instance_) {
    delete rds_;
  }
}

void move_info::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* move_info::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return move_info_descriptor_;
}

const move_info& move_info::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2erole_5fmove_5fmsg_2eproto();
  return *default_instance_;
}

move_info* move_info::default_instance_ = NULL;

move_info* move_info::New() const {
  return new move_info;
}

void move_info::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_rds()) {
      if (rds_ != NULL) rds_->::client::role_data::Clear();
    }
  }
  mps_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool move_info::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .client.role_data rds = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_rds()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_mps;
        break;
      }

      // repeated .client.client_map_pos mps = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_mps:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_mps()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_mps;
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

void move_info::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .client.role_data rds = 1;
  if (has_rds()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->rds(), output);
  }

  // repeated .client.client_map_pos mps = 2;
  for (int i = 0; i < this->mps_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->mps(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* move_info::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .client.role_data rds = 1;
  if (has_rds()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->rds(), target);
  }

  // repeated .client.client_map_pos mps = 2;
  for (int i = 0; i < this->mps_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->mps(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int move_info::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .client.role_data rds = 1;
    if (has_rds()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->rds());
    }

  }
  // repeated .client.client_map_pos mps = 2;
  total_size += 1 * this->mps_size();
  for (int i = 0; i < this->mps_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->mps(i));
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

void move_info::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const move_info* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const move_info*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void move_info::MergeFrom(const move_info& from) {
  GOOGLE_CHECK_NE(&from, this);
  mps_.MergeFrom(from.mps_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_rds()) {
      mutable_rds()->::client::role_data::MergeFrom(from.rds());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void move_info::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void move_info::CopyFrom(const move_info& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool move_info::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_rds()) {
    if (!this->rds().IsInitialized()) return false;
  }
  for (int i = 0; i < mps_size(); i++) {
    if (!this->mps(i).IsInitialized()) return false;
  }
  return true;
}

void move_info::Swap(move_info* other) {
  if (other != this) {
    std::swap(rds_, other->rds_);
    mps_.Swap(&other->mps_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata move_info::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = move_info_descriptor_;
  metadata.reflection = move_info_reflection_;
  return metadata;
}


// ===================================================================

const ::google::protobuf::EnumDescriptor* role_move_msg_MOVE_TYPE_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return role_move_msg_MOVE_TYPE_descriptor_;
}
bool role_move_msg_MOVE_TYPE_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const role_move_msg_MOVE_TYPE role_move_msg::CLIENT_NOTIFY;
const role_move_msg_MOVE_TYPE role_move_msg::MOVE_CONFIRM;
const role_move_msg_MOVE_TYPE role_move_msg::MOVE_TYPE_MIN;
const role_move_msg_MOVE_TYPE role_move_msg::MOVE_TYPE_MAX;
const int role_move_msg::MOVE_TYPE_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int role_move_msg::kMoveTypeFieldNumber;
const int role_move_msg::kMiFieldNumber;
const int role_move_msg::kTimestampFieldNumber;
const int role_move_msg::kSvrHeadFieldNumber;
#endif  // !_MSC_VER

role_move_msg::role_move_msg()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void role_move_msg::InitAsDefaultInstance() {
  svr_head_ = const_cast< ::client::server_head*>(&::client::server_head::default_instance());
}

role_move_msg::role_move_msg(const role_move_msg& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void role_move_msg::SharedCtor() {
  _cached_size_ = 0;
  move_type_ = 0;
  timestamp_ = 0u;
  svr_head_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

role_move_msg::~role_move_msg() {
  SharedDtor();
}

void role_move_msg::SharedDtor() {
  if (this != default_instance_) {
    delete svr_head_;
  }
}

void role_move_msg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* role_move_msg::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return role_move_msg_descriptor_;
}

const role_move_msg& role_move_msg::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2erole_5fmove_5fmsg_2eproto();
  return *default_instance_;
}

role_move_msg* role_move_msg::default_instance_ = NULL;

role_move_msg* role_move_msg::New() const {
  return new role_move_msg;
}

void role_move_msg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    move_type_ = 0;
    timestamp_ = 0u;
    if (has_svr_head()) {
      if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
    }
  }
  mi_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool role_move_msg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .client.role_move_msg.MOVE_TYPE move_type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::client::role_move_msg_MOVE_TYPE_IsValid(value)) {
            set_move_type(static_cast< ::client::role_move_msg_MOVE_TYPE >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_mi;
        break;
      }

      // repeated .client.move_info mi = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_mi:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_mi()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_mi;
        if (input->ExpectTag(24)) goto parse_timestamp;
        break;
      }

      // optional uint32 timestamp = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_timestamp:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &timestamp_)));
          set_has_timestamp();
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

void role_move_msg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .client.role_move_msg.MOVE_TYPE move_type = 1;
  if (has_move_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->move_type(), output);
  }

  // repeated .client.move_info mi = 2;
  for (int i = 0; i < this->mi_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->mi(i), output);
  }

  // optional uint32 timestamp = 3;
  if (has_timestamp()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->timestamp(), output);
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

::google::protobuf::uint8* role_move_msg::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .client.role_move_msg.MOVE_TYPE move_type = 1;
  if (has_move_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->move_type(), target);
  }

  // repeated .client.move_info mi = 2;
  for (int i = 0; i < this->mi_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->mi(i), target);
  }

  // optional uint32 timestamp = 3;
  if (has_timestamp()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->timestamp(), target);
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

int role_move_msg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .client.role_move_msg.MOVE_TYPE move_type = 1;
    if (has_move_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->move_type());
    }

    // optional uint32 timestamp = 3;
    if (has_timestamp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->timestamp());
    }

    // optional .client.server_head svr_head = 1000;
    if (has_svr_head()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->svr_head());
    }

  }
  // repeated .client.move_info mi = 2;
  total_size += 1 * this->mi_size();
  for (int i = 0; i < this->mi_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->mi(i));
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

void role_move_msg::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const role_move_msg* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const role_move_msg*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void role_move_msg::MergeFrom(const role_move_msg& from) {
  GOOGLE_CHECK_NE(&from, this);
  mi_.MergeFrom(from.mi_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_move_type()) {
      set_move_type(from.move_type());
    }
    if (from.has_timestamp()) {
      set_timestamp(from.timestamp());
    }
    if (from.has_svr_head()) {
      mutable_svr_head()->::client::server_head::MergeFrom(from.svr_head());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void role_move_msg::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void role_move_msg::CopyFrom(const role_move_msg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool role_move_msg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  for (int i = 0; i < mi_size(); i++) {
    if (!this->mi(i).IsInitialized()) return false;
  }
  return true;
}

void role_move_msg::Swap(role_move_msg* other) {
  if (other != this) {
    std::swap(move_type_, other->move_type_);
    mi_.Swap(&other->mi_);
    std::swap(timestamp_, other->timestamp_);
    std::swap(svr_head_, other->svr_head_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata role_move_msg::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = role_move_msg_descriptor_;
  metadata.reflection = role_move_msg_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int role_postion_msg::kXFieldNumber;
const int role_postion_msg::kYFieldNumber;
const int role_postion_msg::kZFieldNumber;
const int role_postion_msg::kTimestampFieldNumber;
const int role_postion_msg::kRdFieldNumber;
const int role_postion_msg::kSvrHeadFieldNumber;
#endif  // !_MSC_VER

role_postion_msg::role_postion_msg()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void role_postion_msg::InitAsDefaultInstance() {
  rd_ = const_cast< ::client::role_data*>(&::client::role_data::default_instance());
  svr_head_ = const_cast< ::client::server_head*>(&::client::server_head::default_instance());
}

role_postion_msg::role_postion_msg(const role_postion_msg& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void role_postion_msg::SharedCtor() {
  _cached_size_ = 0;
  x_ = 0;
  y_ = 0;
  z_ = 0;
  timestamp_ = 0u;
  rd_ = NULL;
  svr_head_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

role_postion_msg::~role_postion_msg() {
  SharedDtor();
}

void role_postion_msg::SharedDtor() {
  if (this != default_instance_) {
    delete rd_;
    delete svr_head_;
  }
}

void role_postion_msg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* role_postion_msg::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return role_postion_msg_descriptor_;
}

const role_postion_msg& role_postion_msg::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2erole_5fmove_5fmsg_2eproto();
  return *default_instance_;
}

role_postion_msg* role_postion_msg::default_instance_ = NULL;

role_postion_msg* role_postion_msg::New() const {
  return new role_postion_msg;
}

void role_postion_msg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    x_ = 0;
    y_ = 0;
    z_ = 0;
    timestamp_ = 0u;
    if (has_rd()) {
      if (rd_ != NULL) rd_->::client::role_data::Clear();
    }
    if (has_svr_head()) {
      if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool role_postion_msg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 x = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &x_)));
          set_has_x();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_y;
        break;
      }

      // required int32 y = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_y:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &y_)));
          set_has_y();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_z;
        break;
      }

      // required int32 z = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_z:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &z_)));
          set_has_z();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_timestamp;
        break;
      }

      // required uint32 timestamp = 4 [default = 0];
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_timestamp:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &timestamp_)));
          set_has_timestamp();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_rd;
        break;
      }

      // optional .client.role_data rd = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_rd:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_rd()));
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

void role_postion_msg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 x = 1;
  if (has_x()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->x(), output);
  }

  // required int32 y = 2;
  if (has_y()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->y(), output);
  }

  // required int32 z = 3;
  if (has_z()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->z(), output);
  }

  // required uint32 timestamp = 4 [default = 0];
  if (has_timestamp()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->timestamp(), output);
  }

  // optional .client.role_data rd = 5;
  if (has_rd()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      5, this->rd(), output);
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

::google::protobuf::uint8* role_postion_msg::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 x = 1;
  if (has_x()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->x(), target);
  }

  // required int32 y = 2;
  if (has_y()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->y(), target);
  }

  // required int32 z = 3;
  if (has_z()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->z(), target);
  }

  // required uint32 timestamp = 4 [default = 0];
  if (has_timestamp()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->timestamp(), target);
  }

  // optional .client.role_data rd = 5;
  if (has_rd()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        5, this->rd(), target);
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

int role_postion_msg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 x = 1;
    if (has_x()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->x());
    }

    // required int32 y = 2;
    if (has_y()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->y());
    }

    // required int32 z = 3;
    if (has_z()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->z());
    }

    // required uint32 timestamp = 4 [default = 0];
    if (has_timestamp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->timestamp());
    }

    // optional .client.role_data rd = 5;
    if (has_rd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->rd());
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

void role_postion_msg::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const role_postion_msg* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const role_postion_msg*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void role_postion_msg::MergeFrom(const role_postion_msg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_x()) {
      set_x(from.x());
    }
    if (from.has_y()) {
      set_y(from.y());
    }
    if (from.has_z()) {
      set_z(from.z());
    }
    if (from.has_timestamp()) {
      set_timestamp(from.timestamp());
    }
    if (from.has_rd()) {
      mutable_rd()->::client::role_data::MergeFrom(from.rd());
    }
    if (from.has_svr_head()) {
      mutable_svr_head()->::client::server_head::MergeFrom(from.svr_head());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void role_postion_msg::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void role_postion_msg::CopyFrom(const role_postion_msg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool role_postion_msg::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;

  if (has_rd()) {
    if (!this->rd().IsInitialized()) return false;
  }
  return true;
}

void role_postion_msg::Swap(role_postion_msg* other) {
  if (other != this) {
    std::swap(x_, other->x_);
    std::swap(y_, other->y_);
    std::swap(z_, other->z_);
    std::swap(timestamp_, other->timestamp_);
    std::swap(rd_, other->rd_);
    std::swap(svr_head_, other->svr_head_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata role_postion_msg::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = role_postion_msg_descriptor_;
  metadata.reflection = role_postion_msg_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace client

// @@protoc_insertion_point(global_scope)