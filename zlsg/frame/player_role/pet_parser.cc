#include "config_beauty.h"
#include "pet_parser.h"

pet_parser::pet_parser()
{

}

pet_parser::~pet_parser()
{
}

int pet_parser::remove_all()
{
    pet_db_info_map_.clear();
    return 0;
}

int pet_parser::parse_pet(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::pet_db_info pet_info;
        if (!pet_info.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        change_pet_info(pet_info);
    }

    return 0;
}

int pet_parser::change_pet_status(uint32 pet_id,uint32 status)
{
    std::map<uint32, client::pet_db_info>::iterator itr = pet_db_info_map_.find(pet_id);

    if (itr == pet_db_info_map_.end()){
        return -1;
    }

    itr->second.set_status(status);

    return 0;
}

bool pet_parser::change_pet_cur_hp(uint32 pet_id,uint32 cur_hp)
{
    std::map<uint32, client::pet_db_info>::iterator itr = pet_db_info_map_.find(pet_id);

    if (itr == pet_db_info_map_.end()){
        return false;
    }

    itr->second.set_cur_hp(cur_hp);

    return true;
}

int pet_parser::get_pet_status(uint32 pet_id,uint32& pet_status)
{
    std::map<uint32, client::pet_db_info>::iterator itr = pet_db_info_map_.find(pet_id);

    if (itr == pet_db_info_map_.end()){
        return -1;
    }

    pet_status = itr->second.status();

    return 0;
}

int pet_parser::change_pet_info(const client::pet_db_info& bi)
{
    std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.find(bi.pet_id());
    if (it != pet_db_info_map_.end()) {
        it->second.CopyFrom(bi);
    } else {
        pet_db_info_map_.insert(std::pair<uint32, client::pet_db_info>(bi.pet_id(), bi));
    }

    return 0;
}

bool pet_parser::is_has_pet_fighting()
{
    std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.begin();
    for (;it != pet_db_info_map_.end(); ++it) {
        if (it->second.status() == PET_STATUS_FIGHTING) {
            return true;
        }
    }

    return false;
}

bool pet_parser::is_has_pet_prepare()
{
    std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.begin();
    for (;it != pet_db_info_map_.end(); ++it) {
        if (it->second.status() == PET_STATUS_PREPARE) {
            return true;
        }
    }

    return false;
}

uint32 pet_parser::get_fighting_pet_id()
{
	std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.begin();
	for (;it != pet_db_info_map_.end(); ++it) {
		if (it->second.status() == PET_STATUS_PREPARE) {
			return it->first;
		}
	}

	return 0;
}

int pet_parser::remove_pet(uint32 pet_id)
{
    std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.find(pet_id);
    if (it == pet_db_info_map_.end()) {
        return 0;
    }

    pet_db_info_map_.erase(it);

    return 0;
}

int pet_parser::serialize_pet(string& out)
{
    char buf[MAX_PET_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;

    std::map<uint32, client::pet_db_info>::iterator it; 
    for (it = pet_db_info_map_.begin(); it != pet_db_info_map_.end(); ++it) {
        client::pet_db_info& bi = it->second;
        uint32 msg_len = bi.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_PET_BOLB_LEN) {
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

bool pet_parser::get_pet_info(uint32 pet_id,client::pet_db_info& pet_info)
{
    std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.find(pet_id);
    if (it != pet_db_info_map_.end()) {
        pet_info = it->second;
        return true;
    }

    return false;
}

bool pet_parser::get_query_fighting_pet(client::pet_db_info& pet_info)
{
	if (pet_db_info_map_.size() == 0){
		return false;
	}

	std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.begin();
	for (;it != pet_db_info_map_.end(); ++it){
		if (it->second.status() == PET_STATUS_FIGHTING){
			pet_info.CopyFrom(it->second);
			return true;
			break;
		}
	}

	return false;
}

client::pet_db_info* pet_parser::mutable_pet_info(uint32 pet_id)
{
    std::map<uint32, client::pet_db_info>::iterator it = pet_db_info_map_.find(pet_id);
    if (it != pet_db_info_map_.end()) {
        return &it->second;
    }

    return NULL;
}

bool pet_parser::create_pet(uint32 pet_id,client::pet_db_info& pet_info,uint32 master_hp)
{
    const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(pet_id,1);
    if (base_info == NULL){
        return false;
    }
    uint32 pet_hp_max = static_cast<uint32>(master_hp * (base_info->get_fight_hppercent()/10000.0f));
    pet_info.set_pet_id(pet_id);
    pet_info.set_pet_level(base_info->get_general_level());
    pet_info.set_pet_exp(0);
    pet_info.set_cur_hp(pet_hp_max);
    pet_info.set_status(PET_STATUS_TRUCE);
    pet_info.set_lingering_count(0);
    pet_info.set_magic_star(0);
    pet_info.set_debris_exp(0);
	pet_info.set_levels_status(0);
    pet_db_info_map_.insert(std::pair<uint32,client::pet_db_info>(pet_id,pet_info));

    return true;
};