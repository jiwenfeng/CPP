// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.role_bows.proto

#ifndef PROTOBUF_client_2erole_5fbows_2eproto__INCLUDED
#define PROTOBUF_client_2erole_5fbows_2eproto__INCLUDED

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
#include "client.comm.pb.h"
// @@protoc_insertion_point(includes)

namespace client {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

class bows_info;
class bows_msg;
class bows_level_rsp;
class upgrade_bows_req;
class upgrade_bows_rsp;
class bows_bless_time_req;
class bows_bless_time_rsp;
class bows_skill_fight_rsp;

// ===================================================================

class bows_info : public ::google::protobuf::Message {
 public:
  bows_info();
  virtual ~bows_info();

  bows_info(const bows_info& from);

  inline bows_info& operator=(const bows_info& from) {
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
  static const bows_info& default_instance();

  void Swap(bows_info* other);

  // implements Message ----------------------------------------------

  bows_info* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const bows_info& from);
  void MergeFrom(const bows_info& from);
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

  // required uint32 bow_id = 1;
  inline bool has_bow_id() const;
  inline void clear_bow_id();
  static const int kBowIdFieldNumber = 1;
  inline ::google::protobuf::uint32 bow_id() const;
  inline void set_bow_id(::google::protobuf::uint32 value);

  // required uint32 advance_time = 2;
  inline bool has_advance_time() const;
  inline void clear_advance_time();
  static const int kAdvanceTimeFieldNumber = 2;
  inline ::google::protobuf::uint32 advance_time() const;
  inline void set_advance_time(::google::protobuf::uint32 value);

  // required uint32 advance_bless = 3;
  inline bool has_advance_bless() const;
  inline void clear_advance_bless();
  static const int kAdvanceBlessFieldNumber = 3;
  inline ::google::protobuf::uint32 advance_bless() const;
  inline void set_advance_bless(::google::protobuf::uint32 value);

  // required uint32 bow_level = 4;
  inline bool has_bow_level() const;
  inline void clear_bow_level();
  static const int kBowLevelFieldNumber = 4;
  inline ::google::protobuf::uint32 bow_level() const;
  inline void set_bow_level(::google::protobuf::uint32 value);

  // required uint32 bow_exp = 5;
  inline bool has_bow_exp() const;
  inline void clear_bow_exp();
  static const int kBowExpFieldNumber = 5;
  inline ::google::protobuf::uint32 bow_exp() const;
  inline void set_bow_exp(::google::protobuf::uint32 value);

  // optional uint32 bow_power = 6;
  inline bool has_bow_power() const;
  inline void clear_bow_power();
  static const int kBowPowerFieldNumber = 6;
  inline ::google::protobuf::uint32 bow_power() const;
  inline void set_bow_power(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:client.bows_info)
 private:
  inline void set_has_bow_id();
  inline void clear_has_bow_id();
  inline void set_has_advance_time();
  inline void clear_has_advance_time();
  inline void set_has_advance_bless();
  inline void clear_has_advance_bless();
  inline void set_has_bow_level();
  inline void clear_has_bow_level();
  inline void set_has_bow_exp();
  inline void clear_has_bow_exp();
  inline void set_has_bow_power();
  inline void clear_has_bow_power();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 bow_id_;
  ::google::protobuf::uint32 advance_time_;
  ::google::protobuf::uint32 advance_bless_;
  ::google::protobuf::uint32 bow_level_;
  ::google::protobuf::uint32 bow_exp_;
  ::google::protobuf::uint32 bow_power_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(6 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static bows_info* default_instance_;
};
// -------------------------------------------------------------------

class bows_msg : public ::google::protobuf::Message {
 public:
  bows_msg();
  virtual ~bows_msg();

  bows_msg(const bows_msg& from);

  inline bows_msg& operator=(const bows_msg& from) {
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
  static const bows_msg& default_instance();

  void Swap(bows_msg* other);

  // implements Message ----------------------------------------------

  bows_msg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const bows_msg& from);
  void MergeFrom(const bows_msg& from);
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

  // repeated .client.bows_info mi = 1;
  inline int mi_size() const;
  inline void clear_mi();
  static const int kMiFieldNumber = 1;
  inline const ::client::bows_info& mi(int index) const;
  inline ::client::bows_info* mutable_mi(int index);
  inline ::client::bows_info* add_mi();
  inline const ::google::protobuf::RepeatedPtrField< ::client::bows_info >&
      mi() const;
  inline ::google::protobuf::RepeatedPtrField< ::client::bows_info >*
      mutable_mi();

