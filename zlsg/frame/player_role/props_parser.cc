#include "random.h"
#include "config_props.h"
#include "config_npc.h"
#include "config_upequip.h"
#include "props_parser.h"
#include "config_equip_upgrade.h"
#include "comm_define.h"
props_parser::props_parser(uint32 index)
        : beg_index(index)
        , sort_time(0)
{
	props_smyid& ppsid = prop_smy_id[index];
	ppsid.set(index,0);
}

props_parser::~props_parser()
{
    clear();
}

int props_parser::resize(uint32 max_size)
{
    if (props_list.size() >= max_size) {
        return -1;
    }

    uint32 i = props_list.size();
    props_list.resize(max_size);
    for (; i < max_size; ++i) {
        props_list[i].set_grid_index(beg_index + i);
        props_list[i].set_props_id(0);
    }

    return 0;
}

int props_parser::parse_props(const string& in)
{
    clear();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;
    uint32 max_size = props_list.size();
    
    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::props_info rg;
        if (!rg.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        
        offset += msg_len;

        uint32 grid_index = rg.grid_index();
        int index = grid_index - beg_index;
        if (grid_index < beg_index || index >= (int)max_size) {
            continue;
        }

        props_list[index] = rg;
		//printf("props_parser::parse_props ::: item_id = %d,smy_id = %d \n",rg.props_id(),rg.system_id());
		set_props_smy_id(rg);
    }

    return 0;
}

int props_parser::serialize_props(string& out)
{
    uint32 size = props_list.size();
    if (size == 0) {
        return 0;
    }

    int count = 0; 
    char buf[MAX_PROPS_BOLB_LEN];
    uint32 offset = 0;
    for (uint32 i = 0; i < size; ++i) {
        grid_role_props& grg = props_list[i];
        if (!grg.has_props()) {
            continue;
        }

        uint32 msg_len = grg.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_PROPS_BOLB_LEN) {
            break;
        }

        *(uint16*)(buf + offset) = (uint16)msg_len;
        offset += sizeof(uint16);
        
        grg.SerializeToArray((void*)(buf + offset), msg_len);
        offset += msg_len;
        ++count;
    }
    
    out.append(buf, offset);

    return count;
}

void props_parser::clear()
{
    uint32 size = props_list.size();
    for (uint32 i = 0; i < size; ++i) {
        props_list[i].reset();
    }

    change_flag.reset();
}

#ifndef __LIB_MINI_SERVER__
void props_parser::fill_role_props(inner::backpackGoods* data) const
{
    uint32 size = props_list.size();
    for (uint32 i = 0; i < size; ++i) {
        if (props_list[i].has_props()) {
            const config_props* cg = CONFMGR->get_config_props_mgr().get_config_props(props_list[i].props_id());
            if (cg == NULL) {
                continue;
            }
            
            inner::inner_goods* props = data->add_goods();
            props->set_index(props_list[i].grid_index());
            props->set_goods_id(props_list[i].props_id());
            props->set_goods_name(cg->get_item_name());
            props->set_goods_num(props_list[i].count());
            props->set_goods_type(cg->get_item_type());
        }
    }
}
#else
void props_parser::set_props(std::vector<client::props_info>& pis)
{
    props_list.resize(MAX_EQUIP_SIZE);
    for (size_t i = 0; i < pis.size(); ++i) {
        uint32 index = pis[i].grid_index() - beg_index;
        if (index >= props_list.size()) {
            continue;
        }

        props_list[index].CopyFrom(pis[i]);
    }
}
#endif //__LIB_MINI_SERVER__

void props_parser::fill_all_props_info(client::role_props_msg& rpm)
{
    uint32 size = props_list.size();
    for (uint32 i = 0; i < size; ++i) {
        if (props_list[i].has_props()) {
            fill_props_info(props_list[i], *rpm.add_pi());
        }
    }

    change_flag.reset();
}

void props_parser::fill_change_props_info(client::role_props_msg& rpm)
{
    if (change_flag.count() <= 0) {
        return;
    }

    uint32 size = props_list.size();
    for (uint32 i = 0; i < size; ++i) {
        if (change_flag[i]) {
            if (props_list[i].has_props()) {
                fill_props_info(props_list[i], *rpm.add_pi());
            } else {
                fill_null_props_info(*rpm.add_pi(), beg_index + i);
            }
        }
    }

    change_flag.reset();
}

