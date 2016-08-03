// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.t_recharge.proto

#ifndef PROTOBUF_mysql_2et_5frecharge_2eproto__INCLUDED
#define PROTOBUF_mysql_2et_5frecharge_2eproto__INCLUDED

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
// @@protoc_insertion_point(includes)

namespace mysql {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_mysql_2et_5frecharge_2eproto();
void protobuf_AssignDesc_mysql_2et_5frecharge_2eproto();
void protobuf_ShutdownFile_mysql_2et_5frecharge_2eproto();

class t_recharge;

// ===================================================================

class t_recharge : public ::google::protobuf::Message {
 public:
  t_recharge();
  virtual ~t_recharge();

  t_recharge(const t_recharge& from);

  inline t_recharge& operator=(const t_recharge& from) {
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
  static const t_recharge& default_instance();

  void Swap(t_recharge* other);

  // implements Message ----------------------------------------------

  t_recharge* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const t_recharge& from);
  void MergeFrom(const t_recharge& from);
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

  // required uint32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);

  // required string sid = 2;
  inline bool has_sid() const;
  inline void clear_sid();
  static const int kSidFieldNumber = 2;
  inline const ::std::string& sid() const;
  inline void set_sid(const ::std::string& value);
  inline void set_sid(const char* value);
  inline void set_sid(const char* value, size_t size);
  inline ::std::string* mutable_sid();
  inline ::std::string* release_sid();
  inline void set_allocated_sid(::std::string* sid);

  // required string did = 3;
  inline bool has_did() const;
  inline void clear_did();
  static const int kDidFieldNumber = 3;
  inline const ::std::string& did() const;
  inline void set_did(const ::std::string& value);
  inline void set_did(const char* value);
  inline void set_did(const char* value, size_t size);
  inline ::std::string* mutable_did();
  inline ::std::string* release_did();
  inline void set_allocated_did(::std::string* did);

  // required string accountName = 4;
  inline bool has_accountname() const;
  inline void clear_accountname();
  static const int kAccountNameFieldNumber = 4;
  inline const ::std::string& accountname() const;
  inline void set_accountname(const ::std::string& value);
  inline void set_accountname(const char* value);
  inline void set_accountname(const char* value, size_t size);
  inline ::std::string* mutable_accountname();
  inline ::std::string* release_accountname();
  inline void set_allocated_accountname(::std::string* accountname);

  // required string playerName = 5;
  inline bool has_playername() const;
  inline void clear_playername();
  static const int kPlayerNameFieldNumber = 5;
  inline const ::std::string& playername() const;
  inline void set_playername(const ::std::string& value);
  inline void set_playername(const char* value);
  inline void set_playername(const char* value, size_t size);
  inline ::std::string* mutable_playername();
  inline ::std::string* release_playername();
  inline void set_allocated_playername(::std::string* playername);

  // required uint32 playerLevel = 6;
  inline bool has_playerlevel() const;
  inline void clear_playerlevel();
  static const int kPlayerLevelFieldNumber = 6;
  inline ::google::protobuf::uint32 playerlevel() const;
  inline void set_playerlevel(::google::protobuf::uint32 value);

  // required string transactionId = 7;
  inline bool has_transactionid() const;
  inline void clear_transactionid();
  static const int kTransactionIdFieldNumber = 7;
  inline const ::std::string& transactionid() const;
  inline void set_transactionid(const ::std::string& value);
  inline void set_transactionid(const char* value);
  inline void set_transactionid(const char* value, size_t size);
  inline ::std::string* mutable_transactionid();
  inline ::std::string* release_transactionid();
  inline void set_allocated_transactionid(::std::string* transactionid);

  // required string currency = 8;
  inline bool has_currency() const;
  inline void clear_currency();
  static const int kCurrencyFieldNumber = 8;
  inline const ::std::string& currency() const;
  inline void set_currency(const ::std::string& value);
  inline void set_currency(const char* value);
  inline void set_currency(const char* value, size_t size);
  inline ::std::string* mutable_currency();
  inline ::std::string* release_currency();
  inline void set_allocated_currency(::std::string* currency);

