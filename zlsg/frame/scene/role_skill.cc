#include "config_effect.h"
#include "config_beauty.h"
#include "role_skill.h"

skill::skill() 
{
    mas_val   = 0;
    ser_index = 0;
    cd        = 0;
    id        = 0;
    level     = 0;
    effect_id = 0;
}

skill::skill(const config_skill& cs, uint32 effect_level /*= 1*/)
    : skill_effect(cs)
{
    mas_val   = 0;
    ser_index = 0;
    cd        = 0;
    id        = cs.get_skill_id();
    level     = effect_level;
    effect_id = 0;

    init();
}

skill::skill(const mysql::tb_role_skill& tss, const config_skill& cs)
    : skill_effect(cs), id(tss.skill_id())
{
    mas_val   = tss.skillful();
    ser_index = tss.skill_grid_index();
    level     = tss.skill_level();
    cd        = tss.end_cd_time();
    effect_id = tss.effect_id();

    init();
}

skill::skill(const client::skill_data& sd, const config_skill& cs)
    : skill_effect(cs), id(sd.id())
{
    mas_val   = sd.mas();
    ser_index = sd.serial();
    level     = sd.level();
    cd        = sd.cd();
    effect_id = sd.effect_id();

    init();
}

skill::skill(const skill& rhs)
    : skill_effect(rhs) 
{
    operator=(rhs);
}

skill& skill::operator=(const skill& rhs)
{
    skill_effect::operator=(rhs);

    id         = rhs.id;
    level      = rhs.level;	
    mas_val    = rhs.mas_val;
    ser_index  = rhs.ser_index;
    cd         = rhs.cd;
    effect_id  = rhs.effect_id;

    distance   = rhs.distance;
    range      = rhs.range;
    max_target = rhs.max_target;

    memcpy(&cost, rhs.cost, sizeof(cost));

    return *this;
}

bool skill::operator==(uint32 sk_id)
{
    return (id == sk_id);
}

int skill::init(const config_skill* pcs /* = NULL */)
{
    if (pcs != NULL) {
        _cs = *pcs;
    }

    if (_cs.get_skill_id() == 0) {
        return -1;
    }

    if (effect_id == 0) {
        const config_effect* role_skill_info = CONFMGR->get_config_effect_mgr().get_config_effect_by_key(_cs.get_key(), level);
        if (role_skill_info != NULL) {
            effect_id  = role_skill_info->get_effect_id();
        }
    }

    id         = _cs.get_skill_id();
    //level      = _cs.get_skill_level();
    //mas_val    = 0;
    //ser_index  = 0;
    cd         = 0;
    range      = 1;
    distance   = _cs.get_distance() > 1 ? _cs.get_distance() : 1;
    max_target = _cs.get_affect_object_max();

    switch (_cs.get_skills_belonging()) {
    case SB_ANGER: { cost[SK_COST_ANGER].val = _cs.get_consume_mp(); break; }
    default: break;
    }

    return 0;
}

bool skill::cd_pass(uint32 now, uint32 speed /*= 0*/)
{
    if (cd == 0) {
        return true;
    }

    uint32 cooling_time = _cs.get_cool_tick() / 10;
    cooling_time = (uint32)(cooling_time / (1.0f + speed / 1000.0f));
    return (now - cd >= cooling_time);
}

role_skill::role_skill()
{
    _used_index     = 0;
    _min_atk_range = 0;
    _max_ser_index  = 0;
    _max_anger     = 0;
    _order         = 0;
}

role_skill::role_skill(const role_skill& rhs)
{
    operator=(rhs);
}

role_skill::~role_skill()
{
}

bool role_skill::add_skill(const skill& sk,uint32 serial_index)
{
    if (sk.ser_index > _max_ser_index) {
        _max_ser_index = sk.ser_index;
    }
	const config_skill* skill_base = CONFMGR->get_config_skill(sk.get_skill_id());
	if (skill_base == NULL){
		return false;
	}

	uint32 belong_type = skill_base->get_skills_belonging();
	if (!is_gem_skill(belong_type) && !is_bow_skill(belong_type)){
		vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), sk.get_skill_id());
		if (it == _have_sk.end()) {
			_have_sk.push_back(sk);

			uint32 tmp = sk.get_distance();
			if (_min_atk_range == 0 || (tmp > 0 && tmp < _min_atk_range)) {
				_min_atk_range = tmp;
			}

			tmp = sk.get_cost_anger();
			if (tmp > _max_anger) {
				_max_anger = tmp;
			}

			_order = _have_sk.size();

			return true;
		}
	}else{
		bool is_have = false;
		for (size_t i = 0; i < _have_sk.size(); ++i){
			if (_have_sk[i].ser_index != 0 && _have_sk[i].ser_index == serial_index && _have_sk[i].get_skill_id() == sk.get_skill_id()){
				is_have = true;
				break;
			}
		}

		if (!is_have){
			_have_sk.push_back(sk);

			uint32 tmp = sk.get_distance();
			if (_min_atk_range == 0 || (tmp > 0 && tmp < _min_atk_range)) {
				_min_atk_range = tmp;
			}

			tmp = sk.get_cost_anger();
			if (tmp > _max_anger) {
				_max_anger = tmp;
			}

			_order = _have_sk.size();

			return true;
		}
	}

    return false;
}

