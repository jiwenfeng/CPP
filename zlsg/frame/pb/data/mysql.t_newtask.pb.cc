// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.t_newtask.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.t_newtask.pb.h"

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

const ::google::protobuf::Descriptor* t_newtask_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  t_newtask_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2et_5fnewtask_2eproto() {
  protobuf_AddDesc_mysql_2et_5fnewtask_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.t_newtask.proto");
  GOOGLE_CHECK(file != NULL);
  t_newtask_descriptor_ = file->message_type(0);
  static const int t_newtask_offsets_[9] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, sid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, did_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, accountname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, playername_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, taskid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, status_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, quality_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, createdate_),
  };
  t_newtask_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      t_newtask_descriptor_,
      t_newtask::default_instance_,
      t_newtask_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_newtask, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(t_newtask));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2et_5fnewtask_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    t_newtask_descriptor_, &t_newtask::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2et_5fnewtask_2eproto() {
  delete t_newtask::default_instance_;
  delete t_newtask_reflection_;
}

void protobuf_AddDesc_mysql_2et_5fnewtask_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\025mysql.t_newtask.proto\022\005mysql\"\237\001\n\tt_new"
    "task\022\n\n\002id\030\001 \002(\r\022\013\n\003sid\030\002 \002(\t\022\013\n\003did\030\003 \002"
    "(\t\022\023\n\013accountName\030\004 \002(\t\022\022\n\nplayerName\030\005 "
    "\002(\t\022\016\n\006taskId\030\006 \002(\r\022\016\n\006status\030\007 \002(\r\022\017\n\007q"
    "uality\030\010 \002(\r\022\022\n\ncreateDate\030\t \002(\r", 192);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.t_newtask.proto", &protobuf_RegisterTypes);
  t_newtask::default_instance_ = new t_newtask();
  t_newtask::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2et_5fnewtask_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2et_5fnewtask_2eproto {
  StaticDescriptorInitializer_mysql_2et_5fnewtask_2eproto() {
    protobuf_AddDesc_mysql_2et_5fnewtask_2eproto();
  }
} static_descriptor_initializer_mysql_2et_5fnewtask_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int t_newtask::kIdFieldNumber;
const int t_newtask::kSidFieldNumber;
const int t_newtask::kDidFieldNumber;
const int t_newtask::kAccountNameFieldNumber;
const int t_newtask::kPlayerNameFieldNumber;
const int t_newtask::kTaskIdFieldNumber;
const int t_newtask::kStatusFieldNumber;
const int t_newtask::kQualityFieldNumber;
const int t_newtask::kCreateDateFieldNumber;
#endif  // !_MSC_VER

t_newtask::t_newtask()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void t_newtask::InitAsDefaultInstance() {
}

t_newtask::t_newtask(const t_newtask& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void t_newtask::SharedCtor() {
  _cached_size_ = 0;
  id_ = 0u;
  sid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  did_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  accountname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  playername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  taskid_ = 0u;
  status_ = 0u;
  quality_ = 0u;
  createdate_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

t_newtask::~t_newtask() {
  SharedDtor();
}

void t_newtask::SharedDtor() {
  if (sid_ != &::google::protobuf::internal::kEmptyString) {
    delete sid_;
  }
  if (did_ != &::google::protobuf::internal::kEmptyString) {
    delete did_;
  }
  if (accountname_ != &::google::protobuf::internal::kEmptyString) {
    delete accountname_;
  }
  if (playername_ != &::google::protobuf::internal::kEmptyString) {
    delete playername_;
  }
  if (this != default_instance_) {
  }
}

void t_newtask::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* t_newtask::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return t_newtask_descriptor_;
}

const t_newtask& t_newtask::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2et_5fnewtask_2eproto();
  return *default_instance_;
}

t_newtask* t_newtask::default_instance_ = NULL;

t_newtask* t_newtask::New() const {
  return new t_newtask;
}

