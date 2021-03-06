// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.randnpc.proto

#ifndef PROTOBUF_client_2erandnpc_2eproto__INCLUDED
#define PROTOBUF_client_2erandnpc_2eproto__INCLUDED

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
void  protobuf_AddDesc_client_2erandnpc_2eproto();
void protobuf_AssignDesc_client_2erandnpc_2eproto();
void protobuf_ShutdownFile_client_2erandnpc_2eproto();

class randnpc_postion;
class randnpc;
class randnpc_set;

// ===================================================================

class randnpc_postion : public ::google::protobuf::Message {
 public:
  randnpc_postion();
  virtual ~randnpc_postion();

  randnpc_postion(const randnpc_postion& from);

  inline randnpc_postion& operator=(const randnpc_postion& from) {
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
  static const randnpc_postion& default_instance();

  void Swap(randnpc_postion* other);

  // implements Message ----------------------------------------------

  randnpc_postion* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const randnpc_postion& from);
  void MergeFrom(const randnpc_postion& from);
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

  // required int32 x = 1 [default = 0];
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 1;
  inline ::google::protobuf::int32 x() const;
  inline void set_x(::google::protobuf::int32 value);

  // required int32 y = 2 [default = 0];
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 2;
  inline ::google::protobuf::int32 y() const;
  inline void set_y(::google::protobuf::int32 value);

  // required int32 z = 3 [default = 0];
  inline bool has_z() const;
  inline void clear_z();
  static const int kZFieldNumber = 3;
  inline ::google::protobuf::int32 z() const;
  inline void set_z(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:client.randnpc_postion)
 private:
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();
  inline void set_has_z();
  inline void clear_has_z();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 x_;
  ::google::protobuf::int32 y_;
  ::google::protobuf::int32 z_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erandnpc_2eproto();
  friend void protobuf_AssignDesc_client_2erandnpc_2eproto();
  friend void protobuf_ShutdownFile_client_2erandnpc_2eproto();

  void InitAsDefaultInstance();
  static randnpc_postion* default_instance_;
};
// -------------------------------------------------------------------

class randnpc : public ::google::protobuf::Message {
 public:
  randnpc();
  virtual ~randnpc();

  randnpc(const randnpc& from);

  inline randnpc& operator=(const randnpc& from) {
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
  static const randnpc& default_instance();

  void Swap(randnpc* other);

  // implements Message ----------------------------------------------

  randnpc* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const randnpc& from);
  void MergeFrom(const randnpc& from);
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

  // required uint32 role_id = 1 [default = 0];
  inline bool has_role_id() const;
  inline void clear_role_id();
  static const int kRoleIdFieldNumber = 1;
  inline ::google::protobuf::uint32 role_id() const;
  inline void set_role_id(::google::protobuf::uint32 value);

  // required uint32 map_id = 2 [default = 0];
  inline bool has_map_id() const;
  inline void clear_map_id();
  static const int kMapIdFieldNumber = 2;
  inline ::google::protobuf::uint32 map_id() const;
  inline void set_map_id(::google::protobuf::uint32 value);

  // required uint32 scene_id = 3 [default = 0];
  inline bool has_scene_id() const;
  inline void clear_scene_id();
  static const int kSceneIdFieldNumber = 3;
  inline ::google::protobuf::uint32 scene_id() const;
  inline void set_scene_id(::google::protobuf::uint32 value);

  // required .client.randnpc_postion born_pos = 4;
  inline bool has_born_pos() const;
  inline void clear_born_pos();
  static const int kBornPosFieldNumber = 4;
  inline const ::client::randnpc_postion& born_pos() const;
  inline ::client::randnpc_postion* mutable_born_pos();
  inline ::client::randnpc_postion* release_born_pos();
  inline void set_allocated_born_pos(::client::randnpc_postion* born_pos);

  // repeated .client.randnpc_postion path_rule = 5;
  inline int path_rule_size() const;
  inline void clear_path_rule();
  static const int kPathRuleFieldNumber = 5;
  inline const ::client::randnpc_postion& path_rule(int index) const;
  inline ::client::randnpc_postion* mutable_path_rule(int index);
  inline ::client::randnpc_postion* add_path_rule();
  inline const ::google::protobuf::RepeatedPtrField< ::client::randnpc_postion >&
      path_rule() const;
  inline ::google::protobuf::RepeatedPtrField< ::client::randnpc_postion >*
      mutable_path_rule();

