// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_role_recharge.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mysql.tb_role_recharge.pb.h"

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

const ::google::protobuf::Descriptor* tb_role_recharge_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  tb_role_recharge_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_mysql_2etb_5frole_5frecharge_2eproto() {
  protobuf_AddDesc_mysql_2etb_5frole_5frecharge_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mysql.tb_role_recharge.proto");
  GOOGLE_CHECK(file != NULL);
  tb_role_recharge_descriptor_ = file->message_type(0);
  static const int tb_role_recharge_offsets_[14] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, openid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, openkey_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, pf_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, ts_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, payitem_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, token_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, billno_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, zoneid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, providetype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, amt_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, payamt_coins_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, pubacct_payamt_coins_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, state_),
  };
  tb_role_recharge_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      tb_role_recharge_descriptor_,
      tb_role_recharge::default_instance_,
      tb_role_recharge_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(tb_role_recharge, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(tb_role_recharge));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mysql_2etb_5frole_5frecharge_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    tb_role_recharge_descriptor_, &tb_role_recharge::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mysql_2etb_5frole_5frecharge_2eproto() {
  delete tb_role_recharge::default_instance_;
  delete tb_role_recharge_reflection_;
}

void protobuf_AddDesc_mysql_2etb_5frole_5frecharge_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\034mysql.tb_role_recharge.proto\022\005mysql\"\374\001"
    "\n\020tb_role_recharge\022\n\n\002id\030\001 \002(\r\022\016\n\006openid"
    "\030\002 \002(\t\022\017\n\007openkey\030\003 \002(\t\022\n\n\002pf\030\004 \002(\t\022\n\n\002t"
    "s\030\005 \002(\r\022\017\n\007payitem\030\006 \002(\t\022\r\n\005token\030\007 \002(\t\022"
    "\016\n\006billno\030\010 \002(\t\022\016\n\006zoneid\030\t \002(\r\022\023\n\013provi"
    "detype\030\n \002(\r\022\013\n\003amt\030\013 \002(\r\022\024\n\014payamt_coin"
    "s\030\014 \002(\r\022\034\n\024pubacct_payamt_coins\030\r \002(\r\022\r\n"
    "\005state\030\016 \002(\r", 292);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mysql.tb_role_recharge.proto", &protobuf_RegisterTypes);
  tb_role_recharge::default_instance_ = new tb_role_recharge();
  tb_role_recharge::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mysql_2etb_5frole_5frecharge_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mysql_2etb_5frole_5frecharge_2eproto {
  StaticDescriptorInitializer_mysql_2etb_5frole_5frecharge_2eproto() {
    protobuf_AddDesc_mysql_2etb_5frole_5frecharge_2eproto();
  }
} static_descriptor_initializer_mysql_2etb_5frole_5frecharge_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int tb_role_recharge::kIdFieldNumber;
const int tb_role_recharge::kOpenidFieldNumber;
const int tb_role_recharge::kOpenkeyFieldNumber;
const int tb_role_recharge::kPfFieldNumber;
const int tb_role_recharge::kTsFieldNumber;
const int tb_role_recharge::kPayitemFieldNumber;
const int tb_role_recharge::kTokenFieldNumber;
const int tb_role_recharge::kBillnoFieldNumber;
const int tb_role_recharge::kZoneidFieldNumber;
const int tb_role_recharge::kProvidetypeFieldNumber;
const int tb_role_recharge::kAmtFieldNumber;
const int tb_role_recharge::kPayamtCoinsFieldNumber;
const int tb_role_recharge::kPubacctPayamtCoinsFieldNumber;
const int tb_role_recharge::kStateFieldNumber;
#endif  // !_MSC_VER

tb_role_recharge::tb_role_recharge()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void tb_role_recharge::InitAsDefaultInstance() {
}

tb_role_recharge::tb_role_recharge(const tb_role_recharge& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void tb_role_recharge::SharedCtor() {
  _cached_size_ = 0;
  id_ = 0u;
  openid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  openkey_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  pf_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ts_ = 0u;
  payitem_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  token_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  billno_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  zoneid_ = 0u;
  providetype_ = 0u;
  amt_ = 0u;
  payamt_coins_ = 0u;
  pubacct_payamt_coins_ = 0u;
  state_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

tb_role_recharge::~tb_role_recharge() {
  SharedDtor();
}

void tb_role_recharge::SharedDtor() {
  if (openid_ != &::google::protobuf::internal::kEmptyString) {
    delete openid_;
  }
  if (openkey_ != &::google::protobuf::internal::kEmptyString) {
    delete openkey_;
  }
  if (pf_ != &::google::protobuf::internal::kEmptyString) {
    delete pf_;
  }
  if (payitem_ != &::google::protobuf::internal::kEmptyString) {
    delete payitem_;
  }
  if (token_ != &::google::protobuf::internal::kEmptyString) {
    delete token_;
  }
  if (billno_ != &::google::protobuf::internal::kEmptyString) {
    delete billno_;
  }
  if (this != default_instance_) {
  }
}

void tb_role_recharge::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* tb_role_recharge::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return tb_role_recharge_descriptor_;
}

const tb_role_recharge& tb_role_recharge::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mysql_2etb_5frole_5frecharge_2eproto();
  return *default_instance_;
}