void props_parser::fill_single_props_info(client::role_props_msg& rpm, uint32 grid_index)
{
    uint32 size = props_list.size();
    int index = grid_index - beg_index;
    if (index < 0 || index >= (int)size) {
        return;
    }

    if (props_list[index].has_props()) {
        fill_props_info(props_list[index], *rpm.add_pi());
    }

    change_flag.set(index, 0);
}

void props_parser::fill_interval_props_info(client::role_query_info& query_info,uint32 grindex)
{
	uint32 size = props_list.size();
	if (grindex > size){
		return;
	}

	uint32 index = grindex -1;

	if (props_list[index].has_props()) {
		client::props_info* role_equip = query_info.add_equip_info();
		fill_props_info(props_list[index],*role_equip);
	}

	change_flag.reset();
}

void props_parser::fill_pet_props_info(client::pet_query_info& query_info,uint32 grindex)
{
	uint32 size = props_list.size();
	if (grindex > size){
		return;
	}

	uint32 index = grindex -1;

	if (props_list[index].has_props()) {
		client::props_info* role_equip = query_info.add_equip_info();
		fill_props_info(props_list[index],*role_equip);
	}

	change_flag.reset();
}

int props_parser::pop_props(uint32 grid_index, client::props_info& rg, uint32 num /* = 0 */)
{
    uint32 size = props_list.size();
    int index = grid_index - beg_index;
    if (index < 0 || index >= (int)size){
        return -1;
    }

    int ret = props_list[index].pop_props(rg, num);
    if (ret != 0){
        return -2;
    }

    change_flag.set(index);

    return 0;
}

int props_parser::remove_props(uint32 props_id, uint32 num)
{
    if (get_props_num(props_id) < num) {
        return -1;
    }

	//uint32 size = props_list.size();
	//for (uint32 i = 0; i < size; ++i) {
	//	if (!props_list[i].has_props() || props_list[i].props_id() != props_id) {
	//		continue;
	//	}

	//	uint32 rest_num = 0;
	//	if (num > props_list[i].count()) {
	//		rest_num = num - props_list[i].count();
	//		remove_props(beg_index+i, props_list[i].count(), props_id, NULL);
	//	} else {
	//		remove_props(beg_index+i, num, props_id, NULL);
	//	}

	//	if (rest_num == 0) {
	//		break;
	//	}

	//	num = rest_num;
	//}

	int size = props_list.size()-1;
	while (size >= 0){
		if (!props_list[size].has_props() || props_list[size].props_id() != props_id) {
			size--;
			continue;
		}
		// 过期的忽略
		if (props_list[size].has_is_due()){
			if (props_list[size].is_due()){
				size--;
				continue;
			}
		}

		uint32 rest_num = 0;
		if (num > props_list[size].count()) {
			rest_num = num - props_list[size].count();
			remove_props(beg_index+size, props_list[size].count(), props_id, NULL);
		} else {
			remove_props(beg_index+size, num, props_id, NULL);
		}

		if (rest_num == 0) {
			break;
		}

		num = rest_num;

		size--;
	}

    return 0;
}

int props_parser::remove_props(uint32 props_id, uint32 num, bool& bind)
{
    if (get_props_num(props_id) < num) {
        return -1;
    }
    
    bind = false;
    uint32 binding = 0;
    uint32 size = props_list.size();
    for (uint32 i = 0; i < size; ++i) {
        if (!props_list[i].has_props() || props_list[i].props_id() != props_id) {
            continue;
        }
        
        uint32 rest_num = 0;
        if (num > props_list[i].count() ) {
            rest_num = num - props_list[i].count();
            remove_props(beg_index + i, props_list[i].count(), props_id, &binding);
        } else {
            remove_props(beg_index + i, num, props_id, &binding);
        }

        if (!bind) {
            bind = binding == 1;
        }
        
        if (rest_num == 0) {
            break;
        }

        num = rest_num;
    }

    return 0;
}