bool role_skill::delete_skill(uint32 skill_id,uint32 serial_index)
{
	const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);
	if (skill_base == NULL){
		return false;
	}

	uint32 belong_type = skill_base->get_skills_belonging();
	if (!is_gem_skill(belong_type) && !is_bow_skill(belong_type)){
		vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), skill_id);
		if (it != _have_sk.end()){
			_have_sk.erase(it);
			return true;
		}
	}else{
		for (vector<skill>::iterator itr = _have_sk.begin(); itr != _have_sk.end();){
			if (itr->ser_index != 0 && itr->get_skill_id() == skill_id && itr->ser_index == serial_index){
				_have_sk.erase(itr++);
				return true;
			}else{
				itr++;
			}
		}
	}

    return false;
}

bool role_skill::check_old_skill()
{
	for (size_t i = 0; i < _have_sk.size(); ++i){
		const config_skill* skill_base = CONFMGR->get_config_skill(_have_sk[i].get_skill_id());
		if (skill_base == NULL){
			continue;
		}

		if (!is_bow_skill(skill_base->get_skills_belonging())&& ! is_gem_skill(skill_base->get_skills_belonging())){
			_have_sk[i].set_ser_index(0);
		}
	}


	return true;
}

bool role_skill::remove_all_skill()
{
    _have_sk.clear();

    return true;
}

bool role_skill::upgrade_skill_effect(uint32 skill_id,uint32 upgrade_level)
{
    std::vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), skill_id);
    if (it == _have_sk.end()) {
        return false;
    }

    const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);
    if (skill_base == NULL) {
        return false;
    }

    uint32 new_level = it->level + upgrade_level;
    const config_effect* base_info = CONFMGR->get_config_effect_mgr().get_config_effect_by_key(skill_base->get_key(), new_level);
    if (base_info == NULL) {
        return false;
    }

	if (base_info->get_upgrade_effect_id() == 0){
		return false;
	}

    it->set_effect_id(base_info->get_effect_id());
    it->set_level(new_level);

    return true;
}

bool role_skill::set_skill_effect(uint32 skill_id,uint32 effect_id,uint32 level,uint32 ser_index)
{
	const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);
	if (skill_base == NULL){
		return false;
	}

	if (!is_bow_skill(skill_base->get_skills_belonging()) && ! is_gem_skill(skill_base->get_skills_belonging())){
		std::vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), skill_id);
		if (it == _have_sk.end()) {
			return false;
		}

		it->set_effect_id(effect_id);
		it->set_level(level);
	}else{
		for (size_t i = 0; i < _have_sk.size(); ++i){
			if (_have_sk[i].ser_index != 0 && _have_sk[i].ser_index == ser_index && _have_sk[i].get_skill_id() == skill_id){
				_have_sk[i].set_effect_id(effect_id);
				_have_sk[i].set_level(level);
			}
		}
	}

	return true;
}
// 坐骑技能升级同时触发效果的ID改变
bool role_skill::upgrade_mount_skill_effect(uint32 skill_id)
{
    std::vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), skill_id);
    if (it == _have_sk.end()) {
        return false;
    }

    const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);
    if (skill_base == NULL) {
        return false;
    }

    const std::vector<int>&   skill_effect_vec = skill_base->get_method_list();
    if (skill_effect_vec.size() == 0){
        return false;
    }

    it->set_effect_id(skill_effect_vec[0]);
    it->set_level(1);

    return true;
}

void role_skill::set_mounts_skill_exp(uint32 skill_id, uint32 exp)
{
    std::vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), skill_id);
    if (it != _have_sk.end()) {
        it->set_mas(exp);
    }
}

