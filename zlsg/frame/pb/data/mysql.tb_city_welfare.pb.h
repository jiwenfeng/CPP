// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_city_welfare.proto

#ifndef PROTOBUF_mysql_2etb_5fcity_5fwelfare_2eproto__INCLUDED
#define PROTOBUF_mysql_2etb_5fcity_5fwelfare_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2etb_5fcity_5fwelfare_2eproto();
void protobuf_AssignDesc_mysql_2etb_5fcity_5fwelfare_2eproto();
void protobuf_ShutdownFile_mysql_2etb_5fcity_5fwelfare_2eproto();

class tb_city_welfare;

// ===================================================================

class tb_city_welfare : public ::google::protobuf::Message {
 public:
  tb_city_welfare();
  virtual ~tb_city_welfare();

  tb_city_welfare(const tb_city_welfare& from);

  inline tb_city_welfare& operator=(const tb_city_welfare& from) {
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
  static const tb_city_welfare& default_instance();

  void Swap(tb_city_welfare* other);

  // implements Message ----------------------------------------------

  tb_city_welfare* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const tb_city_welfare& from);
  void MergeFrom(const tb_city_welfare& from);
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

  // required uint32 family_id = 2;
  inline bool has_family_id() const;
  inline void clear_family_id();
  static const int kFamilyIdFieldNumber = 2;
  inline ::google::protobuf::uint32 family_id() const;
  inline void set_family_id(::google::protobuf::uint32 value);

  // required uint32 official_1 = 3;
  inline bool has_official_1() const;
  inline void clear_official_1();
  static const int kOfficial1FieldNumber = 3;
  inline ::google::protobuf::uint32 official_1() const;
  inline void set_official_1(::google::protobuf::uint32 value);

  // required uint32 official_2 = 4;
  inline bool has_official_2() const;
  inline void clear_official_2();
  static const int kOfficial2FieldNumber = 4;
  inline ::google::protobuf::uint32 official_2() const;
  inline void set_official_2(::google::protobuf::uint32 value);

  // required uint32 official_3 = 5;
  inline bool has_official_3() const;
  inline void clear_official_3();
  static const int kOfficial3FieldNumber = 5;
  inline ::google::protobuf::uint32 official_3() const;
  inline void set_official_3(::google::protobuf::uint32 value);

  // required uint32 official_4 = 6;
  inline bool has_official_4() const;
  inline void clear_official_4();
  static const int kOfficial4FieldNumber = 6;
  inline ::google::protobuf::uint32 official_4() const;
  inline void set_official_4(::google::protobuf::uint32 value);

  // required uint32 official_5 = 7;
  inline bool has_official_5() const;
  inline void clear_official_5();
  static const int kOfficial5FieldNumber = 7;
  inline ::google::protobuf::uint32 official_5() const;
  inline void set_official_5(::google::protobuf::uint32 value);