  // @@protoc_insertion_point(class_scope:client.bows_msg)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::client::bows_info > mi_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static bows_msg* default_instance_;
};
// -------------------------------------------------------------------

class bows_level_rsp : public ::google::protobuf::Message {
 public:
  bows_level_rsp();
  virtual ~bows_level_rsp();

  bows_level_rsp(const bows_level_rsp& from);

  inline bows_level_rsp& operator=(const bows_level_rsp& from) {
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
  static const bows_level_rsp& default_instance();

  void Swap(bows_level_rsp* other);

  // implements Message ----------------------------------------------

  bows_level_rsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const bows_level_rsp& from);
  void MergeFrom(const bows_level_rsp& from);
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

  // repeated .client.bows_info mi = 1;
  inline int mi_size() const;
  inline void clear_mi();
  static const int kMiFieldNumber = 1;
  inline const ::client::bows_info& mi(int index) const;
  inline ::client::bows_info* mutable_mi(int index);
  inline ::client::bows_info* add_mi();
  inline const ::google::protobuf::RepeatedPtrField< ::client::bows_info >&
      mi() const;
  inline ::google::protobuf::RepeatedPtrField< ::client::bows_info >*
      mutable_mi();

  // @@protoc_insertion_point(class_scope:client.bows_level_rsp)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::client::bows_info > mi_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static bows_level_rsp* default_instance_;
};
// -------------------------------------------------------------------

class upgrade_bows_req : public ::google::protobuf::Message {
 public:
  upgrade_bows_req();
  virtual ~upgrade_bows_req();

  upgrade_bows_req(const upgrade_bows_req& from);

  inline upgrade_bows_req& operator=(const upgrade_bows_req& from) {
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
  static const upgrade_bows_req& default_instance();

  void Swap(upgrade_bows_req* other);

  // implements Message ----------------------------------------------

  upgrade_bows_req* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const upgrade_bows_req& from);
  void MergeFrom(const upgrade_bows_req& from);
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

  // required uint32 bow_id = 1;
  inline bool has_bow_id() const;
  inline void clear_bow_id();
  static const int kBowIdFieldNumber = 1;
  inline ::google::protobuf::uint32 bow_id() const;
  inline void set_bow_id(::google::protobuf::uint32 value);

  // optional .client.server_head svr_head = 1000;
  inline bool has_svr_head() const;
  inline void clear_svr_head();
  static const int kSvrHeadFieldNumber = 1000;
  inline const ::client::server_head& svr_head() const;
  inline ::client::server_head* mutable_svr_head();
  inline ::client::server_head* release_svr_head();
  inline void set_allocated_svr_head(::client::server_head* svr_head);

  // @@protoc_insertion_point(class_scope:client.upgrade_bows_req)
 private:
  inline void set_has_bow_id();
  inline void clear_has_bow_id();
  inline void set_has_svr_head();
  inline void clear_has_svr_head();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::client::server_head* svr_head_;
  ::google::protobuf::uint32 bow_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static upgrade_bows_req* default_instance_;
};
// -------------------------------------------------------------------

class upgrade_bows_rsp : public ::google::protobuf::Message {
 public:
  upgrade_bows_rsp();
  virtual ~upgrade_bows_rsp();

  upgrade_bows_rsp(const upgrade_bows_rsp& from);

  inline upgrade_bows_rsp& operator=(const upgrade_bows_rsp& from) {
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
  static const upgrade_bows_rsp& default_instance();

  void Swap(upgrade_bows_rsp* other);

  // implements Message ----------------------------------------------

  upgrade_bows_rsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const upgrade_bows_rsp& from);
  void MergeFrom(const upgrade_bows_rsp& from);
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

  // required uint32 ret = 1;
  inline bool has_ret() const;
  inline void clear_ret();
  static const int kRetFieldNumber = 1;
  inline ::google::protobuf::uint32 ret() const;
  inline void set_ret(::google::protobuf::uint32 value);

