#include "cfg_parser.h"
#include "config_lottery.h"

config_lottery::config_lottery(void)
{
}

config_lottery::~config_lottery(void)
{
}

config_lottery_mgr::config_lottery_mgr(TTableGroup_s* p)
        : _lottery(p)
{
    init(p);
}


config_lottery_mgr::~config_lottery_mgr(void)
{
    clear();
}

int config_lottery_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _lottery = p;
    _table = _lottery->GetTable("t_treasurebox");
    if (_table == NULL) {
        printf("not found t_treasure\n");
        return -2;
    }
    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) 
	{
		config_lottery *cl = new config_lottery;
		cl->_lottery_id = _table->GetField(i, "LotteryID");
		cl->_job = _table->GetField(i, "Job");
		cl->_prop_id = _table->GetField(i, "ItemID");;
		cl->_type = _table->GetField(i, "TreasureType");;
		cl->_item_type = _table->GetField(i, "ItemType");
		cl->_max = _table->GetField(i, "AmountMaxToday");
		cl->_probability = _table->GetField(i, "Probability");
		cl->_min_lvl = _table->GetField(i, "LevelMin");
		cl->_max_lvl = _table->GetField(i, "LevelMax");
		cl->_is_group = _table->GetField(i, "IsGroup");
		_list.insert(make_pair(cl->_lottery_id, cl));
		if(cl->_type == 0)			// »Æ½ð
		{
			_gold.push_back(cl->_lottery_id);
		}
		else
		{
			_bronze.push_back(cl->_lottery_id);
		}
	}
    return 0;
}

int config_lottery_mgr::clear()
{
	std::map<uint32, config_lottery *>::iterator it;
	for(it = _list.begin(); it != _list.end(); ++it)
	{
		delete it->second;
	}
	_list.clear();
	_gold.clear();
	_bronze.clear();
    return 0;
}

const std::vector<uint32>& config_lottery_mgr::get_config_lottery_list(uint32 type) const
{
	if(type == 0)
	{
		return _gold;
	}
	return _bronze;
}

const config_lottery *config_lottery_mgr::get_config_lottery(uint32 id)
{
	map<uint32, config_lottery *>::iterator it = _list.find(id);
	if(it != _list.end())
	{
		return it->second;
	}
	return NULL;
}