  // required uint32 official_6 = 8;
  inline bool has_official_6() const;
  inline void clear_official_6();
  static const int kOfficial6FieldNumber = 8;
  inline ::google::protobuf::uint32 official_6() const;
  inline void set_official_6(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:mysql.tb_city_welfare)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_family_id();
  inline void clear_has_family_id();
  inline void set_has_official_1();
  inline void clear_has_official_1();
  inline void set_has_official_2();
  inline void clear_has_official_2();
  inline void set_has_official_3();
  inline void clear_has_official_3();
  inline void set_has_official_4();
  inline void clear_has_official_4();
  inline void set_has_official_5();
  inline void clear_has_official_5();
  inline void set_has_official_6();
  inline void clear_has_official_6();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 family_id_;
  ::google::protobuf::uint32 official_1_;
  ::google::protobuf::uint32 official_2_;
  ::google::protobuf::uint32 official_3_;
  ::google::protobuf::uint32 official_4_;
  ::google::protobuf::uint32 official_5_;
  ::google::protobuf::uint32 official_6_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(8 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2etb_5fcity_5fwelfare_2eproto();
  friend void protobuf_AssignDesc_mysql_2etb_5fcity_5fwelfare_2eproto();
  friend void protobuf_ShutdownFile_mysql_2etb_5fcity_5fwelfare_2eproto();

  void InitAsDefaultInstance();
  static tb_city_welfare* default_instance_;
};
// ===================================================================


// ===================================================================

// tb_city_welfare

// required uint32 id = 1;
inline bool tb_city_welfare::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void tb_city_welfare::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void tb_city_welfare::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void tb_city_welfare::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 tb_city_welfare::id() const {
  return id_;
}
inline void tb_city_welfare::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required uint32 family_id = 2;
inline bool tb_city_welfare::has_family_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void tb_city_welfare::set_has_family_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void tb_city_welfare::clear_has_family_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void tb_city_welfare::clear_family_id() {
  family_id_ = 0u;
  clear_has_family_id();
}
inline ::google::protobuf::uint32 tb_city_welfare::family_id() const {
  return family_id_;
}
inline void tb_city_welfare::set_family_id(::google::protobuf::uint32 value) {
  set_has_family_id();
  family_id_ = value;
}

// required uint32 official_1 = 3;
inline bool tb_city_welfare::has_official_1() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void tb_city_welfare::set_has_official_1() {
  _has_bits_[0] |= 0x00000004u;
}
inline void tb_city_welfare::clear_has_official_1() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void tb_city_welfare::clear_official_1() {
  official_1_ = 0u;
  clear_has_official_1();
}
inline ::google::protobuf::uint32 tb_city_welfare::official_1() const {
  return official_1_;
}
inline void tb_city_welfare::set_official_1(::google::protobuf::uint32 value) {
  set_has_official_1();
  official_1_ = value;
}

// required uint32 official_2 = 4;
inline bool tb_city_welfare::has_official_2() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void tb_city_welfare::set_has_official_2() {
  _has_bits_[0] |= 0x00000008u;
}
inline void tb_city_welfare::clear_has_official_2() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void tb_city_welfare::clear_official_2() {
  official_2_ = 0u;
  clear_has_official_2();
}
inline ::google::protobuf::uint32 tb_city_welfare::official_2() const {
  return official_2_;
}
inline void tb_city_welfare::set_official_2(::google::protobuf::uint32 value) {
  set_has_official_2();
  official_2_ = value;
}

// required uint32 official_3 = 5;
inline bool tb_city_welfare::has_official_3() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void tb_city_welfare::set_has_official_3() {
  _has_bits_[0] |= 0x00000010u;
}
inline void tb_city_welfare::clear_has_official_3() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void tb_city_welfare::clear_official_3() {
  official_3_ = 0u;
  clear_has_official_3();
}
inline ::google::protobuf::uint32 tb_city_welfare::official_3() const {
  return official_3_;
}
inline void tb_city_welfare::set_official_3(::google::protobuf::uint32 value) {
  set_has_official_3();
  official_3_ = value;
}

// required uint32 official_4 = 6;
inline bool tb_city_welfare::has_official_4() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void tb_city_welfare::set_has_official_4() {
  _has_bits_[0] |= 0x00000020u;
}
inline void tb_city_welfare::clear_has_official_4() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void tb_city_welfare::clear_official_4() {
  official_4_ = 0u;
  clear_has_official_4();
}
inline ::google::protobuf::uint32 tb_city_welfare::official_4() const {
  return official_4_;
}
inline void tb_city_welfare::set_official_4(::google::protobuf::uint32 value) {
  set_has_official_4();
  official_4_ = value;
}

// required uint32 official_5 = 7;
inline bool tb_city_welfare::has_official_5() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void tb_city_welfare::set_has_official_5() {
  _has_bits_[0] |= 0x00000040u;
}
inline void tb_city_welfare::clear_has_official_5() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void tb_city_welfare::clear_official_5() {
  official_5_ = 0u;
  clear_has_official_5();
}
inline ::google::protobuf::uint32 tb_city_welfare::official_5() const {
  return official_5_;
}
inline void tb_city_welfare::set_official_5(::google::protobuf::uint32 value) {
  set_has_official_5();
  official_5_ = value;
}

// required uint32 official_6 = 8;
inline bool tb_city_welfare::has_official_6() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void tb_city_welfare::set_has_official_6() {
  _has_bits_[0] |= 0x00000080u;
}
inline void tb_city_welfare::clear_has_official_6() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void tb_city_welfare::clear_official_6() {
  official_6_ = 0u;
  clear_has_official_6();
}
inline ::google::protobuf::uint32 tb_city_welfare::official_6() const {
  return official_6_;
}
inline void tb_city_welfare::set_official_6(::google::protobuf::uint32 value) {
  set_has_official_6();
  official_6_ = value;
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

#endif  // PROTOBUF_mysql_2etb_5fcity_5fwelfare_2eproto__INCLUDED