int props_parser::remove_props(uint32 grid_index, uint32 num, uint32& props_id, uint32* binding)
{
    uint32 size = props_list.size();
    int index = grid_index - beg_index;
    if (num == 0 || index < 0 || index >= (int)size) {
        return -1;
    }

    int ret = props_list[index].remove_props(num, props_id, binding);
    if (ret != 0) {
        return -2;
    }

    change_flag.set(index);

    return 0;
}

const grid_role_props* props_parser::get_props(uint32 grid_index) const
{
    uint32 size = props_list.size();
    int index = grid_index - beg_index;
    if (index < 0 || index >= (int)size) {
        return NULL;
    }

    return &props_list[index];
}

const uint32 props_parser::get_index_props_num(uint32 grindex_id) const
{
	uint32 number = 0;
	uint32 size = props_list.size();
	int index = grindex_id - beg_index;
	if (index < 0 || index >= (int)size) {
		return 0;
	}

	number = props_list[index].count();

	return number;
}

grid_role_props* props_parser::mutable_props(uint32 grid_index)
{
    size_t size  = props_list.size();
    uint32 index = grid_index - beg_index;
    if (index >= size) {
        return NULL;
    }

    return &props_list[index];
}

const uint32 props_parser::get_props_num(uint32 props_id) const
{
    uint32 size = props_list.size();
    uint32 num = 0;
    for (uint32 i = 0; i < size; ++i){
        num += props_list[i].get_props_num(props_id);
    }

    return num;
}

bool props_sort (const grid_role_props& A, const grid_role_props& B)
{
    if (!A.has_props()) {
        return false;
    } else if (!B.has_props()) {
        return true;
    }

    return A.props_id() < B.props_id();
}

void props_parser::sort()
{
    uint32 now = (uint32)time(NULL);
    if (sort_time + 5 < now) {
        sort_time = now;
    } else {
        return ;
    }
    
    uint32 size = props_list.size();
    std::sort(props_list.begin(), props_list.end(), props_sort);
    /*for (uint32 i = 0; i < size; ++i) {
      printf("id = %d, num = %d\n", props_list[i].props_id(), props_list[i].overlaped_num());
      }*/

    change_flag.set(0);
    for (uint32 i = 1; i < size; ++i) {
        change_flag.set(i);
        if (!props_list[i].has_props()) {
            continue;
        }
        
        if (!props_list[i - 1].has_props() 
            || same_props(props_list[i], props_list[i - 1]) 
            || (props_list[i - 1].count() == 0 && props_list[i].has_props())) {
            move_props(props_list[i].grid_index(), props_list[i - 1].grid_index());
        } 
    }
}

bool props_parser::is_full()
{
    uint32 size = props_list.size();
    for (uint32 i = 0; i < size; ++i) {
        if (!props_list[i].has_props()) {
            return false;
        }
    }
    
    return true;
}

bool props_parser::is_has_prop()
{
	uint32 size = props_list.size();
	for (uint32 i = 0; i < size; ++i) {
		if (props_list[i].has_props()) {
			return true;
		}
	}
	
	return false;
}

bool props_parser::is_bind(uint32 grid_index)
{
    grid_role_props* rg = mutable_props(grid_index);
    if (rg == NULL) {
        return true;
    }

    return rg->bind() > 0;
}

void props_parser::grid_change_flag(uint32 grid_index)
{
    change_flag.set(grid_index - beg_index);
}