  // required string inpourType = 9;
  inline bool has_inpourtype() const;
  inline void clear_inpourtype();
  static const int kInpourTypeFieldNumber = 9;
  inline const ::std::string& inpourtype() const;
  inline void set_inpourtype(const ::std::string& value);
  inline void set_inpourtype(const char* value);
  inline void set_inpourtype(const char* value, size_t size);
  inline ::std::string* mutable_inpourtype();
  inline ::std::string* release_inpourtype();
  inline void set_allocated_inpourtype(::std::string* inpourtype);

  // required int32 addcoin = 10;
  inline bool has_addcoin() const;
  inline void clear_addcoin();
  static const int kAddcoinFieldNumber = 10;
  inline ::google::protobuf::int32 addcoin() const;
  inline void set_addcoin(::google::protobuf::int32 value);

  // required double money = 11;
  inline bool has_money() const;
  inline void clear_money();
  static const int kMoneyFieldNumber = 11;
  inline double money() const;
  inline void set_money(double value);

  // required uint32 gold = 12;
  inline bool has_gold() const;
  inline void clear_gold();
  static const int kGoldFieldNumber = 12;
  inline ::google::protobuf::uint32 gold() const;
  inline void set_gold(::google::protobuf::uint32 value);

  // required int32 exchangeType = 13;
  inline bool has_exchangetype() const;
  inline void clear_exchangetype();
  static const int kExchangeTypeFieldNumber = 13;
  inline ::google::protobuf::int32 exchangetype() const;
  inline void set_exchangetype(::google::protobuf::int32 value);

  // required double Depay = 14;
  inline bool has_depay() const;
  inline void clear_depay();
  static const int kDepayFieldNumber = 14;
  inline double depay() const;
  inline void set_depay(double value);

  // required double gDepay = 15;
  inline bool has_gdepay() const;
  inline void clear_gdepay();
  static const int kGDepayFieldNumber = 15;
  inline double gdepay() const;
  inline void set_gdepay(double value);

  // required int32 state = 16;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 16;
  inline ::google::protobuf::int32 state() const;
  inline void set_state(::google::protobuf::int32 value);

