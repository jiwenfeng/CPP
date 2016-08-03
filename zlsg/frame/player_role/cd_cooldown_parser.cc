#include "cd_cooldown_parser.h"
#include "config_cd_time.h"
cd_cooldown_parser::cd_cooldown_parser()
{

}

cd_cooldown_parser::~cd_cooldown_parser()
{
}

int cd_cooldown_parser::remove_all()
{
    cd_info_map_.clear();
    return 0;
}

int cd_cooldown_parser::parse_cd(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::cd_info bi;
        if (!bi.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        add_cd(bi);
    }

    return 0;
}

int cd_cooldown_parser::add_cd(const client::cd_info& bi)
{
	std::map<uint32, client::cd_info>::iterator it = cd_info_map_.find(bi.type_id());
	if (it != cd_info_map_.end()) {
		it->second.CopyFrom(bi);
	} else {
		cd_info_map_.insert(std::pair<uint32, client::cd_info>(bi.type_id(), bi));
	}

    return 0;
}

int cd_cooldown_parser::remove_cd(uint32 type_id)
{
    std::map<uint32, client::cd_info>::iterator it = cd_info_map_.find(type_id);
    if (it == cd_info_map_.end()) {
        return 0;
    }

    cd_info_map_.erase(it);

    return 0;
}
 
int cd_cooldown_parser::serialize_cd(string& out)
{
    char buf[MAX_CD_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;

    std::map<uint32, client::cd_info>::iterator it; 
    for (it = cd_info_map_.begin(); it != cd_info_map_.end(); ++it) {
		// 这里需要添加检测是否需要存这CD
        client::cd_info& bi = it->second;
        uint32 msg_len = bi.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_CD_BOLB_LEN) {
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

int cd_cooldown_parser::get_cd_info(uint32 type_id,client::cd_info& cd_info)
{
	std::map<uint32, client::cd_info>::iterator it = cd_info_map_.find(type_id);
	if (it != cd_info_map_.end()){
		cd_info = it->second;
	}
	else{
		// 初始化相关
		cd_info_map_.insert(std::pair<uint32,client::cd_info>(type_id,cd_info));
	}

	return 0;
}

bool cd_cooldown_parser::add_cooldown_time(uint32 type_id)
{
	// 读配置表
	const config_cd_time* cd_base = CONFMGR->get_config_cd_time_mgr().get_config_cd_time(type_id);
	if (cd_base == NULL){
		return false;
	}
	uint32 now = (uint32)time(NULL);
	uint32 life_time = now + cd_base->get_life_time();
	std::map<uint32, client::cd_info>::iterator it = cd_info_map_.find(type_id);
	if (it != cd_info_map_.end()){
		it->second.set_life_time(life_time);
	}else {
		client::cd_info info;
		info.set_type_id(type_id);
		info.set_cooldown_type(cd_base->get_type());
		info.set_life_time(life_time);
		// 初始化相关
		cd_info_map_.insert(std::pair<uint32,client::cd_info>(type_id,info));
	}

	return true;
}

uint32 cd_cooldown_parser::get_cooldown_time(uint32 type_id) const
{
	std::map<uint32, client::cd_info>::const_iterator it = cd_info_map_.find(type_id);
	if (it != cd_info_map_.end()){
		return it->second.life_time();
	}

	return 0;
}

bool cd_cooldown_parser::is_can_operate_by_cd(uint32 type_id)
{
	uint32 now = (uint32)time(NULL);

	std::map<uint32, client::cd_info>::const_iterator it = cd_info_map_.find(type_id);
	if (it == cd_info_map_.end()){
		return false;
	}
	
	return it->second.life_time() <= now;
}