#include "scene_role.h"
#include "scene_role_attr.h"

// 需要广播的属性在这里
bool scene_role_attr::fill_broadcast_attr(const client::role_attr& attr, client::role_attr& broadcast_attr) const
{
    int ret = 0;
    if (attr.has_sex()) {
        broadcast_attr.set_sex(attr.sex());
        ++ret;
    }

    if (attr.has_profession()) {
        broadcast_attr.set_profession(attr.profession());
        ++ret;
    }

    if (attr.has_level()) {
        broadcast_attr.set_level(attr.level());
        ++ret;
    }

    if (attr.has_family_id()) {
        broadcast_attr.set_family_id(attr.family_id());
        ++ret;
    }

    if (attr.has_map_id()) {
        broadcast_attr.set_map_id(attr.map_id());
        ++ret;
    }

    if (attr.has_x_pos()) {
        broadcast_attr.set_x_pos(attr.x_pos());
        ++ret;
    }

    if (attr.has_y_pos()) {
        broadcast_attr.set_y_pos(attr.y_pos());
        ++ret;
    }

    if (attr.has_max_hp()) {
        broadcast_attr.set_max_hp(attr.max_hp());
        ++ret;
    }

    if (attr.has_cur_hp()) {
        broadcast_attr.set_cur_hp(attr.cur_hp());
        ++ret;
    }

    if (attr.has_max_mp()) {
        broadcast_attr.set_max_mp(attr.max_mp());
        ++ret;
    }

    if (attr.has_cur_mp()) {
        broadcast_attr.set_cur_mp(attr.cur_mp());
        ++ret;
    }

    if (attr.has_move_speed()) {
        broadcast_attr.set_move_speed(attr.move_speed());
        ++ret;
    }

    if (attr.has_armor_id()) {
        broadcast_attr.set_armor_id(attr.armor_id());
        ++ret;
    }

    if (attr.has_weapon_id()) {
        broadcast_attr.set_weapon_id(attr.weapon_id());
        ++ret;
    }

    if (attr.has_mounts_id()) {
        broadcast_attr.set_mounts_id(attr.mounts_id());
        ++ret;
    }

    if (attr.has_fighting_power()) {
        broadcast_attr.set_fighting_power(attr.fighting_power());
        ++ret;
    }

	if (attr.has_team_id()) {
		broadcast_attr.set_team_id(attr.team_id());
		++ret;
	}

	if (attr.has_camp_id()) {
		broadcast_attr.set_camp_id(attr.camp_id());
		++ret;
	}

	if(attr.has_vip_level()) {
		broadcast_attr.set_vip_level(attr.vip_level());
		++ret;
	}
    return ret > 0;
}

scene_role_attr::scene_role_attr() 
    : _ref(GetReflection())
    , _des(GetDescriptor()) 
{
    modify_attr = NULL;
    change_attr = false;
    _pr  = NULL;
}

scene_role_attr::scene_role_attr(const scene_role_attr& rhs) 
    : client::role_attr(rhs) 
{
    _ref = rhs._ref;
    _des = rhs._des;
    modify_attr = rhs.modify_attr;
    change_attr = false;
    _pr  = NULL;
}

scene_role_attr::~scene_role_attr()
{

}

int scene_role_attr::init(scene_role* pr)
{
    _pr = pr;

    return 0;
}

scene_role_attr& scene_role_attr::operator=(const scene_role_attr& rhs)
{
    client::role_attr::operator=(rhs);
    return *this;
}

void scene_role_attr::clear_modify_attr()
{
    if (modify_attr != NULL) {
        modify_attr->Clear();
    }

    change_attr = false;
}

bool scene_role_attr::fill_modify_attr(client::role_attr& attr, client::role_attr& broadcast_attr, bool& need_broadcast)
{
    if (!change_attr || modify_attr == NULL) {
        return false;
    }

    attr = *modify_attr;
    modify_attr->Clear();
    need_broadcast = fill_broadcast_attr(attr, broadcast_attr);
    change_attr = false;

    return true;
}

void scene_role_attr::fill_all_attr(client::role_attr& attr)
{
    attr.CopyFrom(*this);
    if (modify_attr != NULL) {
        modify_attr->Clear();
        change_attr = false;
    }
}

uint32 scene_role_attr::get_val(ROLE_ATTRIBUTE index) const
{
    return _ref->GetUInt32(*this, _des->field(index));
}

uint64 scene_role_attr::get_val_64(ROLE_ATTRIBUTE index) const
{
    return _ref->GetUInt64(*this, _des->field(index));
}

