#include "pb_coder.h"
#include <stdio.h>

namespace server_space {

    bool decode_type_name(char* buf, size_t sz, char* type_name, uint32 package_index)
    {
        if (sz < (size_t)PB_PACKAGE_MIN_LEN) {
            return false;
        }

        char* beg = buf;
        uint32 namelen = ntohl(*((uint32*)buf));
        buf += sizeof(uint32);
        size_t need_min_len = sz - sizeof(uint32);
        if (package_index > 0) {
            need_min_len -= sizeof(uint32);
        }

        if (namelen <= 0 || namelen >= MAX_PB_NAME_LEN || namelen >= need_min_len) {
            return false;
        }

        if ((sz - (buf - beg)) <= sizeof(uint32)) {
            return false;
        }

        if (package_index > 0) {
            uint32 post_index = ntohl(*((uint32*)buf));
            buf += sizeof(uint32);
            post_index = decryption(post_index);
            if (post_index != package_index) {
                return false;
            }
        }

        memcpy(type_name, buf, namelen - 1);
        type_name[namelen - 1] = '\0';
        buf += namelen;
        if ((int)sz <= (buf - beg)) {
            return false;
        }

        return true;
    }

    const char* decode_pb_bin(char* buf, size_t& sz, const char* type_name, uint32 package_index)
    {
        uint32 fix = sizeof(uint32) + strlen(type_name) + 1;
        sz -= fix;
        return buf + fix;
    }

    google::protobuf::Message* decode_pb(char* buf, size_t sz, char* type_name, uint32 package_index)
    {
        google::protobuf::Message* msg = create_msg(type_name);
        if (msg == NULL) {
            return NULL;
        }

        uint32 fix = sizeof(uint32) + strlen(type_name) + 1;
        buf += fix;
        if (!msg->ParseFromArray((const void*)buf, sz - fix)) {
            delete msg;
            return NULL;
        }

        return msg;
    }

    void* encode(const google::protobuf::Message& msg, size_t& len)
    {
        size_t name_len = msg.GetTypeName().size() + 1;
        len = 8 + sizeof(int) + name_len + msg.ByteSize(); // 头8个字节用于client模块往里面填充数据
        uint8* buf = (uint8*)malloc(len);
        if (buf == NULL) {
            return NULL;
        }

        uint8* pos = buf + 8;
        *((int*)pos) = htonl(name_len);
        pos += sizeof(int);

        sprintf((char*)pos, "%s", msg.GetTypeName().c_str());
        pos += name_len;

        msg.SerializeToArray((void*)pos, msg.ByteSize());

        return (void*)buf;
    }

    void* encode(const char* name, const char* bin, uint32& len)
    {
        size_t name_len = strlen(name) + 1;
        uint32 all_len = 8 + sizeof(int) + name_len + len; // 头8个字节用于client模块往里面填充数据
        uint8* buf = (uint8*)malloc(all_len);
        if (buf == NULL) {
            return NULL;
        }

        uint8* pos = buf + 8;
        *((int*)pos) = htonl(name_len);
        pos += sizeof(int);

        sprintf((char*)pos, "%s", name);
        pos += name_len;

        memcpy(pos, bin, len);
        len = all_len;

        return (void*)buf;
    }

    google::protobuf::Message* create_msg(const std::string& type_name)
    {
        google::protobuf::Message* msg = NULL;
        const google::protobuf::Descriptor* dsc = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
        if (dsc) {
            const google::protobuf::Message* type = google::protobuf::MessageFactory::generated_factory()->GetPrototype(dsc);
            if (type) {
                msg = type->New();
            }
        }

        return msg;
    }

    int get_val(google::protobuf::Message* msg, const char* name, int& ret)
    {
        const google::protobuf::Reflection* ref = msg->GetReflection();
        const google::protobuf::Descriptor* desc = msg->GetDescriptor();
        assert(ref != NULL && desc != NULL);
        int count = desc->field_count();
        for (int i = 0; i < count; ++i) {
            const google::protobuf::FieldDescriptor* field = desc->field(i);
            if (field->name().compare(name) == 0) {
                ret = ref->GetInt32(*msg, field);
                return 0;
            }
        }

        return -2;
    }

    uint32 decryption(uint32 val)
    {
        bitset<32l> bval((int)val);
        bval[1] = !bval[1];
        bval[7] = !bval[7];
        bval[15] = !bval[15];
        bval[23] = !bval[23];
        bval[30] = !bval[30];
        return (uint32)(bval.to_ulong());
    }

