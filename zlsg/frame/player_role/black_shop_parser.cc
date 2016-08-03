#include "black_shop_parser.h"

black_shop_parser::black_shop_parser()
{

}

black_shop_parser::~black_shop_parser()
{
}

int black_shop_parser::remove_all()
{
    black_shop_map_.clear();
    return 0;
}

int black_shop_parser::parse_black_shop(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::black_shop_db_info bi;
        if (!bi.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        add_blacl_shop_data(bi);
    }

    return 0;
}

int black_shop_parser::add_blacl_shop_data(const client::black_shop_db_info& bi)
{
	std::map<uint32, client::black_shop_db_info>::iterator it = black_shop_map_.find(bi.item_slot());
	if (it != black_shop_map_.end()) {
		it->second.CopyFrom(bi);
	} else {
		black_shop_map_.insert(std::pair<uint32, client::black_shop_db_info>(bi.item_slot(), bi));
	}

    return 0;
}

int black_shop_parser::remove_black_shop_data(uint32 slot_id)
{
    std::map<uint32, client::black_shop_db_info>::iterator it = black_shop_map_.find(slot_id);
    if (it == black_shop_map_.end()) {
        return 0;
    }

    black_shop_map_.erase(it);

    return 0;
}
 
int black_shop_parser::serialize_black_shop(string& out)
{
    char buf[MAX_POTENTIAL_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;

    std::map<uint32, client::black_shop_db_info>::iterator it; 
    for (it = black_shop_map_.begin(); it != black_shop_map_.end(); ++it) {
        client::black_shop_db_info& bi = it->second;
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

int black_shop_parser::get_black_shop_data_info(uint32 item_slot,client::black_shop_db_info& slot_info)
{
	std::map<uint32, client::black_shop_db_info>::iterator it = black_shop_map_.find(item_slot);
	//if (it != potential_map_.end()){
	//	acupoint_info = it->second;
	//}
	//else{
	//	black_shop_db_info.set_index(index);
	//	acupoint_info.set_status(0);
	//	acupoint_info.set_act_count(0);
	//	potential_map_.insert(std::pair<uint32,client::acupoint_info>(index,acupoint_info));
	//}

	return 0;
}