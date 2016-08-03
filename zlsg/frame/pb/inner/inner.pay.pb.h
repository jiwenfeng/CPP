// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: inner.pay.proto

#ifndef PROTOBUF_inner_2epay_2eproto__INCLUDED
#define PROTOBUF_inner_2epay_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "inner.comm.pb.h"
// @@protoc_insertion_point(includes)

namespace inner {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_inner_2epay_2eproto();
void protobuf_AssignDesc_inner_2epay_2eproto();
void protobuf_ShutdownFile_inner_2epay_2eproto();

class pay_req;
class pay_rsp;

// ===================================================================

class pay_req : public ::google::protobuf::Message {
 public:
  pay_req();
  virtual ~pay_req();

  pay_req(const pay_req& from);

  inline pay_req& operator=(const pay_req& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const pay_req& default_instance();

  void Swap(pay_req* other);

  // implements Message ----------------------------------------------

  pay_req* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const pay_req& from);
  void MergeFrom(const pay_req& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 amt = 1;
  inline bool has_amt() const;
  inline void clear_amt();
  static const int kAmtFieldNumber = 1;
  inline ::google::protobuf::uint32 amt() const;
  inline void set_amt(::google::protobuf::uint32 value);

  // required .inner.inner_user user = 2;
  inline bool has_user() const;
  inline void clear_user();
  static const int kUserFieldNumber = 2;
  inline const ::inner::inner_user& user() const;
  inline ::inner::inner_user* mutable_user();
  inline ::inner::inner_user* release_user();
  inline void set_allocated_user(::inner::inner_user* user);

  // optional string pay_id = 3;
  inline bool has_pay_id() const;
  inline void clear_pay_id();
  static const int kPayIdFieldNumber = 3;
  inline const ::std::string& pay_id() const;
  inline void set_pay_id(const ::std::string& value);
  inline void set_pay_id(const char* value);
  inline void set_pay_id(const char* value, size_t size);
  inline ::std::string* mutable_pay_id();
  inline ::std::string* release_pay_id();
  inline void set_allocated_pay_id(::std::string* pay_id);

  // optional string currency = 4;
  inline bool has_currency() const;
  inline void clear_currency();
  static const int kCurrencyFieldNumber = 4;
  inline const ::std::string& currency() const;
  inline void set_currency(const ::std::string& value);
  inline void set_currency(const char* value);
  inline void set_currency(const char* value, size_t size);
  inline ::std::string* mutable_currency();
  inline ::std::string* release_currency();
  inline void set_allocated_currency(::std::string* currency);

  // optional string inpour_type = 5;
  inline bool has_inpour_type() const;
  inline void clear_inpour_type();
  static const int kInpourTypeFieldNumber = 5;
  inline const ::std::string& inpour_type() const;
  inline void set_inpour_type(const ::std::string& value);
  inline void set_inpour_type(const char* value);
  inline void set_inpour_type(const char* value, size_t size);
  inline ::std::string* mutable_inpour_type();
  inline ::std::string* release_inpour_type();
  inline void set_allocated_inpour_type(::std::string* inpour_type);

  // optional uint32 addcoin = 6;
  inline bool has_addcoin() const;
  inline void clear_addcoin();
  static const int kAddcoinFieldNumber = 6;
  inline ::google::protobuf::uint32 addcoin() const;
  inline void set_addcoin(::google::protobuf::uint32 value);

  // optional uint32 ex_type = 7;
  inline bool has_ex_type() const;
  inline void clear_ex_type();
  static const int kExTypeFieldNumber = 7;
  inline ::google::protobuf::uint32 ex_type() const;
  inline void set_ex_type(::google::protobuf::uint32 value);

  // optional double depay = 8;
  inline bool has_depay() const;
  inline void clear_depay();
  static const int kDepayFieldNumber = 8;
  inline double depay() const;
  inline void set_depay(double value);

  // optional double g_depay = 9;
  inline bool has_g_depay() const;
  inline void clear_g_depay();
  static const int kGDepayFieldNumber = 9;
  inline double g_depay() const;
  inline void set_g_depay(double value);

  // optional .inner.inner_head head = 1000;
  inline bool has_head() const;
  inline void clear_head();
  static const int kHeadFieldNumber = 1000;
  inline const ::inner::inner_head& head() const;
  inline ::inner::inner_head* mutable_head();
  inline ::inner::inner_head* release_head();
  inline void set_allocated_head(::inner::inner_head* head);

  // @@protoc_insertion_point(class_scope:inner.pay_req)
 private:
  inline void set_has_amt();
  inline void clear_has_amt();
  inline void set_has_user();
  inline void clear_has_user();
  inline void set_has_pay_id();
  inline void clear_has_pay_id();
  inline void set_has_currency();
  inline void clear_has_currency();
  inline void set_has_inpour_type();
  inline void clear_has_inpour_type();
  inline void set_has_addcoin();
  inline void clear_has_addcoin();
  inline void set_has_ex_type();
  inline void clear_has_ex_type();
  inline void set_has_depay();
  inline void clear_has_depay();
  inline void set_has_g_depay();
  inline void clear_has_g_depay();
  inline void set_has_head();
  inline void clear_has_head();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::inner::inner_user* user_;
  ::std::string* pay_id_;
  ::google::protobuf::uint32 amt_;
  ::google::protobuf::uint32 addcoin_;
  ::std::string* currency_;
  ::std::string* inpour_type_;
  double depay_;
  double g_depay_;
  ::inner::inner_head* head_;
  ::google::protobuf::uint32 ex_type_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(10 + 31) / 32];

