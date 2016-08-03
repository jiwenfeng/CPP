// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_center_devil.proto

#ifndef PROTOBUF_mysql_2etb_5fcenter_5fdevil_2eproto__INCLUDED
#define PROTOBUF_mysql_2etb_5fcenter_5fdevil_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2etb_5fcenter_5fdevil_2eproto();
void protobuf_AssignDesc_mysql_2etb_5fcenter_5fdevil_2eproto();
void protobuf_ShutdownFile_mysql_2etb_5fcenter_5fdevil_2eproto();

class tb_center_devil;

// ===================================================================

class tb_center_devil : public ::google::protobuf::Message {
 public:
  tb_center_devil();
  virtual ~tb_center_devil();

  tb_center_devil(const tb_center_devil& from);

  inline tb_center_devil& operator=(const tb_center_devil& from) {
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
  static const tb_center_devil& default_instance();

  void Swap(tb_center_devil* other);

  // implements Message ----------------------------------------------

  tb_center_devil* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const tb_center_devil& from);
  void MergeFrom(const tb_center_devil& from);
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

  // required uint32 role_id = 2;
  inline bool has_role_id() const;
  inline void clear_role_id();
  static const int kRoleIdFieldNumber = 2;
  inline ::google::protobuf::uint32 role_id() const;
  inline void set_role_id(::google::protobuf::uint32 value);

  // required uint32 time = 3;
  inline bool has_time() const;
  inline void clear_time();
  static const int kTimeFieldNumber = 3;
  inline ::google::protobuf::uint32 time() const;
  inline void set_time(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:mysql.tb_center_devil)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_role_id();
  inline void clear_has_role_id();
  inline void set_has_time();
  inline void clear_has_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 role_id_;
  ::google::protobuf::uint32 time_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2etb_5fcenter_5fdevil_2eproto();
  friend void protobuf_AssignDesc_mysql_2etb_5fcenter_5fdevil_2eproto();
  friend void protobuf_ShutdownFile_mysql_2etb_5fcenter_5fdevil_2eproto();

  void InitAsDefaultInstance();
  static tb_center_devil* default_instance_;
};
// ===================================================================


// ===================================================================

// tb_center_devil

// required uint32 id = 1;
inline bool tb_center_devil::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void tb_center_devil::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void tb_center_devil::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void tb_center_devil::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 tb_center_devil::id() const {
  return id_;
}
inline void tb_center_devil::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required uint32 role_id = 2;
inline bool tb_center_devil::has_role_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void tb_center_devil::set_has_role_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void tb_center_devil::clear_has_role_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void tb_center_devil::clear_role_id() {
  role_id_ = 0u;
  clear_has_role_id();
}
inline ::google::protobuf::uint32 tb_center_devil::role_id() const {
  return role_id_;
}
inline void tb_center_devil::set_role_id(::google::protobuf::uint32 value) {
  set_has_role_id();
  role_id_ = value;
}

// required uint32 time = 3;
inline bool tb_center_devil::has_time() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void tb_center_devil::set_has_time() {
  _has_bits_[0] |= 0x00000004u;
}
inline void tb_center_devil::clear_has_time() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void tb_center_devil::clear_time() {
  time_ = 0u;
  clear_has_time();
}
inline ::google::protobuf::uint32 tb_center_devil::time() const {
  return time_;
}
inline void tb_center_devil::set_time(::google::protobuf::uint32 value) {
  set_has_time();
  time_ = value;
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

#endif  // PROTOBUF_mysql_2etb_5fcenter_5fdevil_2eproto__INCLUDED