int props_parser::move_props(uint32 src_grid, uint32 dst_grid, uint32 num)
{
    uint32 size   = props_list.size();
    int src_index = src_grid - beg_index;
    int dst_index = dst_grid - beg_index;
    if (src_index < 0 || src_index >= (int)size
        || !props_list[src_index].has_props()
        || num > props_list[src_index].count()
        || dst_index < 0 || dst_index >= (int)size) {
        return -1;
    }

    grid_role_props& src = props_list[src_index];
    if (num == 0) {
        num = src.count();
    }
    
    const config_props* cg = CONFMGR->get_config_props(src.props_id());
    if (cg == NULL) {
        return -3;
    }
    
    bool moved = false;
    grid_role_props& dst = props_list[dst_index];
    if (dst.has_props()) {
        if (!same_props(src, dst)) {
            return -3;
        }
        // 重叠
        overlaped(dst_index, dst, src, cg, num);
        moved = true;
    }

    if (!moved) { // 到这里目标格子一定是没放东西的了
        if (num >= src.count()) {
            dst = src;
            src.reset();
        } else { // 部分物品移动
            dst = src;
            dst.set_count(num);
            src.set_count(src.count() - num);
        }
    }
    
    change_flag.set(src_index);
    change_flag.set(dst_index);
    
    return 0;
}
bool is_floor(uint32 attr_type)
{
	switch(attr_type)
	{
	case 2:// 攻击下限
		return true;
		break;
	case 4: // 防御下限
		return true;
		break;

	}

	return false;
}
bool props_parser::strengthen_equip(uint32 old_level,uint32 new_level,uint32 grid_index)
{
	grid_role_props* props_info = mutable_props(grid_index);
	if (props_info == NULL){
		return false;
	}

	const config_equip_upgrade* next_upgrade_info = CONFMGR->get_config_equip_upgrade_mgr().get_config_equip_upgrade(new_level);
	if (next_upgrade_info == NULL) {
		return false;
	}
	// 设置强化属性
	const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(props_info->props_id());
	if (base_info == NULL){
		return false;
	}
	// 成长值
	uint32 grow_value = base_info->get_basic_upgrade();

	// 清除强化的属性
	props_info->clear_upgrade_attr();
	//// 附加新的强化属性
	uint32 add_value = 0;

	add_value = grow_value * (new_level + (static_cast<uint32>(new_level /10.0f)) * 5);

	const std::map<int, int>&  attr_list_ = base_info->get_game_attribute();
	std::map<int, int>::const_iterator itr = attr_list_.begin();
	for (;itr != attr_list_.end(); ++itr){
		if (is_floor(static_cast<uint32>(itr->first))){
			continue;
		}
		client::attribute* attr = props_info->add_upgrade_attr();
		attr->set_type(static_cast<uint32>(itr->first));
		attr->set_value(add_value);
	}

	props_info->set_level(new_level);

	int index = grid_index - beg_index;
	change_flag.set(index);

	return true;
}

int props_parser::strengthen_cast_equip(client::props_info& props_info,const config_upequip* upequip_info)
{
	// 对client_props_info 处理加成
	uint32  total_value = 0;
	std::map<int ,int>::const_iterator itr = upequip_info->get_improve_update_attr().begin();
	for (int i = 0; i < props_info.attr_size(); ++i)
	{
		client::attribute* attr = props_info.mutable_attr(i);
		for (;itr != upequip_info->get_improve_update_attr().end(); ++itr)
		{
			if (attr->type() == static_cast<uint32>(itr->first)) {

				total_value = attr->value() + static_cast<uint32>(itr->second);

				attr->set_value(total_value);
			}
		}
	}
	
	return 0;
}

void props_parser::fill_null_props_info(client::props_info& pi, uint32 grid_index)
{
    pi.set_pos       (0);
    pi.set_grid_index(grid_index);
    pi.set_props_id  (0);
    pi.set_count     (0);
    pi.set_quality   (0);
    pi.set_bind      (0);
}

void props_parser::fill_props_info(const client::props_info& rg, client::props_info& pi)
{
    pi.CopyFrom(rg);
}

uint32 props_parser::get_free_space()
{
    uint32 count = 0;
    size_t size = props_list.size();
    for (size_t i = 0; i < size; ++i) {
        if (!props_list[i].has_props()) {
            ++count;
        }
    }

    return count;
}

uint32 props_parser::get_use_space()
{
    uint32 count = 0;
    size_t size = props_list.size();
    for (size_t i = 0; i < size; ++i) {
        if (props_list[i].has_props()) {
            ++count;
        }
    }

    return count;
}

int props_parser::add_props(uint32 props_id, uint32 number, vector<client::props_info>& vec_prop,uint32 quality /* = 0 */, uint32 binding /* = 0 */, uint32 profession /* = 0 */)
{
	uint32 add_count = 0;
	int ret = 0;
	for (uint32 i = 0; i < number; ++i){
		const config_props* cg = CONFMGR->get_config_props(props_id);
		if (cg == NULL) {
			break;
		}

		quality = cg->get_name_color_index();
		binding = cg->get_bind_type() == PBT_BIND ? PBT_BIND : PBT_NOT_BIND;

		client::props_info rg;
		if (makeup_props(props_id, number, quality, binding, *cg, rg) != 0) {
			continue;
		}

		ret = add(rg, cg);
		if (ret == -1){
			// 移动到临时背包放
			vec_prop.push_back(rg);
		}

		++add_count;
	}
	
    return add_count;
}

