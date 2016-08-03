#include "potential_parser.h"

potential_parser::potential_parser()
{

}

potential_parser::~potential_parser()
{
}

int potential_parser::remove_all()
{
    potential_map_.clear();
    return 0;
}

int potential_parser::parse_potential(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::acupoint_info bi;
        if (!bi.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        add_potential(bi);
    }

    return 0;
}

int potential_parser::add_potential(const client::acupoint_info& bi)
{
	std::map<uint32, client::acupoint_info>::iterator it = potential_map_.find(bi.index());
	if (it != potential_map_.end()) {
		it->second.CopyFrom(bi);
	} else {
		potential_map_.insert(std::pair<uint32, client::acupoint_info>(bi.index(), bi));
	}

    return 0;
}

int potential_parser::remove_potential(uint32 index)
{
    std::map<uint32, client::acupoint_info>::iterator it = potential_map_.find(index);
    if (it == potential_map_.end()) {
        return 0;
    }

    potential_map_.erase(it);

    return 0;
}
 
int potential_parser::serialize_potential(string& out)
{
    char buf[MAX_POTENTIAL_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;

    std::map<uint32, client::acupoint_info>::iterator it; 
    for (it = potential_map_.begin(); it != potential_map_.end(); ++it) {
        client::acupoint_info& bi = it->second;
        uint32 msg_len = bi.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_POTENTIAL_BOLB_LEN) {
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

int potential_parser::get_acupoint_info(uint32 index,client::acupoint_info& acupoint_info)
{
	std::map<uint32, client::acupoint_info>::iterator it = potential_map_.find(index);
	if (it != potential_map_.end()){
		acupoint_info = it->second;
	}
	else{
		acupoint_info.set_index(index);
		acupoint_info.set_status(0);
		acupoint_info.set_act_count(0);
		potential_map_.insert(std::pair<uint32,client::acupoint_info>(index,acupoint_info));
	}

	return 0;
}