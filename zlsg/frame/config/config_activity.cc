#include "cfg_parser.h"
#include "config_activity.h"

config_activity::config_activity()
{
}

config_activity::~config_activity()
{

}

config_activity_mgr::config_activity_mgr(TTableGroup_s* p)
        : _activity(p)
{
    init(p);
}


config_activity_mgr::~config_activity_mgr(void)
{
    clear();
}

int config_activity_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _activity = p;
    _table = _activity->GetTable("t_activity");
    if (_table == NULL) {
        printf("not found t_activity\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "activityID"         ) == 0) {
            continue;
        }

        config_activity* p = new config_activity;
        p->activity_id                       = _table->GetField      (i, "activityID"              );
		p->open_type                         = _table->GetField      (i, "openType"                );
		p->interval_day                      = _table->GetField      (i, "intervalDay"             );
		p->start_year                        = _table->GetField      (i, "startYear"               );
		p->start_month                       = _table->GetField      (i, "startMonth"              );
		p->start_day                         = _table->GetField      (i, "startDay"                );
		p->start_hour                        = _table->GetField      (i, "startHour"               );
		p->start_minute                      = _table->GetField      (i, "startMin"                );
		p->start_second                      = _table->GetField      (i, "startSecond"             );
		p->last_time                         = _table->GetField      (i, "lastTime"                );
		p->fixed_day                         = _table->GetField      (i, "fixedDay"                );
		p->repeat_time                         = _table->GetField    (i, "repeatTime"              );

		_cp.insert(std::pair<int32, config_activity*>(p->activity_id,p));
    }

    return 0;
}

int config_activity_mgr::clear()
{
    for (std::map<uint32, config_activity*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_activity* config_activity_mgr::get_config_activity(uint32 activity_id) const
{
    std::map<uint32, config_activity*>::const_iterator itr = _cp.find(activity_id);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}

const std::map<uint32, config_activity*>& config_activity_mgr::get_config_activity_list() const
{
	return _cp;
}