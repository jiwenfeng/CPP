#include "random.h"
#include "config_name.h"


config_name_mgr::config_name_mgr(TTableGroup_s* p)
    : _name(p)
{
    init(p);
}


config_name_mgr::~config_name_mgr(void)
{
    clear();
}

int config_name_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _name = p;
    _table = _name->GetTable("t_randomName");
    if (_table == NULL) {
        printf("not found t_randomName\n");
        return -2;
    }
    for (int i = 1; i <= _table->m_recordCount; ++i) 
	{
		uint32 type = _table->GetField(i, "Type");
		string str = _table->GetFieldString(i, "Name");
		switch(type)
		{
			case 1:
				_first_name.push_back(str);
				break;
			case 2:
				_m_second_name.push_back(str);
				break;
			case 3:
				_f_second_name.push_back(str);
				break;
			default:
				break;
		}
    }
    return 0;
}

int config_name_mgr::clear()
{
    return 0;
}