tb_role_recharge* tb_role_recharge::default_instance_ = NULL;

tb_role_recharge* tb_role_recharge::New() const {
  return new tb_role_recharge;
}

void tb_role_recharge::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    id_ = 0u;
    if (has_openid()) {
      if (openid_ != &::google::protobuf::internal::kEmptyString) {
        openid_->clear();
      }
    }
    if (has_openkey()) {
      if (openkey_ != &::google::protobuf::internal::kEmptyString) {
        openkey_->clear();
      }
    }
    if (has_pf()) {
      if (pf_ != &::google::protobuf::internal::kEmptyString) {
        pf_->clear();
      }
    }
    ts_ = 0u;
    if (has_payitem()) {
      if (payitem_ != &::google::protobuf::internal::kEmptyString) {
        payitem_->clear();
      }
    }
    if (has_token()) {
      if (token_ != &::google::protobuf::internal::kEmptyString) {
        token_->clear();
      }
    }
    if (has_billno()) {
      if (billno_ != &::google::protobuf::internal::kEmptyString) {
        billno_->clear();
      }
    }
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    zoneid_ = 0u;
    providetype_ = 0u;
    amt_ = 0u;
    payamt_coins_ = 0u;
    pubacct_payamt_coins_ = 0u;
    state_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool tb_role_recharge::MergePartialFromCodedStream(
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
        if (input->ExpectTag(18)) goto parse_openid;
        break;
      }

      // required string openid = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_openid:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_openid()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->openid().data(), this->openid().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_openkey;
        break;
      }

      // required string openkey = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_openkey:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_openkey()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->openkey().data(), this->openkey().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_pf;
        break;
      }

      // required string pf = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_pf:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_pf()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->pf().data(), this->pf().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_ts;
        break;
      }

      // required uint32 ts = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_ts:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &ts_)));
          set_has_ts();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_payitem;
        break;
      }

      // required string payitem = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_payitem:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_payitem()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->payitem().data(), this->payitem().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_token;
        break;
      }

      // required string token = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_token:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_token()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->token().data(), this->token().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_billno;
        break;
      }

      // required string billno = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_billno:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_billno()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->billno().data(), this->billno().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(72)) goto parse_zoneid;
        break;
      }

      // required uint32 zoneid = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_zoneid:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &zoneid_)));
          set_has_zoneid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(80)) goto parse_providetype;
        break;
      }

      // required uint32 providetype = 10;
      case 10: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_providetype:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &providetype_)));
          set_has_providetype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(88)) goto parse_amt;
        break;
      }

      // required uint32 amt = 11;
      case 11: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_amt:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &amt_)));
          set_has_amt();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(96)) goto parse_payamt_coins;
        break;
      }

      // required uint32 payamt_coins = 12;
      case 12: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_payamt_coins:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &payamt_coins_)));
          set_has_payamt_coins();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(104)) goto parse_pubacct_payamt_coins;
        break;
      }

      // required uint32 pubacct_payamt_coins = 13;
      case 13: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_pubacct_payamt_coins:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &pubacct_payamt_coins_)));
          set_has_pubacct_payamt_coins();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(112)) goto parse_state;
        break;
      }

      // required uint32 state = 14;
      case 14: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_state:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &state_)));
          set_has_state();
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

