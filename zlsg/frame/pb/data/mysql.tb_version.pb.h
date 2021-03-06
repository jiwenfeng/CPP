// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.tb_version.proto

#ifndef PROTOBUF_mysql_2etb_5fversion_2eproto__INCLUDED
#define PROTOBUF_mysql_2etb_5fversion_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2etb_5fversion_2eproto();
void protobuf_AssignDesc_mysql_2etb_5fversion_2eproto();
void protobuf_ShutdownFile_mysql_2etb_5fversion_2eproto();

class tb_version;

// ===================================================================

class tb_version : public ::google::protobuf::Message {
 public:
  tb_version();
  virtual ~tb_version();

  tb_version(const tb_version& from);

  inline tb_version& operator=(const tb_version& from) {
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
  static const tb_version& default_instance();

  void Swap(tb_version* other);

  // implements Message ----------------------------------------------

  tb_version* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const tb_version& from);
  void MergeFrom(const tb_version& from);
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

  // required string database = 2;
  inline bool has_database() const;
  inline void clear_database();
  static const int kDatabaseFieldNumber = 2;
  inline const ::std::string& database() const;
  inline void set_database(const ::std::string& value);
  inline void set_database(const char* value);
  inline void set_database(const char* value, size_t size);
  inline ::std::string* mutable_database();
  inline ::std::string* release_database();
  inline void set_allocated_database(::std::string* database);

  // required string server = 3;
  inline bool has_server() const;
  inline void clear_server();
  static const int kServerFieldNumber = 3;
  inline const ::std::string& server() const;
  inline void set_server(const ::std::string& value);
  inline void set_server(const char* value);
  inline void set_server(const char* value, size_t size);
  inline ::std::string* mutable_server();
  inline ::std::string* release_server();
  inline void set_allocated_server(::std::string* server);

  // required string client = 4;
  inline bool has_client() const;
  inline void clear_client();
  static const int kClientFieldNumber = 4;
  inline const ::std::string& client() const;
  inline void set_client(const ::std::string& value);
  inline void set_client(const char* value);
  inline void set_client(const char* value, size_t size);
  inline ::std::string* mutable_client();
  inline ::std::string* release_client();
  inline void set_allocated_client(::std::string* client);

  // required string time = 5;
  inline bool has_time() const;
  inline void clear_time();
  static const int kTimeFieldNumber = 5;
  inline const ::std::string& time() const;
  inline void set_time(const ::std::string& value);
  inline void set_time(const char* value);
  inline void set_time(const char* value, size_t size);
  inline ::std::string* mutable_time();
  inline ::std::string* release_time();
  inline void set_allocated_time(::std::string* time);

  // @@protoc_insertion_point(class_scope:mysql.tb_version)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_database();
  inline void clear_has_database();
  inline void set_has_server();
  inline void clear_has_server();
  inline void set_has_client();
  inline void clear_has_client();
  inline void set_has_time();
  inline void clear_has_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* database_;
  ::std::string* server_;
  ::std::string* client_;
  ::std::string* time_;
  ::google::protobuf::uint32 id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2etb_5fversion_2eproto();
  friend void protobuf_AssignDesc_mysql_2etb_5fversion_2eproto();
  friend void protobuf_ShutdownFile_mysql_2etb_5fversion_2eproto();

  void InitAsDefaultInstance();
  static tb_version* default_instance_;
};
// ===================================================================


// ===================================================================

// tb_version

