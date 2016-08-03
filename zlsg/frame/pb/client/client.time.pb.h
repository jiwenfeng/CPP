// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.time.proto

#ifndef PROTOBUF_client_2etime_2eproto__INCLUDED
#define PROTOBUF_client_2etime_2eproto__INCLUDED

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
void  protobuf_AddDesc_client_2etime_2eproto();
void protobuf_AssignDesc_client_2etime_2eproto();
void protobuf_ShutdownFile_client_2etime_2eproto();

class server_time_req;
class server_time_rsp;
class server_open_activity_msg;

enum server_open_activity_msg_ACTIVITY_TYPE {
  server_open_activity_msg_ACTIVITY_TYPE_SPA = 1,
  server_open_activity_msg_ACTIVITY_TYPE_CONVOY = 2,
  server_open_activity_msg_ACTIVITY_TYPE_GODCRAFT = 3,
  server_open_activity_msg_ACTIVITY_TYPE_FAMILY_BOSS = 4,
  server_open_activity_msg_ACTIVITY_TYPE_BATTLE = 5,
  server_open_activity_msg_ACTIVITY_TYPE_FAMILY_SCENE = 6,
  server_open_activity_msg_ACTIVITY_TYPE_DOMAIN_BATTLE = 7,
  server_open_activity_msg_ACTIVITY_TYPE_SIEGE = 8,
  server_open_activity_msg_ACTIVITY_TYPE_SECT = 9,
  server_open_activity_msg_ACTIVITY_TYPE_FILED_SCORE = 10,
  server_open_activity_msg_ACTIVITY_TYPE_GEM = 11
};
bool server_open_activity_msg_ACTIVITY_TYPE_IsValid(int value);
const server_open_activity_msg_ACTIVITY_TYPE server_open_activity_msg_ACTIVITY_TYPE_ACTIVITY_TYPE_MIN = server_open_activity_msg_ACTIVITY_TYPE_SPA;
const server_open_activity_msg_ACTIVITY_TYPE server_open_activity_msg_ACTIVITY_TYPE_ACTIVITY_TYPE_MAX = server_open_activity_msg_ACTIVITY_TYPE_GEM;
const int server_open_activity_msg_ACTIVITY_TYPE_ACTIVITY_TYPE_ARRAYSIZE = server_open_activity_msg_ACTIVITY_TYPE_ACTIVITY_TYPE_MAX + 1;

const ::google::protobuf::EnumDescriptor* server_open_activity_msg_ACTIVITY_TYPE_descriptor();
inline const ::std::string& server_open_activity_msg_ACTIVITY_TYPE_Name(server_open_activity_msg_ACTIVITY_TYPE value) {
  return ::google::protobuf::internal::NameOfEnum(
    server_open_activity_msg_ACTIVITY_TYPE_descriptor(), value);
}
inline bool server_open_activity_msg_ACTIVITY_TYPE_Parse(
    const ::std::string& name, server_open_activity_msg_ACTIVITY_TYPE* value) {
  return ::google::protobuf::internal::ParseNamedEnum<server_open_activity_msg_ACTIVITY_TYPE>(
    server_open_activity_msg_ACTIVITY_TYPE_descriptor(), name, value);
}
// ===================================================================

class server_time_req : public ::google::protobuf::Message {
 public:
  server_time_req();
  virtual ~server_time_req();

  server_time_req(const server_time_req& from);

  inline server_time_req& operator=(const server_time_req& from) {
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
  static const server_time_req& default_instance();

  void Swap(server_time_req* other);

  // implements Message ----------------------------------------------

  server_time_req* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const server_time_req& from);
  void MergeFrom(const server_time_req& from);
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

  // @@protoc_insertion_point(class_scope:client.server_time_req)
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

  friend void  protobuf_AddDesc_client_2etime_2eproto();
  friend void protobuf_AssignDesc_client_2etime_2eproto();
  friend void protobuf_ShutdownFile_client_2etime_2eproto();

  void InitAsDefaultInstance();
  static server_time_req* default_instance_;
};
// -------------------------------------------------------------------

class server_time_rsp : public ::google::protobuf::Message {
 public:
  server_time_rsp();
  virtual ~server_time_rsp();

  server_time_rsp(const server_time_rsp& from);