int props_parser::add_props(vector<client::props_info>& vpi, uint32& count,vector<client::props_info>& vec_prop)
{
    uint32 size = vpi.size();
	int ret = 0;
    for (uint32 i = 0; i < size; ++i) {
        const config_props* cg = CONFMGR->get_config_props(vpi[i].props_id());
        if (cg == NULL) {
            continue;
        }

        grid_role_props rg;
        makeup_props(vpi[i], *cg, rg);
        ret = add(rg, cg);
		if (ret == -1){
			vec_prop.push_back(rg);
		}
        //if (rg.count() > 0) {
        //    break;
        //}

        ++count;
    }

    return 0;
}

int props_parser::add_props(const client::props_info& pi)
{
    const config_props* cg = CONFMGR->get_config_props(pi.props_id());
    if (cg == NULL) {
        return -2;
    }

	client::props_info rg;
	makeup_props(pi, *cg, rg);
	int ret = add(rg, cg);
	if (ret == -1){
		return ret;
	}

	return rg.count();
}

int props_parser::add_props_equip(const client::props_info& pi)
{
	const config_props* cg = CONFMGR->get_config_props(pi.props_id());
	if (cg == NULL) {
		return -1;
	}

	client::props_info rg;
	makeup_props(pi, *cg, rg);
	add(rg, cg);

	return rg.grid_index();
}

int props_parser::add_cast_props(client::props_info& pi)
{
	const config_props* cg = CONFMGR->get_config_props(pi.props_id());
	if (cg == NULL) {
		return -1;
	}

	add(pi, cg);

	return pi.count();
}
int props_parser::add_props(client::props_info& rg, uint32 grid_index)
{
    uint32 size = props_list.size();
    int index = grid_index - beg_index;
    if (index < 0 || index >= (int)size) {
        return -1;
    }

    const config_props* cg = CONFMGR->get_config_props(rg.props_id());
    if (cg == NULL) {
        return -1;
    }

    change_flag.set(index);
    grid_role_props& src = props_list[index];
    if (src.has_props()) {
        if (src.count() > 0) {
            overlaped(index, src, rg, cg);
            return rg.count();
        }
    } 

    src.CopyFrom(rg);
    src.set_pos(get_pos());
    src.set_grid_index(grid_index);
    rg.set_count(0);

    return rg.count();
}

int props_parser::makeup_props(const client::props_info& pi, const config_props& cg, client::props_info& rg)
{	
    rg.CopyFrom(pi);
	if (rg.system_id()== 0){
		uint32 psid = 0;
		if (generate_props_smy_id(psid)){
			rg.set_system_id(psid);
		}
	}

	//printf("props_parser::----------smy_id = %d\n",rg.system_id());
    return 0;
}
int props_parser::makeup_props(uint32 props_id, uint32 number, uint32 quality, uint32 binding, const config_props& cg, client::props_info& rg)
{
	if (quality > 4 || quality < 0){
		quality = 0;
	}

	rg.set_pos           (get_pos());
	rg.set_grid_index    (0);
	rg.set_props_id      (props_id);
	rg.set_count         (1);
	rg.set_quality       (quality);
	rg.set_bind          (binding);

	if (rg.system_id() == 0){
		uint32 psid = 0;
		if (generate_props_smy_id(psid)){
			rg.set_system_id(psid);
		}
	}

	//printf("props_parser::----------smy_id = %d\n",rg.system_id());
	rand_attr(&cg, rg);

    return 0;
}

int props_parser::overlaped(uint32 index, client::props_info& src, client::props_info& dst, const config_props* tg, uint32 num)
{
    if (!can_overlaped(src, dst, tg) || num > dst.count()) {
        return -1;
    }

    change_flag.set(index);
    if (num == 0) {
        num = dst.count();
    }

    int rest_num  = dst.count() - num;
    int total_num = src.count() + num;
    if (total_num <= tg->get_total()) {
        src.set_count(total_num);
        dst.set_count(rest_num);
    } else {
        rest_num += (total_num - tg->get_total());
        src.set_count(tg->get_total());
        dst.set_count(rest_num);
    }

    return 0;
}

