#include "config_legion.h"
#include "config_mgr.h"
#include "family.h"

family::family(mysql::tb_family* tf/* = NULL*/)
    : _family(tf)
    , change(false)
{
    writer_type = WOTE_NULL;
}

family::~family()
{
    if (_family != NULL) {
        delete _family;
        _family = NULL;
    }
}

int family::save(amtsvr_context* ctx)
{
    if (!change || _family == NULL) {
        return -1;
    }

    mysql::tb_family* tf = new mysql::tb_family(*_family);
    CACHE_KEY k(tf->id(), 0);
    if (writer_type != WOTE_REMOVE){
    }

    write(ctx, k, tf);
    change = false;
    writer_type = WOTE_NULL;
    
    return 0;
}

int family::create(uint32 id, client::create_family_req_use type, const string& name, uint32 flag_id, const string& flag_name, uint32 chief_id,const std::string creater_name)
{
    if (_family != NULL) {
        return -1;
    }

	uint32 now = (uint32)time(NULL);

    _family = new mysql::tb_family;
    _family->set_id					(id                        );
    _family->set_name				(name                      );
    _family->set_flag_id			(flag_id                   );
    _family->set_flag_name			(flag_name                 );
    _family->set_lvl				(type > 0 ? 1 : 1          );
    _family->set_chief_id			(chief_id                  );
	_family->set_chief_name			(creater_name              );
    _family->set_deputy_id			(0                         );
    _family->set_post				(""                        );
    _family->set_money				(0                         );
    _family->set_power				(0                         );
    _family->set_domain				(0                         );
    _family->set_king				(0                         );
    _family->set_emperor			(0                         );
    _family->set_cur_member_num		(0                         );
    _family->set_approval			(0                         );
    _family->set_activity			(0                         );
    _family->set_qq					(0                         );
    _family->set_yy					(0                         );
    _family->set_boss_time			(0x00120000                );
    _family->set_call_boss_time		(0                         );
    _family->set_op_tag				(0                         );
    _family->set_op_tag_time		(0                         );
    _family->set_status				(1                         );
    _family->set_belong_sect		(0                         );
	_family->set_dragon_count		(0						   );
	_family->set_tiger_count		(0						   );
	_family->set_rosefinch_count	(0						   );
	_family->set_tortoise_count		(0						   );
	_family->set_create_time		(now					   );
    writer_type = WOTE_INSERT;
    change = true;

    return 0;
}

int family::family_info(client::family_info* fi)
{
    if (get() == NULL || fi == NULL) {
        return -1;
    }

    //const google::protobuf::Reflection* src_ref = get()->GetReflection();
    //const google::protobuf::Descriptor* src_des = get()->GetDescriptor();
    //const google::protobuf::Reflection* dst_ref = fi->GetReflection();
    //const google::protobuf::Descriptor* dst_des = fi->GetDescriptor();

    //for (int32 i = 0; i < dst_des->field_count(); ++i) {
    //    switch (dst_des->field(i)->type()) {
    //    case google::protobuf::FieldDescriptor::TYPE_UINT32: {
    //        uint32 val = src_ref->GetUInt32(*get(), src_des->field(i));
    //        dst_ref->SetUInt32(fi, dst_des->field(i), val);
    //        break;
    //    }
    //    case google::protobuf::FieldDescriptor::TYPE_STRING: {
    //        std::string val = src_ref->GetString(*get(), src_des->field(i));
    //        dst_ref->SetString(fi, dst_des->field(i), val);
    //        break;
    //    }
    //    case google::protobuf::FieldDescriptor::TYPE_BOOL: {
    //        bool val = src_ref->GetInt32(*get(), src_des->field(i)) > 0;
    //        dst_ref->SetBool(fi, dst_des->field(i), val);
    //        break;
    //    }
    //    default: break;
    //    }
    //}

	fi->set_id(get()->id());
	fi->set_name(get()->name());
	fi->set_flag_id(get()->flag_id());
	fi->set_flag_name(get()->flag_name());
	fi->set_lvl(get()->lvl());
	fi->set_chief_id(get()->chief_id());
	fi->set_chief_name(get()->chief_name());
	fi->set_deputy_id(get()->deputy_id());
	fi->set_post(get()->post());
	fi->set_money(get()->money());
	fi->set_power(get()->power());
	fi->set_domain(get()->domain());
	fi->set_king(get()->king() != 0);
	fi->set_emperor(get()->emperor() != 0);
	fi->set_cur_member_num(get()->cur_member_num());
	fi->set_approval(get()->approval() != 0);
	fi->set_leader_online(0);
	fi->set_create_time(get()->create_time());
	fi->set_dragon_count(get()->dragon_count());
	fi->set_tiger_count(get()->tiger_count());
	fi->set_rosefinch_count(get()->rosefinch_count());
	fi->set_tortoise_count(get()->tortoise_count());

    return 0;
}