  // @@protoc_insertion_point(class_scope:client.randnpc)
 private:
  inline void set_has_role_id();
  inline void clear_has_role_id();
  inline void set_has_map_id();
  inline void clear_has_map_id();
  inline void set_has_scene_id();
  inline void clear_has_scene_id();
  inline void set_has_born_pos();
  inline void clear_has_born_pos();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 role_id_;
  ::google::protobuf::uint32 map_id_;
  ::client::randnpc_postion* born_pos_;
  ::google::protobuf::RepeatedPtrField< ::client::randnpc_postion > path_rule_;
  ::google::protobuf::uint32 scene_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erandnpc_2eproto();
  friend void protobuf_AssignDesc_client_2erandnpc_2eproto();
  friend void protobuf_ShutdownFile_client_2erandnpc_2eproto();

  void InitAsDefaultInstance();
  static randnpc* default_instance_;
};
// -------------------------------------------------------------------

class randnpc_set : public ::google::protobuf::Message {
 public:
  randnpc_set();
  virtual ~randnpc_set();

  randnpc_set(const randnpc_set& from);

  inline randnpc_set& operator=(const randnpc_set& from) {
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
  static const randnpc_set& default_instance();

  void Swap(randnpc_set* other);

  // implements Message ----------------------------------------------

  randnpc_set* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const randnpc_set& from);
  void MergeFrom(const randnpc_set& from);
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

  // repeated .client.randnpc rn = 1;
  inline int rn_size() const;
  inline void clear_rn();
  static const int kRnFieldNumber = 1;
  inline const ::client::randnpc& rn(int index) const;
  inline ::client::randnpc* mutable_rn(int index);
  inline ::client::randnpc* add_rn();
  inline const ::google::protobuf::RepeatedPtrField< ::client::randnpc >&
      rn() const;
  inline ::google::protobuf::RepeatedPtrField< ::client::randnpc >*
      mutable_rn();

  // @@protoc_insertion_point(class_scope:client.randnpc_set)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::client::randnpc > rn_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erandnpc_2eproto();
  friend void protobuf_AssignDesc_client_2erandnpc_2eproto();
  friend void protobuf_ShutdownFile_client_2erandnpc_2eproto();

  void InitAsDefaultInstance();
  static randnpc_set* default_instance_;
};
// ===================================================================


// ===================================================================

// randnpc_postion

// required int32 x = 1 [default = 0];
inline bool randnpc_postion::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void randnpc_postion::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
inline void randnpc_postion::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void randnpc_postion::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline ::google::protobuf::int32 randnpc_postion::x() const {
  return x_;
}
inline void randnpc_postion::set_x(::google::protobuf::int32 value) {
  set_has_x();
  x_ = value;
}

// required int32 y = 2 [default = 0];
inline bool randnpc_postion::has_y() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void randnpc_postion::set_has_y() {
  _has_bits_[0] |= 0x00000002u;
}
inline void randnpc_postion::clear_has_y() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void randnpc_postion::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline ::google::protobuf::int32 randnpc_postion::y() const {
  return y_;
}
inline void randnpc_postion::set_y(::google::protobuf::int32 value) {
  set_has_y();
  y_ = value;
}

// required int32 z = 3 [default = 0];
inline bool randnpc_postion::has_z() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void randnpc_postion::set_has_z() {
  _has_bits_[0] |= 0x00000004u;
}
inline void randnpc_postion::clear_has_z() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void randnpc_postion::clear_z() {
  z_ = 0;
  clear_has_z();
}
inline ::google::protobuf::int32 randnpc_postion::z() const {
  return z_;
}
inline void randnpc_postion::set_z(::google::protobuf::int32 value) {
  set_has_z();
  z_ = value;
}

// -------------------------------------------------------------------

// randnpc

