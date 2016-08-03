#include "config_npc.h"
#include "origin_npc.h"

origin_npc::origin_npc(const origin_npc& rhs) 
{
    operator=(rhs);
}

origin_npc::origin_npc(const config_npc* p) 
    : _cn(p)
{
    if (get_config_npc() == NULL) {
        return ;
    }

    set_role_type(RTE_NPC);

    _ai.init(get_config_npc());
    set_role_id(get_config_npc()->get_npc_id());

#ifdef _WIN32
    char* str = U2G(get_config_npc()->get_npc_name().c_str());
    if (strlen(str) >= MAX_NAME_LEN) {
        snprintf(_role_name, sizeof(_role_name), "%d", get_config_npc()->get_npc_id());
    } else {
        snprintf(_role_name, sizeof(_role_name), "%s", str);
    }
    delete str;
#else
    snprintf(_role_name, sizeof(_role_name), "%s", p->get_npc_name().c_str());
#endif // _WIN32

    set_feature(get_config_npc()->get_npc_type());
    _chase_range   = get_config_npc()->get_follow_range();
    _visible_range = get_config_npc()->get_visible_range();
    _attk_type     = get_config_npc()->get_attack_mode();
    _revi_time     = get_config_npc()->get_rebirth_time();
    _revi_code     = get_config_npc()->get_rebirth_time() == 0 ? NRTE_NEVER : NRTE_INTERVAL;
    _attack_cool   = get_config_npc()->get_attack_cool();

    init_attr(_attr);

    _rand_move       = get_config_npc()->get_random_move_range() > 0;
    _rand_move_space = get_config_npc()->get_move_cool();
    _monster_type    = (MONSTER_TYPE)get_config_npc()->get_npc_type();
    _disappear       = get_config_npc()->get_monster_disappear() / 10;
}

origin_npc::~origin_npc()
{

}

origin_npc& origin_npc::operator=(const origin_npc& rhs)
{
    role::operator=(rhs);

    _cn            = rhs._cn;
    _ai            = rhs._ai;

    _chase_range     = rhs._chase_range;
    _visible_range   = rhs._visible_range;
    _attk_type       = rhs._attk_type;
    _attack_cool     = rhs._attack_cool;
    _revi_code       = rhs._revi_code;
    _revi_time       = rhs._revi_time;
    _rand_move       = rhs._rand_move;
    _rand_move_space = rhs._rand_move_space;
    _monster_type    = rhs._monster_type;
    _disappear       = rhs._disappear;

    return *this;
}

int origin_npc::init()
{
    if (init_skill() != 0) {
        return -1;
    }

    return 0;
}

int origin_npc::init_attr(scene_role_attr& a) const
{
    init_original_attr(a);
    a.set_sex       (0                                    );
    a.set_profession(0                                    );
    a.set_experience(get_config_npc()->get_experience()   );
    a.set_level     (get_config_npc()->get_monster_level());
    a.set_cur_hp    (a.max_hp());
    a.set_cur_mp    (a.max_mp());

    return 0;
}

int origin_npc::init_skill()
{
    if (get_config_npc() != NULL) {
        for (size_t i = 0; i < get_config_npc()->get_skills().size(); ++i) {
            const config_skill* pcs = CONFMGR->get_config_skill(get_config_npc()->get_skills().at(i));
            if (pcs != NULL) {
                mutable_skill()->add_skill(skill(*pcs));
            }
        }
    }

    mutable_skill()->init_min_atk_range();
    mutable_skill()->init_max_anger();

    return 0;
}

uint32 origin_npc::get_attack_range(skill* s) const
{
    uint32 r = (s == NULL ? 1 : role::get_attack_range(s));
    if (s != NULL) {
        return r;
    }

    return (r > _visible_range ? r : _visible_range);
}

void origin_npc::init_original_attr(client::role_attr& a) const
{
    if (get_config_npc() == NULL) {
        return ;
    }

    const google::protobuf::Reflection* ref = a.GetReflection();
    const google::protobuf::Descriptor* des = a.GetDescriptor();
    if (ref == NULL || des == NULL) {
        return ;
    }

    const std::map<int, int>& ga = get_config_npc()->get_game_attribute();
    for (std::map<int, int>::const_iterator i = ga.begin(); i != ga.end(); ++i) {
        ROLE_ATTRIBUTE index = ATTR_CFG_TO_CODE[i->first];
        if (des->field_count() <= index || index < MAX_HP || index > MOVE_SPEED) {
            continue;
        }

        switch (des->field(index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            ref->SetUInt32(&a, des->field(index), i->second);
            break;
                                                             }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            ref->SetUInt64(&a, des->field(index), i->second);
            break;
                                                             }
        default: break;
        }
    }
}

bool origin_npc::can_create(uint32 now) const
{
    return true;
}

bool origin_npc::can_relive() const
{
    return (_revi_code == NRTE_DEFAULT || _revi_code == NRTE_ONTIME || _revi_code == NRTE_INTERVAL);
}

bool origin_npc::is_active_attack() const
{
    if (get_attk_type() != NATE_NULL 
        && (get_feature() == NPC_TYPE_GUARD 
        ||  get_feature() == NPC_TYPE_MONSTER 
        ||  get_feature() == NPC_TYPE_ELITE
        ||  get_feature() == NPC_TYPE_BOSS
        ||  get_feature() == NPC_TYPE_PICK)) {
            return true;
    }

    return false;
}

bool origin_npc::is_monster() const
{
    return get_monster_type() < MT_NPC || (get_monster_type() > MT_NPC && get_monster_type() != MT_TASK_NPC);
}

bool origin_npc::is_boss() const
{
    return get_feature() == NPC_TYPE_WILDBOSS || get_feature() == NPC_TYPE_BOSS || get_feature() == NPC_TYPE_ELITE; 
}

bool origin_npc::is_world_boss() const
{
	return get_feature() == NPC_TYPE_BOSS;
//    return get_feature() == NPC_TYPE_WILDBOSS;
}

bool origin_npc::is_picker() const
{
    return get_feature() == NPC_TYPE_PICK;
}

bool origin_npc::is_initiative_attack() const
{
    return _attk_type == NATE_INITIATIVE;
}