  inline server_time_rsp& operator=(const server_time_rsp& from) {
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
  static const server_time_rsp& default_instance();

  void Swap(server_time_rsp* other);

  // implements Message ----------------------------------------------

  server_time_rsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const server_time_rsp& from);
  void MergeFrom(const server_time_rsp& from);
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

  // @@protoc_insertion_point(class_scope:client.server_time_rsp)
 private:
  inline void set_has_unixtime();
  inline void clear_has_unixtime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 unixtime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_client_2etime_2eproto();
  friend void protobuf_AssignDesc_client_2etime_2eproto();
  friend void protobuf_ShutdownFile_client_2etime_2eproto();

  void InitAsDefaultInstance();
  static server_time_rsp* default_instance_;
};
// -------------------------------------------------------------------

class server_open_activity_msg : public ::google::protobuf::Message {
 public:
  server_open_activity_msg();
  virtual ~server_open_activity_msg();

  server_open_activity_msg(const server_open_activity_msg& from);

  inline server_open_activity_msg& operator=(const server_open_activity_msg& from) {
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
  static const server_open_activity_msg& default_instance();

  void Swap(server_open_activity_msg* other);

  // implements Message ----------------------------------------------

  server_open_activity_msg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const server_open_activity_msg& from);
  void MergeFrom(const server_open_activity_msg& from);
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

