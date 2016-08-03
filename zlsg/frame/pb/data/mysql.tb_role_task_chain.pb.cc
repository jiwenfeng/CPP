// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_role_task_chain.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_role_task_chain.pb.h"

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

const ::google::protobuf::Descriptor* tb_role_task_chain_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_role_task_chain_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5frole_5ftask_5fchain_2eproto() {
  protobuf_AddDesc_mysql_2etb_5frole_5ftask_5fchain_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_role_task_chain.proto");
  GOOGLE_CHECK(file != NULL);
  tb_role_task_chain_descriptor_ = file->message_type(0);
  static const int tb_role_task_chain_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, role_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, chain_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, succ_times_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, submit_index_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, created_date_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, cnt_),
  };
  tb_role_task_chain_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_role_task_chain_descriptor_,
      tb_role_task_chain::default_instance_,
      tb_role_task_chain_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_task_chain, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_role_task_chain));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5frole_5ftask_5fchain_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_role_task_chain_descriptor_, &tb_role_task_chain::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5frole_5ftask_5fchain_2eproto() {
  delete tb_role_task_chain::default_instance_;
  delete tb_role_task_chain_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5frole_5ftask_5fchain_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\036mysql.tb_role_task_chain.proto\022\005mysql\""
    "\204\001\n\022tb_role_task_chain\022\017\n\007role_id\030\001 \002(\r\022"
    "\020\n\010chain_id\030\002 \002(\r\022\022\n\nsucc_times\030\003 \002(\r\022\024\n"
    "\014submit_index\030\004 \002(\r\022\024\n\014created_date\030\005 \002("
    "\r\022\013\n\003cnt\030\006 \002(\r", 174);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_role_task_chain.proto", &protobuf_RegisterTypes);
  tb_role_task_chain::default_instance_ = new tb_role_task_chain();
  tb_role_task_chain::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5frole_5ftask_5fchain_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5frole_5ftask_5fchain_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5frole_5ftask_5fchain_2eproto() {
    protobuf_AddDesc_mysql_2etb_5frole_5ftask_5fchain_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5frole_5ftask_5fchain_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_role_task_chain::kRoleIdFieldNumber;
const int tb_role_task_chain::kChainIdFieldNumber;
const int tb_role_task_chain::kSuccTimesFieldNumber;
const int tb_role_task_chain::kSubmitIndexFieldNumber;
const int tb_role_task_chain::kCreatedDateFieldNumber;
const int tb_role_task_chain::kCntFieldNumber;
#endif  // !_MSC_VER

tb_role_task_chain::tb_role_task_chain()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_role_task_chain::InitAsDefaultInstance() {
}

tb_role_task_chain::tb_role_task_chain(const tb_role_task_chain& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_role_task_chain::SharedCtor() {
  _cached_size_ = 0;
  role_id_ = 0u;
  chain_id_ = 0u;
  succ_times_ = 0u;
  submit_index_ = 0u;
  created_date_ = 0u;
  cnt_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_role_task_chain::~tb_role_task_chain() {
  SharedDtor();
}

void tb_role_task_chain::SharedDtor() {
  if (this != default_instance_) {
  }
}

void tb_role_task_chain::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_role_task_chain::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_role_task_chain_descriptor_;
}

const tb_role_task_chain& tb_role_task_chain::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5frole_5ftask_5fchain_2eproto();
  return *default_instance_;
}

tb_role_task_chain* tb_role_task_chain::default_instance_ = NULL;

tb_role_task_chain* tb_role_task_chain::New() const {
  return new tb_role_task_chain;
}

void tb_role_task_chain::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    role_id_ = 0u;
    chain_id_ = 0u;
    succ_times_ = 0u;
    submit_index_ = 0u;
    created_date_ = 0u;
    cnt_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_role_task_chain::MergePartialFromCodedStream(
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
        if (input->ExpectTag(16)) goto parse_chain_id;
        break;
      }

      // required uint32 chain_id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_chain_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &chain_id_)));
          set_has_chain_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_succ_times;
        break;
      }

      // required uint32 succ_times = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_succ_times:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &succ_times_)));
          set_has_succ_times();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_submit_index;
        break;
      }

      // required uint32 submit_index = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_submit_index:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &submit_index_)));
          set_has_submit_index();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_created_date;
        break;
      }

      // required uint32 created_date = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_created_date:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &created_date_)));
          set_has_created_date();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_cnt;
        break;
      }

      // required uint32 cnt = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_cnt:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &cnt_)));
          set_has_cnt();
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

void tb_role_task_chain::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->role_id(), output);
  }

  // required uint32 chain_id = 2;
  if (has_chain_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->chain_id(), output);
  }

  // required uint32 succ_times = 3;
  if (has_succ_times()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->succ_times(), output);
  }

  // required uint32 submit_index = 4;
  if (has_submit_index()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->submit_index(), output);
  }

  // required uint32 created_date = 5;
  if (has_created_date()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->created_date(), output);
  }

  // required uint32 cnt = 6;
  if (has_cnt()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(6, this->cnt(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_role_task_chain::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 role_id = 1;
  if (has_role_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->role_id(), target);
  }

  // required uint32 chain_id = 2;
  if (has_chain_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->chain_id(), target);
  }

  // required uint32 succ_times = 3;
  if (has_succ_times()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->succ_times(), target);
  }

  // required uint32 submit_index = 4;
  if (has_submit_index()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->submit_index(), target);
  }

  // required uint32 created_date = 5;
  if (has_created_date()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(5, this->created_date(), target);
  }

  // required uint32 cnt = 6;
  if (has_cnt()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(6, this->cnt(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_role_task_chain::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 role_id = 1;
    if (has_role_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->role_id());
    }

    // required uint32 chain_id = 2;
    if (has_chain_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->chain_id());
    }

    // required uint32 succ_times = 3;
    if (has_succ_times()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->succ_times());
    }

    // required uint32 submit_index = 4;
    if (has_submit_index()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->submit_index());
    }

    // required uint32 created_date = 5;
    if (has_created_date()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->created_date());
    }

    // required uint32 cnt = 6;
    if (has_cnt()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->cnt());
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

void tb_role_task_chain::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_role_task_chain* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_role_task_chain*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_role_task_chain::MergeFrom(const tb_role_task_chain& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_role_id()) {
      set_role_id(from.role_id());
    }
    if (from.has_chain_id()) {
      set_chain_id(from.chain_id());
    }
    if (from.has_succ_times()) {
      set_succ_times(from.succ_times());
    }
    if (from.has_submit_index()) {
      set_submit_index(from.submit_index());
    }
    if (from.has_created_date()) {
      set_created_date(from.created_date());
    }
    if (from.has_cnt()) {
      set_cnt(from.cnt());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_role_task_chain::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_role_task_chain::CopyFrom(const tb_role_task_chain& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_role_task_chain::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000003f) != 0x0000003f) return false;

  return true;
}

void tb_role_task_chain::Swap(tb_role_task_chain* other) {
  if (other != this) {
    std::swap(role_id_, other->role_id_);
    std::swap(chain_id_, other->chain_id_);
    std::swap(succ_times_, other->succ_times_);
    std::swap(submit_index_, other->submit_index_);
    std::swap(created_date_, other->created_date_);
    std::swap(cnt_, other->cnt_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_role_task_chain::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_role_task_chain_descriptor_;
  metadata.reflection = tb_role_task_chain_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
