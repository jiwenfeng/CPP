// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.node.proto

#ifndef PROTOBUF_client_2enode_2eproto__INCLUDED
#define PROTOBUF_client_2enode_2eproto__INCLUDED

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
void  protobuf_AddDesc_client_2enode_2eproto();
void protobuf_AssignDesc_client_2enode_2eproto();
void protobuf_ShutdownFile_client_2enode_2eproto();

class agent_ready;
class user_data;

// ===================================================================

class agent_ready : public ::google::protobuf::Message {
 public:
  agent_ready();
  virtual ~agent_ready();

  agent_ready(const agent_ready& from);

  inline agent_ready& operator=(const agent_ready& from) {
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
  static const agent_ready& default_instance();

  void Swap(agent_ready* other);

  // implements Message ----------------------------------------------

  agent_ready* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const agent_ready& from);
  void MergeFrom(const agent_ready& from);
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

  // @@protoc_insertion_point(class_scope:client.agent_ready)
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

  friend void  protobuf_AddDesc_client_2enode_2eproto();
  friend void protobuf_AssignDesc_client_2enode_2eproto();
  friend void protobuf_ShutdownFile_client_2enode_2eproto();

  void InitAsDefaultInstance();
  static agent_ready* default_instance_;
};
// -------------------------------------------------------------------

class user_data : public ::google::protobuf::Message {
 public:
  user_data();
  virtual ~user_data();

  user_data(const user_data& from);

  inline user_data& operator=(const user_data& from) {
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
  static const user_data& default_instance();

  void Swap(user_data* other);

  // implements Message ----------------------------------------------

  user_data* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const user_data& from);
  void MergeFrom(const user_data& from);
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

  // required bytes db = 1;
  inline bool has_db() const;
  inline void clear_db();
  static const int kDbFieldNumber = 1;
  inline const ::std::string& db() const;
  inline void set_db(const ::std::string& value);
  inline void set_db(const char* value);
  inline void set_db(const void* value, size_t size);
  inline ::std::string* mutable_db();
  inline ::std::string* release_db();
  inline void set_allocated_db(::std::string* db);

  // required uint32 scene_id = 2;
  inline bool has_scene_id() const;
  inline void clear_scene_id();
  static const int kSceneIdFieldNumber = 2;
  inline ::google::protobuf::uint32 scene_id() const;
  inline void set_scene_id(::google::protobuf::uint32 value);

  // optional .client.server_head svr_head = 1000;
  inline bool has_svr_head() const;
  inline void clear_svr_head();
  static const int kSvrHeadFieldNumber = 1000;
  inline const ::client::server_head& svr_head() const;
  inline ::client::server_head* mutable_svr_head();
  inline ::client::server_head* release_svr_head();
  inline void set_allocated_svr_head(::client::server_head* svr_head);

  // @@protoc_insertion_point(class_scope:client.user_data)
 private:
  inline void set_has_db();
  inline void clear_has_db();
  inline void set_has_scene_id();
  inline void clear_has_scene_id();
  inline void set_has_svr_head();
  inline void clear_has_svr_head();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* db_;
  ::client::server_head* svr_head_;
  ::google::protobuf::uint32 scene_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_client_2enode_2eproto();
  friend void protobuf_AssignDesc_client_2enode_2eproto();
  friend void protobuf_ShutdownFile_client_2enode_2eproto();

  void InitAsDefaultInstance();
  static user_data* default_instance_;
};
// ===================================================================


// ===================================================================

// agent_ready

// required uint32 unixtime = 1;
inline bool agent_ready::has_unixtime() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void agent_ready::set_has_unixtime() {
  _has_bits_[0] |= 0x00000001u;
}
inline void agent_ready::clear_has_unixtime() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void agent_ready::clear_unixtime() {
  unixtime_ = 0u;
  clear_has_unixtime();
}
inline ::google::protobuf::uint32 agent_ready::unixtime() const {
  return unixtime_;
}
inline void agent_ready::set_unixtime(::google::protobuf::uint32 value) {
  set_has_unixtime();
  unixtime_ = value;
}

