// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client.role_remove_msg.proto

#ifndef PROTOBUF_client_2erole_5fremove_5fmsg_2eproto__INCLUDED
#define PROTOBUF_client_2erole_5fremove_5fmsg_2eproto__INCLUDED

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
void  protobuf_AddDesc_client_2erole_5fremove_5fmsg_2eproto();
void protobuf_AssignDesc_client_2erole_5fremove_5fmsg_2eproto();
void protobuf_ShutdownFile_client_2erole_5fremove_5fmsg_2eproto();

class role_remove_msg;

// ===================================================================

class role_remove_msg : public ::google::protobuf::Message {
 public:
  role_remove_msg();
  virtual ~role_remove_msg();

  role_remove_msg(const role_remove_msg& from);

  inline role_remove_msg& operator=(const role_remove_msg& from) {
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
  static const role_remove_msg& default_instance();

  void Swap(role_remove_msg* other);

  // implements Message ----------------------------------------------

  role_remove_msg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const role_remove_msg& from);
  void MergeFrom(const role_remove_msg& from);
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

  // repeated .client.role_data rd = 1;
  inline int rd_size() const;
  inline void clear_rd();
  static const int kRdFieldNumber = 1;
  inline const ::client::role_data& rd(int index) const;
  inline ::client::role_data* mutable_rd(int index);
  inline ::client::role_data* add_rd();
  inline const ::google::protobuf::RepeatedPtrField< ::client::role_data >&
      rd() const;
  inline ::google::protobuf::RepeatedPtrField< ::client::role_data >*
      mutable_rd();

  // @@protoc_insertion_point(class_scope:client.role_remove_msg)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::client::role_data > rd_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_client_2erole_5fremove_5fmsg_2eproto();
  friend void protobuf_AssignDesc_client_2erole_5fremove_5fmsg_2eproto();
  friend void protobuf_ShutdownFile_client_2erole_5fremove_5fmsg_2eproto();

  void InitAsDefaultInstance();
  static role_remove_msg* default_instance_;
};
// ===================================================================


// ===================================================================

// role_remove_msg

// repeated .client.role_data rd = 1;
inline int role_remove_msg::rd_size() const {
  return rd_.size();
}
inline void role_remove_msg::clear_rd() {
  rd_.Clear();
}
inline const ::client::role_data& role_remove_msg::rd(int index) const {
  return rd_.Get(index);
}
inline ::client::role_data* role_remove_msg::mutable_rd(int index) {
  return rd_.Mutable(index);
}
inline ::client::role_data* role_remove_msg::add_rd() {
  return rd_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::client::role_data >&
role_remove_msg::rd() const {
  return rd_;
}
inline ::google::protobuf::RepeatedPtrField< ::client::role_data >*
role_remove_msg::mutable_rd() {
  return &rd_;
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

#endif  // PROTOBUF_client_2erole_5fremove_5fmsg_2eproto__INCLUDED
