#include "treasure_parser.h"

treasure_parser::treasure_parser()
{

}

treasure_parser::~treasure_parser()
{
}

int treasure_parser::remove_all()
{
    tresure_db_info_map_.clear();
    return 0;
}

int treasure_parser::parse_treasure(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::treasure_db_info treasure_info;
        if (!treasure_info.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        change_treasure_info(treasure_info,treasure_info.shop_id());
    }

    return 0;
}

int treasure_parser::change_treasure_info(const client::treasure_db_info& bi,uint32 shop_id)
{
	std::multimap<uint32, client::treasure_db_info>::iterator it = tresure_db_info_map_.find(bi.item_id());
	for (;it != tresure_db_info_map_.end(); ++it){
		if (it->second.shop_id() == shop_id && it->first == bi.item_id()){
			it->second.CopyFrom(bi);
			return 0;
		}
	}

	tresure_db_info_map_.insert(std::pair<uint32, client::treasure_db_info>(bi.item_id(), bi));

    return 0;
}

int treasure_parser::serialize_treasure(string& out)
{
	char buf[MAX_TREASURE_BOLB_LEN] = {0};
    uint32 offset = 0;
    int count = 0;

    std::multimap<uint32, client::treasure_db_info>::iterator it =  tresure_db_info_map_.begin(); 
    for (; it != tresure_db_info_map_.end(); ++it) {
        client::treasure_db_info& bi = it->second;
        uint32 msg_len = bi.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_TREASURE_BOLB_LEN) {
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

bool treasure_parser::get_treasure_info(uint32 item_id,client::treasure_db_info& treasure_info,uint32 shop_id)
{
	std::multimap<uint32, client::treasure_db_info>::iterator it = tresure_db_info_map_.find(item_id);
	for (;it != tresure_db_info_map_.end(); ++it){
		if (it->second.shop_id() == shop_id && it->first == item_id){
			treasure_info.CopyFrom(it->second);
			return true;
		}
	}

	return false;
}

bool treasure_parser::create_treasure_info(uint32 item_id,client::treasure_db_info& treasure_info,uint32 shop_id)
{
	treasure_info.set_item_id(item_id);
	treasure_info.set_item_count(0);
	treasure_info.set_shop_id(shop_id);
	tresure_db_info_map_.insert(std::pair<uint32, client::treasure_db_info>(treasure_info.item_id(), treasure_info));

	return true;
}

void treasure_parser::get_shop_treasure_db_info(uint32 shop_id,std::map<uint32, client::treasure_db_info>& shop_limit_map)
{
	std::multimap<uint32, client::treasure_db_info>::iterator it = tresure_db_info_map_.begin();
	for (;it != tresure_db_info_map_.end(); ++it){
		if (it->second.shop_id() == shop_id){
			shop_limit_map.insert(std::pair<uint32,client::treasure_db_info>(it->first,it->second));
		}
	}
}