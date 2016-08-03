// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_family_record.proto

#ifndef PROTOBUF_mysql_2etb_5ffamily_5frecord_2eproto__INCLUDED
#define PROTOBUF_mysql_2etb_5ffamily_5frecord_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2etb_5ffamily_5frecord_2eproto();
void protobuf_AssignDesc_mysql_2etb_5ffamily_5frecord_2eproto();
void protobuf_ShutdownFile_mysql_2etb_5ffamily_5frecord_2eproto();

class tb_family_record;

// ===================================================================

class tb_family_record : public ::google::protobuf::Message {
 public:
  tb_family_record();
  virtual ~tb_family_record();

  tb_family_record(const tb_family_record& from);

  inline tb_family_record& operator=(const tb_family_record& from) {
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
  static const tb_family_record& default_instance();

  void Swap(tb_family_record* other);

  // implements Message ----------------------------------------------

  tb_family_record* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const tb_family_record& from);
  void MergeFrom(const tb_family_record& from);
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

  // required uint32 type = 2;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 2;
  inline ::google::protobuf::uint32 type() const;
  inline void set_type(::google::protobuf::uint32 value);

  // required uint32 killer_family = 3;
  inline bool has_killer_family() const;
  inline void clear_killer_family();
  static const int kKillerFamilyFieldNumber = 3;
  inline ::google::protobuf::uint32 killer_family() const;
  inline void set_killer_family(::google::protobuf::uint32 value);

  // required uint32 killer = 4;
  inline bool has_killer() const;
  inline void clear_killer();
  static const int kKillerFieldNumber = 4;
  inline ::google::protobuf::uint32 killer() const;
  inline void set_killer(::google::protobuf::uint32 value);

  // required uint32 slain_family = 5;
  inline bool has_slain_family() const;
  inline void clear_slain_family();
  static const int kSlainFamilyFieldNumber = 5;
  inline ::google::protobuf::uint32 slain_family() const;
  inline void set_slain_family(::google::protobuf::uint32 value);

  // required uint32 slain_title = 6;
  inline bool has_slain_title() const;
  inline void clear_slain_title();
  static const int kSlainTitleFieldNumber = 6;
  inline ::google::protobuf::uint32 slain_title() const;
  inline void set_slain_title(::google::protobuf::uint32 value);

  // required uint32 slain = 7;
  inline bool has_slain() const;
  inline void clear_slain();
  static const int kSlainFieldNumber = 7;
  inline ::google::protobuf::uint32 slain() const;
  inline void set_slain(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:mysql.tb_family_record)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_killer_family();
  inline void clear_has_killer_family();
  inline void set_has_killer();
  inline void clear_has_killer();
  inline void set_has_slain_family();
  inline void clear_has_slain_family();
  inline void set_has_slain_title();
  inline void clear_has_slain_title();
  inline void set_has_slain();
  inline void clear_has_slain();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 type_;
  ::google::protobuf::uint32 killer_family_;
  ::google::protobuf::uint32 killer_;
  ::google::protobuf::uint32 slain_family_;
  ::google::protobuf::uint32 slain_title_;
  ::google::protobuf::uint32 slain_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2etb_5ffamily_5frecord_2eproto();
  friend void protobuf_AssignDesc_mysql_2etb_5ffamily_5frecord_2eproto();
  friend void protobuf_ShutdownFile_mysql_2etb_5ffamily_5frecord_2eproto();

  void InitAsDefaultInstance();
  static tb_family_record* default_instance_;
};
// ===================================================================


// ===================================================================

// tb_family_record

// required uint32 id = 1;
inline bool tb_family_record::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void tb_family_record::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void tb_family_record::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void tb_family_record::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 tb_family_record::id() const {
  return id_;
}
inline void tb_family_record::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required uint32 type = 2;
inline bool tb_family_record::has_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void tb_family_record::set_has_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void tb_family_record::clear_has_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void tb_family_record::clear_type() {
  type_ = 0u;
  clear_has_type();
}
inline ::google::protobuf::uint32 tb_family_record::type() const {
  return type_;
}
inline void tb_family_record::set_type(::google::protobuf::uint32 value) {
  set_has_type();
  type_ = value;
}

// required uint32 killer_family = 3;
inline bool tb_family_record::has_killer_family() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void tb_family_record::set_has_killer_family() {
  _has_bits_[0] |= 0x00000004u;
}
inline void tb_family_record::clear_has_killer_family() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void tb_family_record::clear_killer_family() {
  killer_family_ = 0u;
  clear_has_killer_family();
}
inline ::google::protobuf::uint32 tb_family_record::killer_family() const {
  return killer_family_;
}
inline void tb_family_record::set_killer_family(::google::protobuf::uint32 value) {
  set_has_killer_family();
  killer_family_ = value;
}

// required uint32 killer = 4;
inline bool tb_family_record::has_killer() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void tb_family_record::set_has_killer() {
  _has_bits_[0] |= 0x00000008u;
}
inline void tb_family_record::clear_has_killer() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void tb_family_record::clear_killer() {
  killer_ = 0u;
  clear_has_killer();
}
inline ::google::protobuf::uint32 tb_family_record::killer() const {
  return killer_;
}
inline void tb_family_record::set_killer(::google::protobuf::uint32 value) {
  set_has_killer();
  killer_ = value;
}

// required uint32 slain_family = 5;
inline bool tb_family_record::has_slain_family() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void tb_family_record::set_has_slain_family() {
  _has_bits_[0] |= 0x00000010u;
}
inline void tb_family_record::clear_has_slain_family() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void tb_family_record::clear_slain_family() {
  slain_family_ = 0u;
  clear_has_slain_family();
}
inline ::google::protobuf::uint32 tb_family_record::slain_family() const {
  return slain_family_;
}
inline void tb_family_record::set_slain_family(::google::protobuf::uint32 value) {
  set_has_slain_family();
  slain_family_ = value;
}

// required uint32 slain_title = 6;
inline bool tb_family_record::has_slain_title() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void tb_family_record::set_has_slain_title() {
  _has_bits_[0] |= 0x00000020u;
}
inline void tb_family_record::clear_has_slain_title() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void tb_family_record::clear_slain_title() {
  slain_title_ = 0u;
  clear_has_slain_title();
}
inline ::google::protobuf::uint32 tb_family_record::slain_title() const {
  return slain_title_;
}
inline void tb_family_record::set_slain_title(::google::protobuf::uint32 value) {
  set_has_slain_title();
  slain_title_ = value;
}

// required uint32 slain = 7;
inline bool tb_family_record::has_slain() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void tb_family_record::set_has_slain() {
  _has_bits_[0] |= 0x00000040u;
}
inline void tb_family_record::clear_has_slain() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void tb_family_record::clear_slain() {
  slain_ = 0u;
  clear_has_slain();
}
inline ::google::protobuf::uint32 tb_family_record::slain() const {
  return slain_;
}
inline void tb_family_record::set_slain(::google::protobuf::uint32 value) {
  set_has_slain();
  slain_ = value;
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

#endif  // PROTOBUF_mysql_2etb_5ffamily_5frecord_2eproto__INCLUDED
