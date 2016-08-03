#ifndef __PB_CODER_H__
#define __PB_CODER_H__

#include "define.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <string> 
#include <stdint.h>
#ifndef _WIN32
#include <arpa/inet.h>
#endif // _WIN32
#include <assert.h>
#include <stdio.h>
#include <bitset>
//#include <zlib.h>  // adler32

namespace server_space {

const int PB_HEADER_LEN = sizeof(uint32);
const int PB_PACKAGE_MIN_LEN = PB_HEADER_LEN;

// |---->pb名称长度<----|---->pb名称<----|---->pb包体<----|
void* encode(const google::protobuf::Message& msg, size_t& len);
void* encode(const char* name, const char* bin, uint32& len);

google::protobuf::Message* create_msg(const std::string& type_name);

int get_val(google::protobuf::Message* msg, const char* name, int& ret);

uint32 decryption(uint32 val);

// |---->pb名称长度<----|---->pb名称<----|---->pb包体<----|
google::protobuf::Message* decode(const std::string& buf, std::string& type_name);
google::protobuf::Message* decode(char* buf, size_t sz, char* type_name, uint32 package_index);

bool decode_type_name(char* buf, size_t sz, char* type_name, uint32 package_index);
const char* decode_pb_bin(char* buf, size_t& sz, const char* type_name, uint32 package_index);
google::protobuf::Message* decode_pb(char* buf, size_t sz, char* type_name, uint32 package_index);

google::protobuf::Message* decode_inner(char* buf, size_t sz, char* type_name);

int asint32(const char* buf);

void* svr_encode(const google::protobuf::Message& msg, int& len);
google::protobuf::Message* svr_decode(const char* buf, int len);


int client_encode(const google::protobuf::Message& msg, char* buf, uint32 buf_len, uint32& len);
google::protobuf::Message* client_decode(char* buf, size_t& sz);

};

using namespace server_space;

#endif // __PB_CODER_H__