skill* role_skill::use_skill(const uint32 sk_id, uint32 attack_speed /* = 0 */)
{
    std::vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), sk_id);
    if (it == _have_sk.end()) {
        return NULL;
    }

    skill& sk = *it;
    uint32 tenms = time_utils::tenms();
    if (sk.level == 0 || !sk.cd_pass(tenms, attack_speed)) {
        const config_skill* cs = CONFMGR->get_config_skill(sk_id);
        if (cs != NULL) {
            amtsvr_log("skill:%d cd:%d, now:%d, cool:%d\n", sk_id, sk.cd, tenms, cs->get_cool_tick() / 10);
        } else {
            amtsvr_log("skill:%d cd:%d, now:%d, not found\n", sk_id, sk.cd, tenms);
        }

        return NULL;
    }

    sk.set_cd(tenms);

    return &sk;
}

skill* role_skill::get_skill(const uint32 sk_id,uint32 ser_index)
{
	const config_skill* skill_base = CONFMGR->get_config_skill(sk_id);
	if (skill_base == NULL){
		return NULL;
	}

	if (!is_bow_skill(skill_base->get_skills_belonging())&& !is_gem_skill(skill_base->get_skills_belonging())){
		std::vector<skill>::iterator it = find(_have_sk.begin(), _have_sk.end(), sk_id);
		if (it == _have_sk.end()) {
			return NULL;
		}

		skill& sk = *it;

		return &sk;
	}else{
		for (size_t i = 0; i < _have_sk.size(); ++i){
			if (_have_sk[i].get_skill_id() == sk_id && _have_sk[i].get_ser_index() == ser_index){
				skill& sk = _have_sk[i];

				return &sk;
			}
		}
	}

	return NULL;
}

skill* role_skill::get_skill()
{
    _used_index = 0;
    uint32 tenms = time_utils::tenms();
    size_t count = _have_sk.size() + _order;
    bool next = false;
    uint8 fix = 0;
    if (_have_sk.size() > 1) {
        fix = 1;
    } else {
        fix = 0;
    }

    for (uint8 i = 0; i < count; ++i) {
        uint8 index = (_order + fix + i) % _have_sk.size();
        skill* sk = &_have_sk[index];
        if (sk == NULL || sk->level == 0 || !sk->cd_pass(tenms) || sk->get_config_skill()->get_skill_type() != 1) {
            //printf("cd error %d, %d, %d\n", index, _order, sk->get_skill_id());
            continue;
        }

        if (_order == index && _have_sk.size() != 1) {
            next = true;
            //printf("order error %d, %d, %d\n", index, _order, sk->get_skill_id());
            continue;
        }

        _order = index;
        //printf("get %d, %d, %d\n", index, _order, sk->get_skill_id());
        return sk;
    }

    if (next) {
        ++_order;
        return get_skill();
    }

    return NULL;
}

void role_skill::get_skill_by_type(SKILL_TYPE st, std::vector<skill*>& vs,uint32 skill_belong)
{
    uint32 tenms = time_utils::tenms();
    for (size_t i = 0; i < _have_sk.size(); ++i) {
        const config_skill* cs = _have_sk[i].get_config_skill();
        if (cs == NULL) {
            continue;
        }

		if (skill_belong == 0){
			if (cs->get_skill_type() == (uint32)st && _have_sk[i].cd_pass(tenms)) {
				vs.push_back(&_have_sk[i]);
			}
		}else{
			if (cs->get_skill_type() == (uint32)st && _have_sk[i].cd_pass(tenms) && cs->get_skills_belonging() == skill_belong) {
				vs.push_back(&_have_sk[i]);
			}
		}
    }
}

void role_skill::get_skill_by_belong(uint32 skill_belong,std::vector<skill*>& vs)
{
	for (size_t i = 0; i < _have_sk.size(); ++i) {
		const config_skill* cs = _have_sk[i].get_config_skill();
		if (cs == NULL) {
			continue;
		}

		if (cs->get_skills_belonging() == skill_belong){
			vs.push_back(&_have_sk[i]);
		}
	}
}

void role_skill::fill_skill_data(client::role_skill_msg& rsm)
{
    uint32 count = _have_sk.size();
    for (uint32 i = 0; i < count; ++i ) {
        client::skill_data *sd = rsm.add_sk();
        sd->set_id(_have_sk[i].id);
        sd->set_cd(_have_sk[i].cd);
        sd->set_mas(_have_sk[i].mas_val);
        sd->set_serial(_have_sk[i].ser_index);
        sd->set_level(_have_sk[i].level);
        sd->set_effect_id(_have_sk[i].effect_id);
    }
}

