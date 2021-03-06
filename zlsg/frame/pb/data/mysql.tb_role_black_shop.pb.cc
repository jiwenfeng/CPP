// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_role_black_shop.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_role_black_shop.pb.h"

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

const ::google::protobuf::Descriptor* tb_role_black_shop_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_role_black_shop_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5frole_5fblack_5fshop_2eproto() {
  protobuf_AddDesc_mysql_2etb_5frole_5fblack_5fshop_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_role_black_shop.proto");
  GOOGLE_CHECK(file != NULL);
  tb_role_black_shop_descriptor_ = file->message_type(0);
  static const int tb_role_black_shop_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_black_shop, role_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_black_shop, black_shops_),
  };
  tb_role_black_shop_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_role_black_shop_descriptor_,
      tb_role_black_shop::default_instance_,
      tb_role_black_shop_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_black_shop, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_black_shop, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_role_black_shop));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5frole_5fblack_5fshop_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_role_black_shop_descriptor_, &tb_role_black_shop::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5frole_5fblack_5fshop_2eproto() {
  delete tb_role_black_shop::default_instance_;
  delete tb_role_black_shop_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5frole_5fblack_5fshop_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\036mysql.tb_role_black_shop.proto\022\005mysql\""
    ":\n\022tb_role_black_shop\022\017\n\007role_id\030\001 \002(\r\022\023"
    "\n\013black_shops\030\002 \002(\014", 99);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_role_black_shop.proto", &protobuf_RegisterTypes);
  tb_role_black_shop::default_instance_ = new tb_role_black_shop();
  tb_role_black_shop::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5frole_5fblack_5fshop_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5frole_5fblack_5fshop_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5frole_5fblack_5fshop_2eproto() {
    protobuf_AddDesc_mysql_2etb_5frole_5fblack_5fshop_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5frole_5fblack_5fshop_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_role_black_shop::kRoleIdFieldNumber;
const int tb_role_black_shop::kBlackShopsFieldNumber;
#endif  // !_MSC_VER

tb_role_black_shop::tb_role_black_shop()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_role_black_shop::InitAsDefaultInstance() {
}

tb_role_black_shop::tb_role_black_shop(const tb_role_black_shop& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_role_black_shop::SharedCtor() {
  _cached_size_ = 0;
  role_id_ = 0u;
  black_shops_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_role_black_shop::~tb_role_black_shop() {
  SharedDtor();
}

void tb_role_black_shop::SharedDtor() {
  if (black_shops_ != &::google::protobuf::internal::kEmptyString) {
    delete black_shops_;
  }
  if (this != default_instance_) {
  }
}

void tb_role_black_shop::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_role_black_shop::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_role_black_shop_descriptor_;
}

const tb_role_black_shop& tb_role_black_shop::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5frole_5fblack_5fshop_2eproto();
  return *default_instance_;
}

tb_role_black_shop* tb_role_black_shop::default_instance_ = NULL;

tb_role_black_shop* tb_role_black_shop::New() const {
  return new tb_role_black_shop;
}

void tb_role_black_shop::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    role_id_ = 0u;
    if (has_black_shops()) {
      if (black_shops_ != &::google::protobuf::internal::kEmptyString) {
        black_shops_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_role_black_shop::MergePartialFromCodedStream(
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
        if (input->ExpectTag(18)) goto parse_black_shops;
        break;
      }

      // required bytes black_shops = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_black_shops:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_black_shops()));
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

void tb_role_black_shop::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->role_id(), output);
  }

  // required bytes black_shops = 2;
  if (has_black_shops()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->black_shops(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_role_black_shop::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->role_id(), target);
  }

  // required bytes black_shops = 2;
  if (has_black_shops()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->black_shops(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_role_black_shop::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 role_id = 1;
    if (has_role_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->role_id());
    }

    // required bytes black_shops = 2;
    if (has_black_shops()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->black_shops());
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

void tb_role_black_shop::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_role_black_shop* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_role_black_shop*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_role_black_shop::MergeFrom(const tb_role_black_shop& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_role_id()) {
      set_role_id(from.role_id());
    }
    if (from.has_black_shops()) {
      set_black_shops(from.black_shops());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_role_black_shop::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_role_black_shop::CopyFrom(const tb_role_black_shop& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_role_black_shop::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void tb_role_black_shop::Swap(tb_role_black_shop* other) {
  if (other != this) {
    std::swap(role_id_, other->role_id_);
    std::swap(black_shops_, other->black_shops_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_role_black_shop::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_role_black_shop_descriptor_;
  metadata.reflection = tb_role_black_shop_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