  typedef server_open_activity_msg_ACTIVITY_TYPE ACTIVITY_TYPE;
  static const ACTIVITY_TYPE SPA = server_open_activity_msg_ACTIVITY_TYPE_SPA;
  static const ACTIVITY_TYPE CONVOY = server_open_activity_msg_ACTIVITY_TYPE_CONVOY;
  static const ACTIVITY_TYPE GODCRAFT = server_open_activity_msg_ACTIVITY_TYPE_GODCRAFT;
  static const ACTIVITY_TYPE FAMILY_BOSS = server_open_activity_msg_ACTIVITY_TYPE_FAMILY_BOSS;
  static const ACTIVITY_TYPE BATTLE = server_open_activity_msg_ACTIVITY_TYPE_BATTLE;
  static const ACTIVITY_TYPE FAMILY_SCENE = server_open_activity_msg_ACTIVITY_TYPE_FAMILY_SCENE;
  static const ACTIVITY_TYPE DOMAIN_BATTLE = server_open_activity_msg_ACTIVITY_TYPE_DOMAIN_BATTLE;
  static const ACTIVITY_TYPE SIEGE = server_open_activity_msg_ACTIVITY_TYPE_SIEGE;
  static const ACTIVITY_TYPE SECT = server_open_activity_msg_ACTIVITY_TYPE_SECT;
  static const ACTIVITY_TYPE FILED_SCORE = server_open_activity_msg_ACTIVITY_TYPE_FILED_SCORE;
  static const ACTIVITY_TYPE GEM = server_open_activity_msg_ACTIVITY_TYPE_GEM;
  static inline bool ACTIVITY_TYPE_IsValid(int value) {
    return server_open_activity_msg_ACTIVITY_TYPE_IsValid(value);
  }
  static const ACTIVITY_TYPE ACTIVITY_TYPE_MIN =
    server_open_activity_msg_ACTIVITY_TYPE_ACTIVITY_TYPE_MIN;
  static const ACTIVITY_TYPE ACTIVITY_TYPE_MAX =
    server_open_activity_msg_ACTIVITY_TYPE_ACTIVITY_TYPE_MAX;
  static const int ACTIVITY_TYPE_ARRAYSIZE =
    server_open_activity_msg_ACTIVITY_TYPE_ACTIVITY_TYPE_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  ACTIVITY_TYPE_descriptor() {
    return server_open_activity_msg_ACTIVITY_TYPE_descriptor();
  }
  static inline const ::std::string& ACTIVITY_TYPE_Name(ACTIVITY_TYPE value) {
    return server_open_activity_msg_ACTIVITY_TYPE_Name(value);
  }
  static inline bool ACTIVITY_TYPE_Parse(const ::std::string& name,
      ACTIVITY_TYPE* value) {
    return server_open_activity_msg_ACTIVITY_TYPE_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required .client.server_open_activity_msg.ACTIVITY_TYPE activity_type = 1;
  inline bool has_activity_type() const;
  inline void clear_activity_type();
  static const int kActivityTypeFieldNumber = 1;
  inline ::client::server_open_activity_msg_ACTIVITY_TYPE activity_type() const;
  inline void set_activity_type(::client::server_open_activity_msg_ACTIVITY_TYPE value);

  // @@protoc_insertion_point(class_scope:client.server_open_activity_msg)
 private:
  inline void set_has_activity_type();
  inline void clear_has_activity_type();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  int activity_type_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_client_2etime_2eproto();
  friend void protobuf_AssignDesc_client_2etime_2eproto();
  friend void protobuf_ShutdownFile_client_2etime_2eproto();

  void InitAsDefaultInstance();
  static server_open_activity_msg* default_instance_;
};
// ===================================================================


// ===================================================================

// server_time_req

// required uint32 unixtime = 1;
inline bool server_time_req::has_unixtime() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void server_time_req::set_has_unixtime() {
  _has_bits_[0] |= 0x00000001u;
}
inline void server_time_req::clear_has_unixtime() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void server_time_req::clear_unixtime() {
  unixtime_ = 0u;
  clear_has_unixtime();
}
inline ::google::protobuf::uint32 server_time_req::unixtime() const {
  return unixtime_;
}
inline void server_time_req::set_unixtime(::google::protobuf::uint32 value) {
  set_has_unixtime();
  unixtime_ = value;
}

// optional .client.server_head svr_head = 1000;
inline bool server_time_req::has_svr_head() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void server_time_req::set_has_svr_head() {
  _has_bits_[0] |= 0x00000002u;
}
inline void server_time_req::clear_has_svr_head() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void server_time_req::clear_svr_head() {
  if (svr_head_ != NULL) svr_head_->::client::server_head::Clear();
  clear_has_svr_head();
}
inline const ::client::server_head& server_time_req::svr_head() const {
  return svr_head_ != NULL ? *svr_head_ : *default_instance_->svr_head_;
}
inline ::client::server_head* server_time_req::mutable_svr_head() {
  set_has_svr_head();
  if (svr_head_ == NULL) svr_head_ = new ::client::server_head;
  return svr_head_;
}
inline ::client::server_head* server_time_req::release_svr_head() {
  clear_has_svr_head();
  ::client::server_head* temp = svr_head_;
  svr_head_ = NULL;
  return temp;
}
inline void server_time_req::set_allocated_svr_head(::client::server_head* svr_head) {
  delete svr_head_;
  svr_head_ = svr_head;
  if (svr_head) {
    set_has_svr_head();
  } else {
    clear_has_svr_head();
  }
}

// -------------------------------------------------------------------

// server_time_rsp

// required uint32 unixtime = 1;
inline bool server_time_rsp::has_unixtime() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void server_time_rsp::set_has_unixtime() {
  _has_bits_[0] |= 0x00000001u;
}
inline void server_time_rsp::clear_has_unixtime() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void server_time_rsp::clear_unixtime() {
  unixtime_ = 0u;
  clear_has_unixtime();
}
inline ::google::protobuf::uint32 server_time_rsp::unixtime() const {
  return unixtime_;
}
inline void server_time_rsp::set_unixtime(::google::protobuf::uint32 value) {
  set_has_unixtime();
  unixtime_ = value;
}

// -------------------------------------------------------------------

// server_open_activity_msg

// required .client.server_open_activity_msg.ACTIVITY_TYPE activity_type = 1;
inline bool server_open_activity_msg::has_activity_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void server_open_activity_msg::set_has_activity_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void server_open_activity_msg::clear_has_activity_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void server_open_activity_msg::clear_activity_type() {
  activity_type_ = 1;
  clear_has_activity_type();
}
inline ::client::server_open_activity_msg_ACTIVITY_TYPE server_open_activity_msg::activity_type() const {
  return static_cast< ::client::server_open_activity_msg_ACTIVITY_TYPE >(activity_type_);
}
inline void server_open_activity_msg::set_activity_type(::client::server_open_activity_msg_ACTIVITY_TYPE value) {
  assert(::client::server_open_activity_msg_ACTIVITY_TYPE_IsValid(value));
  set_has_activity_type();
  activity_type_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace client

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::client::server_open_activity_msg_ACTIVITY_TYPE>() {
  return ::client::server_open_activity_msg_ACTIVITY_TYPE_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_client_2etime_2eproto__INCLUDED
