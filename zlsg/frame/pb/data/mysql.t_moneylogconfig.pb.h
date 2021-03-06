// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mysql.t_moneylogconfig.proto

#ifndef PROTOBUF_mysql_2et_5fmoneylogconfig_2eproto__INCLUDED
#define PROTOBUF_mysql_2et_5fmoneylogconfig_2eproto__INCLUDED

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
void  protobuf_AddDesc_mysql_2et_5fmoneylogconfig_2eproto();
void protobuf_AssignDesc_mysql_2et_5fmoneylogconfig_2eproto();
void protobuf_ShutdownFile_mysql_2et_5fmoneylogconfig_2eproto();

class t_moneylogconfig;

// ===================================================================

class t_moneylogconfig : public ::google::protobuf::Message {
 public:
  t_moneylogconfig();
  virtual ~t_moneylogconfig();

  t_moneylogconfig(const t_moneylogconfig& from);

  inline t_moneylogconfig& operator=(const t_moneylogconfig& from) {
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
  static const t_moneylogconfig& default_instance();

  void Swap(t_moneylogconfig* other);

  // implements Message ----------------------------------------------

  t_moneylogconfig* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const t_moneylogconfig& from);
  void MergeFrom(const t_moneylogconfig& from);
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

  // required uint32 rootid = 2;
  inline bool has_rootid() const;
  inline void clear_rootid();
  static const int kRootidFieldNumber = 2;
  inline ::google::protobuf::uint32 rootid() const;
  inline void set_rootid(::google::protobuf::uint32 value);

  // required string rootname = 3;
  inline bool has_rootname() const;
  inline void clear_rootname();
  static const int kRootnameFieldNumber = 3;
  inline const ::std::string& rootname() const;
  inline void set_rootname(const ::std::string& value);
  inline void set_rootname(const char* value);
  inline void set_rootname(const char* value, size_t size);
  inline ::std::string* mutable_rootname();
  inline ::std::string* release_rootname();
  inline void set_allocated_rootname(::std::string* rootname);

  // required uint32 typeid = 4;
  inline bool has_typeid_() const;
  inline void clear_typeid_();
  static const int kTypeidFieldNumber = 4;
  inline ::google::protobuf::uint32 typeid_() const;
  inline void set_typeid_(::google::protobuf::uint32 value);

  // required string typename = 5;
  inline bool has_typename_() const;
  inline void clear_typename_();
  static const int kTypenameFieldNumber = 5;
  inline const ::std::string& typename_() const;
  inline void set_typename_(const ::std::string& value);
  inline void set_typename_(const char* value);
  inline void set_typename_(const char* value, size_t size);
  inline ::std::string* mutable_typename_();
  inline ::std::string* release_typename_();
  inline void set_allocated_typename_(::std::string* typename_);

  // @@protoc_insertion_point(class_scope:mysql.t_moneylogconfig)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_rootid();
  inline void clear_has_rootid();
  inline void set_has_rootname();
  inline void clear_has_rootname();
  inline void set_has_typeid_();
  inline void clear_has_typeid_();
  inline void set_has_typename_();
  inline void clear_has_typename_();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 rootid_;
  ::std::string* rootname_;
  ::std::string* typename__;
  ::google::protobuf::uint32 typeid__;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_mysql_2et_5fmoneylogconfig_2eproto();
  friend void protobuf_AssignDesc_mysql_2et_5fmoneylogconfig_2eproto();
  friend void protobuf_ShutdownFile_mysql_2et_5fmoneylogconfig_2eproto();

  void InitAsDefaultInstance();
  static t_moneylogconfig* default_instance_;
};
// ===================================================================


// ===================================================================

// t_moneylogconfig

// required uint32 id = 1;
inline bool t_moneylogconfig::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void t_moneylogconfig::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void t_moneylogconfig::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void t_moneylogconfig::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 t_moneylogconfig::id() const {
  return id_;
}
inline void t_moneylogconfig::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required uint32 rootid = 2;
inline bool t_moneylogconfig::has_rootid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void t_moneylogconfig::set_has_rootid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void t_moneylogconfig::clear_has_rootid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void t_moneylogconfig::clear_rootid() {
  rootid_ = 0u;
  clear_has_rootid();
}
inline ::google::protobuf::uint32 t_moneylogconfig::rootid() const {
  return rootid_;
}
inline void t_moneylogconfig::set_rootid(::google::protobuf::uint32 value) {
  set_has_rootid();
  rootid_ = value;
}

