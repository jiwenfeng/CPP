// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_mail_center.proto

#ifndef PROTOBUF_mysql_2etb_5fmail_5fcenter_2eproto__INCLUDED
#define PROTOBUF_mysql_2etb_5fmail_5fcenter_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2etb_5fmail_5fcenter_2eproto();
void protobuf_AssignDesc_mysql_2etb_5fmail_5fcenter_2eproto();
void protobuf_ShutdownFile_mysql_2etb_5fmail_5fcenter_2eproto();

class tb_mail_center;

// ===================================================================

class tb_mail_center : public ::google::protobuf::Message {
 public:
  tb_mail_center();
  virtual ~tb_mail_center();

  tb_mail_center(const tb_mail_center& from);

  inline tb_mail_center& operator=(const tb_mail_center& from) {
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
  static const tb_mail_center& default_instance();

  void Swap(tb_mail_center* other);

  // implements Message ----------------------------------------------

  tb_mail_center* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const tb_mail_center& from);
  void MergeFrom(const tb_mail_center& from);
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

  // required uint32 role_id = 1;
  inline bool has_role_id() const;
  inline void clear_role_id();
  static const int kRoleIdFieldNumber = 1;
  inline ::google::protobuf::uint32 role_id() const;
  inline void set_role_id(::google::protobuf::uint32 value);

  // required bytes mail = 2;
  inline bool has_mail() const;
  inline void clear_mail();
  static const int kMailFieldNumber = 2;
  inline const ::std::string& mail() const;
  inline void set_mail(const ::std::string& value);
  inline void set_mail(const char* value);
  inline void set_mail(const void* value, size_t size);
  inline ::std::string* mutable_mail();
  inline ::std::string* release_mail();
  inline void set_allocated_mail(::std::string* mail);

  // @@protoc_insertion_point(class_scope:mysql.tb_mail_center)
 private:
  inline void set_has_role_id();
  inline void clear_has_role_id();
  inline void set_has_mail();
  inline void clear_has_mail();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* mail_;
  ::google::protobuf::uint32 role_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2etb_5fmail_5fcenter_2eproto();
  friend void protobuf_AssignDesc_mysql_2etb_5fmail_5fcenter_2eproto();
  friend void protobuf_ShutdownFile_mysql_2etb_5fmail_5fcenter_2eproto();

  void InitAsDefaultInstance();
  static tb_mail_center* default_instance_;
};
// ===================================================================


// ===================================================================

// tb_mail_center

// required uint32 role_id = 1;
inline bool tb_mail_center::has_role_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void tb_mail_center::set_has_role_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void tb_mail_center::clear_has_role_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void tb_mail_center::clear_role_id() {
  role_id_ = 0u;
  clear_has_role_id();
}
inline ::google::protobuf::uint32 tb_mail_center::role_id() const {
  return role_id_;
}
inline void tb_mail_center::set_role_id(::google::protobuf::uint32 value) {
  set_has_role_id();
  role_id_ = value;
}

// required bytes mail = 2;
inline bool tb_mail_center::has_mail() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void tb_mail_center::set_has_mail() {
  _has_bits_[0] |= 0x00000002u;
}
inline void tb_mail_center::clear_has_mail() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void tb_mail_center::clear_mail() {
  if (mail_ != &::google::protobuf::internal::kEmptyString) {
    mail_->clear();
  }
  clear_has_mail();
}
inline const ::std::string& tb_mail_center::mail() const {
  return *mail_;
}
inline void tb_mail_center::set_mail(const ::std::string& value) {
  set_has_mail();
  if (mail_ == &::google::protobuf::internal::kEmptyString) {
    mail_ = new ::std::string;
  }
  mail_->assign(value);
}
inline void tb_mail_center::set_mail(const char* value) {
  set_has_mail();
  if (mail_ == &::google::protobuf::internal::kEmptyString) {
    mail_ = new ::std::string;
  }
  mail_->assign(value);
}
inline void tb_mail_center::set_mail(const void* value, size_t size) {
  set_has_mail();
  if (mail_ == &::google::protobuf::internal::kEmptyString) {
    mail_ = new ::std::string;
  }
  mail_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* tb_mail_center::mutable_mail() {
  set_has_mail();
  if (mail_ == &::google::protobuf::internal::kEmptyString) {
    mail_ = new ::std::string;
  }
  return mail_;
}
inline ::std::string* tb_mail_center::release_mail() {
  clear_has_mail();
  if (mail_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = mail_;
    mail_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void tb_mail_center::set_allocated_mail(::std::string* mail) {
  if (mail_ != &::google::protobuf::internal::kEmptyString) {
    delete mail_;
  }
  if (mail) {
    set_has_mail();
    mail_ = mail;
  } else {
    clear_has_mail();
    mail_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
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

#endif  // PROTOBUF_mysql_2etb_5fmail_5fcenter_2eproto__INCLUDED