void t_newtask::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    id_ = 0u;
    if (has_sid()) {
      if (sid_ != &::google::protobuf::internal::kEmptyString) {
        sid_->clear();
      }
    }
    if (has_did()) {
      if (did_ != &::google::protobuf::internal::kEmptyString) {
        did_->clear();
      }
    }
    if (has_accountname()) {
      if (accountname_ != &::google::protobuf::internal::kEmptyString) {
        accountname_->clear();
      }
    }
    if (has_playername()) {
      if (playername_ != &::google::protobuf::internal::kEmptyString) {
        playername_->clear();
      }
    }
    taskid_ = 0u;
    status_ = 0u;
    quality_ = 0u;
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    createdate_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool t_newtask::MergePartialFromCodedStream(
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
        if (input->ExpectTag(18)) goto parse_sid;
        break;
      }

      // required string sid = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_sid:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_sid()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->sid().data(), this->sid().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_did;
        break;
      }

      // required string did = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_did:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_did()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->did().data(), this->did().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_accountName;
        break;
      }

      // required string accountName = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_accountName:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_accountname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->accountname().data(), this->accountname().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_playerName;
        break;
      }

      // required string playerName = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_playerName:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_playername()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->playername().data(), this->playername().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_taskId;
        break;
      }

      // required uint32 taskId = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_taskId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &taskid_)));
          set_has_taskid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_status;
        break;
      }

      // required uint32 status = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_status:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &status_)));
          set_has_status();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(64)) goto parse_quality;
        break;
      }

      // required uint32 quality = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_quality:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &quality_)));
          set_has_quality();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(72)) goto parse_createDate;
        break;
      }

      // required uint32 createDate = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_createDate:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &createdate_)));
          set_has_createdate();
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

void t_newtask::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 id = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->id(), output);
  }

  // required string sid = 2;
  if (has_sid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->sid().data(), this->sid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->sid(), output);
  }

  // required string did = 3;
  if (has_did()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->did().data(), this->did().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->did(), output);
  }

  // required string accountName = 4;
  if (has_accountname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->accountname().data(), this->accountname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->accountname(), output);
  }

  // required string playerName = 5;
  if (has_playername()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->playername().data(), this->playername().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->playername(), output);
  }

  // required uint32 taskId = 6;
  if (has_taskid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(6, this->taskid(), output);
  }

  // required uint32 status = 7;
  if (has_status()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(7, this->status(), output);
  }

  // required uint32 quality = 8;
  if (has_quality()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(8, this->quality(), output);
  }

  // required uint32 createDate = 9;
  if (has_createdate()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(9, this->createdate(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* t_newtask::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 id = 1;
  if (has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->id(), target);
  }

  // required string sid = 2;
  if (has_sid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->sid().data(), this->sid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->sid(), target);
  }

  // required string did = 3;
  if (has_did()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->did().data(), this->did().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->did(), target);
  }

  // required string accountName = 4;
  if (has_accountname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->accountname().data(), this->accountname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->accountname(), target);
  }

  // required string playerName = 5;
  if (has_playername()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->playername().data(), this->playername().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->playername(), target);
  }

  // required uint32 taskId = 6;
  if (has_taskid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(6, this->taskid(), target);
  }

  // required uint32 status = 7;
  if (has_status()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(7, this->status(), target);
  }

  // required uint32 quality = 8;
  if (has_quality()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(8, this->quality(), target);
  }

  // required uint32 createDate = 9;
  if (has_createdate()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(9, this->createdate(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int t_newtask::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->id());
    }

    // required string sid = 2;
    if (has_sid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->sid());
    }

    // required string did = 3;
    if (has_did()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->did());
    }

    // required string accountName = 4;
    if (has_accountname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->accountname());
    }

    // required string playerName = 5;
    if (has_playername()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->playername());
    }

    // required uint32 taskId = 6;
    if (has_taskid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->taskid());
    }

    // required uint32 status = 7;
    if (has_status()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->status());
    }

    // required uint32 quality = 8;
    if (has_quality()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->quality());
    }

  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // required uint32 createDate = 9;
    if (has_createdate()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->createdate());
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

void t_newtask::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const t_newtask* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const t_newtask*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void t_newtask::MergeFrom(const t_newtask& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_sid()) {
      set_sid(from.sid());
    }
    if (from.has_did()) {
      set_did(from.did());
    }
    if (from.has_accountname()) {
      set_accountname(from.accountname());
    }
    if (from.has_playername()) {
      set_playername(from.playername());
    }
    if (from.has_taskid()) {
      set_taskid(from.taskid());
    }
    if (from.has_status()) {
      set_status(from.status());
    }
    if (from.has_quality()) {
      set_quality(from.quality());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from.has_createdate()) {
      set_createdate(from.createdate());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void t_newtask::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void t_newtask::CopyFrom(const t_newtask& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool t_newtask::IsInitialized() const {
  if ((_has_bits_[0] & 0x000001ff) != 0x000001ff) return false;

  return true;
}

void t_newtask::Swap(t_newtask* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(sid_, other->sid_);
    std::swap(did_, other->did_);
    std::swap(accountname_, other->accountname_);
    std::swap(playername_, other->playername_);
    std::swap(taskid_, other->taskid_);
    std::swap(status_, other->status_);
    std::swap(quality_, other->quality_);
    std::swap(createdate_, other->createdate_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata t_newtask::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = t_newtask_descriptor_;
  metadata.reflection = t_newtask_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)