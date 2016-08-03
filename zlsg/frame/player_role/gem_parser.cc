#include "gem_parser.h"
#include "config_gem.h"
#include "props_parser.h"
gem_parser::gem_parser()
{

}

gem_parser::~gem_parser()
{
}

int gem_parser::remove_all()
{
    gem_map_.clear();
    return 0;
}

int gem_parser::parse_gem(const string& in)
{
    remove_all();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;

    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::gem_info bi;
        if (!bi.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        add_gem(bi);
    }

    return 0;
}

int gem_parser::add_gem(const client::gem_info& bi)
{
	std::map<uint32, client::gem_info>::iterator it = gem_map_.find(bi.hole_id());
	if (it != gem_map_.end()) {
		it->second.CopyFrom(bi);
	} else {
		gem_map_.insert(std::pair<uint32, client::gem_info>(bi.hole_id(), bi));
	}

    return 0;
}

int gem_parser::remove_gem(uint32 gem_pos)
{
    std::map<uint32, client::gem_info>::iterator it = gem_map_.find(gem_pos);
    if (it == gem_map_.end()) {
        return 0;
    }

    gem_map_.erase(it);

    return 0;
}
 
int gem_parser::serialize_gem(string& out)
{
    char buf[MAX_GEM_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;

    std::map<uint32, client::gem_info>::iterator it; 
    for (it = gem_map_.begin(); it != gem_map_.end(); ++it) {
        client::gem_info& bi = it->second;
        uint32 msg_len = bi.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_GEM_BOLB_LEN) {
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

uint32 gem_parser::get_gem_type(uint32 gem_pos)
{
	switch(gem_pos)
	{
	case EQUIP_GRID_BEG:// 武器
		return 1;
	case EQUIP_ARMOR:// 衣服
		return 2;
	case EQUIP_BELT: // 腰带
		return 2;
	case EQUIP_HELMET: // 头盔
		return 3;
	case EQUIP_NECKLACE: // 项链
		return 5;
	case EQUIP_SHOE: // 鞋子
		return 4;
	default:
		break;
	}
	
	return 0;
}

bool gem_parser::gem_level_is_max(uint32 gem_type,uint32 gem_level)
{
	const config_gem* gem_base = CONFMGR->get_config_gem(gem_type,gem_level+1);
	if (gem_base == NULL){
		return true;
	}

	return false;
}

void gem_parser::create_gem_info(uint32 gem_pos,uint32 open_hole)
{
	if (open_hole == 0){
		return;
	}
	// 组装宝石坑ID
	uint32 need_hole = gem_pos * 10 + open_hole;
	std::map<uint32, client::gem_info>::iterator it = gem_map_.find(need_hole);
	if (it != gem_map_.end()){
		return;
	}else{
		client::gem_info gem_info;
		gem_info.set_hole_id(need_hole);
		gem_info.set_gem_type(get_gem_type(gem_pos));
		gem_info.set_gem_level(0);
		gem_info.set_gem_exp(0);

		gem_map_.insert(std::pair<uint32,client::gem_info>(need_hole,gem_info));
	}
}

void gem_parser::get_pos_gem_list(uint32 gem_pos,std::map<uint32, client::gem_info>& pos_gem_list_)
{
	std::map<uint32, client::gem_info>::iterator it = gem_map_.begin();
	for (;it != gem_map_.end(); ++it){
		if ((it->second.hole_id() / 10) == gem_pos){
			pos_gem_list_.insert(std::pair<uint32,client::gem_info>(it->first,it->second));
		}
	}
}

bool gem_level_sort(const client::gem_info& a, const client::gem_info& b)
{
	return a.gem_level() < b.gem_level();
}

bool gem_parser::find_gem_upgrade_id(uint32 gem_pos,uint32& hole_id)
{
	bool is_has_exp = false;

	bool is_same_level = false;

	std::map<uint32, client::gem_info> pos_gem_list_;
	pos_gem_list_.clear();
	
	get_pos_gem_list(gem_pos,pos_gem_list_);

	if (pos_gem_list_.size() == 0){
		return false;
	}

	// 按顺序为优先级执行规则：
	// 规则 1：先找有经验的来升
	std::map<uint32, client::gem_info>::iterator it_pos = pos_gem_list_.begin();
	for (; it_pos != pos_gem_list_.end(); ++it_pos){
		if (it_pos->second.gem_exp() != 0){
			is_has_exp = true;
			hole_id = it_pos->second.hole_id();
		}
	}

	// 规则 2：找最低级的来升
	if (!is_has_exp){
		uint32 cmp_level = 0;
		int diff_count = -1;
		std::vector<client::gem_info> vec_gem;
		vec_gem.clear();
		
		std::map<uint32, client::gem_info>::iterator it_low = pos_gem_list_.begin();
		for (; it_low != pos_gem_list_.end(); ++it_low){
			vec_gem.push_back(it_low->second);
			if (cmp_level != it_low->second.gem_level()){
				cmp_level = it_low->second.gem_level();
				diff_count = diff_count + 1;
			}
		}
		
		if (diff_count == 0){
			// 执行下一个规则
			is_same_level = true;
		}else{
			if (vec_gem.size() != 0){
				std::sort(vec_gem.begin(),vec_gem.end(),gem_level_sort);
				
				hole_id = vec_gem[0].hole_id();
			}
		}
	}

	// 规则 3：找编号最低的来升级
	if (is_same_level){
		std::map<uint32, client::gem_info>::iterator it_index = pos_gem_list_.begin();
		hole_id = it_index->first;
	}

	return true;
}

const uint32 gem_parser::get_hole_gem_level(uint32 hole_id)
{
	std::map<uint32, client::gem_info>::iterator it = gem_map_.find(hole_id);
	if (it != gem_map_.end()){
		return it->second.gem_level();
	}

	return 0;
}

bool gem_parser::add_gem_exp(uint32 gem_pos,uint32& last_exp,bool& level_is_change)
{
	uint32 hole_id = 0;

	find_gem_upgrade_id(gem_pos,hole_id);
	
	add_hole_exp(hole_id,last_exp,level_is_change);


	return true;
}

bool gem_parser::add_hole_exp(uint32 hole_id,uint32& exp,bool& level_is_change)
{
	std::map<uint32, client::gem_info>::iterator it = gem_map_.find(hole_id);
	if (it != gem_map_.end()){
		
		const config_gem* gem_base_info = CONFMGR->get_config_gem(it->second.gem_type(),it->second.gem_level());
		uint32 total_exp = exp + it->second.gem_exp();
		if (gem_base_info->get_up_exp() > total_exp){
			it->second.set_gem_exp(total_exp);
			exp = 0;
		}else{
			if (gem_level_is_max(it->second.gem_type(),it->second.gem_level())){
				// 极限情况，升爆了！
				it->second.set_gem_exp(0);
				exp = 0;
			}else{
				it->second.set_gem_level(it->second.gem_level() + 1);
				it->second.set_gem_exp(0);
				exp = total_exp - gem_base_info->get_up_exp();
				level_is_change = true;
			}
		}
	}else{
		exp = 0;
		printf("NOT FOUND HOLE ID!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}

	return true;
}

uint32 gem_parser::get_gem_total_level()
{
	uint32 total_level = 0;

	std::map<uint32, client::gem_info>::iterator it = gem_map_.begin();
	for (; it != gem_map_.end(); ++it){
		total_level += it->second.gem_level();
	}

	return total_level;
}