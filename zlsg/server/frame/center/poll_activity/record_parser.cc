#include "record_parser.h"

record_parser::record_parser()
{

}

record_parser::~record_parser()
{
}

int record_parser::remove_all()
{
	_record_info.Clear();

    return 0;
}

int record_parser::parse_record(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::act_db_info bi;
        if (!bi.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        add_record(bi);
    }

    return 0;
}

int record_parser::add_record(const client::act_db_info& bi)
{
	_record_info.CopyFrom(bi);

    return 0;
}

int record_parser::serialize_record(string& out)
{
    char buf[MAX_RECORD_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;


    client::act_db_info& bi = _record_info;
    uint32 msg_len = bi.ByteSize();
    if (offset + sizeof(uint16) + msg_len > MAX_RECORD_BOLB_LEN) {
        return -1;
    }

    *(uint16*)(buf + offset) = (uint16)msg_len;
    offset += sizeof(uint16);

    bi.SerializeToArray((void*)(buf + offset), msg_len);
    offset += msg_len;
    ++count;

    out.append(buf, offset);

    return count;
}