void tb_role_recharge::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 id = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->id(), output);
  }

  // required string openid = 2;
  if (has_openid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->openid().data(), this->openid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->openid(), output);
  }

  // required string openkey = 3;
  if (has_openkey()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->openkey().data(), this->openkey().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->openkey(), output);
  }

  // required string pf = 4;
  if (has_pf()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->pf().data(), this->pf().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->pf(), output);
  }

  // required uint32 ts = 5;
  if (has_ts()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->ts(), output);
  }

  // required string payitem = 6;
  if (has_payitem()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->payitem().data(), this->payitem().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->payitem(), output);
  }

  // required string token = 7;
  if (has_token()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->token().data(), this->token().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      7, this->token(), output);
  }

  // required string billno = 8;
  if (has_billno()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->billno().data(), this->billno().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->billno(), output);
  }

  // required uint32 zoneid = 9;
  if (has_zoneid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(9, this->zoneid(), output);
  }

  // required uint32 providetype = 10;
  if (has_providetype()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(10, this->providetype(), output);
  }

  // required uint32 amt = 11;
  if (has_amt()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(11, this->amt(), output);
  }

  // required uint32 payamt_coins = 12;
  if (has_payamt_coins()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(12, this->payamt_coins(), output);
  }

  // required uint32 pubacct_payamt_coins = 13;
  if (has_pubacct_payamt_coins()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(13, this->pubacct_payamt_coins(), output);
  }

  // required uint32 state = 14;
  if (has_state()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(14, this->state(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* tb_role_recharge::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 id = 1;
  if (has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->id(), target);
  }

  // required string openid = 2;
  if (has_openid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->openid().data(), this->openid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->openid(), target);
  }

  // required string openkey = 3;
  if (has_openkey()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->openkey().data(), this->openkey().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->openkey(), target);
  }

  // required string pf = 4;
  if (has_pf()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->pf().data(), this->pf().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->pf(), target);
  }

  // required uint32 ts = 5;
  if (has_ts()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(5, this->ts(), target);
  }

  // required string payitem = 6;
  if (has_payitem()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->payitem().data(), this->payitem().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->payitem(), target);
  }

  // required string token = 7;
  if (has_token()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->token().data(), this->token().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        7, this->token(), target);
  }

  // required string billno = 8;
  if (has_billno()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->billno().data(), this->billno().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->billno(), target);
  }

  // required uint32 zoneid = 9;
  if (has_zoneid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(9, this->zoneid(), target);
  }

  // required uint32 providetype = 10;
  if (has_providetype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(10, this->providetype(), target);
  }

  // required uint32 amt = 11;
  if (has_amt()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(11, this->amt(), target);
  }

  // required uint32 payamt_coins = 12;
  if (has_payamt_coins()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(12, this->payamt_coins(), target);
  }

  // required uint32 pubacct_payamt_coins = 13;
  if (has_pubacct_payamt_coins()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(13, this->pubacct_payamt_coins(), target);
  }

  // required uint32 state = 14;
  if (has_state()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(14, this->state(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int tb_role_recharge::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->id());
    }

    // required string openid = 2;
    if (has_openid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->openid());
    }

    // required string openkey = 3;
    if (has_openkey()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->openkey());
    }

    // required string pf = 4;
    if (has_pf()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->pf());
    }

    // required uint32 ts = 5;
    if (has_ts()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->ts());
    }

    // required string payitem = 6;
    if (has_payitem()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->payitem());
    }

    // required string token = 7;
    if (has_token()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->token());
    }

    // required string billno = 8;
    if (has_billno()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->billno());
    }

  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // required uint32 zoneid = 9;
    if (has_zoneid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->zoneid());
    }

    // required uint32 providetype = 10;
    if (has_providetype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->providetype());
    }

    // required uint32 amt = 11;
    if (has_amt()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->amt());
    }

    // required uint32 payamt_coins = 12;
    if (has_payamt_coins()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->payamt_coins());
    }

    // required uint32 pubacct_payamt_coins = 13;
    if (has_pubacct_payamt_coins()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->pubacct_payamt_coins());
    }

    // required uint32 state = 14;
    if (has_state()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->state());
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

void tb_role_recharge::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const tb_role_recharge* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const tb_role_recharge*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void tb_role_recharge::MergeFrom(const tb_role_recharge& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_openid()) {
      set_openid(from.openid());
    }
    if (from.has_openkey()) {
      set_openkey(from.openkey());
    }
    if (from.has_pf()) {
      set_pf(from.pf());
    }
    if (from.has_ts()) {
      set_ts(from.ts());
    }
    if (from.has_payitem()) {
      set_payitem(from.payitem());
    }
    if (from.has_token()) {
      set_token(from.token());
    }
    if (from.has_billno()) {
      set_billno(from.billno());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from.has_zoneid()) {
      set_zoneid(from.zoneid());
    }
    if (from.has_providetype()) {
      set_providetype(from.providetype());
    }
    if (from.has_amt()) {
      set_amt(from.amt());
    }
    if (from.has_payamt_coins()) {
      set_payamt_coins(from.payamt_coins());
    }
    if (from.has_pubacct_payamt_coins()) {
      set_pubacct_payamt_coins(from.pubacct_payamt_coins());
    }
    if (from.has_state()) {
      set_state(from.state());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void tb_role_recharge::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void tb_role_recharge::CopyFrom(const tb_role_recharge& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool tb_role_recharge::IsInitialized() const {
  if ((_has_bits_[0] & 0x00003fff) != 0x00003fff) return false;

  return true;
}

void tb_role_recharge::Swap(tb_role_recharge* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(openid_, other->openid_);
    std::swap(openkey_, other->openkey_);
    std::swap(pf_, other->pf_);
    std::swap(ts_, other->ts_);
    std::swap(payitem_, other->payitem_);
    std::swap(token_, other->token_);
    std::swap(billno_, other->billno_);
    std::swap(zoneid_, other->zoneid_);
    std::swap(providetype_, other->providetype_);
    std::swap(amt_, other->amt_);
    std::swap(payamt_coins_, other->payamt_coins_);
    std::swap(pubacct_payamt_coins_, other->pubacct_payamt_coins_);
    std::swap(state_, other->state_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata tb_role_recharge::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = tb_role_recharge_descriptor_;
  metadata.reflection = tb_role_recharge_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

// @@protoc_insertion_point(global_scope)