// required uint32 role_id = 1 [default = 0];
inline bool randnpc::has_role_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void randnpc::set_has_role_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void randnpc::clear_has_role_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void randnpc::clear_role_id() {
  role_id_ = 0u;
  clear_has_role_id();
}
inline ::google::protobuf::uint32 randnpc::role_id() const {
  return role_id_;
}
inline void randnpc::set_role_id(::google::protobuf::uint32 value) {
  set_has_role_id();
  role_id_ = value;
}

// required uint32 map_id = 2 [default = 0];
inline bool randnpc::has_map_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void randnpc::set_has_map_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void randnpc::clear_has_map_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void randnpc::clear_map_id() {
  map_id_ = 0u;
  clear_has_map_id();
}
inline ::google::protobuf::uint32 randnpc::map_id() const {
  return map_id_;
}
inline void randnpc::set_map_id(::google::protobuf::uint32 value) {
  set_has_map_id();
  map_id_ = value;
}

// required uint32 scene_id = 3 [default = 0];
inline bool randnpc::has_scene_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void randnpc::set_has_scene_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void randnpc::clear_has_scene_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void randnpc::clear_scene_id() {
  scene_id_ = 0u;
  clear_has_scene_id();
}
inline ::google::protobuf::uint32 randnpc::scene_id() const {
  return scene_id_;
}
inline void randnpc::set_scene_id(::google::protobuf::uint32 value) {
  set_has_scene_id();
  scene_id_ = value;
}

// required .client.randnpc_postion born_pos = 4;
inline bool randnpc::has_born_pos() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void randnpc::set_has_born_pos() {
  _has_bits_[0] |= 0x00000008u;
}
inline void randnpc::clear_has_born_pos() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void randnpc::clear_born_pos() {
  if (born_pos_ != NULL) born_pos_->::client::randnpc_postion::Clear();
  clear_has_born_pos();
}
inline const ::client::randnpc_postion& randnpc::born_pos() const {
  return born_pos_ != NULL ? *born_pos_ : *default_instance_->born_pos_;
}
inline ::client::randnpc_postion* randnpc::mutable_born_pos() {
  set_has_born_pos();
  if (born_pos_ == NULL) born_pos_ = new ::client::randnpc_postion;
  return born_pos_;
}
inline ::client::randnpc_postion* randnpc::release_born_pos() {
  clear_has_born_pos();
  ::client::randnpc_postion* temp = born_pos_;
  born_pos_ = NULL;
  return temp;
}
inline void randnpc::set_allocated_born_pos(::client::randnpc_postion* born_pos) {
  delete born_pos_;
  born_pos_ = born_pos;
  if (born_pos) {
    set_has_born_pos();
  } else {
    clear_has_born_pos();
  }
}

// repeated .client.randnpc_postion path_rule = 5;
inline int randnpc::path_rule_size() const {
  return path_rule_.size();
}
inline void randnpc::clear_path_rule() {
  path_rule_.Clear();
}
inline const ::client::randnpc_postion& randnpc::path_rule(int index) const {
  return path_rule_.Get(index);
}
inline ::client::randnpc_postion* randnpc::mutable_path_rule(int index) {
  return path_rule_.Mutable(index);
}
inline ::client::randnpc_postion* randnpc::add_path_rule() {
  return path_rule_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::client::randnpc_postion >&
randnpc::path_rule() const {
  return path_rule_;
}
inline ::google::protobuf::RepeatedPtrField< ::client::randnpc_postion >*
randnpc::mutable_path_rule() {
  return &path_rule_;
}

// -------------------------------------------------------------------

// randnpc_set

// repeated .client.randnpc rn = 1;
inline int randnpc_set::rn_size() const {
  return rn_.size();
}
inline void randnpc_set::clear_rn() {
  rn_.Clear();
}
inline const ::client::randnpc& randnpc_set::rn(int index) const {
  return rn_.Get(index);
}
inline ::client::randnpc* randnpc_set::mutable_rn(int index) {
  return rn_.Mutable(index);
}
inline ::client::randnpc* randnpc_set::add_rn() {
  return rn_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::client::randnpc >&
randnpc_set::rn() const {
  return rn_;
}
inline ::google::protobuf::RepeatedPtrField< ::client::randnpc >*
randnpc_set::mutable_rn() {
  return &rn_;
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

#endif  // PROTOBUF_client_2erandnpc_2eproto__INCLUDED