// required string rootname = 3;
inline bool t_moneylogconfig::has_rootname() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void t_moneylogconfig::set_has_rootname() {
  _has_bits_[0] |= 0x00000004u;
}
inline void t_moneylogconfig::clear_has_rootname() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void t_moneylogconfig::clear_rootname() {
  if (rootname_ != &::google::protobuf::internal::kEmptyString) {
    rootname_->clear();
  }
  clear_has_rootname();
}
inline const ::std::string& t_moneylogconfig::rootname() const {
  return *rootname_;
}
inline void t_moneylogconfig::set_rootname(const ::std::string& value) {
  set_has_rootname();
  if (rootname_ == &::google::protobuf::internal::kEmptyString) {
    rootname_ = new ::std::string;
  }
  rootname_->assign(value);
}
inline void t_moneylogconfig::set_rootname(const char* value) {
  set_has_rootname();
  if (rootname_ == &::google::protobuf::internal::kEmptyString) {
    rootname_ = new ::std::string;
  }
  rootname_->assign(value);
}
inline void t_moneylogconfig::set_rootname(const char* value, size_t size) {
  set_has_rootname();
  if (rootname_ == &::google::protobuf::internal::kEmptyString) {
    rootname_ = new ::std::string;
  }
  rootname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_moneylogconfig::mutable_rootname() {
  set_has_rootname();
  if (rootname_ == &::google::protobuf::internal::kEmptyString) {
    rootname_ = new ::std::string;
  }
  return rootname_;
}
inline ::std::string* t_moneylogconfig::release_rootname() {
  clear_has_rootname();
  if (rootname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = rootname_;
    rootname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_moneylogconfig::set_allocated_rootname(::std::string* rootname) {
  if (rootname_ != &::google::protobuf::internal::kEmptyString) {
    delete rootname_;
  }
  if (rootname) {
    set_has_rootname();
    rootname_ = rootname;
  } else {
    clear_has_rootname();
    rootname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 typeid = 4;
inline bool t_moneylogconfig::has_typeid_() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void t_moneylogconfig::set_has_typeid_() {
  _has_bits_[0] |= 0x00000008u;
}
inline void t_moneylogconfig::clear_has_typeid_() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void t_moneylogconfig::clear_typeid_() {
  typeid__ = 0u;
  clear_has_typeid_();
}
inline ::google::protobuf::uint32 t_moneylogconfig::typeid_() const {
  return typeid__;
}
inline void t_moneylogconfig::set_typeid_(::google::protobuf::uint32 value) {
  set_has_typeid_();
  typeid__ = value;
}

// required string typename = 5;
inline bool t_moneylogconfig::has_typename_() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void t_moneylogconfig::set_has_typename_() {
  _has_bits_[0] |= 0x00000010u;
}
inline void t_moneylogconfig::clear_has_typename_() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void t_moneylogconfig::clear_typename_() {
  if (typename__ != &::google::protobuf::internal::kEmptyString) {
    typename__->clear();
  }
  clear_has_typename_();
}
inline const ::std::string& t_moneylogconfig::typename_() const {
  return *typename__;
}
inline void t_moneylogconfig::set_typename_(const ::std::string& value) {
  set_has_typename_();
  if (typename__ == &::google::protobuf::internal::kEmptyString) {
    typename__ = new ::std::string;
  }
  typename__->assign(value);
}
inline void t_moneylogconfig::set_typename_(const char* value) {
  set_has_typename_();
  if (typename__ == &::google::protobuf::internal::kEmptyString) {
    typename__ = new ::std::string;
  }
  typename__->assign(value);
}
inline void t_moneylogconfig::set_typename_(const char* value, size_t size) {
  set_has_typename_();
  if (typename__ == &::google::protobuf::internal::kEmptyString) {
    typename__ = new ::std::string;
  }
  typename__->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* t_moneylogconfig::mutable_typename_() {
  set_has_typename_();
  if (typename__ == &::google::protobuf::internal::kEmptyString) {
    typename__ = new ::std::string;
  }
  return typename__;
}
inline ::std::string* t_moneylogconfig::release_typename_() {
  clear_has_typename_();
  if (typename__ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = typename__;
    typename__ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void t_moneylogconfig::set_allocated_typename_(::std::string* typename_) {
  if (typename__ != &::google::protobuf::internal::kEmptyString) {
    delete typename__;
  }
  if (typename_) {
    set_has_typename_();
    typename__ = typename_;
  } else {
    clear_has_typename_();
    typename__ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
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

#endif  // PROTOBUF_mysql_2et_5fmoneylogconfig_2eproto__INCLUDED