  // repeated .client.bows_info mi = 2;
  inline int mi_size() const;
  inline void clear_mi();
  static const int kMiFieldNumber = 2;
  inline const ::client::bows_info& mi(int index) const;
  inline ::client::bows_info* mutable_mi(int index);
  inline ::client::bows_info* add_mi();
  inline const ::google::protobuf::RepeatedPtrField< ::client::bows_info >&
      mi() const;
  inline ::google::protobuf::RepeatedPtrField< ::client::bows_info >*
      mutable_mi();

  // @@protoc_insertion_point(class_scope:client.upgrade_bows_rsp)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::client::bows_info > mi_;
  ::google::protobuf::uint32 ret_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static upgrade_bows_rsp* default_instance_;
};
// -------------------------------------------------------------------

class bows_bless_time_req : public ::google::protobuf::Message {
 public:
  bows_bless_time_req();
  virtual ~bows_bless_time_req();

  bows_bless_time_req(const bows_bless_time_req& from);

  inline bows_bless_time_req& operator=(const bows_bless_time_req& from) {
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
  static const bows_bless_time_req& default_instance();

  void Swap(bows_bless_time_req* other);

  // implements Message ----------------------------------------------

  bows_bless_time_req* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const bows_bless_time_req& from);
  void MergeFrom(const bows_bless_time_req& from);
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

  // required uint32 unixtime = 1;
  inline bool has_unixtime() const;
  inline void clear_unixtime();
  static const int kUnixtimeFieldNumber = 1;
  inline ::google::protobuf::uint32 unixtime() const;
  inline void set_unixtime(::google::protobuf::uint32 value);

  // optional .client.server_head svr_head = 1000;
  inline bool has_svr_head() const;
  inline void clear_svr_head();
  static const int kSvrHeadFieldNumber = 1000;
  inline const ::client::server_head& svr_head() const;
  inline ::client::server_head* mutable_svr_head();
  inline ::client::server_head* release_svr_head();
  inline void set_allocated_svr_head(::client::server_head* svr_head);

  // @@protoc_insertion_point(class_scope:client.bows_bless_time_req)
 private:
  inline void set_has_unixtime();
  inline void clear_has_unixtime();
  inline void set_has_svr_head();
  inline void clear_has_svr_head();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::client::server_head* svr_head_;
  ::google::protobuf::uint32 unixtime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static bows_bless_time_req* default_instance_;
};
// -------------------------------------------------------------------

class bows_bless_time_rsp : public ::google::protobuf::Message {
 public:
  bows_bless_time_rsp();
  virtual ~bows_bless_time_rsp();

  bows_bless_time_rsp(const bows_bless_time_rsp& from);

  inline bows_bless_time_rsp& operator=(const bows_bless_time_rsp& from) {
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
  static const bows_bless_time_rsp& default_instance();

  void Swap(bows_bless_time_rsp* other);

  // implements Message ----------------------------------------------

  bows_bless_time_rsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const bows_bless_time_rsp& from);
  void MergeFrom(const bows_bless_time_rsp& from);
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

  // required uint32 remain_time = 1;
  inline bool has_remain_time() const;
  inline void clear_remain_time();
  static const int kRemainTimeFieldNumber = 1;
  inline ::google::protobuf::uint32 remain_time() const;
  inline void set_remain_time(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:client.bows_bless_time_rsp)
 private:
  inline void set_has_remain_time();
  inline void clear_has_remain_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 remain_time_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static bows_bless_time_rsp* default_instance_;
};
// -------------------------------------------------------------------

class bows_skill_fight_rsp : public ::google::protobuf::Message {
 public:
  bows_skill_fight_rsp();
  virtual ~bows_skill_fight_rsp();

  bows_skill_fight_rsp(const bows_skill_fight_rsp& from);

  inline bows_skill_fight_rsp& operator=(const bows_skill_fight_rsp& from) {
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
  static const bows_skill_fight_rsp& default_instance();

  void Swap(bows_skill_fight_rsp* other);

  // implements Message ----------------------------------------------

  bows_skill_fight_rsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const bows_skill_fight_rsp& from);
  void MergeFrom(const bows_skill_fight_rsp& from);
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

  // required uint32 attack_count = 1;
  inline bool has_attack_count() const;
  inline void clear_attack_count();
  static const int kAttackCountFieldNumber = 1;
  inline ::google::protobuf::uint32 attack_count() const;
  inline void set_attack_count(::google::protobuf::uint32 value);

