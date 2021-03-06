// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.role_cd_cooldown.proto

#ifndef PROTOBUF_client_2erole_5fcd_5fcooldown_2eproto__INCLUDED
#define PROTOBUF_client_2erole_5fcd_5fcooldown_2eproto__INCLUDED

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "client.comm.pb.h"
// @@protoc_insertion_point(includes)

namespace client {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto();
void protobuf_AssignDesc_client_2erole_5fcd_5fcooldown_2eproto();
void protobuf_ShutdownFile_client_2erole_5fcd_5fcooldown_2eproto();

class cd_info;
class cd_rsp;

enum CD_COOLDOWN_TYPE_ID_E {
  CD_COOLDOW_TYPE_ID_TELEPORTATION = 1,
  CD_COOLDOW_TYPE_ID_BLACK_SHOP = 2
};
bool CD_COOLDOWN_TYPE_ID_E_IsValid(int value);
const CD_COOLDOWN_TYPE_ID_E CD_COOLDOWN_TYPE_ID_E_MIN = CD_COOLDOW_TYPE_ID_TELEPORTATION;
const CD_COOLDOWN_TYPE_ID_E CD_COOLDOWN_TYPE_ID_E_MAX = CD_COOLDOW_TYPE_ID_BLACK_SHOP;
const int CD_COOLDOWN_TYPE_ID_E_ARRAYSIZE = CD_COOLDOWN_TYPE_ID_E_MAX + 1;

const ::google::protobuf::EnumDescriptor* CD_COOLDOWN_TYPE_ID_E_descriptor();
inline const ::std::string& CD_COOLDOWN_TYPE_ID_E_Name(CD_COOLDOWN_TYPE_ID_E value) {
  return ::google::protobuf::internal::NameOfEnum(
    CD_COOLDOWN_TYPE_ID_E_descriptor(), value);
}
inline bool CD_COOLDOWN_TYPE_ID_E_Parse(
    const ::std::string& name, CD_COOLDOWN_TYPE_ID_E* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CD_COOLDOWN_TYPE_ID_E>(
    CD_COOLDOWN_TYPE_ID_E_descriptor(), name, value);
}
enum COOL_DOWN_FAGS_E {
  CDF_NONE = 0,
  CDF_NEEDSAVE = 1,
  CDF_NEEDSENDTOCLIENT = 2
};
bool COOL_DOWN_FAGS_E_IsValid(int value);
const COOL_DOWN_FAGS_E COOL_DOWN_FAGS_E_MIN = CDF_NONE;
const COOL_DOWN_FAGS_E COOL_DOWN_FAGS_E_MAX = CDF_NEEDSENDTOCLIENT;
const int COOL_DOWN_FAGS_E_ARRAYSIZE = COOL_DOWN_FAGS_E_MAX + 1;

const ::google::protobuf::EnumDescriptor* COOL_DOWN_FAGS_E_descriptor();
inline const ::std::string& COOL_DOWN_FAGS_E_Name(COOL_DOWN_FAGS_E value) {
  return ::google::protobuf::internal::NameOfEnum(
    COOL_DOWN_FAGS_E_descriptor(), value);
}
inline bool COOL_DOWN_FAGS_E_Parse(
    const ::std::string& name, COOL_DOWN_FAGS_E* value) {
  return ::google::protobuf::internal::ParseNamedEnum<COOL_DOWN_FAGS_E>(
    COOL_DOWN_FAGS_E_descriptor(), name, value);
}
enum COOLDOWN_TYPE_E {
  CDT_COUNTDOWN = 0,
  CDT_REMAIN = 1,
  CDT_MAX_LIMIT = 2
};
bool COOLDOWN_TYPE_E_IsValid(int value);
const COOLDOWN_TYPE_E COOLDOWN_TYPE_E_MIN = CDT_COUNTDOWN;
const COOLDOWN_TYPE_E COOLDOWN_TYPE_E_MAX = CDT_MAX_LIMIT;
const int COOLDOWN_TYPE_E_ARRAYSIZE = COOLDOWN_TYPE_E_MAX + 1;

const ::google::protobuf::EnumDescriptor* COOLDOWN_TYPE_E_descriptor();
inline const ::std::string& COOLDOWN_TYPE_E_Name(COOLDOWN_TYPE_E value) {
  return ::google::protobuf::internal::NameOfEnum(
    COOLDOWN_TYPE_E_descriptor(), value);
}
inline bool COOLDOWN_TYPE_E_Parse(
    const ::std::string& name, COOLDOWN_TYPE_E* value) {
  return ::google::protobuf::internal::ParseNamedEnum<COOLDOWN_TYPE_E>(
    COOLDOWN_TYPE_E_descriptor(), name, value);
}
enum CD_RESULT_TYPE_E {
  CD_RESULT_TYPE_NOT_ENOUGH = 0,
  CD_RESULT_TYPE_ACCUMULATION = 1
};
bool CD_RESULT_TYPE_E_IsValid(int value);
const CD_RESULT_TYPE_E CD_RESULT_TYPE_E_MIN = CD_RESULT_TYPE_NOT_ENOUGH;
const CD_RESULT_TYPE_E CD_RESULT_TYPE_E_MAX = CD_RESULT_TYPE_ACCUMULATION;
const int CD_RESULT_TYPE_E_ARRAYSIZE = CD_RESULT_TYPE_E_MAX + 1;

const ::google::protobuf::EnumDescriptor* CD_RESULT_TYPE_E_descriptor();
inline const ::std::string& CD_RESULT_TYPE_E_Name(CD_RESULT_TYPE_E value) {
  return ::google::protobuf::internal::NameOfEnum(
    CD_RESULT_TYPE_E_descriptor(), value);
}
inline bool CD_RESULT_TYPE_E_Parse(
    const ::std::string& name, CD_RESULT_TYPE_E* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CD_RESULT_TYPE_E>(
    CD_RESULT_TYPE_E_descriptor(), name, value);
}
// ===================================================================

class cd_info : public ::google::protobuf::Message {
 public:
  cd_info();
  virtual ~cd_info();

