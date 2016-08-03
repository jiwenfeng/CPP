#ifndef __CONFIG_PET_SKILL_H__
#define __CONFIG_PET_SKILL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_pet_skill
{
public:
	config_pet_skill() {};
	~config_pet_skill() {};

	const inline uint32 get_pet_id() const { return _id; }
	const inline uint32 get_pet_level() const { return _lv; }
	const inline uint32 get_pet_default_skill() const { return _default_skill; }
	const inline std::vector<uint32> &get_pet_skill() const { return _skills; }
	const inline std::map<int, int> & get_pet_attribute() const { return _attribute; }

public:
	uint32 _id;
	uint32 _lv;
	uint32 _default_skill;
	std::vector<uint32> _skills;
	std::map<int, int> _attribute;

};

class config_pet_skill_mgr : public config_base_mgr
{
public:
	config_pet_skill_mgr() {};
	~config_pet_skill_mgr();
	int init(TTableGroup_s *p);
	const config_pet_skill *get_config_pet_skill(uint32 id, uint32 lv) const;

private:
	void insert_config_pet_skill(config_pet_skill *cps);

private:
	TTableGroup_s *_pet_skill;
	TTable *_table;
	std::map<uint32, std::map<uint32, config_pet_skill *> > _list;
};

#endif
