// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.t_moneylogconfig.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.t_moneylogconfig.pb.h"

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

const ::google::protobuf::Descriptor* t_moneylogconfig_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  t_moneylogconfig_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2et_5fmoneylogconfig_2eproto() {
  protobuf_AddDesc_mysql_2et_5fmoneylogconfig_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.t_moneylogconfig.proto");
  GOOGLE_CHECK(file != NULL);
  t_moneylogconfig_descriptor_ = file->message_type(0);
  static const int t_moneylogconfig_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_moneylogconfig, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_moneylogconfig, rootid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_moneylogconfig, rootname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_moneylogconfig, typeid__),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_moneylogconfig, typename__),
  };
  t_moneylogconfig_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      t_moneylogconfig_descriptor_,
      t_moneylogconfig::default_instance_,
      t_moneylogconfig_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_moneylogconfig, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(t_moneylogconfig, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(t_moneylogconfig));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2et_5fmoneylogconfig_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    t_moneylogconfig_descriptor_, &t_moneylogconfig::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2et_5fmoneylogconfig_2eproto() {
  delete t_moneylogconfig::default_instance_;
  delete t_moneylogconfig_reflection_;
}

void protobuf_AddDesc_mysql_2et_5fmoneylogconfig_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\034mysql.t_moneylogconfig.proto\022\005mysql\"b\n"
    "\020t_moneylogconfig\022\n\n\002id\030\001 \002(\r\022\016\n\006rootid\030"
    "\002 \002(\r\022\020\n\010rootname\030\003 \002(\t\022\016\n\006typeid\030\004 \002(\r\022"
    "\020\n\010typename\030\005 \002(\t", 137);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.t_moneylogconfig.proto", &protobuf_RegisterTypes);
  t_moneylogconfig::default_instance_ = new t_moneylogconfig();
  t_moneylogconfig::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2et_5fmoneylogconfig_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2et_5fmoneylogconfig_2eproto {
  StaticDescriptorInitializer_mysql_2et_5fmoneylogconfig_2eproto() {
    protobuf_AddDesc_mysql_2et_5fmoneylogconfig_2eproto();
  }
} static_descriptor_initializer_mysql_2et_5fmoneylogconfig_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int t_moneylogconfig::kIdFieldNumber;
const int t_moneylogconfig::kRootidFieldNumber;
const int t_moneylogconfig::kRootnameFieldNumber;
const int t_moneylogconfig::kTypeidFieldNumber;
const int t_moneylogconfig::kTypenameFieldNumber;
#endif  // !_MSC_VER

t_moneylogconfig::t_moneylogconfig()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void t_moneylogconfig::InitAsDefaultInstance() {
}

t_moneylogconfig::t_moneylogconfig(const t_moneylogconfig& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void t_moneylogconfig::SharedCtor() {
  _cached_size_ = 0;
  id_ = 0u;
  rootid_ = 0u;
  rootname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  typeid__ = 0u;
  typename__ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

t_moneylogconfig::~t_moneylogconfig() {
  SharedDtor();
}

void t_moneylogconfig::SharedDtor() {
  if (rootname_ != &::google::protobuf::internal::kEmptyString) {
    delete rootname_;
  }
  if (typename__ != &::google::protobuf::internal::kEmptyString) {
    delete typename__;
  }
  if (this != default_instance_) {
  }
}

void t_moneylogconfig::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* t_moneylogconfig::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return t_moneylogconfig_descriptor_;
}

const t_moneylogconfig& t_moneylogconfig::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2et_5fmoneylogconfig_2eproto();
  return *default_instance_;
}

t_moneylogconfig* t_moneylogconfig::default_instance_ = NULL;

t_moneylogconfig* t_moneylogconfig::New() const {
  return new t_moneylogconfig;
}

void t_moneylogconfig::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    id_ = 0u;
    rootid_ = 0u;
    if (has_rootname()) {
      if (rootname_ != &::google::protobuf::internal::kEmptyString) {
        rootname_->clear();
      }
    }
    typeid__ = 0u;
    if (has_typename_()) {
      if (typename__ != &::google::protobuf::internal::kEmptyString) {
        typename__->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool t_moneylogconfig::MergePartialFromCodedStream(
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
        if (input->ExpectTag(16)) goto parse_rootid;
        break;
      }

      // required uint32 rootid = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_rootid:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &rootid_)));
          set_has_rootid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_rootname;
        break;
      }

      // required string rootname = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_rootname:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_rootname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->rootname().data(), this->rootname().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_typeid;
        break;
      }

      // required uint32 typeid = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_typeid:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &typeid__)));
          set_has_typeid_();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_typename;
        break;
      }

      // required string typename = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_typename:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_typename_()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->typename_().data(), this->typename_().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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

void t_moneylogconfig::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 id = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->id(), output);
  }

  // required uint32 rootid = 2;
  if (has_rootid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->rootid(), output);
  }

  // required string rootname = 3;
  if (has_rootname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->rootname().data(), this->rootname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->rootname(), output);
  }

  // required uint32 typeid = 4;
  if (has_typeid_()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->typeid_(), output);
  }

  // required string typename = 5;
  if (has_typename_()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->typename_().data(), this->typename_().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->typename_(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* t_moneylogconfig::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 id = 1;
  if (has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->id(), target);
  }

  // required uint32 rootid = 2;
  if (has_rootid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->rootid(), target);
  }

  // required string rootname = 3;
  if (has_rootname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->rootname().data(), this->rootname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->rootname(), target);
  }

  // required uint32 typeid = 4;
  if (has_typeid_()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->typeid_(), target);
  }

  // required string typename = 5;
  if (has_typename_()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->typename_().data(), this->typename_().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->typename_(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int t_moneylogconfig::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->id());
    }

    // required uint32 rootid = 2;
    if (has_rootid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->rootid());
    }

    // required string rootname = 3;
    if (has_rootname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->rootname());
    }

    // required uint32 typeid = 4;
    if (has_typeid_()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->typeid_());
    }

    // required string typename = 5;
    if (has_typename_()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->typename_());
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

void t_moneylogconfig::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const t_moneylogconfig* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const t_moneylogconfig*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void t_moneylogconfig::MergeFrom(const t_moneylogconfig& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_rootid()) {
      set_rootid(from.rootid());
    }
    if (from.has_rootname()) {
      set_rootname(from.rootname());
    }
    if (from.has_typeid_()) {
      set_typeid_(from.typeid_());
    }
    if (from.has_typename_()) {
      set_typename_(from.typename_());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void t_moneylogconfig::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void t_moneylogconfig::CopyFrom(const t_moneylogconfig& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool t_moneylogconfig::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;

  return true;
}

void t_moneylogconfig::Swap(t_moneylogconfig* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(rootid_, other->rootid_);
    std::swap(rootname_, other->rootname_);
    std::swap(typeid__, other->typeid__);
    std::swap(typename__, other->typename__);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata t_moneylogconfig::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = t_moneylogconfig_descriptor_;
  metadata.reflection = t_moneylogconfig_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