  friend void  protobuf_AddDesc_inner_2epay_2eproto();
  friend void protobuf_AssignDesc_inner_2epay_2eproto();
  friend void protobuf_ShutdownFile_inner_2epay_2eproto();

  void InitAsDefaultInstance();
  static pay_req* default_instance_;
};
// -------------------------------------------------------------------

class pay_rsp : public ::google::protobuf::Message {
 public:
  pay_rsp();
  virtual ~pay_rsp();

  pay_rsp(const pay_rsp& from);

  inline pay_rsp& operator=(const pay_rsp& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const pay_rsp& default_instance();

  void Swap(pay_rsp* other);

  // implements Message ----------------------------------------------

  pay_rsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const pay_rsp& from);
  void MergeFrom(const pay_rsp& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 amt = 1;
  inline bool has_amt() const;
  inline void clear_amt();
  static const int kAmtFieldNumber = 1;
  inline ::google::protobuf::uint32 amt() const;
  inline void set_amt(::google::protobuf::uint32 value);

  // required .inner.inner_user user = 2;
  inline bool has_user() const;
  inline void clear_user();
  static const int kUserFieldNumber = 2;
  inline const ::inner::inner_user& user() const;
  inline ::inner::inner_user* mutable_user();
  inline ::inner::inner_user* release_user();
  inline void set_allocated_user(::inner::inner_user* user);

  // optional string pay_id = 3;
  inline bool has_pay_id() const;
  inline void clear_pay_id();
  static const int kPayIdFieldNumber = 3;
  inline const ::std::string& pay_id() const;
  inline void set_pay_id(const ::std::string& value);
  inline void set_pay_id(const char* value);
  inline void set_pay_id(const char* value, size_t size);
  inline ::std::string* mutable_pay_id();
  inline ::std::string* release_pay_id();
  inline void set_allocated_pay_id(::std::string* pay_id);

  // optional string currency = 4;
  inline bool has_currency() const;
  inline void clear_currency();
  static const int kCurrencyFieldNumber = 4;
  inline const ::std::string& currency() const;
  inline void set_currency(const ::std::string& value);
  inline void set_currency(const char* value);
  inline void set_currency(const char* value, size_t size);
  inline ::std::string* mutable_currency();
  inline ::std::string* release_currency();
  inline void set_allocated_currency(::std::string* currency);

  // optional string inpour_type = 5;
  inline bool has_inpour_type() const;
  inline void clear_inpour_type();
  static const int kInpourTypeFieldNumber = 5;
  inline const ::std::string& inpour_type() const;
  inline void set_inpour_type(const ::std::string& value);
  inline void set_inpour_type(const char* value);
  inline void set_inpour_type(const char* value, size_t size);
  inline ::std::string* mutable_inpour_type();
  inline ::std::string* release_inpour_type();
  inline void set_allocated_inpour_type(::std::string* inpour_type);

  // optional uint32 addcoin = 6;
  inline bool has_addcoin() const;
  inline void clear_addcoin();
  static const int kAddcoinFieldNumber = 6;
  inline ::google::protobuf::uint32 addcoin() const;
  inline void set_addcoin(::google::protobuf::uint32 value);

  // optional uint32 ex_type = 7;
  inline bool has_ex_type() const;
  inline void clear_ex_type();
  static const int kExTypeFieldNumber = 7;
  inline ::google::protobuf::uint32 ex_type() const;
  inline void set_ex_type(::google::protobuf::uint32 value);

  // optional double depay = 8;
  inline bool has_depay() const;
  inline void clear_depay();
  static const int kDepayFieldNumber = 8;
  inline double depay() const;
  inline void set_depay(double value);

  // optional double g_depay = 9;
  inline bool has_g_depay() const;
  inline void clear_g_depay();
  static const int kGDepayFieldNumber = 9;
  inline double g_depay() const;
  inline void set_g_depay(double value);

  // required .inner.inner_tail tail = 1000;
  inline bool has_tail() const;
  inline void clear_tail();
  static const int kTailFieldNumber = 1000;
  inline const ::inner::inner_tail& tail() const;
  inline ::inner::inner_tail* mutable_tail();
  inline ::inner::inner_tail* release_tail();
  inline void set_allocated_tail(::inner::inner_tail* tail);

  // @@protoc_insertion_point(class_scope:inner.pay_rsp)
 private:
  inline void set_has_amt();
  inline void clear_has_amt();
  inline void set_has_user();
  inline void clear_has_user();
  inline void set_has_pay_id();
  inline void clear_has_pay_id();
  inline void set_has_currency();
  inline void clear_has_currency();
  inline void set_has_inpour_type();
  inline void clear_has_inpour_type();
  inline void set_has_addcoin();
  inline void clear_has_addcoin();
  inline void set_has_ex_type();
  inline void clear_has_ex_type();
  inline void set_has_depay();
  inline void clear_has_depay();
  inline void set_has_g_depay();
  inline void clear_has_g_depay();
  inline void set_has_tail();
  inline void clear_has_tail();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::inner::inner_user* user_;
  ::std::string* pay_id_;
  ::google::protobuf::uint32 amt_;
  ::google::protobuf::uint32 addcoin_;
  ::std::string* currency_;
  ::std::string* inpour_type_;
  double depay_;
  double g_depay_;
  ::inner::inner_tail* tail_;
  ::google::protobuf::uint32 ex_type_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(10 + 31) / 32];

