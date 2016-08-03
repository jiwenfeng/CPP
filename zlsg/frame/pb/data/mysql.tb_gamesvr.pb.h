// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_gamesvr.proto

#ifndef PROTOBUF_mysql_2etb_5fgamesvr_2eproto__INCLUDED
#define PROTOBUF_mysql_2etb_5fgamesvr_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2etb_5fgamesvr_2eproto();
void protobuf_AssignDesc_mysql_2etb_5fgamesvr_2eproto();
void protobuf_ShutdownFile_mysql_2etb_5fgamesvr_2eproto();

class tb_gamesvr;

// ===================================================================

class tb_gamesvr : public ::google::protobuf::Message {
 public:
  tb_gamesvr();
  virtual ~tb_gamesvr();

  tb_gamesvr(const tb_gamesvr& from);

  inline tb_gamesvr& operator=(const tb_gamesvr& from) {
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
  static const tb_gamesvr& default_instance();

  void Swap(tb_gamesvr* other);

  // implements Message ----------------------------------------------

  tb_gamesvr* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const tb_gamesvr& from);
  void MergeFrom(const tb_gamesvr& from);
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

  // required uint32 svr_id = 1;
  inline bool has_svr_id() const;
  inline void clear_svr_id();
  static const int kSvrIdFieldNumber = 1;
  inline ::google::protobuf::uint32 svr_id() const;
  inline void set_svr_id(::google::protobuf::uint32 value);

  // required string svr_name = 2;
  inline bool has_svr_name() const;
  inline void clear_svr_name();
  static const int kSvrNameFieldNumber = 2;
  inline const ::std::string& svr_name() const;
  inline void set_svr_name(const ::std::string& value);
  inline void set_svr_name(const char* value);
  inline void set_svr_name(const char* value, size_t size);
  inline ::std::string* mutable_svr_name();
  inline ::std::string* release_svr_name();
  inline void set_allocated_svr_name(::std::string* svr_name);

  // required uint32 player_port = 3;
  inline bool has_player_port() const;
  inline void clear_player_port();
  static const int kPlayerPortFieldNumber = 3;
  inline ::google::protobuf::uint32 player_port() const;
  inline void set_player_port(::google::protobuf::uint32 value);

  // required uint32 inner_port = 4;
  inline bool has_inner_port() const;
  inline void clear_inner_port();
  static const int kInnerPortFieldNumber = 4;
  inline ::google::protobuf::uint32 inner_port() const;
  inline void set_inner_port(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:mysql.tb_gamesvr)
 private:
  inline void set_has_svr_id();
  inline void clear_has_svr_id();
  inline void set_has_svr_name();
  inline void clear_has_svr_name();
  inline void set_has_player_port();
  inline void clear_has_player_port();
  inline void set_has_inner_port();
  inline void clear_has_inner_port();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* svr_name_;
  ::google::protobuf::uint32 svr_id_;
  ::google::protobuf::uint32 player_port_;
  ::google::protobuf::uint32 inner_port_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2etb_5fgamesvr_2eproto();
  friend void protobuf_AssignDesc_mysql_2etb_5fgamesvr_2eproto();
  friend void protobuf_ShutdownFile_mysql_2etb_5fgamesvr_2eproto();

  void InitAsDefaultInstance();
  static tb_gamesvr* default_instance_;
};
// ===================================================================


// ===================================================================

// tb_gamesvr

// required uint32 svr_id = 1;
inline bool tb_gamesvr::has_svr_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void tb_gamesvr::set_has_svr_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void tb_gamesvr::clear_has_svr_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void tb_gamesvr::clear_svr_id() {
  svr_id_ = 0u;
  clear_has_svr_id();
}
inline ::google::protobuf::uint32 tb_gamesvr::svr_id() const {
  return svr_id_;
}
inline void tb_gamesvr::set_svr_id(::google::protobuf::uint32 value) {
  set_has_svr_id();
  svr_id_ = value;
}

// required string svr_name = 2;
inline bool tb_gamesvr::has_svr_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void tb_gamesvr::set_has_svr_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void tb_gamesvr::clear_has_svr_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void tb_gamesvr::clear_svr_name() {
  if (svr_name_ != &::google::protobuf::internal::kEmptyString) {
    svr_name_->clear();
  }
  clear_has_svr_name();
}
inline const ::std::string& tb_gamesvr::svr_name() const {
  return *svr_name_;
}
inline void tb_gamesvr::set_svr_name(const ::std::string& value) {
  set_has_svr_name();
  if (svr_name_ == &::google::protobuf::internal::kEmptyString) {
    svr_name_ = new ::std::string;
  }
  svr_name_->assign(value);
}
inline void tb_gamesvr::set_svr_name(const char* value) {
  set_has_svr_name();
  if (svr_name_ == &::google::protobuf::internal::kEmptyString) {
    svr_name_ = new ::std::string;
  }
  svr_name_->assign(value);
}
inline void tb_gamesvr::set_svr_name(const char* value, size_t size) {
  set_has_svr_name();
  if (svr_name_ == &::google::protobuf::internal::kEmptyString) {
    svr_name_ = new ::std::string;
  }
  svr_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* tb_gamesvr::mutable_svr_name() {
  set_has_svr_name();
  if (svr_name_ == &::google::protobuf::internal::kEmptyString) {
    svr_name_ = new ::std::string;
  }
  return svr_name_;
}
inline ::std::string* tb_gamesvr::release_svr_name() {
  clear_has_svr_name();
  if (svr_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = svr_name_;
    svr_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void tb_gamesvr::set_allocated_svr_name(::std::string* svr_name) {
  if (svr_name_ != &::google::protobuf::internal::kEmptyString) {
    delete svr_name_;
  }
  if (svr_name) {
    set_has_svr_name();
    svr_name_ = svr_name;
  } else {
    clear_has_svr_name();
    svr_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 player_port = 3;
inline bool tb_gamesvr::has_player_port() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void tb_gamesvr::set_has_player_port() {
  _has_bits_[0] |= 0x00000004u;
}
inline void tb_gamesvr::clear_has_player_port() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void tb_gamesvr::clear_player_port() {
  player_port_ = 0u;
  clear_has_player_port();
}
inline ::google::protobuf::uint32 tb_gamesvr::player_port() const {
  return player_port_;
}
inline void tb_gamesvr::set_player_port(::google::protobuf::uint32 value) {
  set_has_player_port();
  player_port_ = value;
}

// required uint32 inner_port = 4;
inline bool tb_gamesvr::has_inner_port() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void tb_gamesvr::set_has_inner_port() {
  _has_bits_[0] |= 0x00000008u;
}
inline void tb_gamesvr::clear_has_inner_port() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void tb_gamesvr::clear_inner_port() {
  inner_port_ = 0u;
  clear_has_inner_port();
}
inline ::google::protobuf::uint32 tb_gamesvr::inner_port() const {
  return inner_port_;
}
inline void tb_gamesvr::set_inner_port(::google::protobuf::uint32 value) {
  set_has_inner_port();
  inner_port_ = value;
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

#endif  // PROTOBUF_mysql_2etb_5fgamesvr_2eproto__INCLUDED