void role_skill::fill_mounts_skill_data(client::role_skill_msg& rsm)
{
    for (uint32 index = 0; index < _have_sk.size(); ++index) {
        const config_skill* skill_info = _have_sk[index].get_config_skill();
        if (skill_info == NULL) {
            return;
        }

        if (skill_info->get_skills_belonging() != SB_MOUNTS) {
            continue;
        }

        client::skill_data *sd = rsm.add_sk();
        sd->set_id       (_have_sk[index].id       );
        sd->set_cd       (_have_sk[index].cd       );
        sd->set_mas      (_have_sk[index].mas_val  );
        sd->set_serial   (_have_sk[index].ser_index);
        sd->set_level    (_have_sk[index].level    );
        sd->set_effect_id(0                        );
    }
}

uint32 role_skill::get_gem_skill_count()
{
	uint32 gem_skill_count = 0;
	for (uint32 index = 0; index < _have_sk.size(); ++index) {
		const config_skill* skill_info = _have_sk[index].get_config_skill();
		if (skill_info == NULL) {
			continue;
		}

		if (skill_info->get_skills_belonging() == SB_GEM) {
			++gem_skill_count;
		}
	}


	return gem_skill_count;
}

uint32 role_skill::get_bow_skill_count()
{
	uint32 bow_skill_count = 0;
	for (uint32 index = 0; index < _have_sk.size(); ++index) {
		const config_skill* skill_info = _have_sk[index].get_config_skill();
		if (skill_info == NULL) {
			continue;
		}

		if (skill_info->get_skills_belonging() == SB_ARCHER) {
			++bow_skill_count;
		}
	}

	return bow_skill_count;
}

void role_skill::get_bow_skill(std::map<uint32,uint32>& bow_skill_map_)
{
	for (uint32 index = 0; index < _have_sk.size(); ++index) {
		const config_skill* skill_info = _have_sk[index].get_config_skill();
		if (skill_info == NULL) {
			return;
		}

		if (skill_info->get_skills_belonging() == SB_ARCHER) {
			bow_skill_map_.insert(std::pair<uint32,uint32>(_have_sk[index].get_ser_index(),_have_sk[index].get_skill_id()));
		}
	}
}

void role_skill::fill_bows_skill_data(client::role_skill_msg& rsm)
{
	for (uint32 index = 0; index < _have_sk.size(); ++index) {
		const config_skill* skill_info = _have_sk[index].get_config_skill();
		if (skill_info == NULL) {
			return;
		}

		if (skill_info->get_skills_belonging() != SB_ARCHER) {
			continue;
		}

		client::skill_data *sd = rsm.add_sk();
		sd->set_id       (_have_sk[index].id       );
		sd->set_cd       (_have_sk[index].cd       );
		sd->set_mas      (_have_sk[index].mas_val  );
		sd->set_serial   (_have_sk[index].ser_index);
		sd->set_level    (_have_sk[index].level    );
		sd->set_effect_id(_have_sk[index].effect_id);
	}
}

void role_skill::fill_all_gem_skill_data(client::role_skill_msg& rsm)
{
	for (uint32 index = 0; index < _have_sk.size(); ++index) {
		const config_skill* skill_info = _have_sk[index].get_config_skill();
		if (skill_info == NULL) {
			return;
		}

		if (skill_info->get_skills_belonging() != SB_GEM) {
			continue;
		}

		client::skill_data *sd = rsm.add_sk();
		sd->set_id       (_have_sk[index].id       );
		sd->set_cd       (_have_sk[index].cd       );
		sd->set_mas      (_have_sk[index].mas_val  );
		sd->set_serial   (_have_sk[index].ser_index);
		sd->set_level    (_have_sk[index].level    );
		sd->set_effect_id(_have_sk[index].effect_id);
	}

}

void role_skill::fill_pets_skill_data(client::role_skill_msg& rsm, uint32 pet_id, uint32 pet_level)
{
	const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(pet_id, pet_level);
	uint32 pet_skill_type = base_info->get_skill_belong();
	if (base_info == NULL){
		return;
	}

    for (uint32 index = 0; index < _have_sk.size(); ++index){
        const config_skill* skill_info = _have_sk[index].get_config_skill();
        if (skill_info == NULL) {
            return;
        }

        if (skill_info->get_skills_belonging() != pet_skill_type) {
            continue;
        }

        client::skill_data *sd = rsm.add_sk();
        sd->set_id       (_have_sk[index].id       );
        sd->set_cd       (_have_sk[index].cd       );
        sd->set_mas      (_have_sk[index].mas_val  );
        sd->set_serial   (_have_sk[index].ser_index);
        sd->set_level    (_have_sk[index].level    );
        sd->set_effect_id(_have_sk[index].effect_id);
    }
}

