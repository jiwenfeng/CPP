#ifndef __config_name_H__
#define __config_name_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "client.role_props.pb.h"
#include "config_base.h"

class config_name_mgr : public config_base_mgr
{
public:
	config_name_mgr(TTableGroup_s* p = NULL);
	~config_name_mgr(void);

public:
    int                                  init(TTableGroup_s* p);
    int                                  clear();

public:
	inline const vector<string>& get_first_name() const { return _first_name; }
	inline const vector<string>& get_male_second_name() const { return _m_second_name; }
	inline const vector<string>& get_female_second_name() const { return _f_second_name; }

private:
    TTableGroup_s*                 _name;
    TTable*                       _table;

	vector<string>				 _first_name;		// ÐÕ
	vector<string>				 _f_second_name;	// Å®Ãû
	vector<string>				 _m_second_name;	// ÄÐÃû
};

#endif // __config_npc_H__
