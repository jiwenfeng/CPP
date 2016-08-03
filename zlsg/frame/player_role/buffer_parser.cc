#include "buffer_parser.h"

buffer_parser::buffer_parser()
{

}

buffer_parser::~buffer_parser()
{
}

int buffer_parser::remove_all()
{
    buffer_map.clear();
    return 0;
}

int buffer_parser::parse_buffer(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::buffer_info bi;
        if (!bi.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        add_buffer(bi);
    }

    return 0;
}

int buffer_parser::add_buffer(const client::buffer_info& bi)
{
    multimap<uint32, client::buffer_info>::iterator it = buffer_map.find(bi.effect_id());
    if (it != buffer_map.end()) {
        it->second.CopyFrom(bi);
    } else {
        buffer_map.insert(std::pair<uint32, client::buffer_info>(bi.effect_id(), bi));
    }

    return 0;
}

int buffer_parser::remove_buffer(uint32 effect_id)
{
    multimap<uint32, client::buffer_info>::iterator it = buffer_map.find(effect_id);
    if (it == buffer_map.end()) {
        return 0;
    }

    buffer_map.erase(it);

    return 0;
}
 
int buffer_parser::serialize_buffer(string& out)
{
    char buf[MAX_BUFFER_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;

    multimap<uint32, client::buffer_info>::iterator it; 
    for (it = buffer_map.begin(); it != buffer_map.end(); ++it) {
        client::buffer_info& bi = it->second;
        uint32 msg_len = bi.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_BUFFER_BOLB_LEN) {
            break;
        }

        *(uint16*)(buf + offset) = (uint16)msg_len;
        offset += sizeof(uint16);

        bi.SerializeToArray((void*)(buf + offset), msg_len);
        offset += msg_len;
        ++count;
    }

    out.append(buf, offset);

    return count;
}