// required uint32 id = 1;
inline bool tb_version::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void tb_version::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void tb_version::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void tb_version::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 tb_version::id() const {
  return id_;
}
inline void tb_version::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required string database = 2;
inline bool tb_version::has_database() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void tb_version::set_has_database() {
  _has_bits_[0] |= 0x00000002u;
}
inline void tb_version::clear_has_database() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void tb_version::clear_database() {
  if (database_ != &::google::protobuf::internal::kEmptyString) {
    database_->clear();
  }
  clear_has_database();
}
inline const ::std::string& tb_version::database() const {
  return *database_;
}
inline void tb_version::set_database(const ::std::string& value) {
  set_has_database();
  if (database_ == &::google::protobuf::internal::kEmptyString) {
    database_ = new ::std::string;
  }
  database_->assign(value);
}
inline void tb_version::set_database(const char* value) {
  set_has_database();
  if (database_ == &::google::protobuf::internal::kEmptyString) {
    database_ = new ::std::string;
  }
  database_->assign(value);
}
inline void tb_version::set_database(const char* value, size_t size) {
  set_has_database();
  if (database_ == &::google::protobuf::internal::kEmptyString) {
    database_ = new ::std::string;
  }
  database_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* tb_version::mutable_database() {
  set_has_database();
  if (database_ == &::google::protobuf::internal::kEmptyString) {
    database_ = new ::std::string;
  }
  return database_;
}
inline ::std::string* tb_version::release_database() {
  clear_has_database();
  if (database_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = database_;
    database_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void tb_version::set_allocated_database(::std::string* database) {
  if (database_ != &::google::protobuf::internal::kEmptyString) {
    delete database_;
  }
  if (database) {
    set_has_database();
    database_ = database;
  } else {
    clear_has_database();
    database_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string server = 3;
inline bool tb_version::has_server() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void tb_version::set_has_server() {
  _has_bits_[0] |= 0x00000004u;
}
inline void tb_version::clear_has_server() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void tb_version::clear_server() {
  if (server_ != &::google::protobuf::internal::kEmptyString) {
    server_->clear();
  }
  clear_has_server();
}
inline const ::std::string& tb_version::server() const {
  return *server_;
}
inline void tb_version::set_server(const ::std::string& value) {
  set_has_server();
  if (server_ == &::google::protobuf::internal::kEmptyString) {
    server_ = new ::std::string;
  }
  server_->assign(value);
}
inline void tb_version::set_server(const char* value) {
  set_has_server();
  if (server_ == &::google::protobuf::internal::kEmptyString) {
    server_ = new ::std::string;
  }
  server_->assign(value);
}
inline void tb_version::set_server(const char* value, size_t size) {
  set_has_server();
  if (server_ == &::google::protobuf::internal::kEmptyString) {
    server_ = new ::std::string;
  }
  server_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* tb_version::mutable_server() {
  set_has_server();
  if (server_ == &::google::protobuf::internal::kEmptyString) {
    server_ = new ::std::string;
  }
  return server_;
}
inline ::std::string* tb_version::release_server() {
  clear_has_server();
  if (server_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = server_;
    server_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void tb_version::set_allocated_server(::std::string* server) {
  if (server_ != &::google::protobuf::internal::kEmptyString) {
    delete server_;
  }
  if (server) {
    set_has_server();
    server_ = server;
  } else {
    clear_has_server();
    server_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string client = 4;
inline bool tb_version::has_client() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void tb_version::set_has_client() {
  _has_bits_[0] |= 0x00000008u;
}
inline void tb_version::clear_has_client() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void tb_version::clear_client() {
  if (client_ != &::google::protobuf::internal::kEmptyString) {
    client_->clear();
  }
  clear_has_client();
}
inline const ::std::string& tb_version::client() const {
  return *client_;
}
inline void tb_version::set_client(const ::std::string& value) {
  set_has_client();
  if (client_ == &::google::protobuf::internal::kEmptyString) {
    client_ = new ::std::string;
  }
  client_->assign(value);
}
inline void tb_version::set_client(const char* value) {
  set_has_client();
  if (client_ == &::google::protobuf::internal::kEmptyString) {
    client_ = new ::std::string;
  }
  client_->assign(value);
}
inline void tb_version::set_client(const char* value, size_t size) {
  set_has_client();
  if (client_ == &::google::protobuf::internal::kEmptyString) {
    client_ = new ::std::string;
  }
  client_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* tb_version::mutable_client() {
  set_has_client();
  if (client_ == &::google::protobuf::internal::kEmptyString) {
    client_ = new ::std::string;
  }
  return client_;
}
inline ::std::string* tb_version::release_client() {
  clear_has_client();
  if (client_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = client_;
    client_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void tb_version::set_allocated_client(::std::string* client) {
  if (client_ != &::google::protobuf::internal::kEmptyString) {
    delete client_;
  }
  if (client) {
    set_has_client();
    client_ = client;
  } else {
    clear_has_client();
    client_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string time = 5;
inline bool tb_version::has_time() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void tb_version::set_has_time() {
  _has_bits_[0] |= 0x00000010u;
}
inline void tb_version::clear_has_time() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void tb_version::clear_time() {
  if (time_ != &::google::protobuf::internal::kEmptyString) {
    time_->clear();
  }
  clear_has_time();
}
inline const ::std::string& tb_version::time() const {
  return *time_;
}
inline void tb_version::set_time(const ::std::string& value) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::kEmptyString) {
    time_ = new ::std::string;
  }
  time_->assign(value);
}
inline void tb_version::set_time(const char* value) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::kEmptyString) {
    time_ = new ::std::string;
  }
  time_->assign(value);
}
inline void tb_version::set_time(const char* value, size_t size) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::kEmptyString) {
    time_ = new ::std::string;
  }
  time_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* tb_version::mutable_time() {
  set_has_time();
  if (time_ == &::google::protobuf::internal::kEmptyString) {
    time_ = new ::std::string;
  }
  return time_;
}
inline ::std::string* tb_version::release_time() {
  clear_has_time();
  if (time_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = time_;
    time_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void tb_version::set_allocated_time(::std::string* time) {
  if (time_ != &::google::protobuf::internal::kEmptyString) {
    delete time_;
  }
  if (time) {
    set_has_time();
    time_ = time;
  } else {
    clear_has_time();
    time_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
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

#endif  // PROTOBUF_mysql_2etb_5fversion_2eproto__INCLUDED