  friend void  protobuf_AddDesc_inner_2epay_2eproto();
  friend void protobuf_AssignDesc_inner_2epay_2eproto();
  friend void protobuf_ShutdownFile_inner_2epay_2eproto();

  void InitAsDefaultInstance();
  static pay_rsp* default_instance_;
};
// ===================================================================


// ===================================================================

// pay_req

// required uint32 amt = 1;
inline bool pay_req::has_amt() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void pay_req::set_has_amt() {
  _has_bits_[0] |= 0x00000001u;
}
inline void pay_req::clear_has_amt() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void pay_req::clear_amt() {
  amt_ = 0u;
  clear_has_amt();
}
inline ::google::protobuf::uint32 pay_req::amt() const {
  return amt_;
}
inline void pay_req::set_amt(::google::protobuf::uint32 value) {
  set_has_amt();
  amt_ = value;
}

// required .inner.inner_user user = 2;
inline bool pay_req::has_user() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void pay_req::set_has_user() {
  _has_bits_[0] |= 0x00000002u;
}
inline void pay_req::clear_has_user() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void pay_req::clear_user() {
  if (user_ != NULL) user_->::inner::inner_user::Clear();
  clear_has_user();
}
inline const ::inner::inner_user& pay_req::user() const {
  return user_ != NULL ? *user_ : *default_instance_->user_;
}
inline ::inner::inner_user* pay_req::mutable_user() {
  set_has_user();
  if (user_ == NULL) user_ = new ::inner::inner_user;
  return user_;
}
inline ::inner::inner_user* pay_req::release_user() {
  clear_has_user();
  ::inner::inner_user* temp = user_;
  user_ = NULL;
  return temp;
}
inline void pay_req::set_allocated_user(::inner::inner_user* user) {
  delete user_;
  user_ = user;
  if (user) {
    set_has_user();
  } else {
    clear_has_user();
  }
}

// optional string pay_id = 3;
inline bool pay_req::has_pay_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void pay_req::set_has_pay_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void pay_req::clear_has_pay_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void pay_req::clear_pay_id() {
  if (pay_id_ != &::google::protobuf::internal::kEmptyString) {
    pay_id_->clear();
  }
  clear_has_pay_id();
}
inline const ::std::string& pay_req::pay_id() const {
  return *pay_id_;
}
inline void pay_req::set_pay_id(const ::std::string& value) {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  pay_id_->assign(value);
}
inline void pay_req::set_pay_id(const char* value) {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  pay_id_->assign(value);
}
inline void pay_req::set_pay_id(const char* value, size_t size) {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  pay_id_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* pay_req::mutable_pay_id() {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  return pay_id_;
}
inline ::std::string* pay_req::release_pay_id() {
  clear_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = pay_id_;
    pay_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void pay_req::set_allocated_pay_id(::std::string* pay_id) {
  if (pay_id_ != &::google::protobuf::internal::kEmptyString) {
    delete pay_id_;
  }
  if (pay_id) {
    set_has_pay_id();
    pay_id_ = pay_id;
  } else {
    clear_has_pay_id();
    pay_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string currency = 4;
inline bool pay_req::has_currency() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void pay_req::set_has_currency() {
  _has_bits_[0] |= 0x00000008u;
}
inline void pay_req::clear_has_currency() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void pay_req::clear_currency() {
  if (currency_ != &::google::protobuf::internal::kEmptyString) {
    currency_->clear();
  }
  clear_has_currency();
}
inline const ::std::string& pay_req::currency() const {
  return *currency_;
}
inline void pay_req::set_currency(const ::std::string& value) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(value);
}
inline void pay_req::set_currency(const char* value) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(value);
}
inline void pay_req::set_currency(const char* value, size_t size) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* pay_req::mutable_currency() {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  return currency_;
}
inline ::std::string* pay_req::release_currency() {
  clear_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = currency_;
    currency_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void pay_req::set_allocated_currency(::std::string* currency) {
  if (currency_ != &::google::protobuf::internal::kEmptyString) {
    delete currency_;
  }
  if (currency) {
    set_has_currency();
    currency_ = currency;
  } else {
    clear_has_currency();
    currency_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string inpour_type = 5;
inline bool pay_req::has_inpour_type() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void pay_req::set_has_inpour_type() {
  _has_bits_[0] |= 0x00000010u;
}
inline void pay_req::clear_has_inpour_type() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void pay_req::clear_inpour_type() {
  if (inpour_type_ != &::google::protobuf::internal::kEmptyString) {
    inpour_type_->clear();
  }
  clear_has_inpour_type();
}
inline const ::std::string& pay_req::inpour_type() const {
  return *inpour_type_;
}
inline void pay_req::set_inpour_type(const ::std::string& value) {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  inpour_type_->assign(value);
}
inline void pay_req::set_inpour_type(const char* value) {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  inpour_type_->assign(value);
}
inline void pay_req::set_inpour_type(const char* value, size_t size) {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  inpour_type_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* pay_req::mutable_inpour_type() {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  return inpour_type_;
}
inline ::std::string* pay_req::release_inpour_type() {
  clear_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = inpour_type_;
    inpour_type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void pay_req::set_allocated_inpour_type(::std::string* inpour_type) {
  if (inpour_type_ != &::google::protobuf::internal::kEmptyString) {
    delete inpour_type_;
  }
  if (inpour_type) {
    set_has_inpour_type();
    inpour_type_ = inpour_type;
  } else {
    clear_has_inpour_type();
    inpour_type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional uint32 addcoin = 6;
inline bool pay_req::has_addcoin() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void pay_req::set_has_addcoin() {
  _has_bits_[0] |= 0x00000020u;
}
inline void pay_req::clear_has_addcoin() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void pay_req::clear_addcoin() {
  addcoin_ = 0u;
  clear_has_addcoin();
}
inline ::google::protobuf::uint32 pay_req::addcoin() const {
  return addcoin_;
}
inline void pay_req::set_addcoin(::google::protobuf::uint32 value) {
  set_has_addcoin();
  addcoin_ = value;
}

// optional uint32 ex_type = 7;
inline bool pay_req::has_ex_type() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void pay_req::set_has_ex_type() {
  _has_bits_[0] |= 0x00000040u;
}
inline void pay_req::clear_has_ex_type() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void pay_req::clear_ex_type() {
  ex_type_ = 0u;
  clear_has_ex_type();
}
inline ::google::protobuf::uint32 pay_req::ex_type() const {
  return ex_type_;
}
inline void pay_req::set_ex_type(::google::protobuf::uint32 value) {
  set_has_ex_type();
  ex_type_ = value;
}

// optional double depay = 8;
inline bool pay_req::has_depay() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void pay_req::set_has_depay() {
  _has_bits_[0] |= 0x00000080u;
}
inline void pay_req::clear_has_depay() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void pay_req::clear_depay() {
  depay_ = 0;
  clear_has_depay();
}
inline double pay_req::depay() const {
  return depay_;
}
inline void pay_req::set_depay(double value) {
  set_has_depay();
  depay_ = value;
}

// optional double g_depay = 9;
inline bool pay_req::has_g_depay() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void pay_req::set_has_g_depay() {
  _has_bits_[0] |= 0x00000100u;
}
inline void pay_req::clear_has_g_depay() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void pay_req::clear_g_depay() {
  g_depay_ = 0;
  clear_has_g_depay();
}
inline double pay_req::g_depay() const {
  return g_depay_;
}
inline void pay_req::set_g_depay(double value) {
  set_has_g_depay();
  g_depay_ = value;
}

// optional .inner.inner_head head = 1000;
inline bool pay_req::has_head() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void pay_req::set_has_head() {
  _has_bits_[0] |= 0x00000200u;
}
inline void pay_req::clear_has_head() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void pay_req::clear_head() {
  if (head_ != NULL) head_->::inner::inner_head::Clear();
  clear_has_head();
}
inline const ::inner::inner_head& pay_req::head() const {
  return head_ != NULL ? *head_ : *default_instance_->head_;
}
inline ::inner::inner_head* pay_req::mutable_head() {
  set_has_head();
  if (head_ == NULL) head_ = new ::inner::inner_head;
  return head_;
}
inline ::inner::inner_head* pay_req::release_head() {
  clear_has_head();
  ::inner::inner_head* temp = head_;
  head_ = NULL;
  return temp;
}
inline void pay_req::set_allocated_head(::inner::inner_head* head) {
  delete head_;
  head_ = head;
  if (head) {
    set_has_head();
  } else {
    clear_has_head();
  }
}

// -------------------------------------------------------------------

// pay_rsp

// required uint32 amt = 1;
inline bool pay_rsp::has_amt() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void pay_rsp::set_has_amt() {
  _has_bits_[0] |= 0x00000001u;
}
inline void pay_rsp::clear_has_amt() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void pay_rsp::clear_amt() {
  amt_ = 0u;
  clear_has_amt();
}
inline ::google::protobuf::uint32 pay_rsp::amt() const {
  return amt_;
}
inline void pay_rsp::set_amt(::google::protobuf::uint32 value) {
  set_has_amt();
  amt_ = value;
}

// required .inner.inner_user user = 2;
inline bool pay_rsp::has_user() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void pay_rsp::set_has_user() {
  _has_bits_[0] |= 0x00000002u;
}
inline void pay_rsp::clear_has_user() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void pay_rsp::clear_user() {
  if (user_ != NULL) user_->::inner::inner_user::Clear();
  clear_has_user();
}
inline const ::inner::inner_user& pay_rsp::user() const {
  return user_ != NULL ? *user_ : *default_instance_->user_;
}
inline ::inner::inner_user* pay_rsp::mutable_user() {
  set_has_user();
  if (user_ == NULL) user_ = new ::inner::inner_user;
  return user_;
}
inline ::inner::inner_user* pay_rsp::release_user() {
  clear_has_user();
  ::inner::inner_user* temp = user_;
  user_ = NULL;
  return temp;
}
inline void pay_rsp::set_allocated_user(::inner::inner_user* user) {
  delete user_;
  user_ = user;
  if (user) {
    set_has_user();
  } else {
    clear_has_user();
  }
}

// optional string pay_id = 3;
inline bool pay_rsp::has_pay_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void pay_rsp::set_has_pay_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void pay_rsp::clear_has_pay_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void pay_rsp::clear_pay_id() {
  if (pay_id_ != &::google::protobuf::internal::kEmptyString) {
    pay_id_->clear();
  }
  clear_has_pay_id();
}
inline const ::std::string& pay_rsp::pay_id() const {
  return *pay_id_;
}
inline void pay_rsp::set_pay_id(const ::std::string& value) {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  pay_id_->assign(value);
}
inline void pay_rsp::set_pay_id(const char* value) {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  pay_id_->assign(value);
}
inline void pay_rsp::set_pay_id(const char* value, size_t size) {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  pay_id_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* pay_rsp::mutable_pay_id() {
  set_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    pay_id_ = new ::std::string;
  }
  return pay_id_;
}
inline ::std::string* pay_rsp::release_pay_id() {
  clear_has_pay_id();
  if (pay_id_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = pay_id_;
    pay_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void pay_rsp::set_allocated_pay_id(::std::string* pay_id) {
  if (pay_id_ != &::google::protobuf::internal::kEmptyString) {
    delete pay_id_;
  }
  if (pay_id) {
    set_has_pay_id();
    pay_id_ = pay_id;
  } else {
    clear_has_pay_id();
    pay_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string currency = 4;
inline bool pay_rsp::has_currency() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void pay_rsp::set_has_currency() {
  _has_bits_[0] |= 0x00000008u;
}
inline void pay_rsp::clear_has_currency() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void pay_rsp::clear_currency() {
  if (currency_ != &::google::protobuf::internal::kEmptyString) {
    currency_->clear();
  }
  clear_has_currency();
}
inline const ::std::string& pay_rsp::currency() const {
  return *currency_;
}
inline void pay_rsp::set_currency(const ::std::string& value) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(value);
}
inline void pay_rsp::set_currency(const char* value) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(value);
}
inline void pay_rsp::set_currency(const char* value, size_t size) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* pay_rsp::mutable_currency() {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  return currency_;
}
inline ::std::string* pay_rsp::release_currency() {
  clear_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = currency_;
    currency_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void pay_rsp::set_allocated_currency(::std::string* currency) {
  if (currency_ != &::google::protobuf::internal::kEmptyString) {
    delete currency_;
  }
  if (currency) {
    set_has_currency();
    currency_ = currency;
  } else {
    clear_has_currency();
    currency_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string inpour_type = 5;
inline bool pay_rsp::has_inpour_type() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void pay_rsp::set_has_inpour_type() {
  _has_bits_[0] |= 0x00000010u;
}
inline void pay_rsp::clear_has_inpour_type() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void pay_rsp::clear_inpour_type() {
  if (inpour_type_ != &::google::protobuf::internal::kEmptyString) {
    inpour_type_->clear();
  }
  clear_has_inpour_type();
}
inline const ::std::string& pay_rsp::inpour_type() const {
  return *inpour_type_;
}
inline void pay_rsp::set_inpour_type(const ::std::string& value) {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  inpour_type_->assign(value);
}
inline void pay_rsp::set_inpour_type(const char* value) {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  inpour_type_->assign(value);
}
inline void pay_rsp::set_inpour_type(const char* value, size_t size) {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  inpour_type_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* pay_rsp::mutable_inpour_type() {
  set_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    inpour_type_ = new ::std::string;
  }
  return inpour_type_;
}
inline ::std::string* pay_rsp::release_inpour_type() {
  clear_has_inpour_type();
  if (inpour_type_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = inpour_type_;
    inpour_type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void pay_rsp::set_allocated_inpour_type(::std::string* inpour_type) {
  if (inpour_type_ != &::google::protobuf::internal::kEmptyString) {
    delete inpour_type_;
  }
  if (inpour_type) {
    set_has_inpour_type();
    inpour_type_ = inpour_type;
  } else {
    clear_has_inpour_type();
    inpour_type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional uint32 addcoin = 6;
inline bool pay_rsp::has_addcoin() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void pay_rsp::set_has_addcoin() {
  _has_bits_[0] |= 0x00000020u;
}
inline void pay_rsp::clear_has_addcoin() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void pay_rsp::clear_addcoin() {
  addcoin_ = 0u;
  clear_has_addcoin();
}
inline ::google::protobuf::uint32 pay_rsp::addcoin() const {
  return addcoin_;
}
inline void pay_rsp::set_addcoin(::google::protobuf::uint32 value) {
  set_has_addcoin();
  addcoin_ = value;
}

// optional uint32 ex_type = 7;
inline bool pay_rsp::has_ex_type() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void pay_rsp::set_has_ex_type() {
  _has_bits_[0] |= 0x00000040u;
}
inline void pay_rsp::clear_has_ex_type() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void pay_rsp::clear_ex_type() {
  ex_type_ = 0u;
  clear_has_ex_type();
}
inline ::google::protobuf::uint32 pay_rsp::ex_type() const {
  return ex_type_;
}
inline void pay_rsp::set_ex_type(::google::protobuf::uint32 value) {
  set_has_ex_type();
  ex_type_ = value;
}

// optional double depay = 8;
inline bool pay_rsp::has_depay() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void pay_rsp::set_has_depay() {
  _has_bits_[0] |= 0x00000080u;
}
inline void pay_rsp::clear_has_depay() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void pay_rsp::clear_depay() {
  depay_ = 0;
  clear_has_depay();
}
inline double pay_rsp::depay() const {
  return depay_;
}
inline void pay_rsp::set_depay(double value) {
  set_has_depay();
  depay_ = value;
}

// optional double g_depay = 9;
inline bool pay_rsp::has_g_depay() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void pay_rsp::set_has_g_depay() {
  _has_bits_[0] |= 0x00000100u;
}
inline void pay_rsp::clear_has_g_depay() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void pay_rsp::clear_g_depay() {
  g_depay_ = 0;
  clear_has_g_depay();
}
inline double pay_rsp::g_depay() const {
  return g_depay_;
}
inline void pay_rsp::set_g_depay(double value) {
  set_has_g_depay();
  g_depay_ = value;
}

// required .inner.inner_tail tail = 1000;
inline bool pay_rsp::has_tail() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void pay_rsp::set_has_tail() {
  _has_bits_[0] |= 0x00000200u;
}
inline void pay_rsp::clear_has_tail() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void pay_rsp::clear_tail() {
  if (tail_ != NULL) tail_->::inner::inner_tail::Clear();
  clear_has_tail();
}
inline const ::inner::inner_tail& pay_rsp::tail() const {
  return tail_ != NULL ? *tail_ : *default_instance_->tail_;
}
inline ::inner::inner_tail* pay_rsp::mutable_tail() {
  set_has_tail();
  if (tail_ == NULL) tail_ = new ::inner::inner_tail;
  return tail_;
}
inline ::inner::inner_tail* pay_rsp::release_tail() {
  clear_has_tail();
  ::inner::inner_tail* temp = tail_;
  tail_ = NULL;
  return temp;
}
inline void pay_rsp::set_allocated_tail(::inner::inner_tail* tail) {
  delete tail_;
  tail_ = tail;
  if (tail) {
    set_has_tail();
  } else {
    clear_has_tail();
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace inner

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_inner_2epay_2eproto__INCLUDED