void role_skill::fill_all_pets_skill_data(client::role_skill_msg& rsm)
{
    for (uint32 index = 0; index < _have_sk.size(); ++index) {
        const config_skill* skill_info = _have_sk[index].get_config_skill();
        if (skill_info == NULL) {
            return;
        }

        if (skill_info->get_skills_belonging() < SB_BEAUTY_MIN || skill_info->get_skills_belonging() > SB_BEAUTY_MAX) {
            continue;
        }

        client::skill_data *sd = rsm.add_sk();
        sd->set_id       (_have_sk[index].id       );
        sd->set_cd       (_have_sk[index].cd       );
        sd->set_mas      (_have_sk[index].mas_val  );
        sd->set_serial   (_have_sk[index].ser_index);
        sd->set_level    (_have_sk[index].level    );
        sd->set_effect_id(_have_sk[index].effect_id);
    }
}

void role_skill::get_pet_skill(uint32 pet_id,uint32 pet_level,std::vector<skill>& pet_skill_id_vec) const
{
    pet_skill_id_vec.clear();
    for (uint32 j = 0; j < _have_sk.size(); ++j) {
        const config_skill* skill_info = _have_sk[j].get_config_skill();
        if (skill_info == NULL){
            return;
        }

        const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(pet_id, pet_level);
        if (base_info == NULL || skill_info->get_skills_belonging() != base_info->get_skill_belong()) {
            continue;
        }

        pet_skill_id_vec.push_back(_have_sk[j]);
    }
}

void role_skill::init_min_atk_range()
{
    uint32 count = _have_sk.size();
    for (uint8 i = 0; i < MAX_SKILL_NUM && i < count; ++i) {
        skill* sk = &_have_sk[i];
        if (sk == NULL) {
            continue;
        }

        uint32 tmp = sk->get_distance();
        if (_min_atk_range == 0 || (tmp > 0 && tmp < _min_atk_range)) {
            _min_atk_range = tmp;
        }
    }
}

void role_skill::init_max_anger()
{
    uint32 count = _have_sk.size();
    for (uint8 i = 0; i < MAX_SKILL_NUM && i < count; ++i) {
        skill* sk = &_have_sk[i];
        if (sk == NULL) {
            continue;
        }

        uint32 tmp = sk->get_cost_anger();
        if (tmp > _max_anger) {
            _max_anger = tmp;
        }
    }
}

role_skill& role_skill::operator=(const role_skill& rhs)
{
    _have_sk       = rhs._have_sk;
    _used_index    = rhs._used_index;
    _min_atk_range = rhs._min_atk_range;
    _max_ser_index = rhs._max_ser_index;
    _max_anger     = rhs._max_anger;

    return *this;
}

skill* role_skill::use_ai_skill(const uint32 skill_id)
{
    skill* ps = get_ai_skill(skill_id);

    return ps;
}

skill* role_skill::get_ai_skill(const uint32 skill_id)
{
    skill& s = _ai_skill[skill_id];
    if (s.get_skill_id() == 0 || s.get_skill_id() != skill_id) {
        const config_skill* pcs = CONFMGR->get_config_skill(skill_id);
        if (pcs == NULL) {
            //     printf("%s:%d not found ai skill %d\n", __FILE__, __LINE__, skill_id);
            return NULL;
        }

        if (s.init(pcs) != 0) {
            //       printf("%s:%d not found ai skill %d\n", __FILE__, __LINE__, skill_id);
            return NULL;
        }
    }

    uint32 tenms = time_utils::tenms();
    if (s.cd_pass(tenms)) {
        s.set_cd(tenms);
        //printf("%s:%d use ai skill %d\n", __FILE__, __LINE__, skill_id);
        return &s;
    }

    //printf("%s:%d ai skill %d cd limit\n", __FILE__, __LINE__, skill_id);

    return NULL;
}

bool role_skill::check_mount_skill(uint32 mount_id, const config_skill* pcs) const
{
    if (pcs != NULL && pcs->get_skills_belonging() == SB_MOUNTS && mount_id == 0) {
        return false;
    }

    return true;
}