  cd_info(const cd_info& from);

  inline cd_info& operator=(const cd_info& from) {
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
  static const cd_info& default_instance();

  void Swap(cd_info* other);

  // implements Message ----------------------------------------------

  cd_info* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cd_info& from);
  void MergeFrom(const cd_info& from);
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

  // required uint32 type_id = 1;
  inline bool has_type_id() const;
  inline void clear_type_id();
  static const int kTypeIdFieldNumber = 1;
  inline ::google::protobuf::uint32 type_id() const;
  inline void set_type_id(::google::protobuf::uint32 value);

  // required uint32 cooldown_type = 2;
  inline bool has_cooldown_type() const;
  inline void clear_cooldown_type();
  static const int kCooldownTypeFieldNumber = 2;
  inline ::google::protobuf::uint32 cooldown_type() const;
  inline void set_cooldown_type(::google::protobuf::uint32 value);

  // required uint32 life_time = 3;
  inline bool has_life_time() const;
  inline void clear_life_time();
  static const int kLifeTimeFieldNumber = 3;
  inline ::google::protobuf::uint32 life_time() const;
  inline void set_life_time(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:client.cd_info)
 private:
  inline void set_has_type_id();
  inline void clear_has_type_id();
  inline void set_has_cooldown_type();
  inline void clear_has_cooldown_type();
  inline void set_has_life_time();
  inline void clear_has_life_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 type_id_;
  ::google::protobuf::uint32 cooldown_type_;
  ::google::protobuf::uint32 life_time_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fcd_5fcooldown_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fcd_5fcooldown_2eproto();

  void InitAsDefaultInstance();
  static cd_info* default_instance_;
};
// -------------------------------------------------------------------

class cd_rsp : public ::google::protobuf::Message {
 public:
  cd_rsp();
  virtual ~cd_rsp();

  cd_rsp(const cd_rsp& from);

  inline cd_rsp& operator=(const cd_rsp& from) {
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
  static const cd_rsp& default_instance();

  void Swap(cd_rsp* other);

  // implements Message ----------------------------------------------

  cd_rsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cd_rsp& from);
  void MergeFrom(const cd_rsp& from);
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

  // required uint32 result = 1;
  inline bool has_result() const;
  inline void clear_result();
  static const int kResultFieldNumber = 1;
  inline ::google::protobuf::uint32 result() const;
  inline void set_result(::google::protobuf::uint32 value);

  // required uint32 type_id = 2;
  inline bool has_type_id() const;
  inline void clear_type_id();
  static const int kTypeIdFieldNumber = 2;
  inline ::google::protobuf::uint32 type_id() const;
  inline void set_type_id(::google::protobuf::uint32 value);