uint64 scene_role_attr::get_val_(ROLE_ATTRIBUTE index) const
{
    switch (_des->field(index)->type()) {
    case google::protobuf::FieldDescriptor::TYPE_UINT32: return get_val(index);
    case google::protobuf::FieldDescriptor::TYPE_UINT64: return get_val_64(index);
    default: break; // 目前属性类型只允许存在32位和64位无符号整型
    }

    return 0;
}

void scene_role_attr::set_val(ROLE_ATTRIBUTE index, uint32 val)
{
    if (index == EXPERIENCE) {
        return set_val_64(index, val);
    }

    if (!change_attr) {
        change_attr = true;
    }

    uint64 old_val = get_val_(index);

    _ref->SetUInt32(this, _des->field(index), val);
    if (modify_attr != NULL) {
        modify_attr->set_val(index, val);
    }

    if (_pr != NULL) {
        _pr->on_attr_change(index, old_val, val);
    }
}

void scene_role_attr::set_val_64(ROLE_ATTRIBUTE index, uint64 val)
{
    if (!change_attr) {
        change_attr = true;
    }

    uint64 old_val = get_val_(index);

    _ref->SetUInt64(this, _des->field(index), val);
    if (modify_attr != NULL) {
        modify_attr->set_val_64(index, val);
    }

    if (_pr != NULL) {
        _pr->on_attr_change(index, old_val, val);
    }
}

void scene_role_attr::set_val_(ROLE_ATTRIBUTE index, uint64 val)
{
    switch (_des->field(index)->type()) {
    case google::protobuf::FieldDescriptor::TYPE_UINT32: return set_val(index, (uint32)val);
    case google::protobuf::FieldDescriptor::TYPE_UINT64: return set_val_64(index, val);
    default: break; // 目前属性类型只允许存在32位和64位无符号整型
    }
}

void scene_role_attr::add_val(ROLE_ATTRIBUTE index, uint32 val)
{
    if (val == 0) {
        return;
    }

    if (!change_attr) {
        change_attr = true;
    }


    if (index == EXPERIENCE) {
        return add_val64(index, val);
    }

    uint32 old = get_val(index);
    uint32 v = old + val;


    if (index == CUR_HP && get_val(MAX_HP) <= v) {
        v = max_hp();
    } else if (index == CUR_MP && get_val(MAX_MP) <= v) {
        v = max_mp();
    }

    set_val(index, v);
    if (modify_attr != NULL) {
        modify_attr->set_val(index, v);
    }
}

void scene_role_attr::add_val64(ROLE_ATTRIBUTE index, uint64 val)
{
    if (val == 0) {
        return;
    }

    if (!change_attr) {
        change_attr = true;
    }

    uint64 old = get_val_64(index);
    uint64 v = old + val;

    set_val_64(index, v);
    if (modify_attr != NULL) {
        modify_attr->set_val_64(index, v);
    }
}

void scene_role_attr::sub_val(ROLE_ATTRIBUTE index, uint32 val)
{
    if (val == 0) {
        return;
    }

    if (!change_attr) {
        change_attr = true;
    }

    if (index == EXPERIENCE) {
        return sub_val64(EXPERIENCE, val);
    }

    uint32 old = get_val(index);
    uint32 v   = (old > val ? old - val : 0); 

    set_val(index, v);
    if (modify_attr != NULL) {
        modify_attr->set_val(index, v);
    }
}

void scene_role_attr::sub_val64(ROLE_ATTRIBUTE index, uint64 val)
{
    if (val == 0) {
        return;
    }

    if (!change_attr) {
        change_attr = true;
    }

    uint64 old = get_val_64(index);
    uint64 v   = (old > val ? old - val : 0); 

    set_val_64(index, v);
    if (modify_attr != NULL) {
        modify_attr->set_val_64(index, v);
    }
}

void scene_role_attr::multi_val(ROLE_ATTRIBUTE index, float val)
{
    if (!change_attr) {
        change_attr = true;
    }

    float old = (float)get_val(index);
    float v = old * val;


    set_val(index, (uint32)v);
    if (modify_attr != NULL) {
        modify_attr->set_val(index, (uint32)v);
    }
}

void scene_role_attr::divide_val(ROLE_ATTRIBUTE index, uint32 val)
{
    if (val == 0) {
        return;
    }

    if (!change_attr) {
        change_attr = true;
    }

    uint32 old = _ref->GetUInt32(*this, _des->field(index-1));
    uint32 v = old / val;

    _ref->SetUInt32(this, _des->field(index-1), v);
    if (modify_attr != NULL) {
        modify_attr->set_val(index, v);
    }

    if (_pr != NULL) {
        _pr->on_attr_change(index, old, v);
    }
}
