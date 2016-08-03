#include "cfg_parser.h"
#include "config_activity_item.h"
#include "amtsvr.h"
config_activity_item::config_activity_item(void)
{
}

config_activity_item::~config_activity_item(void)
{
}

config_activity_item_mgr::config_activity_item_mgr(TTableGroup_s* p)
        : _act_item(p)
{
    init(p);
}


config_activity_item_mgr::~config_activity_item_mgr(void)
{
    clear();
}

int config_activity_item_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _act_item = p;
    _table = _act_item->GetTable("t_activity_item");
    if (_table == NULL) {
        printf("not found t_activity_item\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "activityItemID"         ) == 0) {
            continue;
        }

        config_activity_item* p = new config_activity_item;
        p->activity_type                         = _table->GetField      (i, "activityItemID"              );
		p->activity_id                           = _table->GetField      (i, "activityID"				   );
		p->item_order                            = _table->GetField      (i, "itemOrder"				   );
		p->agio_item                             = _table->GetField      (i, "agioItem"					   );
		p->is_reset                              = _table->GetField      (i, "is_reset"					   );
		p->get_item                              = _table->GetField      (i, "getItem"					   );
		p->start_time                            = _table->GetField      (i, "startTime"				   );
		p->pass_time                             = _table->GetField      (i, "passTime"					   );
		p->equip_order                           = _table->GetField      (i, "equipOrder"				   );

		cpf_0 f0;
		f0.set_cfg(&p->a_conditon);
		cp.parser<cpf_0>(_table->GetFieldString(i, "aConditon").c_str(), ";", f0);

		cpf_0 f2;
		f2.set_cfg(&p->buy_times);
		cp.parser<cpf_0>(_table->GetFieldString(i, "buyTimes").c_str(), ";", f2);

		cpf_1 f3;
		f3.set_cfg(&p->b_item_price2);
		cp.parser<cpf_1>(_table->GetFieldString(i, "bItemPrice2").c_str(), ";", f3);

		_cp.insert(std::pair<int32, config_activity_item*>(p->activity_type,p));
    }

    return 0;
}

int config_activity_item_mgr::clear()
{
    for (std::map<uint32, config_activity_item*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_activity_item* config_activity_item_mgr::get_config_activity_item(uint32 activity_type) const
{
    std::map<uint32, config_activity_item*>::const_iterator itr = _cp.find(activity_type);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}

const uint32 config_activity_item_mgr::get_act_vec(uint32 activity_id,std::vector<config_activity_item*>& item_vec) const
{
	uint32 need_time = 0;
#ifndef __LIB_MINI_SERVER__
	 need_time = (uint32)time(NULL) - static_cast<uint32>(amtsvr_svr_start_time());
#endif
	 std::map<uint32, config_activity_item*>::const_iterator itr = _cp.begin();

	 for (; itr != _cp.end(); ++itr){
		if (itr->second->get_activity_id() == activity_id && need_time >itr->second->get_start_time() && need_time <= itr->second->get_pass_time()){
			item_vec.push_back(itr->second);
		}
	 }

	return 0;
}

const uint32 config_activity_item_mgr::get_act_limit_type(uint32 activity_id)const
{
	std::map<uint32, config_activity_item*>::const_iterator itr = _cp.begin();

	for (; itr != _cp.end(); ++itr){
		if (itr->second->get_activity_id() == activity_id){
			const std::map<int, int>& limit_map_ = itr->second->get_a_conditon();
			std::map<int, int>::const_iterator it = limit_map_.begin();
			if (it != limit_map_.end()){
				return (uint32)it->first;
			}
		}
	}

	return 0;
}