// optional .client.server_head svr_head = 1000;
inline bool agent_ready::has_svr_head() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void agent_ready::set_has_svr_head() {
  _has_bits_[0] |= 0x00000002u;
}
inline void agent_ready::clear_has_svr_head() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void agent_ready::clear_svr_head() {
  if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
  clear_has_svr_head();
}
inline const ::client::server_head& agent_ready::svr_head() const {
  return svr_head_ != NULL ? *svr_head_ : *default_instance_->svr_head_;
}
inline ::client::server_head* agent_ready::mutable_svr_head() {
  set_has_svr_head();
  if (svr_head_ == NULL) svr_head_ = new ::client::server_head;
  return svr_head_;
}
inline ::client::server_head* agent_ready::release_svr_head() {
  clear_has_svr_head();
  ::client::server_head* temp = svr_head_;
  svr_head_ = NULL;
  return temp;
}
inline void agent_ready::set_allocated_svr_head(::client::server_head* svr_head) {
  delete svr_head_;
  svr_head_ = svr_head;
  if (svr_head) {
    set_has_svr_head();
  } else {
    clear_has_svr_head();
  }
}

// -------------------------------------------------------------------

// user_data

// required bytes db = 1;
inline bool user_data::has_db() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void user_data::set_has_db() {
  _has_bits_[0] |= 0x00000001u;
}
inline void user_data::clear_has_db() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void user_data::clear_db() {
  if (db_ != &::google::protobuf::internal::kEmptyString) {
    db_->clear();
  }
  clear_has_db();
}
inline const ::std::string& user_data::db() const {
  return *db_;
}
inline void user_data::set_db(const ::std::string& value) {
  set_has_db();
  if (db_ == &::google::protobuf::internal::kEmptyString) {
    db_ = new ::std::string;
  }
  db_->assign(value);
}
inline void user_data::set_db(const char* value) {
  set_has_db();
  if (db_ == &::google::protobuf::internal::kEmptyString) {
    db_ = new ::std::string;
  }
  db_->assign(value);
}
inline void user_data::set_db(const void* value, size_t size) {
  set_has_db();
  if (db_ == &::google::protobuf::internal::kEmptyString) {
    db_ = new ::std::string;
  }
  db_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* user_data::mutable_db() {
  set_has_db();
  if (db_ == &::google::protobuf::internal::kEmptyString) {
    db_ = new ::std::string;
  }
  return db_;
}
inline ::std::string* user_data::release_db() {
  clear_has_db();
  if (db_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = db_;
    db_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void user_data::set_allocated_db(::std::string* db) {
  if (db_ != &::google::protobuf::internal::kEmptyString) {
    delete db_;
  }
  if (db) {
    set_has_db();
    db_ = db;
  } else {
    clear_has_db();
    db_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 scene_id = 2;
inline bool user_data::has_scene_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void user_data::set_has_scene_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void user_data::clear_has_scene_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void user_data::clear_scene_id() {
  scene_id_ = 0u;
  clear_has_scene_id();
}
inline ::google::protobuf::uint32 user_data::scene_id() const {
  return scene_id_;
}
inline void user_data::set_scene_id(::google::protobuf::uint32 value) {
  set_has_scene_id();
  scene_id_ = value;
}

// optional .client.server_head svr_head = 1000;
inline bool user_data::has_svr_head() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void user_data::set_has_svr_head() {
  _has_bits_[0] |= 0x00000004u;
}
inline void user_data::clear_has_svr_head() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void user_data::clear_svr_head() {
  if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
  clear_has_svr_head();
}
inline const ::client::server_head& user_data::svr_head() const {
  return svr_head_ != NULL ? *svr_head_ : *default_instance_->svr_head_;
}
inline ::client::server_head* user_data::mutable_svr_head() {
  set_has_svr_head();
  if (svr_head_ == NULL) svr_head_ = new ::client::server_head;
  return svr_head_;
}
inline ::client::server_head* user_data::release_svr_head() {
  clear_has_svr_head();
  ::client::server_head* temp = svr_head_;
  svr_head_ = NULL;
  return temp;
}
inline void user_data::set_allocated_svr_head(::client::server_head* svr_head) {
  delete svr_head_;
  svr_head_ = svr_head;
  if (svr_head) {
    set_has_svr_head();
  } else {
    clear_has_svr_head();
  }
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

#endif  // PROTOBUF_client_2enode_2eproto__INCLUDED