int family::family_title_info(client::family_title_info* title_info)
{

	title_info->set_family_id(get()->id());
	title_info->set_name(get()->name());

	return 0;
}

int family::set_post(const string& post)
{
    if (_family == NULL) {
        return -1;
    }
    
    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _family->set_post(post);
    change = true;
    
    return 0;
}

int family::set_approval(uint32 approval)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_approval(approval);
	change = true;

	return 0;
}

int family::get_approval()
{
	return _family->approval();
}

int family::set_qq(uint32 qq)
{
    if (_family == NULL) {
        return -1;
    }
    
    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _family->set_qq(qq);
    change = true;

    return 0;
}

int family::set_yy(uint32 yy)
{
    if (_family == NULL) {
        return -1;
    }
    
    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _family->set_yy(yy);
    change = true;
    
    return 0;
}

int family::get_money()
{
	return _family->money();
}

string family::get_family_name()
{
	return  _family->name();
}

uint32 family::get_family_id()
{
	return _family->id();
}

int family::add_money(uint32 money)
{
    return set_money(_family->money() + money);
}

int family::set_money(uint32 money)
{
    if (_family == NULL) {
        return -1;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _family->set_money(money);
    change = true;

    return 0;
}

int family::set_dragon_count(uint32 count)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_dragon_count(count);
	change = true;

	return 0;
}

int family::set_rosefinch_count(uint32 count)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_rosefinch_count(count);
	change = true;

	return 0;
}
int family::set_tiger_count(uint32 count)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_tiger_count(count);

	change = true;

	return 0;
}

int family::set_tortoise_count(uint32 count)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_tortoise_count(count);

	change = true;

	return 0;
}

int family::upgrade()
{
    if (_family == NULL) {
        return -1;
    }
    
    if (_family->lvl() >= MAX_FAMILY_LVL) {
        return -2;
    }

	uint32 next_level = _family->lvl() + 1;
	const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(_family->lvl());
	if (legion_info == NULL){
		return -2;
	}

	const std::map<int,int>& need_item_list = legion_info->get_upgrade_need_item();
	if (need_item_list.size() == 0){
		return -2;
	}

	uint32 need_money = static_cast<uint32>(legion_info->get_upgrade_need_money());
    
	bool is_not_enough = false;
	uint32 dragon_count = 0;
	uint32 tiger_count = 0;
	uint32 rosefinch_count = 0;
	uint32 tortoise_count = 0;
    // 
	std::map<int,int>::const_iterator itr = need_item_list.begin();
	for (;itr != need_item_list.end(); ++itr){
		if (itr->first == 50145001){
			dragon_count = static_cast<uint32>(itr->second);
		}else if (itr->first == 50145002){
			rosefinch_count = static_cast<uint32>(itr->second);
		}else if (itr->first == 50145003){
			tortoise_count = static_cast<uint32>(itr->second);
		}else if (itr->first == 50145004){
			tiger_count = static_cast<uint32>(itr->second);
		}
	}

	if (_family->dragon_count() < dragon_count || _family->rosefinch_count() < rosefinch_count
		|| _family->tortoise_count() < tortoise_count || _family->tiger_count() < tiger_count){
			is_not_enough = true;
	}

    if (_family->money() < need_money) {
        return -3;
    }

	if (is_not_enough){
		return -3;
	}

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

	// 扣除相关东西
    _family->set_money(_family->money() - need_money);
	_family->set_dragon_count(_family->dragon_count() - dragon_count);
	_family->set_tortoise_count(_family->tortoise_count() - tortoise_count);
	_family->set_rosefinch_count(_family->rosefinch_count() - rosefinch_count);
	_family->set_tiger_count(_family->tiger_count() - tiger_count);
    _family->set_lvl(next_level);
    change = true;
    
    return 0;
}

uint32 family::lvl()
{
    if (_family == NULL) {
        return 0;
    }
    
    return _family->lvl();
}

int family::remove()
{
    if (writer_type == WOTE_INSERT) {
        change = false;
    } else {
        writer_type = WOTE_REMOVE;
        change = true;
    }

    _family->set_status(0);

    return 0;
}

int family::set_update_tag()
{
    if (_family == NULL) {
        return -1;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    change = true;
    
    return 0;
}

int family::set_flag_id(uint32 flag_id)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_flag_id(flag_id);

	change = true;


	return 0;
}

int family::set_flag_name(string flag_name)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_flag_name(flag_name);

	change = true;


	return 0;
}

int family::set_cur_member_num(uint32 member_count)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_cur_member_num(member_count);

	change = true;

	return 0;
}

int family::set_family_power(uint32 power_value)
{
	if (_family == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_family->set_power(power_value);

	change = true;

	return 0;

}