int props_parser::add(client::props_info& dst, const config_props* tg)
{
    //while (dst.count() > 0 && add_ex(dst, tg) == 0) {
    //}

	if (dst.count() > 0){
		return add_ex(dst, tg);
	}

    return 0;
}

int props_parser::add_ex(client::props_info& dst, const config_props* tg)
{
    uint32 size = props_list.size();
    int pos = 0;
    for (uint32 i = 0; i < size; ++i) {
        grid_role_props& src = props_list[i];
        if (!src.has_props()) {
            if (pos == 0) {
                pos = i + 1;
            }

            if (tg->is_equip() || tg->get_total() <= 1) { // 不能叠加的物品
                break;
            }

            continue;
        }

        if (overlaped(i, src, dst, tg) == 0 && dst.count() <= 0) { 
            return 0; // 叠加完成
        }
    }

    if (pos-- == 0) {  // 没有剩余格子
        return -1;
    }

    uint32 reset_num = 0;
    if (dst.count() > (uint32)tg->get_total()) {
        reset_num = dst.count() - tg->get_total();
        dst.set_count(dst.count() - reset_num);
    }
		
    
    change_flag.set(pos);
    props_list[pos] = dst;
    props_list[pos].set_pos(get_pos());
	dst.set_grid_index(props_list[pos].grid_index());
	dst.set_count(reset_num);
    
    return 0; 
}

void props_parser::remove_all()
{
    clear();
}

client::props_info_POS props_parser::get_pos()
{
    switch (beg_index) {
    case PACK_GRID_BEG: {
        return client::props_info_POS_PACK;
    }
    case DEPOT_GRID_BEG: {
        return client::props_info_POS_DEPOT;
    }
    case EQUIP_GRID_BEG: {
        return client::props_info_POS_ROLE; 
    }
    case EXPLORE_GRID_BEG: {
        return client::props_info_POS_EXPLORE; 
    }
    case TASK_GRID_BEG: {
        return client::props_info_POS_TASK;
    }
    default: break;
    }

    return client::props_info_POS_PACK;
}

int props_parser::set_props_smy_id(client::props_info& item_info)
{
	uint32 psid = item_info.system_id();

	map<uint32,props_smyid>::iterator itr = prop_smy_id.find(beg_index);
	if (itr == prop_smy_id.end()){
		return -1;
	}

	uint32 max_psid = itr->second.get_props_smy_id();
	uint32 smy_id = GET_PROP_SMY_ID(psid);
	if (smy_id > max_psid){
		itr->second.set(beg_index,smy_id);
	}

	return 0;
}

bool props_parser::generate_props_smy_id(uint32& psid)
{
	map<uint32,props_smyid>::iterator itr = prop_smy_id.find(beg_index);
	if (itr == prop_smy_id.end()){
		return false;
	}

	psid = itr->second.get();
	if (psid == 0){
		return false;
	}

	return true;
}

bool props_parser::can_overlaped(client::props_info& src, const client::props_info& dst, const config_props* cp)
{
    return same_props(src, dst) && (src.count() < (uint32)cp->get_total());
}

bool props_parser::same_props(const client::props_info& src, const client::props_info& dst)
{
	const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(src.props_id());
	if (base_info == NULL){
		return false;
	}

	if (base_info->get_item_type() == PT_EQUIP || base_info->get_item_type() == PT_PET_EQUIP){
		return false;
	}else{
		if (src.props_id() == dst.props_id()){
			if (src.has_to_type() && src.has_timeout() && dst.has_to_type() && dst.has_timeout()){
				if (src.to_type() == dst.to_type() && src.timeout() == dst.timeout()){
					return true;
				}else{
					return false;
				}
			}else if (src.has_to_type() && src.has_timeout() && !dst.has_to_type() && !dst.has_timeout()){
				return false;
			}else if (!src.has_to_type() && !src.has_timeout() && dst.has_to_type() && dst.has_timeout()){
				return false;
			}

			return true;
		}
	}

	//src.props_id() == dst.props_id() && src.bind() == dst.bind() && src.quality() == dst.quality()
	
	return false;
}