  // required uint32 life_time = 3;
  inline bool has_life_time() const;
  inline void clear_life_time();
  static const int kLifeTimeFieldNumber = 3;
  inline ::google::protobuf::uint32 life_time() const;
  inline void set_life_time(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:client.cd_rsp)
 private:
  inline void set_has_result();
  inline void clear_has_result();
  inline void set_has_type_id();
  inline void clear_has_type_id();
  inline void set_has_life_time();
  inline void clear_has_life_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 result_;
  ::google::protobuf::uint32 type_id_;
  ::google::protobuf::uint32 life_time_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fcd_5fcooldown_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fcd_5fcooldown_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fcd_5fcooldown_2eproto();

  void InitAsDefaultInstance();
  static cd_rsp* default_instance_;
};
// ===================================================================


// ===================================================================

// cd_info

// required uint32 type_id = 1;
inline bool cd_info::has_type_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cd_info::set_has_type_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cd_info::clear_has_type_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cd_info::clear_type_id() {
  type_id_ = 0u;
  clear_has_type_id();
}
inline ::google::protobuf::uint32 cd_info::type_id() const {
  return type_id_;
}
inline void cd_info::set_type_id(::google::protobuf::uint32 value) {
  set_has_type_id();
  type_id_ = value;
}

// required uint32 cooldown_type = 2;
inline bool cd_info::has_cooldown_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void cd_info::set_has_cooldown_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void cd_info::clear_has_cooldown_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void cd_info::clear_cooldown_type() {
  cooldown_type_ = 0u;
  clear_has_cooldown_type();
}
inline ::google::protobuf::uint32 cd_info::cooldown_type() const {
  return cooldown_type_;
}
inline void cd_info::set_cooldown_type(::google::protobuf::uint32 value) {
  set_has_cooldown_type();
  cooldown_type_ = value;
}

// required uint32 life_time = 3;
inline bool cd_info::has_life_time() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void cd_info::set_has_life_time() {
  _has_bits_[0] |= 0x00000004u;
}
inline void cd_info::clear_has_life_time() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void cd_info::clear_life_time() {
  life_time_ = 0u;
  clear_has_life_time();
}
inline ::google::protobuf::uint32 cd_info::life_time() const {
  return life_time_;
}
inline void cd_info::set_life_time(::google::protobuf::uint32 value) {
  set_has_life_time();
  life_time_ = value;
}

// -------------------------------------------------------------------

// cd_rsp

// required uint32 result = 1;
inline bool cd_rsp::has_result() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cd_rsp::set_has_result() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cd_rsp::clear_has_result() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cd_rsp::clear_result() {
  result_ = 0u;
  clear_has_result();
}
inline ::google::protobuf::uint32 cd_rsp::result() const {
  return result_;
}
inline void cd_rsp::set_result(::google::protobuf::uint32 value) {
  set_has_result();
  result_ = value;
}

// required uint32 type_id = 2;
inline bool cd_rsp::has_type_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void cd_rsp::set_has_type_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void cd_rsp::clear_has_type_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void cd_rsp::clear_type_id() {
  type_id_ = 0u;
  clear_has_type_id();
}
inline ::google::protobuf::uint32 cd_rsp::type_id() const {
  return type_id_;
}
inline void cd_rsp::set_type_id(::google::protobuf::uint32 value) {
  set_has_type_id();
  type_id_ = value;
}

// required uint32 life_time = 3;
inline bool cd_rsp::has_life_time() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void cd_rsp::set_has_life_time() {
  _has_bits_[0] |= 0x00000004u;
}
inline void cd_rsp::clear_has_life_time() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void cd_rsp::clear_life_time() {
  life_time_ = 0u;
  clear_has_life_time();
}
inline ::google::protobuf::uint32 cd_rsp::life_time() const {
  return life_time_;
}
inline void cd_rsp::set_life_time(::google::protobuf::uint32 value) {
  set_has_life_time();
  life_time_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace client

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::client::CD_COOLDOWN_TYPE_ID_E>() {
  return ::client::CD_COOLDOWN_TYPE_ID_E_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::client::COOL_DOWN_FAGS_E>() {
  return ::client::COOL_DOWN_FAGS_E_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::client::COOLDOWN_TYPE_E>() {
  return ::client::COOLDOWN_TYPE_E_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::client::CD_RESULT_TYPE_E>() {
  return ::client::CD_RESULT_TYPE_E_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_client_2erole_5fcd_5fcooldown_2eproto__INCLUDED