  // required uint32 skill_index = 2;
  inline bool has_skill_index() const;
  inline void clear_skill_index();
  static const int kSkillIndexFieldNumber = 2;
  inline ::google::protobuf::uint32 skill_index() const;
  inline void set_skill_index(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:client.bows_skill_fight_rsp)
 private:
  inline void set_has_attack_count();
  inline void clear_has_attack_count();
  inline void set_has_skill_index();
  inline void clear_has_skill_index();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 attack_count_;
  ::google::protobuf::uint32 skill_index_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fbows_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fbows_2eproto();

  void InitAsDefaultInstance();
  static bows_skill_fight_rsp* default_instance_;
};
// ===================================================================


// ===================================================================

// bows_info

// required uint32 bow_id = 1;
inline bool bows_info::has_bow_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void bows_info::set_has_bow_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void bows_info::clear_has_bow_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void bows_info::clear_bow_id() {
  bow_id_ = 0u;
  clear_has_bow_id();
}
inline ::google::protobuf::uint32 bows_info::bow_id() const {
  return bow_id_;
}
inline void bows_info::set_bow_id(::google::protobuf::uint32 value) {
  set_has_bow_id();
  bow_id_ = value;
}

// required uint32 advance_time = 2;
inline bool bows_info::has_advance_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void bows_info::set_has_advance_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void bows_info::clear_has_advance_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void bows_info::clear_advance_time() {
  advance_time_ = 0u;
  clear_has_advance_time();
}
inline ::google::protobuf::uint32 bows_info::advance_time() const {
  return advance_time_;
}
inline void bows_info::set_advance_time(::google::protobuf::uint32 value) {
  set_has_advance_time();
  advance_time_ = value;
}

// required uint32 advance_bless = 3;
inline bool bows_info::has_advance_bless() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void bows_info::set_has_advance_bless() {
  _has_bits_[0] |= 0x00000004u;
}
inline void bows_info::clear_has_advance_bless() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void bows_info::clear_advance_bless() {
  advance_bless_ = 0u;
  clear_has_advance_bless();
}
inline ::google::protobuf::uint32 bows_info::advance_bless() const {
  return advance_bless_;
}
inline void bows_info::set_advance_bless(::google::protobuf::uint32 value) {
  set_has_advance_bless();
  advance_bless_ = value;
}

// required uint32 bow_level = 4;
inline bool bows_info::has_bow_level() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void bows_info::set_has_bow_level() {
  _has_bits_[0] |= 0x00000008u;
}
inline void bows_info::clear_has_bow_level() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void bows_info::clear_bow_level() {
  bow_level_ = 0u;
  clear_has_bow_level();
}
inline ::google::protobuf::uint32 bows_info::bow_level() const {
  return bow_level_;
}
inline void bows_info::set_bow_level(::google::protobuf::uint32 value) {
  set_has_bow_level();
  bow_level_ = value;
}

// required uint32 bow_exp = 5;
inline bool bows_info::has_bow_exp() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void bows_info::set_has_bow_exp() {
  _has_bits_[0] |= 0x00000010u;
}
inline void bows_info::clear_has_bow_exp() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void bows_info::clear_bow_exp() {
  bow_exp_ = 0u;
  clear_has_bow_exp();
}
inline ::google::protobuf::uint32 bows_info::bow_exp() const {
  return bow_exp_;
}
inline void bows_info::set_bow_exp(::google::protobuf::uint32 value) {
  set_has_bow_exp();
  bow_exp_ = value;
}

// optional uint32 bow_power = 6;
inline bool bows_info::has_bow_power() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void bows_info::set_has_bow_power() {
  _has_bits_[0] |= 0x00000020u;
}
inline void bows_info::clear_has_bow_power() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void bows_info::clear_bow_power() {
  bow_power_ = 0u;
  clear_has_bow_power();
}
inline ::google::protobuf::uint32 bows_info::bow_power() const {
  return bow_power_;
}
inline void bows_info::set_bow_power(::google::protobuf::uint32 value) {
  set_has_bow_power();
  bow_power_ = value;
}

// -------------------------------------------------------------------

// bows_msg

// repeated .client.bows_info mi = 1;
inline int bows_msg::mi_size() const {
  return mi_.size();
}
inline void bows_msg::clear_mi() {
  mi_.Clear();
}
inline const ::client::bows_info& bows_msg::mi(int index) const {
  return mi_.Get(index);
}
inline ::client::bows_info* bows_msg::mutable_mi(int index) {
  return mi_.Mutable(index);
}
inline ::client::bows_info* bows_msg::add_mi() {
  return mi_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::client::bows_info >&
bows_msg::mi() const {
  return mi_;
}
inline ::google::protobuf::RepeatedPtrField< ::client::bows_info >*
bows_msg::mutable_mi() {
  return &mi_;
}

// -------------------------------------------------------------------

// bows_level_rsp

// repeated .client.bows_info mi = 1;
inline int bows_level_rsp::mi_size() const {
  return mi_.size();
}
inline void bows_level_rsp::clear_mi() {
  mi_.Clear();
}
inline const ::client::bows_info& bows_level_rsp::mi(int index) const {
  return mi_.Get(index);
}
inline ::client::bows_info* bows_level_rsp::mutable_mi(int index) {
  return mi_.Mutable(index);
}
inline ::client::bows_info* bows_level_rsp::add_mi() {
  return mi_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::client::bows_info >&
bows_level_rsp::mi() const {
  return mi_;
}
inline ::google::protobuf::RepeatedPtrField< ::client::bows_info >*
bows_level_rsp::mutable_mi() {
  return &mi_;
}

// -------------------------------------------------------------------

// upgrade_bows_req

// required uint32 bow_id = 1;
inline bool upgrade_bows_req::has_bow_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void upgrade_bows_req::set_has_bow_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void upgrade_bows_req::clear_has_bow_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void upgrade_bows_req::clear_bow_id() {
  bow_id_ = 0u;
  clear_has_bow_id();
}
inline ::google::protobuf::uint32 upgrade_bows_req::bow_id() const {
  return bow_id_;
}
inline void upgrade_bows_req::set_bow_id(::google::protobuf::uint32 value) {
  set_has_bow_id();
  bow_id_ = value;
}

// optional .client.server_head svr_head = 1000;
inline bool upgrade_bows_req::has_svr_head() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void upgrade_bows_req::set_has_svr_head() {
  _has_bits_[0] |= 0x00000002u;
}
inline void upgrade_bows_req::clear_has_svr_head() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void upgrade_bows_req::clear_svr_head() {
  if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
  clear_has_svr_head();
}
inline const ::client::server_head& upgrade_bows_req::svr_head() const {
  return svr_head_ != NULL ? *svr_head_ : *default_instance_->svr_head_;
}
inline ::client::server_head* upgrade_bows_req::mutable_svr_head() {
  set_has_svr_head();
  if (svr_head_ == NULL) svr_head_ = new ::client::server_head;
  return svr_head_;
}
inline ::client::server_head* upgrade_bows_req::release_svr_head() {
  clear_has_svr_head();
  ::client::server_head* temp = svr_head_;
  svr_head_ = NULL;
  return temp;
}
inline void upgrade_bows_req::set_allocated_svr_head(::client::server_head* svr_head) {
  delete svr_head_;
  svr_head_ = svr_head;
  if (svr_head) {
    set_has_svr_head();
  } else {
    clear_has_svr_head();
  }
}

// -------------------------------------------------------------------

// upgrade_bows_rsp

// required uint32 ret = 1;
inline bool upgrade_bows_rsp::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void upgrade_bows_rsp::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void upgrade_bows_rsp::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void upgrade_bows_rsp::clear_ret() {
  ret_ = 0u;
  clear_has_ret();
}
inline ::google::protobuf::uint32 upgrade_bows_rsp::ret() const {
  return ret_;
}
inline void upgrade_bows_rsp::set_ret(::google::protobuf::uint32 value) {
  set_has_ret();
  ret_ = value;
}

// repeated .client.bows_info mi = 2;
inline int upgrade_bows_rsp::mi_size() const {
  return mi_.size();
}
inline void upgrade_bows_rsp::clear_mi() {
  mi_.Clear();
}
inline const ::client::bows_info& upgrade_bows_rsp::mi(int index) const {
  return mi_.Get(index);
}
inline ::client::bows_info* upgrade_bows_rsp::mutable_mi(int index) {
  return mi_.Mutable(index);
}
inline ::client::bows_info* upgrade_bows_rsp::add_mi() {
  return mi_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::client::bows_info >&
upgrade_bows_rsp::mi() const {
  return mi_;
}
inline ::google::protobuf::RepeatedPtrField< ::client::bows_info >*
upgrade_bows_rsp::mutable_mi() {
  return &mi_;
}

// -------------------------------------------------------------------

// bows_bless_time_req

// required uint32 unixtime = 1;
inline bool bows_bless_time_req::has_unixtime() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void bows_bless_time_req::set_has_unixtime() {
  _has_bits_[0] |= 0x00000001u;
}
inline void bows_bless_time_req::clear_has_unixtime() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void bows_bless_time_req::clear_unixtime() {
  unixtime_ = 0u;
  clear_has_unixtime();
}
inline ::google::protobuf::uint32 bows_bless_time_req::unixtime() const {
  return unixtime_;
}
inline void bows_bless_time_req::set_unixtime(::google::protobuf::uint32 value) {
  set_has_unixtime();
  unixtime_ = value;
}

// optional .client.server_head svr_head = 1000;
inline bool bows_bless_time_req::has_svr_head() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void bows_bless_time_req::set_has_svr_head() {
  _has_bits_[0] |= 0x00000002u;
}
inline void bows_bless_time_req::clear_has_svr_head() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void bows_bless_time_req::clear_svr_head() {
  if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
  clear_has_svr_head();
}
inline const ::client::server_head& bows_bless_time_req::svr_head() const {
  return svr_head_ != NULL ? *svr_head_ : *default_instance_->svr_head_;
}
inline ::client::server_head* bows_bless_time_req::mutable_svr_head() {
  set_has_svr_head();
  if (svr_head_ == NULL) svr_head_ = new ::client::server_head;
  return svr_head_;
}
inline ::client::server_head* bows_bless_time_req::release_svr_head() {
  clear_has_svr_head();
  ::client::server_head* temp = svr_head_;
  svr_head_ = NULL;
  return temp;
}
inline void bows_bless_time_req::set_allocated_svr_head(::client::server_head* svr_head) {
  delete svr_head_;
  svr_head_ = svr_head;
  if (svr_head) {
    set_has_svr_head();
  } else {
    clear_has_svr_head();
  }
}

// -------------------------------------------------------------------

// bows_bless_time_rsp

// required uint32 remain_time = 1;
inline bool bows_bless_time_rsp::has_remain_time() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void bows_bless_time_rsp::set_has_remain_time() {
  _has_bits_[0] |= 0x00000001u;
}
inline void bows_bless_time_rsp::clear_has_remain_time() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void bows_bless_time_rsp::clear_remain_time() {
  remain_time_ = 0u;
  clear_has_remain_time();
}
inline ::google::protobuf::uint32 bows_bless_time_rsp::remain_time() const {
  return remain_time_;
}
inline void bows_bless_time_rsp::set_remain_time(::google::protobuf::uint32 value) {
  set_has_remain_time();
  remain_time_ = value;
}

// -------------------------------------------------------------------

// bows_skill_fight_rsp

// required uint32 attack_count = 1;
inline bool bows_skill_fight_rsp::has_attack_count() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void bows_skill_fight_rsp::set_has_attack_count() {
  _has_bits_[0] |= 0x00000001u;
}
inline void bows_skill_fight_rsp::clear_has_attack_count() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void bows_skill_fight_rsp::clear_attack_count() {
  attack_count_ = 0u;
  clear_has_attack_count();
}
inline ::google::protobuf::uint32 bows_skill_fight_rsp::attack_count() const {
  return attack_count_;
}
inline void bows_skill_fight_rsp::set_attack_count(::google::protobuf::uint32 value) {
  set_has_attack_count();
  attack_count_ = value;
}

// required uint32 skill_index = 2;
inline bool bows_skill_fight_rsp::has_skill_index() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void bows_skill_fight_rsp::set_has_skill_index() {
  _has_bits_[0] |= 0x00000002u;
}
inline void bows_skill_fight_rsp::clear_has_skill_index() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void bows_skill_fight_rsp::clear_skill_index() {
  skill_index_ = 0u;
  clear_has_skill_index();
}
inline ::google::protobuf::uint32 bows_skill_fight_rsp::skill_index() const {
  return skill_index_;
}
inline void bows_skill_fight_rsp::set_skill_index(::google::protobuf::uint32 value) {
  set_has_skill_index();
  skill_index_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace client

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_client_2erole_5fbows_2eproto__INCLUDED