    google::protobuf::Message* decode(char* buf, size_t sz, char* type_name, uint32 package_index)
    {
        if (sz < (size_t)PB_PACKAGE_MIN_LEN) {
            return NULL;
        }

        char* beg = buf;
        uint32 namelen = ntohl(*((uint32*)buf));
        buf += sizeof(uint32);
        size_t need_min_len = sz - sizeof(uint32);
        if (package_index > 0) {
            need_min_len -= sizeof(uint32);
        }

        if (namelen <= 0 || namelen >= MAX_PB_NAME_LEN || namelen >= need_min_len) {
            return NULL;
        }

        if ((sz - (buf - beg)) <= sizeof(uint32)) {
            return NULL;
        }

        if (package_index > 0) {
            uint32 post_index = ntohl(*((uint32*)buf));
            buf += sizeof(uint32);
            post_index = decryption(post_index);
            if (post_index != package_index) {
                return NULL;
            }
        }

        memcpy(type_name, buf, namelen - 1);
        type_name[namelen - 1] = '\0';
        buf += namelen;
        if ((int)sz <= (buf - beg)) {
            return NULL;
        }

        google::protobuf::Message* msg = create_msg(type_name);
        if (msg == NULL) {
            return NULL;
        }

        if (!msg->ParseFromArray((const void*)buf, sz - (buf - beg))) {
            delete msg;
            return NULL;
        }

        return msg;
    }

    google::protobuf::Message* decode(const std::string& buf, std::string& type_name)
    {
        int len = static_cast<int>(buf.size());
        if (len < PB_PACKAGE_MIN_LEN) {
            return NULL;
        }

        int namelen = asint32(buf.c_str());
        if (namelen <= 0 || namelen >= MAX_PB_NAME_LEN || namelen >= (int)(len - sizeof(uint32))) {
            return NULL;
        }

        type_name.append(buf, PB_HEADER_LEN, namelen - 1);
        google::protobuf::Message* msg = create_msg(type_name);
        if (msg == NULL) {
            return NULL;
        }

        const char* data = buf.c_str() + PB_HEADER_LEN + namelen;
        int data_len = len - namelen - PB_HEADER_LEN;
        if (!msg->ParseFromArray(data, data_len)) {
            delete msg;
            return NULL;
        }

        return msg;
    }

    google::protobuf::Message* decode_inner(char* buf, size_t sz, char* type_name)
    {
        if (sz < (size_t)PB_PACKAGE_MIN_LEN) {
            return NULL;
        }

        char* beg = buf;
        uint32 namelen = ntohl(*((uint32*)buf));
        buf += sizeof(uint32);
        if (namelen <= 0 || namelen >= MAX_PB_NAME_LEN || namelen >= sz - sizeof(uint32)) {
            return NULL;
        }

        memcpy(type_name, buf, namelen - 1);
        type_name[namelen - 1] = '\0';
        buf += namelen;
        if ((int)sz <= (buf - beg)) {
            return NULL;
        }

        google::protobuf::Message* msg = create_msg(type_name);
        if (msg == NULL) {
            return NULL;
        }

        if (!msg->ParseFromArray((const void*)buf, sz - (buf - beg))) {
            delete msg;
            return NULL;
        }

        return msg;
    }

    int asint32(const char* buf)
    {
        int len = 0;
        memcpy(&len, buf, sizeof(len));
        return ntohl(len);
    }

    void* svr_encode(const google::protobuf::Message& msg, int& len)
    {
        int name_len = msg.GetTypeName().size() + 1;
        len = msg.ByteSize() + name_len;
        uint8* buf = (uint8*)malloc(len);
        sprintf((char*)buf, "%s", msg.GetTypeName().c_str());
        msg.SerializeToArray((void*)(buf + name_len), msg.ByteSize());
        return (void*)buf;
    }

    google::protobuf::Message* svr_decode(const char* buf, int len)
    {
        int name_len = strlen(buf) + 1;
        google::protobuf::Message* pb = create_msg(buf);
        if (pb == NULL){
            return NULL;
        }

        buf += name_len;
        if (!pb->ParseFromArray((const void*)buf, len - name_len)) {
            delete pb;
            return NULL;
        }

        return pb;
    }

    int client_encode(const google::protobuf::Message& msg, char* buf, uint32 buf_len, uint32& len)
    {
        string typeName = msg.GetTypeName();
        int name_len = typeName.size() + 1;
        len = 4 + sizeof(int) + name_len + msg.ByteSize(); // 头8个字节用于client模块往里面填充数据
        if (buf_len < len){
            assert(0);
            return -1;
        }

        char* pos = buf;
        *((int*)pos) = htonl(len-4);
        pos += sizeof(int);

        *((int*)pos) = htonl(name_len);
        pos += sizeof(int);

        sprintf((char*)pos, "%s", msg.GetTypeName().c_str());
        pos += name_len;

        msg.SerializeToArray((void*)pos, msg.ByteSize());
        return 0;
    }

    google::protobuf::Message* client_decode(char* buf, size_t& sz)
    {
        if (sz < sizeof(int)) {
            return NULL;
        }

        uint32 len = ntohl(*((int*)buf));
        if (len > sz) {
            return NULL;
        }

        char type_name[MAX_PB_NAME_LEN] = {0};
        google::protobuf::Message* msg = decode(buf + sizeof(int), sz - sizeof(int), type_name, 0);
        if (msg != NULL) {
            memcpy(buf, buf + len, sz - len);
            sz -= len;
            return msg;
        }

        return NULL;
    }

};