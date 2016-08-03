#include "set_parser.h"

set_parser::set_parser()
{
}

set_parser::~set_parser()
{
}

void set_parser::clear()
{
    _role_set_info.clear_si();
}

int set_parser::parse(const string& in)
{
    clear();

    char* ptr     = (char*)in.c_str();
    /*uint32 len    = in.size();
      uint32 offset = 0;

    //printf("parse\n");
    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);
        client::shortcut_info* si = _role_set_info.add_si();
        if (!si->ParseFromArray(ptr + offset, msg_len)) {
            break;
        }

        //printf("id = %d, type = %d , val = %d\n", si->id(), si->type(), si->val());
        offset += msg_len;
    }

    //printf("\n");
    int32 i = _role_set_info.si_size();
    for (; i <= SHORTCUT_MAX_GRID; ++i) {
        client::shortcut_info* psi = _role_set_info.add_si();
        psi->set_id(i);
        psi->set_type(0);
        psi->set_val(0);
        }*/

    _role_set_info.ParseFromArray(ptr, in.size());
    
    return 0;
}

int set_parser::serialize(string& out)
{
    char buf[MAX_SET_BOLB_LEN];
    uint32 offset = 0;
      /*int32 size = _role_set_info.si_size();
    //printf("serialize\n");
    for (int32 i = 0; i < size; ++i) {
        const client::shortcut_info& si = _role_set_info.si(i);
            
        //printf("id = %d, type = %d , val = %d\n", si.id(), si.type(), si.val());
        uint32 msg_len = si.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_SET_BOLB_LEN) {
            break;
        }

        *(uint16*)(buf + offset) = (uint16)msg_len;
        offset += sizeof(uint16);

        si.SerializeToArray((void*)(buf + offset), msg_len);
        offset += msg_len;
        }*/

    uint32 msg_len = _role_set_info.ByteSize();
    _role_set_info.SerializeToArray((void*)(buf + offset), msg_len);
    offset += msg_len;

    //printf("\n");
    out.append(buf, offset);

    return 0;
}
