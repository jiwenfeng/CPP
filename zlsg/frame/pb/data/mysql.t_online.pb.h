// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.t_online.proto

#ifndef PROTOBUF_mysql_2et_5fonline_2eproto__INCLUDED
#define PROTOBUF_mysql_2et_5fonline_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2et_5fonline_2eproto();
void protobuf_AssignDesc_mysql_2et_5fonline_2eproto();
void protobuf_ShutdownFile_mysql_2et_5fonline_2eproto();

class t_online;

// ===================================================================

class t_online : public ::google::protobuf::Message {
 public:
  t_online();
  virtual ~t_online();

  t_online(const t_online& from);

  inline t_online& operator=(const t_online& from) {
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
  static const t_online& default_instance();

  void Swap(t_online* other);

  // implements Message ----------------------------------------------

  t_online* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const t_online& from);
  void MergeFrom(const t_online& from);
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

  // required uint64 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::uint64 id() const;
  inline void set_id(::google::protobuf::uint64 value);

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

  // required uint64 allcount = 3;
  inline bool has_allcount() const;
  inline void clear_allcount();
  static const int kAllcountFieldNumber = 3;
  inline ::google::protobuf::uint64 allcount() const;
  inline void set_allcount(::google::protobuf::uint64 value);

  // required uint64 cashcount = 4;
  inline bool has_cashcount() const;
  inline void clear_cashcount();
  static const int kCashcountFieldNumber = 4;
  inline ::google::protobuf::uint64 cashcount() const;
  inline void set_cashcount(::google::protobuf::uint64 value);

  // required uint32 createDate = 5;
  inline bool has_createdate() const;
  inline void clear_createdate();
  static const int kCreateDateFieldNumber = 5;
  inline ::google::protobuf::uint32 createdate() const;
  inline void set_createdate(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:mysql.t_online)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_sid();
  inline void clear_has_sid();
  inline void set_has_allcount();
  inline void clear_has_allcount();
  inline void set_has_cashcount();
  inline void clear_has_cashcount();
  inline void set_has_createdate();
  inline void clear_has_createdate();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint64 id_;
  ::std::string* sid_;
  ::google::protobuf::uint64 allcount_;
  ::google::protobuf::uint64 cashcount_;
  ::google::protobuf::uint32 createdate_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2et_5fonline_2eproto();
  friend void protobuf_AssignDesc_mysql_2et_5fonline_2eproto();
  friend void protobuf_ShutdownFile_mysql_2et_5fonline_2eproto();

  void InitAsDefaultInstance();
  static t_online* default_instance_;
};
// ===================================================================


// ===================================================================

// t_online

// required uint64 id = 1;
inline bool t_online::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void t_online::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void t_online::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void t_online::clear_id() {
  id_ = GOOGLE_ULONGLONG(0);
  clear_has_id();
}
inline ::google::protobuf::uint64 t_online::id() const {
  return id_;
}
inline void t_online::set_id(::google::protobuf::uint64 value) {
  set_has_id();
  id_ = value;
}

// required string sid = 2;
inline bool t_online::has_sid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void t_online::set_has_sid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void t_online::clear_has_sid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void t_online::clear_sid() {
  if (sid_ != &::google::protobuf::internal::kEmptyString) {
    sid_->clear();
  }
  clear_has_sid();
}
inline const ::std::string& t_online::sid() const {
  return *sid_;
}
inline void t_online::set_sid(const ::std::string& value) {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  sid_->assign(value);
}
inline void t_online::set_sid(const char* value) {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  sid_->assign(value);
}
inline void t_online::set_sid(const char* value, size_t size) {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  sid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_online::mutable_sid() {
  set_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    sid_ = new ::std::string;
  }
  return sid_;
}
inline ::std::string* t_online::release_sid() {
  clear_has_sid();
  if (sid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = sid_;
    sid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_online::set_allocated_sid(::std::string* sid) {
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

// required uint64 allcount = 3;
inline bool t_online::has_allcount() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void t_online::set_has_allcount() {
  _has_bits_[0] |= 0x00000004u;
}
inline void t_online::clear_has_allcount() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void t_online::clear_allcount() {
  allcount_ = GOOGLE_ULONGLONG(0);
  clear_has_allcount();
}
inline ::google::protobuf::uint64 t_online::allcount() const {
  return allcount_;
}
inline void t_online::set_allcount(::google::protobuf::uint64 value) {
  set_has_allcount();
  allcount_ = value;
}

// required uint64 cashcount = 4;
inline bool t_online::has_cashcount() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void t_online::set_has_cashcount() {
  _has_bits_[0] |= 0x00000008u;
}
inline void t_online::clear_has_cashcount() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void t_online::clear_cashcount() {
  cashcount_ = GOOGLE_ULONGLONG(0);
  clear_has_cashcount();
}
inline ::google::protobuf::uint64 t_online::cashcount() const {
  return cashcount_;
}
inline void t_online::set_cashcount(::google::protobuf::uint64 value) {
  set_has_cashcount();
  cashcount_ = value;
}

// required uint32 createDate = 5;
inline bool t_online::has_createdate() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void t_online::set_has_createdate() {
  _has_bits_[0] |= 0x00000010u;
}
inline void t_online::clear_has_createdate() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void t_online::clear_createdate() {
  createdate_ = 0u;
  clear_has_createdate();
}
inline ::google::protobuf::uint32 t_online::createdate() const {
  return createdate_;
}
inline void t_online::set_createdate(::google::protobuf::uint32 value) {
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

#endif  // PROTOBUF_mysql_2et_5fonline_2eproto__INCLUDED