  // required uint32 createDate = 17;
  inline bool has_createdate() const;
  inline void clear_createdate();
  static const int kCreateDateFieldNumber = 17;
  inline ::google::protobuf::uint32 createdate() const;
  inline void set_createdate(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:mysql.t_recharge)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_sid();
  inline void clear_has_sid();
  inline void set_has_did();
  inline void clear_has_did();
  inline void set_has_accountname();
  inline void clear_has_accountname();
  inline void set_has_playername();
  inline void clear_has_playername();
  inline void set_has_playerlevel();
  inline void clear_has_playerlevel();
  inline void set_has_transactionid();
  inline void clear_has_transactionid();
  inline void set_has_currency();
  inline void clear_has_currency();
  inline void set_has_inpourtype();
  inline void clear_has_inpourtype();
  inline void set_has_addcoin();
  inline void clear_has_addcoin();
  inline void set_has_money();
  inline void clear_has_money();
  inline void set_has_gold();
  inline void clear_has_gold();
  inline void set_has_exchangetype();
  inline void clear_has_exchangetype();
  inline void set_has_depay();
  inline void clear_has_depay();
  inline void set_has_gdepay();
  inline void clear_has_gdepay();
  inline void set_has_state();
  inline void clear_has_state();
  inline void set_has_createdate();
  inline void clear_has_createdate();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* sid_;
  ::std::string* did_;
  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 playerlevel_;
  ::std::string* accountname_;
  ::std::string* playername_;
  ::std::string* transactionid_;
  ::std::string* currency_;
  ::std::string* inpourtype_;
  double money_;
  ::google::protobuf::int32 addcoin_;
  ::google::protobuf::uint32 gold_;
  double depay_;
  ::google::protobuf::int32 exchangetype_;
  ::google::protobuf::int32 state_;
  double gdepay_;
  ::google::protobuf::uint32 createdate_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(17 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2et_5frecharge_2eproto();
  friend void protobuf_AssignDesc_mysql_2et_5frecharge_2eproto();
  friend void protobuf_ShutdownFile_mysql_2et_5frecharge_2eproto();

  void InitAsDefaultInstance();
  static t_recharge* default_instance_;
};
// ===================================================================


// ===================================================================

// t_recharge

// required uint32 id = 1;
inline bool t_recharge::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void t_recharge::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void t_recharge::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void t_recharge::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 t_recharge::id() const {
  return id_;
}
inline void t_recharge::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required string sid = 2;
inline bool t_recharge::has_sid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void t_recharge::set_has_sid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void t_recharge::clear_has_sid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void t_recharge::clear_sid() {
  if (sid_ != &::google::protobuf::internal::kEmptyString) {
    sid_->clear();
  }
  clear_has_sid();
}
inline const ::std::string& t_recharge::sid() const {
  return *sid_;
}
inline void t_recharge::set_sid(const ::std::string& value) {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  sid_->assign(value);
}
inline void t_recharge::set_sid(const char* value) {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  sid_->assign(value);
}
inline void t_recharge::set_sid(const char* value, size_t size) {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  sid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_recharge::mutable_sid() {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  return sid_;
}
inline ::std::string* t_recharge::release_sid() {
  clear_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = sid_;
    sid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_recharge::set_allocated_sid(::std::string* sid) {
  if (sid_ != &::google::protobuf::internal::kEmptyString) {
    delete sid_;
  }
  if (sid) {
    set_has_sid();
    sid_ = sid;
  } else {
    clear_has_sid();
    sid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string did = 3;
inline bool t_recharge::has_did() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void t_recharge::set_has_did() {
  _has_bits_[0] |= 0x00000004u;
}
inline void t_recharge::clear_has_did() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void t_recharge::clear_did() {
  if (did_ != &::google::protobuf::internal::kEmptyString) {
    did_->clear();
  }
  clear_has_did();
}
inline const ::std::string& t_recharge::did() const {
  return *did_;
}
inline void t_recharge::set_did(const ::std::string& value) {
  set_has_did();
  if (did_ == &::google::protobuf::internal::kEmptyString) {
    did_ = new ::std::string;
  }
  did_->assign(value);
}
inline void t_recharge::set_did(const char* value) {
  set_has_did();
  if (did_ == &::google::protobuf::internal::kEmptyString) {
    did_ = new ::std::string;
  }
  did_->assign(value);
}
inline void t_recharge::set_did(const char* value, size_t size) {
  set_has_did();
  if (did_ == &::google::protobuf::internal::kEmptyString) {
    did_ = new ::std::string;
  }
  did_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_recharge::mutable_did() {
  set_has_did();
  if (did_ == &::google::protobuf::internal::kEmptyString) {
    did_ = new ::std::string;
  }
  return did_;
}
inline ::std::string* t_recharge::release_did() {
  clear_has_did();
  if (did_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = did_;
    did_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_recharge::set_allocated_did(::std::string* did) {
  if (did_ != &::google::protobuf::internal::kEmptyString) {
    delete did_;
  }
  if (did) {
    set_has_did();
    did_ = did;
  } else {
    clear_has_did();
    did_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string accountName = 4;
inline bool t_recharge::has_accountname() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void t_recharge::set_has_accountname() {
  _has_bits_[0] |= 0x00000008u;
}
inline void t_recharge::clear_has_accountname() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void t_recharge::clear_accountname() {
  if (accountname_ != &::google::protobuf::internal::kEmptyString) {
    accountname_->clear();
  }
  clear_has_accountname();
}
inline const ::std::string& t_recharge::accountname() const {
  return *accountname_;
}
inline void t_recharge::set_accountname(const ::std::string& value) {
  set_has_accountname();
  if (accountname_ == &::google::protobuf::internal::kEmptyString) {
    accountname_ = new ::std::string;
  }
  accountname_->assign(value);
}
inline void t_recharge::set_accountname(const char* value) {
  set_has_accountname();
  if (accountname_ == &::google::protobuf::internal::kEmptyString) {
    accountname_ = new ::std::string;
  }
  accountname_->assign(value);
}
inline void t_recharge::set_accountname(const char* value, size_t size) {
  set_has_accountname();
  if (accountname_ == &::google::protobuf::internal::kEmptyString) {
    accountname_ = new ::std::string;
  }
  accountname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_recharge::mutable_accountname() {
  set_has_accountname();
  if (accountname_ == &::google::protobuf::internal::kEmptyString) {
    accountname_ = new ::std::string;
  }
  return accountname_;
}
inline ::std::string* t_recharge::release_accountname() {
  clear_has_accountname();
  if (accountname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = accountname_;
    accountname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_recharge::set_allocated_accountname(::std::string* accountname) {
  if (accountname_ != &::google::protobuf::internal::kEmptyString) {
    delete accountname_;
  }
  if (accountname) {
    set_has_accountname();
    accountname_ = accountname;
  } else {
    clear_has_accountname();
    accountname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string playerName = 5;
inline bool t_recharge::has_playername() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void t_recharge::set_has_playername() {
  _has_bits_[0] |= 0x00000010u;
}
inline void t_recharge::clear_has_playername() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void t_recharge::clear_playername() {
  if (playername_ != &::google::protobuf::internal::kEmptyString) {
    playername_->clear();
  }
  clear_has_playername();
}
inline const ::std::string& t_recharge::playername() const {
  return *playername_;
}
inline void t_recharge::set_playername(const ::std::string& value) {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void t_recharge::set_playername(const char* value) {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void t_recharge::set_playername(const char* value, size_t size) {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  playername_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_recharge::mutable_playername() {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  return playername_;
}
inline ::std::string* t_recharge::release_playername() {
  clear_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = playername_;
    playername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_recharge::set_allocated_playername(::std::string* playername) {
  if (playername_ != &::google::protobuf::internal::kEmptyString) {
    delete playername_;
  }
  if (playername) {
    set_has_playername();
    playername_ = playername;
  } else {
    clear_has_playername();
    playername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 playerLevel = 6;
inline bool t_recharge::has_playerlevel() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void t_recharge::set_has_playerlevel() {
  _has_bits_[0] |= 0x00000020u;
}
inline void t_recharge::clear_has_playerlevel() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void t_recharge::clear_playerlevel() {
  playerlevel_ = 0u;
  clear_has_playerlevel();
}
inline ::google::protobuf::uint32 t_recharge::playerlevel() const {
  return playerlevel_;
}
inline void t_recharge::set_playerlevel(::google::protobuf::uint32 value) {
  set_has_playerlevel();
  playerlevel_ = value;
}

// required string transactionId = 7;
inline bool t_recharge::has_transactionid() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void t_recharge::set_has_transactionid() {
  _has_bits_[0] |= 0x00000040u;
}
inline void t_recharge::clear_has_transactionid() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void t_recharge::clear_transactionid() {
  if (transactionid_ != &::google::protobuf::internal::kEmptyString) {
    transactionid_->clear();
  }
  clear_has_transactionid();
}
inline const ::std::string& t_recharge::transactionid() const {
  return *transactionid_;
}
inline void t_recharge::set_transactionid(const ::std::string& value) {
  set_has_transactionid();
  if (transactionid_ == &::google::protobuf::internal::kEmptyString) {
    transactionid_ = new ::std::string;
  }
  transactionid_->assign(value);
}
inline void t_recharge::set_transactionid(const char* value) {
  set_has_transactionid();
  if (transactionid_ == &::google::protobuf::internal::kEmptyString) {
    transactionid_ = new ::std::string;
  }
  transactionid_->assign(value);
}
inline void t_recharge::set_transactionid(const char* value, size_t size) {
  set_has_transactionid();
  if (transactionid_ == &::google::protobuf::internal::kEmptyString) {
    transactionid_ = new ::std::string;
  }
  transactionid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_recharge::mutable_transactionid() {
  set_has_transactionid();
  if (transactionid_ == &::google::protobuf::internal::kEmptyString) {
    transactionid_ = new ::std::string;
  }
  return transactionid_;
}
inline ::std::string* t_recharge::release_transactionid() {
  clear_has_transactionid();
  if (transactionid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = transactionid_;
    transactionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_recharge::set_allocated_transactionid(::std::string* transactionid) {
  if (transactionid_ != &::google::protobuf::internal::kEmptyString) {
    delete transactionid_;
  }
  if (transactionid) {
    set_has_transactionid();
    transactionid_ = transactionid;
  } else {
    clear_has_transactionid();
    transactionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string currency = 8;
inline bool t_recharge::has_currency() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void t_recharge::set_has_currency() {
  _has_bits_[0] |= 0x00000080u;
}
inline void t_recharge::clear_has_currency() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void t_recharge::clear_currency() {
  if (currency_ != &::google::protobuf::internal::kEmptyString) {
    currency_->clear();
  }
  clear_has_currency();
}
inline const ::std::string& t_recharge::currency() const {
  return *currency_;
}
inline void t_recharge::set_currency(const ::std::string& value) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(value);
}
inline void t_recharge::set_currency(const char* value) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(value);
}
inline void t_recharge::set_currency(const char* value, size_t size) {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  currency_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_recharge::mutable_currency() {
  set_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    currency_ = new ::std::string;
  }
  return currency_;
}
inline ::std::string* t_recharge::release_currency() {
  clear_has_currency();
  if (currency_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = currency_;
    currency_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_recharge::set_allocated_currency(::std::string* currency) {
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

// required string inpourType = 9;
inline bool t_recharge::has_inpourtype() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void t_recharge::set_has_inpourtype() {
  _has_bits_[0] |= 0x00000100u;
}
inline void t_recharge::clear_has_inpourtype() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void t_recharge::clear_inpourtype() {
  if (inpourtype_ != &::google::protobuf::internal::kEmptyString) {
    inpourtype_->clear();
  }
  clear_has_inpourtype();
}
inline const ::std::string& t_recharge::inpourtype() const {
  return *inpourtype_;
}
inline void t_recharge::set_inpourtype(const ::std::string& value) {
  set_has_inpourtype();
  if (inpourtype_ == &::google::protobuf::internal::kEmptyString) {
    inpourtype_ = new ::std::string;
  }
  inpourtype_->assign(value);
}
inline void t_recharge::set_inpourtype(const char* value) {
  set_has_inpourtype();
  if (inpourtype_ == &::google::protobuf::internal::kEmptyString) {
    inpourtype_ = new ::std::string;
  }
  inpourtype_->assign(value);
}
inline void t_recharge::set_inpourtype(const char* value, size_t size) {
  set_has_inpourtype();
  if (inpourtype_ == &::google::protobuf::internal::kEmptyString) {
    inpourtype_ = new ::std::string;
  }
  inpourtype_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_recharge::mutable_inpourtype() {
  set_has_inpourtype();
  if (inpourtype_ == &::google::protobuf::internal::kEmptyString) {
    inpourtype_ = new ::std::string;
  }
  return inpourtype_;
}
inline ::std::string* t_recharge::release_inpourtype() {
  clear_has_inpourtype();
  if (inpourtype_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = inpourtype_;
    inpourtype_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_recharge::set_allocated_inpourtype(::std::string* inpourtype) {
  if (inpourtype_ != &::google::protobuf::internal::kEmptyString) {
    delete inpourtype_;
  }
  if (inpourtype) {
    set_has_inpourtype();
    inpourtype_ = inpourtype;
  } else {
    clear_has_inpourtype();
    inpourtype_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required int32 addcoin = 10;
inline bool t_recharge::has_addcoin() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void t_recharge::set_has_addcoin() {
  _has_bits_[0] |= 0x00000200u;
}
inline void t_recharge::clear_has_addcoin() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void t_recharge::clear_addcoin() {
  addcoin_ = 0;
  clear_has_addcoin();
}
inline ::google::protobuf::int32 t_recharge::addcoin() const {
  return addcoin_;
}
inline void t_recharge::set_addcoin(::google::protobuf::int32 value) {
  set_has_addcoin();
  addcoin_ = value;
}

// required double money = 11;
inline bool t_recharge::has_money() const {
  return (_has_bits_[0] & 0x00000400u) != 0;
}
inline void t_recharge::set_has_money() {
  _has_bits_[0] |= 0x00000400u;
}
inline void t_recharge::clear_has_money() {
  _has_bits_[0] &= ~0x00000400u;
}
inline void t_recharge::clear_money() {
  money_ = 0;
  clear_has_money();
}
inline double t_recharge::money() const {
  return money_;
}
inline void t_recharge::set_money(double value) {
  set_has_money();
  money_ = value;
}

// required uint32 gold = 12;
inline bool t_recharge::has_gold() const {
  return (_has_bits_[0] & 0x00000800u) != 0;
}
inline void t_recharge::set_has_gold() {
  _has_bits_[0] |= 0x00000800u;
}
inline void t_recharge::clear_has_gold() {
  _has_bits_[0] &= ~0x00000800u;
}
inline void t_recharge::clear_gold() {
  gold_ = 0u;
  clear_has_gold();
}
inline ::google::protobuf::uint32 t_recharge::gold() const {
  return gold_;
}
inline void t_recharge::set_gold(::google::protobuf::uint32 value) {
  set_has_gold();
  gold_ = value;
}

// required int32 exchangeType = 13;
inline bool t_recharge::has_exchangetype() const {
  return (_has_bits_[0] & 0x00001000u) != 0;
}
inline void t_recharge::set_has_exchangetype() {
  _has_bits_[0] |= 0x00001000u;
}
inline void t_recharge::clear_has_exchangetype() {
  _has_bits_[0] &= ~0x00001000u;
}
inline void t_recharge::clear_exchangetype() {
  exchangetype_ = 0;
  clear_has_exchangetype();
}
inline ::google::protobuf::int32 t_recharge::exchangetype() const {
  return exchangetype_;
}
inline void t_recharge::set_exchangetype(::google::protobuf::int32 value) {
  set_has_exchangetype();
  exchangetype_ = value;
}

// required double Depay = 14;
inline bool t_recharge::has_depay() const {
  return (_has_bits_[0] & 0x00002000u) != 0;
}
inline void t_recharge::set_has_depay() {
  _has_bits_[0] |= 0x00002000u;
}
inline void t_recharge::clear_has_depay() {
  _has_bits_[0] &= ~0x00002000u;
}
inline void t_recharge::clear_depay() {
  depay_ = 0;
  clear_has_depay();
}
inline double t_recharge::depay() const {
  return depay_;
}
inline void t_recharge::set_depay(double value) {
  set_has_depay();
  depay_ = value;
}

// required double gDepay = 15;
inline bool t_recharge::has_gdepay() const {
  return (_has_bits_[0] & 0x00004000u) != 0;
}
inline void t_recharge::set_has_gdepay() {
  _has_bits_[0] |= 0x00004000u;
}
inline void t_recharge::clear_has_gdepay() {
  _has_bits_[0] &= ~0x00004000u;
}
inline void t_recharge::clear_gdepay() {
  gdepay_ = 0;
  clear_has_gdepay();
}
inline double t_recharge::gdepay() const {
  return gdepay_;
}
inline void t_recharge::set_gdepay(double value) {
  set_has_gdepay();
  gdepay_ = value;
}

// required int32 state = 16;
inline bool t_recharge::has_state() const {
  return (_has_bits_[0] & 0x00008000u) != 0;
}
inline void t_recharge::set_has_state() {
  _has_bits_[0] |= 0x00008000u;
}
inline void t_recharge::clear_has_state() {
  _has_bits_[0] &= ~0x00008000u;
}
inline void t_recharge::clear_state() {
  state_ = 0;
  clear_has_state();
}
inline ::google::protobuf::int32 t_recharge::state() const {
  return state_;
}
inline void t_recharge::set_state(::google::protobuf::int32 value) {
  set_has_state();
  state_ = value;
}

// required uint32 createDate = 17;
inline bool t_recharge::has_createdate() const {
  return (_has_bits_[0] & 0x00010000u) != 0;
}
inline void t_recharge::set_has_createdate() {
  _has_bits_[0] |= 0x00010000u;
}
inline void t_recharge::clear_has_createdate() {
  _has_bits_[0] &= ~0x00010000u;
}
inline void t_recharge::clear_createdate() {
  createdate_ = 0u;
  clear_has_createdate();
}
inline ::google::protobuf::uint32 t_recharge::createdate() const {
  return createdate_;
}
inline void t_recharge::set_createdate(::google::protobuf::uint32 value) {
  set_has_createdate();
  createdate_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mysql

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_mysql_2et_5frecharge_2eproto__INCLUDED