bool props_parser::check_props_time()
{
	uint32 size = props_list.size();

	uint32 cur_time = (uint32)time(NULL);

	for (uint32 i = 0; i < size; ++i){
		if (!props_list[i].has_to_type()){
			continue;
		}

		if (!props_list[i].has_timeout()){
			continue;
		}

		if (props_list[i].has_is_due()){
			if (props_list[i].is_due()){
				continue;
			}
		}
		// 判断过期，设置过期标记
		if (props_list[i].to_type() == 2){
			if (props_list[i].timeout() <= cur_time){
				props_list[i].set_is_due(true);
			}else{
				props_list[i].set_timeout(props_list[i].timeout() - 1);
			}
		}else{
			if (props_list[i].timeout() <= cur_time){
				props_list[i].set_is_due(true);
			}
		}
	}

	return true;
}

bool sort_props_time( grid_role_props* a, grid_role_props* b)
{
	if (a->timeout() < b->timeout()){
		return true;
	}else if (a->timeout() == b->timeout()){
		return a->props_id() > b->props_id();
	}

	return false;
}

bool props_parser::compare_props_time(uint32& need_count,const std::vector<int>& sub_item_id,std::map<uint32 ,uint32>& expend_map_)
{
	std::vector<grid_role_props*> props_time_grindex_vec;
	props_time_grindex_vec.clear();

	uint32 size = props_list.size();
	// 找到所需的所有符合要求的格子
	for (uint32 i = 0; i < size; ++i){
		if (props_list[i].has_timeout()){
			if (props_list[i].has_is_due()){
				if (props_list[i].is_due()){
					continue;
				}
			}

			std::vector<int>::const_iterator it = find(sub_item_id.begin(),sub_item_id.end(),static_cast<int>(props_list[i].props_id()));
			if (it != sub_item_id.end()){
				props_time_grindex_vec.push_back(&props_list[i]);
			}
		}
	}


	std::sort(props_time_grindex_vec.begin(),props_time_grindex_vec.end(),sort_props_time);
	

	// 插入消耗的序列
	uint32 cur_count = 0;
	if (props_time_grindex_vec.size() != 0){
		for (uint32 k = 0; k < props_time_grindex_vec.size();++k){
			cur_count = props_time_grindex_vec[k]->count();
			if (cur_count >= need_count){
				expend_map_.insert(std::pair<uint32,uint32>(props_time_grindex_vec[k]->grid_index(),need_count));
				need_count = 0;
				return true;
			}else{
				expend_map_.insert(std::pair<uint32,uint32>(props_time_grindex_vec[k]->grid_index(),cur_count));
				need_count -= cur_count;
			}
		}
	}

	return true;

}

float props_parser::calculate_equip_power(grid_role_props& grid_prop)
{
	float base_value[15] = {0.0f,0.2f,0.0f,5.0f,0.0f,7.5f,2.0f,2.0f,25.0f,1.0f,30.0f,0.0f,5.0f,0.0f,0.0f};
	float total_power = 0; 

	const config_props* item_info = CONFMGR->get_config_props(grid_prop.props_id());
	if (item_info == NULL){
		return total_power;
	}

	const std::map<int, int>&  game_attribute = item_info->get_game_attribute();
	std::map<int, int>::const_iterator it_game = game_attribute.begin();
	for (; it_game != game_attribute.end(); ++it_game){
		total_power += base_value[it_game->first] * it_game->second;
	}

	for (int i = 0; i < grid_prop.attr_size();++i){
		total_power += base_value[grid_prop.attr(i).type()] * grid_prop.attr(i).value();
	}

	for (int j = 0; j < grid_prop.upgrade_attr_size();++j){
		total_power += base_value[grid_prop.upgrade_attr(j).type()] * grid_prop.upgrade_attr(j).value();
	}

	return total_power;
}

float props_parser::calcula_depot_power(uint32 gr_index)
{
	float total_power = 0.0f;
	
	uint32 index = gr_index - beg_index;

	if (!props_list[index].has_props()){
		return total_power;
	}

	const config_props* item_base_info = CONFMGR->get_config_props_mgr().get_config_props(props_list[index].props_id());
	if (item_base_info == NULL){
		return total_power;
	}

	total_power = calculate_equip_power(props_list[index]);

	return total_power;
}
