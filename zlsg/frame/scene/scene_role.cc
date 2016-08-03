#include <math.h>
#include "config_map.h"
#include "config_npc.h"
#include "config_effect.h"
#include "game_map_mgr.h"
#include "npc.h"
#include "halo.h"
#include "scene_pet.h"
#include "scene_inst.h"
#include "scene_role.h"
#include "client.role_bows.pb.h"

#define PI 3.1415926535

scene_role::scene_role(const uint64 id) : _inst_id(id)
{
    _move_node.resize(MAX_MOVE_NODE);
    _in_scene = NULL;
    reset_scene_role();
    _status.set_modify_status(&_modify_status);
    _camp_id = 0;
    _team_id = 0;
    _master  = NULL;
    _region  = 0;
    is_change_attr = false;
    set_force_speed(0);
    mutable_attr()->init(this);
    mutable_status()->init(NULL, this);
    _halos = NULL;
	_has_dead = false;
	_disappear = false;
}

scene_role::scene_role(const scene_role& rhs) :role(rhs), _inst_id(rhs._inst_id)
{
    _in_scene    = rhs._in_scene;
    _create_time = rhs._create_time;
    _dead_time   = rhs._dead_time;
    _fight_time  = rhs._fight_time;

    _move_count = rhs._move_count;
    _move_node  = rhs._move_node;

    _scene_pos  = rhs._scene_pos;
    _region     = rhs._region;

    _last_move_tick    = rhs._last_move_tick;
    _last_update_tick  = rhs._last_update_tick;
    _last_attack_tick  = rhs._last_attack_tick;
    _last_recover_tick = rhs._last_recover_tick;
	//_last_skill_point_tick = rhs._last_skill_point_tick;
    _100ms_tick        = rhs._100ms_tick;
    _status        = rhs._status;
    _modify_status = rhs._modify_status;
    _camp_id       = rhs._camp_id;
    _team_id       = rhs._team_id;
    _facing        = rhs._facing;
    _mt            = rhs._mt;
    //_pets          = rhs._pets;
    _master        = rhs._master;
    set_force_speed(rhs.get_force_speed());
    is_change_attr = false;
}

scene_role::~scene_role()
{
    if (get_role_type() == RTE_NPC && get_master() != NULL) {
        get_master()->remove_pet(this);
    }

    if (get_scene() != NULL) {
        get_scene()->leave_scene(this);
    }

    std::map<uint64, scene_role*>::iterator i = _pets.begin();
    for ( ; i != _pets.end(); ++i) {
        if (i->second->get_role_type() != RTE_PET || i->second == this) {
            continue;
        }

        delete i->second;
    }

    _pets.clear();

    halo_mgr_release();
}

void scene_role::reset_scene_role()
{
    _create_time = time_utils::tenms();
    _dead_time   = 0;
    _fight_time  = 0;

    _move_count  = 0;
    _force_speed = 0;

    _scene_pos  = get_born_pos();

    _last_move_tick    = 0;
    _last_update_tick  = 0;
    _last_attack_tick  = 0;
    _last_recover_tick = 0;
    _100ms_tick        = 0;
    _last_mount_tick   = 0;

    _attack_count  = 0;
    _defense_count = 0;
	_bow_attack_count = 0;

    _status.reset();

    _camp_id = 0;
    _team_id = 0;
	_has_dead = false;
    _facing = NORTH;
    _mt     = MT_NORMAL;
}

int scene_role::attach_scene(scene_inst* s)
{
    if (s == NULL) {
        return -1;
    }

    _map_id   = s->get_map_id();
    _scene_id = s->get_scene_id();
    _in_scene = s;

    clean_hate_map();

    mutable_attr()->set_val(MAP_ID, _map_id);

    get_scene()->on_role_enter(this);

    return 0;
}

int scene_role::detach_scene()
{
    _map_id     = 0;
    _scene_id   = 0;
    _in_scene   = NULL;
    _move_count = 0;

    clean_hate_map();

    return 0;
}

void scene_role::change_cur_pos(const map_postion& to)
{
    map_postion from = _scene_pos;
    _scene_pos = to;
    if (get_scene() != NULL) {
        get_scene()->on_role_move(this, from, _scene_pos);
        _facing = role_facing(from, _scene_pos);

        if (has_halo()) {
            halo_mgr_instance()->move_update(time_utils::tenms());
        }
    }

    for (std::map<uint64, scene_role*>::iterator i = _pets.begin(); i != _pets.end(); ++i) {
		if(i->second == NULL || i->second->get_status().is_dead()) 
		{
			continue;
		}
        i->second->set_born_pos(to);
        i->second->follow(to);
    }

    if (!is_keep_move()) {
        on_move_end();
    }
}

void scene_role::set_scene_pos(const map_postion& p)
{
    if (_move_node.empty()) {
        _move_node.resize(1);
    }

    _move_node[0] = p;
    _move_count   = 0;
    _scene_pos    = p;
}

void scene_role::add_pet(scene_role* sr)
{
    if (sr == NULL || sr == this) {
        return ;
    }

    _pets[sr->get_inst_id()] = sr;
    sr->set_master(this);
}

void scene_role::remove_pet(scene_role* sr)
{
    if (sr == NULL) {
        return ;
    }

    _pets.erase(sr->get_inst_id());
}

void scene_role::delete_pet(scene_role* sr /* = NULL */)
{
    if (sr == NULL) {
        std::map<uint64, scene_role*>::iterator i = _pets.begin();
        while (i != _pets.end()) {
            if (i->second->get_scene() != NULL) {
                i->second->get_scene()->leave_scene(i->second);
            }

            delete i->second;
            ++i;
        }

        _pets.clear();

        return ;
    }

    if (sr->get_scene() != NULL) {
        sr->get_scene()->leave_scene(sr);
    }

    _pets.erase(sr->get_inst_id());
    delete sr;
}

ROLE_TYPE_E scene_role::get_real_role_type()
{
    if (get_master() != NULL) {
        return get_master()->get_real_role_type();
    }

    return (ROLE_TYPE_E)get_role_type();
}

bool scene_role::same_master(scene_role* psr)
{
    if (psr == NULL) {
        return false;
    }

    scene_role* master = psr;
    scene_role* this_master = this;
    do {
        do {
            if (master == this_master) {
                return true;
            }
            this_master = this_master->get_master();
        } while (this_master != NULL);
        master      = master->get_master();
        this_master = this;
    } while (master != NULL);

    return  false;
}

bool scene_role::move_to_pos(const map_postion& p, MOVE_TYPE mt/* = MT_NORMAL*/)
{
    map_postion pos(p);
    get_scene()->fix_pos(pos);
    _mt            = mt;
    if (get_move_count() > 1 && !_move_node.empty() && _move_node[0] == pos) {
        return false;
    }

    if (_move_node.size() < 2) {
        _move_node.resize(2);
    }

    _move_node[0]   = pos;
    _move_node[1]   = get_scene_pos();
    set_move_count(2);
    if (get_last_move_tick() == 0) {
        set_last_move_tick(time_utils::tenms());
    }

    return true;
}

/*
  7 0 1
  6   2
  5 4 3
*/
void scene_role::set_facing(float x, float y)
{
    _facing = role_facing(map_postion(0, 0, 0), map_postion((int)(x * 10), (int)(y * 10), 0));
}

ROLE_FACING scene_role::role_facing(const map_postion& start, const map_postion& end)
{
    double x = end.x - start.x;
    double y = end.y - start.y;
    if (x == 0) {
        if (y > 0) {
            return SOUTH;
        } else {
            return NORTH;
        }
    }

    if (y == 0) {
        if (x > 0) {
            return EAST;
        } else {
            return WEST;
        }
    }

    double radian = atan(y / x);
    if ((x < 0 && y < 0) || (x < 0 && y > 0)) {
        radian += PI;
    }

    radian = radian / PI * 180;
    /*if (get_role_type() == RTE_USER) {
      printf("start %d, %d, end %d, %d, radian = %f \n", start.x, start.y, end.x, end.y, radian);
      }*/

    // 根据客户端的地图坐标系识别方向
    if (radian >= 0.0 && radian < 30.0) {
        return EAST;
    } else if (radian >= 30.0 && radian < 60.0) {
        return SOUTHEAST; //NORTHEAST;
    } else if (radian >= 60.0 && radian < 120.0) {
        return SOUTH;     //NORTH;
    } else if (radian >= 120.0 && radian < 150.0) {
        return SOUTHWEST; //NORTHWEST;
    } else if (radian >= 150.0 && radian < 210.0) {
        return WEST;
    } else if (radian >= 210.0 && radian < 240.0) {
        return NORTHWEST; //SOUTHWEST;
    } else if (radian >= 240.0 && radian < 270.0) {
        return NORTH;     //SOUTH;
    } else if (radian >= 0.0 && radian <= -30.0) {
        return EAST;
    } else if (radian >= -60.0 && radian < -30.0) {
        return NORTHEAST; //SOUTHEAST;
    } else if (radian >= -90.0 && radian < -60.0) {
        return NORTH;     //SOUTH;
    }

    return NORTH;
}

void scene_role::fill_role_data(client::role_data& rd) const
{
    rd.set_role_id(get_role_id());
    rd.set_inst_id(get_inst_id());
    rd.set_role_typ((client::role_data_TYPE)get_role_type());
}

void scene_role::fill_role_data(client::role_data* rd) const
{
    if (rd != NULL) {
        fill_role_data(*rd);
    }
}

bool scene_role::fill_modify_status(client::role_status& rs)
{
    return mutable_status()->pop_modify_status(rs);
}

bool scene_role::fill_buffer(client::buffer_list& bl)
{
    if (mutable_buffer()->fill_all_buffer_list(bl) > 0) {
        fill_role_data(bl.mutable_rd());
        return true;
    }

    return false;
}

void scene_role::fill_client_pos(client::client_map_pos& cmp)
{
    cmp.set_x(_scene_pos.x);
    cmp.set_y(_scene_pos.y);
    cmp.set_z(_scene_pos.z);
}

void scene_role::fill_role_status(client::role_status& rs)
{
    mutable_status()->pop_all_status(rs);
}

bool scene_role::can_attack()
{
    if (get_scene() == NULL || get_status().silent() || get_status().faint() || get_status().is_dead()) {
        return false;
    }

    if (get_skill().get_all_skill().empty()) {
        return false;
    }

    if (get_force_speed() != 0) {
        return false;
    }

    return true;
}

bool scene_role::can_practice()
{
    if (get_scene() == NULL || get_status().silent() || get_status().faint() || get_status().is_dead()) {
        return false;
    }

    return true;
}

bool scene_role::can_attack(const scene_role* dsrole)
{
    if (!can_attack()) {
        return false;
    }

    return true;
}

bool scene_role::can_move()
{
    if (get_force_speed() != 0) {
        return true;
    }

    if (get_status().giddy() || get_status().faint() || get_status().is_dead() || get_attr().move_speed() == 0) {
        return false;
    }

    return true;
}

bool scene_role::can_use_skill(const skill& sk)
{
    return true;
}

bool scene_role::is_in_chase_range(const map_postion& p, uint32 range)
{
    return is_in_range(get_born_pos(), p, range);
}

bool scene_role::is_in_chase_range_cell(const cell_pos& c, uint32 range)
{
    if (get_scene() == NULL) {
        return false;
    }

    cell_pos cp = get_scene()->to_cell_pos(get_born_pos());
    return  cp.get_distance(c) <= range;
}

bool scene_role::is_in_range(const map_postion& p, uint32 range)
{
    return is_in_range(get_scene_pos(), p, range);
}

bool scene_role::is_in_range(const map_postion& src, const map_postion& dst, uint32 range)
{
    if (!src.is_in_same_flat(dst)) {
        return false;
    }

    return  (src.get_distance(dst) <= range);
}

bool scene_role::is_in_range_cell(const cell_pos* c, uint32 range)
{
    if (get_scene() == NULL || c == NULL) {
        return false;
    }

    cell_pos own;
    if (!get_scene()->get_cell().get_cell_by_role(get_inst_id(), own)) {
        own = get_scene()->to_cell_pos(get_scene_pos());
        get_scene()->get_cell().enter_cell(this, own);
    }

    return c->get_distance(own) <= range;
}

void scene_role::fill_role_pos_msg(client::role_pos_msg& rpm)
{
    if (get_scene() == NULL) {
        return ;
    }

    rpm.set_map_id(get_scene()->get_map_id());
    rpm.set_scene_id(get_scene()->get_scene_id());
    rpm.set_inst_id(get_scene()->get_inst_id());
    //rpm.set_type(1);
    fill_role_data(*(rpm.add_rds()));
    fill_client_pos(*(rpm.add_mps()));
}

void scene_role::fill_role_move_msg(client::role_move_msg& rmm)
{
    rmm.set_move_type(client::role_move_msg_MOVE_TYPE_MOVE_CONFIRM);
    client::move_info* mi = rmm.add_mi();
    fill_role_data(mi->mutable_rds());
    if (_move_count > 1 && get_role_type() == RTE_USER && !_move_node.empty()) {
        for (uint8 i = 0; i < _move_count && i < _move_node.size(); ++i) {
            client::client_map_pos* cmp = mi->add_mps();
            cmp->set_x(_move_node[i].x);
            cmp->set_y(_move_node[i].y);
            cmp->set_z(_move_node[i].z);
            cmp->set_t(_move_node[i].t);
        }
    } else {
        client::client_map_pos* cmp = mi->add_mps();
        cmp->set_x(_scene_pos.x);
        cmp->set_y(_scene_pos.y);
        cmp->set_z(_scene_pos.z);
        cmp->set_t(0);
    }
}

void scene_role::fill_role_skill_msg(client::role_skill_msg& rsm)
{
    fill_role_data(rsm.mutable_rd());
    _rsk.fill_skill_data(rsm);
}

void scene_role::fill_mounts_skill_msg(client::role_skill_msg& rsm)
{
    fill_role_data(rsm.mutable_rd());
    _rsk.fill_mounts_skill_data(rsm);
}

uint32 scene_role::get_gem_skill_count()
{
	return _rsk.get_gem_skill_count();
}

uint32 scene_role::get_bow_skill_count()
{
	return _rsk.get_bow_skill_count();
}

void scene_role::get_bow_skill(std::map<uint32,uint32>& bow_skill_map_)
{
	_rsk.get_bow_skill(bow_skill_map_);
}

void scene_role::fill_bows_skill_msg(client::role_skill_msg& rsm)
{
	fill_role_data(rsm.mutable_rd());
	_rsk.fill_bows_skill_data(rsm);
}

void scene_role::fill_gems_skill_msg(client::role_skill_msg& rsm)
{
	fill_role_data(rsm.mutable_rd());
	_rsk.fill_all_gem_skill_data(rsm);
}

void scene_role::fill_pets_skill_msg(client::role_skill_msg& rsm,uint32 pet_id,uint32 pet_level)
{
    fill_role_data(rsm.mutable_rd());
    _rsk.fill_pets_skill_data(rsm,pet_id,pet_level);
}

void scene_role::fill_all_pets_skill_msg(client::role_skill_msg& rsm)
{
	fill_role_data(rsm.mutable_rd());
	_rsk.fill_all_pets_skill_data(rsm);
}

void scene_role::fill_role_name_msg(client::role_name_msg& rnm)
{
    client::role_name* rn = rnm.add_rn();
    fill_role_name(*rn);
}

void scene_role::fill_role_name(client::role_name& rn)
{
    fill_role_data(rn.mutable_rd());
	rn.set_name(get_role_name());
//    rn.set_name(_role_name);
}

void scene_role::fill_role_broadcast_msg(client::role_msg& rm, bool born /* = false */)
{
    mutable_attr()->set_x_pos(get_scene_pos().x);
    mutable_attr()->set_y_pos(get_scene_pos().y);
    fill_role_data(rm.mutable_rd());
    get_attr().fill_broadcast_attr(get_attr(), *(rm.mutable_attr()));
    fill_role_status(*(rm.mutable_status()));
    if (born) {
        rm.set_born(born);
    }
}

void scene_role::fill_role_attr(client::role_attr& ra)
{
    mutable_attr()->set_x_pos(get_scene_pos().x);
    mutable_attr()->set_y_pos(get_scene_pos().y);
    mutable_attr()->fill_all_attr(ra);
}

void scene_role::fill_role_attr(client::role_attr* ra)
{
    if (ra != NULL) {
        fill_role_attr(*ra);
    }
}

bool scene_role::fill_role_modify_msg(client::role_msg& rm, client::role_attr& broadcast_attr, bool& need_broadcast)
{
    bool ret = false;
    fill_role_data(*(rm.mutable_rd()));
    ret = mutable_attr()->fill_modify_attr(*(rm.mutable_attr()), broadcast_attr, need_broadcast);
    if (fill_modify_status(*(rm.mutable_status()))) {
        if (!ret) {
            ret = true;
        }

        if (!need_broadcast) {
            need_broadcast = true;
        }
    }

    return ret;
}

void scene_role::fill_role_remove_msg(client::role_remove_msg& rrm)
{
    fill_role_data(rrm.add_rd());
}

bool scene_role::get_next_move(uint32 tick, move_map_postion& mmp)
{
    if (!can_move() || _move_count <= 1 || _move_count > _move_node.size()) {
        return false;
    }

    move_map_postion&  cur = _move_node[_move_count - 1];
    move_map_postion& next = _move_node[_move_count - 2];
    if (cur == next) {
        --_move_count;
        mmp = cur;
        return true;
    }

    if (get_last_move_tick() == 0 || get_last_move_tick() > tick) {
        set_last_move_tick(tick);
        mmp = cur;
        return true;
    }

    float realSpeed = (float)(get_force_speed() == 0 ? get_speed() : get_force_speed());
#ifdef __LIB_MINI_SERVER__
    realSpeed *= ((tick - get_last_move_tick()) / 200.0f);
    //realSpeed *= ((tick - get_last_move_tick()) / 250.0f);
#else
    realSpeed *= ((tick - get_last_move_tick()) / 80.0f);
#endif  // __LIB_MINI_SERVER__
    if (realSpeed < 15.0f) {
        mmp = cur;
        return true;
     }

    float xoff = (float)(next.x - cur.x);
    float yoff = (float)(next.y - cur.y);
    float l = sqrtf(xoff * xoff + yoff * yoff);
    float fixSpeed = next.t != 0 ? realSpeed / 1.5f : realSpeed;
    float x = cur.x + xoff / l * fixSpeed;
    float y = cur.y + yoff / l * fixSpeed;
    cur.x = (int)ceil(x);
    cur.y = (int)ceil(y);

    if (xoff < 0) {
        if (cur.x < next.x) {
            cur.x = next.x;
        }
    } else {
        if (cur.x > next.x) {
            cur.x = next.x;
        }
    }

    if (yoff < 0) {
        if (cur.y < next.y) {
            cur.y = next.y;
        }
    } else {
        if (cur.y > next.y) {
            cur.y = next.y;
        }
    }

    if (cur == next) {
        --_move_count;
    }

    mmp = cur;
    return true;
}

bool scene_role::move_by_path(uint32 tick, bool notice /* = true */)
{
    move_map_postion mmp;
    if (!get_next_move(tick, mmp)) {
        return false;
    }

    cell_mgr& c  = get_scene()->get_cell();
    cell_pos end = get_scene()->to_cell_pos(mmp);
    cell_pos cp  = end;
    cell_pos now = get_scene()->to_cell_pos(get_scene_pos());

//     if (end == now) {
//         return true;
//     }

    bool block = false;
    do {
        cp = c.step(now, end);
        if (c.is_block(cp)) {
            block = true;
            break;
        }
        now = cp;
    } while (cp != end);

    if (now != get_scene()->to_cell_pos(get_scene_pos())) {
        mmp = get_scene()->to_map_postion(now);
        //if (get_role_type() != RTE_USER) {
        //    printf("src(%d, %d) dst(%d, %d) last(%d), now(%d)\n", get_scene_pos().x, get_scene_pos().y, mmp.x, mmp.y, get_last_move_tick(), tick);
        //}

        set_last_move_tick(tick);
        change_cur_pos(mmp);

        if (this->get_role_type() != RTE_USER && notice && !get_status().giddy()) {
            get_scene()->screen_broadcast_move_pos(*this);
        }
    }

    if (block) {
        stand();
        on_move_stop(tick);
        return false;
    }

    return true;
}

bool scene_role::move_by_cell(uint32 tick)
{
    move_map_postion mmp;
    if (!get_next_move(tick, mmp)) {
        //printf("%s:%d, get_next_move false tick = %d\n", __FILE__, __LINE__, tick);
        return false;
    }

    cell_pos  old = get_scene()->to_cell_pos(get_scene_pos());
    cell_pos  cp  = get_scene()->to_cell_pos(mmp);
    cell_mgr& c   = get_scene()->get_cell();

    if (old != cp) {
        if (!c.is_empty(cp)) {
            on_move_stop(tick);
            //printf("%s:%d, cp(%d, %d) is not empty tick = %d\n", __FILE__, __LINE__, cp._x, cp._y, tick);
            return false;
        }

        set_last_move_tick(tick);
        mmp = get_scene()->to_map_postion(cp);
        change_cur_pos(mmp);

        //client::role_pos_msg rpm;
        //this->fill_role_pos_msg(rpm);
        //get_scene()->screen_broadcast(*this, rpm, false);

        get_scene()->screen_broadcast_move_pos(*this);
    } else {
        //printf("%s:%d, cp(%d, %d) is not change tick = %d\n", __FILE__, __LINE__, cp._x, cp._y, tick);
    }

    return true;
}

bool scene_role::move_ignore_block(uint32 tick)
{
    move_map_postion mmp;
    if (!get_next_move(tick, mmp)) {
        return true;
    }

    if (get_scene_pos() == mmp) {
        return true;
    }

    set_last_move_tick(tick);
    change_cur_pos(mmp);

    return true;
}

bool scene_role::flash_move(const map_postion& mp)
{
    if (!can_move() || get_scene() == NULL) {
        return false;
    }

    get_scene()->trans_postion(this, mp);

    return true;
}

bool scene_role::swap_postion(scene_role* sr)
{
    if (get_scene() == NULL) {
        return false;
    }

    map_postion old = get_scene_pos();
    get_scene()->trans_postion(this, sr->get_scene_pos());
    get_scene()->trans_postion(sr, old);

    return true;
}

void scene_role::stand(bool broadcast /* = true */ )
{
    if (_move_node.empty()) {
        _move_node.resize(1);
    }

    _move_node[0] = _scene_pos;
    _move_count   = 1;
}

int scene_role::on_attacked(scene_role* psr, skill& sk, effect_result& atk_rsp, bool& trigger)
{
    add_defense_count();
    use_passive_skill();

    if (get_scene() != NULL)  {
        get_scene()->on_attack_role(psr, this, sk, atk_rsp);
    }

    return 0;
}

int scene_role::on_killed(scene_role* psr)
{
	set_role_status(true);
    if (get_scene() == NULL) {
        return 0;
    }

    get_scene()->get_cell().leave_cell(this);

    uint32 tick = time_utils::tenms();
    on_dead(tick);

    return 0;
}

int scene_role::on_kill(scene_role* psr)
{
    if (get_scene() != NULL) {
        get_scene()->on_kill_role(this, psr);
    }

    return 0;
}

void scene_role::remove_protect_status()
{
}

void scene_role::on_dead(uint32 tick)
{
    clean_hate_map();              // 重置仇恨列表
    set_dead_time(tick);
    mutable_status()->set_dead();
    mutable_buffer()->on_dead();
    screen_broadcast_change_buf();
    calculate_attr();
    send_client_change_attr();
}

void scene_role::use_skill(skill* ps)
{
    if (get_scene() == NULL) {
        return ;
    }

    std::vector<scene_role*> enemies;
    std::vector<map_postion> miss;
    std::vector<scene_role*> friends;
    get_scene()->select_target(this, ps, enemies, friends, miss);

    effect_result rsp;
    fill_role_data(rsp.mutable_src_rd());
    rsp.set_id(get_attack_count());
    rsp.set_skill_id(ps->get_skill_id());
    attack_damage(ps, enemies, rsp);
    attack_effect(ps, enemies, friends, rsp);
    fill_miss(ps, miss, rsp);
    if (rsp.fd_size() != 0) {
        float x, y;
        get_scene()->get_normalize_d(get_facing(), x, y);
        rsp.mutable_ac()->set_x(x);
        rsp.mutable_ac()->set_y(y);
    }

    get_scene()->screen_broadcast(*this, rsp, false);

    // 攻击者可能不在另一个可见受击者屏幕范围需要以受击者为中心广播消息
    client::attack_rsp def;
    def.CopyFrom(rsp);
    def.set_def(true);
    for (int32 i = 0; i < rsp.fd_size(); ++i) {
        scene_role* psr = get_scene()->get_scene_role(rsp.fd(i).dst_rd());
        if (psr == NULL) {
            psr = get_scene()->get_scene_role(rsp.fd(i).dst_rd().inst_id());
        }

        if (psr == NULL) {
            continue;
        }

        def.clear_fd();
        def.add_fd()->CopyFrom(rsp.fd(i));
        get_scene()->screen_broadcast(*psr, def, false);
    }

    get_scene()->multi_broadcast_modify_role_msg(*this, enemies);

    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i]->get_status().is_dead() && !enemies[i]->has_dead()) {
            enemies[i]->on_killed(this);
            this->on_kill(enemies[i]);
			del_counter_role(enemies[i]);
			enemies[i]->del_counter_role(this);
        }
    }
}

void scene_role::fill_miss(skill* ps, const std::vector<map_postion>& miss, effect_result& rsp)
{
    const config_skill* pcs = ps->get_config_skill();
    if (pcs == NULL || pcs->get_miss_blow() == SMB_NOT_MISS_BLOW) {
        return ;
    }

    for (size_t i = 0; i < miss.size(); ++i) {
        fight_data* fd = rsp.add_fd();
        fd->set_fight_type(client::fight_data_TYPE_attk_miss);
        fd->mutable_pos()->set_x(miss[i].x);
        fd->mutable_pos()->set_y(miss[i].y);
        fd->mutable_pos()->set_z(miss[i].z);
    }
}

static bool del_not_normal_npc(scene_role* sr) {
    if (sr->get_status().is_dead()) {
        amtsvr_log("dead npc %u\n", sr->get_role_id());
    }

    if (sr->get_real_role_type() != RTE_NPC) {
        return true;
    }

    if (sr->get_feature() != MT_NORMAL_MONSTER) {
        return true;
    }

    return false;
}

void scene_role::attack_damage(skill* ps, std::vector<scene_role*>& vsr, effect_result& rsp)
{
    if (ps == NULL) {
        return ;
    }

    fill_role_data(*rsp.mutable_src_rd());

    const config_skill* pcs = ps->get_config_skill();
    if (pcs != NULL) {
        const std::vector<int>& ts = pcs->get_effect_object_type();
        bool attack = false;
        bool attack_normal = false;
        for (size_t i = 0; i < ts.size(); ++i) {
            if (ts[i] == EOT_ENEMY) {
                attack = true;
                break;
            } else if (ts[i] == EOT_ENEMY_NORMAL) {
                attack        = true;
                attack_normal = true;
                break;
            }
        }

        if (!attack) {
            vsr.clear();
            return ;
        }

        if (attack_normal) {
            std::vector<scene_role*>::iterator del = std::remove_if(vsr.begin(), vsr.end(), del_not_normal_npc);
            if (del != vsr.end()) {
                vsr.erase(del, vsr.end());
            }
        }
    }

    for (size_t i = 0; i < vsr.size(); ++i) {
        if (vsr[i] == NULL) {
            continue;
        }

        client::fight_data* fd = rsp.add_fd();
        if (is_hit_point(vsr[i])) {
            uint32 damage = attack_damage(vsr[i], ps);
            damage = 1 > damage ? 1 : damage;
            fd->set_fight_type(client::fight_data_TYPE_attk_normal);
            fd->set_critical  (0                                  );
            fd->set_data      (damage                             );

            uint32 rebound = 0;
            vsr[i]->mutable_buffer()->calculate_rebound(damage, rebound);
            if (rebound > 0 && !get_status().is_dead()) {
                client::buffer_rebound_msg msg;
                this->fill_role_data(msg.mutable_target());
                vsr[i]->fill_role_data(msg.mutable_caster());
                sub_hp(rebound);
                msg.set_hp(rebound);
                get_scene()->screen_broadcast(*this, msg, false);
                if (get_status().is_dead() && !has_dead()) {
                    vsr[i]->on_kill(this);
                    this->on_killed(vsr[i]);
					del_counter_role(vsr[i]);
					vsr[i]->del_counter_role(this);
                    send_client_change_attr();
                }
            }

            vsr[i]->sub_hp(damage);
            if (vsr[i]->get_status().is_dead()) {
                fd->set_dead(true);
            }

            if (vsr[i]->get_attr(CUR_HP) > vsr[i]->get_attr(MAX_HP)) {
                amtsvr_log("hp error %u/%u\n", vsr[i]->get_attr(CUR_HP), vsr[i]->get_attr(MAX_HP));
                vsr[i]->mutable_attr()->set_val(CUR_HP, vsr[i]->get_attr(MAX_HP));
            }

            fd->set_cur_hp(vsr[i]->get_attr(CUR_HP));
        } else {
            fd->set_fight_type(client::fight_data_TYPE_attk_miss);
            fd->set_critical  (0                                );
            fd->set_data      (0                                );
            fd->set_cur_hp    (vsr[i]->get_attr(CUR_HP)         );
        }

        vsr[i]->fill_role_data(*fd->mutable_dst_rd());
        bool t = false;
        vsr[i]->on_attacked(this, *ps, rsp, t);
    }
}

void scene_role::attack_effect(skill* ps, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends, effect_result& rsp)
{
    const config_skill* pcs = ps->get_config_skill();
    if (pcs == NULL) {
        return ;
    }

    attack_effect(pcs, ps->get_effect_id(), enemies, friends, rsp);
}

void scene_role::attack_effect(const config_skill* pcs, uint32 effect_id, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends, effect_result& rsp)
{
    if (pcs == NULL) {
        return ;
    }

    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return ;
    }

    std::vector<int> vml;
    if (!pcs->get_method_list().empty()) {
        vml = pcs->get_method_list();
    }

    if (effect_id != 0) {
        vml.push_back(effect_id);
    }

    for (size_t j = 0; j < vml.size(); ++j) {
        const config_effect* ce = CONFMGR->get_config_effect(vml[j]);
        if (ce == NULL) {
            amtsvr_log("skill id = %u effect id = %u not found\n", pcs->get_skill_id(), vml[j]);
            continue ;
        }

        if (ce->is_scene_buff()) {
            scene_role* target = scene_buff_target(ce, enemies, friends);
            client::terrain_info ti;
            psi->get_terrain_mgr().add_terrain(pcs, ce, target, this, ti);
            psi->scene_broadcast_terrain_change();
            psi->role_on_check_terrain(ti);
            continue;
        }

        if (ce->is_halo()) {
            add_halo(pcs, ce, enemies, friends);
            continue;
        }

        if (ce->is_dismount()) {
            fall_from_mounts(ce, enemies);
            continue;
        }

        if (ce->is_move()) {
            const std::vector<int>& veot = ce->get_effect_object_type();
            for (size_t k = 0; k < veot.size(); ++k) {
                switch (veot[k]) {
                case EOT_ENEMY : move(ce, enemies, rsp); break;
                case EOT_FRIEND: move(ce, friends, rsp); break;
                case EOT_OWN   : move(ce, friends, rsp); break;
                default: break;
                }
            }

            continue;
        }

        if (ce->is_flash()) {
            flash(ce);
            continue;
        }

        if (ce->is_swap()) {
            swap(enemies);
            continue;
        }

        if (ce->is_summon()) {
            call(ce);
            continue;
        }

        if (ce->is_clear_buff()) {
            clear_buffer(ce, enemies, friends);
            continue;
        }

        if (!ce->is_buff()) {
            continue;
        }

        const std::vector<int>& veot = ce->get_effect_object_type();
        for (size_t k = 0; k < veot.size(); ++k) {
            switch (veot[k]) {
            case EOT_ENEMY: {
                for (size_t j = 0; j < enemies.size(); ++j) {
                    if (mtrandom::rand_int32(TEN_THOUSAND) > ce->get_buff_probability()) {
                        continue;
                    }
                    
                    client::role_data rd;
                    this->fill_role_data(rd);
                    int ret = enemies[j]->add_effect(ce, &rd);
                    if (ret == 1) {
                        client::buffer_immune_msg msg;
                        enemies[j]->fill_role_data(msg.mutable_target());
                        msg.mutable_caster()->CopyFrom(rd);
                        msg.set_id(ce->get_effect_id());
                        get_scene()->screen_broadcast(*this, msg, false);
                    } else {
                        attack_effect(ce, enemies[j]);
                    }
//                    enemies[j]->on_update_effect(time_utils::tenms(), 0);
                }
                break;
            }
            case EOT_OWN: {
                if (mtrandom::rand_int32(TEN_THOUSAND) > ce->get_buff_probability()) {
                    break;
                }

                this->mutable_buffer()->add_buf(ce);
//                this->on_update_effect(time_utils::tenms(), 0);
                break;
            }
            case EOT_FRIEND: {
                for (size_t j = 0; j < friends.size(); ++j) {
                    if (mtrandom::rand_int32(TEN_THOUSAND) > ce->get_buff_probability()) {
                        continue;
                    }
                    friends[j]->mutable_buffer()->add_buf(ce);
                    attack_effect(ce, friends[j]);
//                    friends[j]->on_update_effect(time_utils::tenms(), 0);
                }
            }
            default: break;
            }
        }
    }

    const std::vector<int>& vsml = pcs->get_self_method_list();
    for (size_t i = 0; i < vsml.size(); ++i) {
        const config_effect* ce = CONFMGR->get_config_effect(vsml[i]);
        if (ce == NULL) {
            amtsvr_log("skill id = %u self effect id = %u not found\n", pcs->get_skill_id(), vsml[i]);
            continue ;
        }

        if (mtrandom::rand_int32(TEN_THOUSAND) > ce->get_buff_probability()) {
            continue;
        }

        mutable_buffer()->add_buf(ce);
    }

    // 冲撞
    if (pcs->get_collision_distance() != 0) {
        collision(pcs, enemies, rsp);
    }

    // 死亡击退
//     if (pcs->get_is_death_repel() != 0) {
//         death_repel();
//     }
}

void scene_role::attack_effect(const config_effect* pce, scene_role* psr)
{
    if (pce->get_next_trigger_effect_id() == 0) {
        return ;
    }
	if (psr->get_status().is_dead())
	{
		if(psr->get_role_type() == RTE_NPC)
		{
			scene_npc *sn = static_cast<scene_npc *>(psr);
			if(!sn->get_origin_npc().is_boss() || get_scene()->get_scene_type() != MAP_TYPE_WORLD_BOSS)
			{
				return;
			}
		}
		return ;
	}
    if (this == psr && pce->get_next_trigger_effect_object() != 1) {
        return ;
    }

    if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_next_trigger_effect_probability()) {
        return ;
    }

    config_effect_mgr* pmgr = MUTABLE_CONFIG_MGR(config_effect_mgr);
    if (pmgr == NULL) {
        return ;
    }

    const config_effect* next_pce = pmgr->get_config_effect(pce->get_next_trigger_effect_id());
    if (next_pce == NULL) {
        amtsvr_log("skill id = %u self effect id = %u not found\n", next_pce->get_effect_id(), pce->get_next_trigger_effect_id());
        return ;
    }

    client::role_data rd;
    this->fill_role_data(rd);
    for (uint32 i = 0; i < pce->get_next_trigger_effect_count(); ++i) {
        psr->mutable_buffer()->add_buf(next_pce, &rd);
        if (next_pce != pce) {
            attack_effect(next_pce, psr);
        }
    }
}

scene_role* scene_role::scene_buff_target(const config_effect* pce, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends)
{
    if (pce == NULL) {
        return this;
    }

    const std::vector<int>& ts = pce->get_effect_object_type();
    for (size_t i = 0; i < ts.size(); ++i) {
        if (ts[i] == EOT_ENEMY && !enemies.empty()) {
            return enemies[0];
        } else if (ts[i] == EOT_FRIEND && !friends.empty()) {
            return friends[0];
        }
    }

    return this;
}

bool scene_role::is_hit_point(scene_role* sr)
{
    int32 dodge    = sr->get_attr().get_val(DODGE);
    int32 hitpoint = this->get_attr().get_val(HIT_POINT);
    double rate = 0.95 + (hitpoint - dodge) / 5000.0;
    rate = max(0.5, rate);
    rate = min(rate, 1.0);

    uint32 irate = (uint32)(rate * 100);
    uint32 ibase = mtrandom::rand_int32(100);
//     double base = mtrandom::rand_realc0c1();
//     amtsvr_log("%f, %f", rate, base);
//     amtsvr_log("%u, %u", irate, ibase);
    return irate >= ibase;
}

uint32 scene_role::attack_damage(scene_role* sr, skill* ps)
{
    if (sr == NULL || ps == NULL || ps->get_config_skill() == NULL) {
        return 0;
    }

    if (((float)sr->get_attr(CUR_HP) / (float)sr->get_attr(MAX_HP)) <= (get_buffer().get_seckill().val()  / 10000.0f) ||
        sr->get_attr(CUR_HP) <= (uint32)get_buffer().get_seckill().max_val()) {
        return sr->get_attr(CUR_HP);
    }

    if ((get_status().maimed())) {
        return 1;
    }

	std::map<int, int> attr = ps->get_config_skill()->get_game_attribute();
	uint32 damage = 0;
    uint32 attack_min = this->get_attr().get_val(ATTACK_MIN) + attr[GAME_ATTRIBUTE_ATTACK_MIN];
	uint32 attack_max = this->get_attr().get_val(ATTACK_MAX) + attr[GAME_ATTRIBUTE_ATTACK_MAX];
    if (attack_min > attack_max) {
        attack_min = attack_max;
    }

	float  const_table_attack_coefficient  = 50 / 100.0f;
	uint32 stability = this->get_attr().get_val(STABILITY) + attr[GAME_ATTRIBUTE_STABILITY];
	float  skill = ps->get_config_skill()->get_correction() / 10000.0f;
	uint32 attack = (uint32)(mtrandom::rand_mid32((uint32)((attack_min + (attack_max - attack_min) * const_table_attack_coefficient) + ((attack_max - (attack_min + (attack_max - attack_min)) * const_table_attack_coefficient)) * stability / 10000.0f), attack_max) * skill);

    uint32 defense_min = sr->get_attr().get_val(DEFENSE_MIN);
    uint32 defense_max = sr->get_attr().get_val(DEFENSE_MAX);
    if (defense_min > defense_max) {
        defense_min = defense_max;
    }

    if (sr->get_status().injury()) {
        defense_min = 1;
        defense_max = 1;
    }

    float  const_table_defense_coefficient = 50 / 100.0f;
    uint32 ignore_defense_ratio = this->get_attr().get_val(IGNORE_DEFENSE_RATIO);
    uint32 defense = (uint32)((mtrandom::rand_mid32((uint32)(defense_min + (defense_max - defense_min) * const_table_defense_coefficient), defense_max)) * (1 - ignore_defense_ratio / 100.0f));

    int32 output_hurt_promote_ratio = min((int)this->get_attr().get_val(OUTPUT_HURT_PROMOTE_RATIO), 0) + attr[GAME_ATTRIBUTE_OUTPUT_HURT_PROMOTE_RATIO];
    int32 suffer_hurt_reduce_ratio  = min((int)sr->get_attr().get_val(SUFFER_HURT_REDUCE_RATIO), 0);
    if (attack > defense && (100 + output_hurt_promote_ratio - suffer_hurt_reduce_ratio) > 0) {
        damage = (uint32)((attack - defense) * ((100 + output_hurt_promote_ratio - suffer_hurt_reduce_ratio) / 100.0f));
    } else {
        damage = 1;
    }

    return damage;
}

uint32 scene_role::sub_hp(uint32& hp)
{
    uint32 old = get_attr().get_val(CUR_HP);
    mutable_buffer()->calculate_shield(mutable_attr(), hp);
    if (get_attr().get_val(CUR_HP) > old) {
        uint32 s = get_attr().get_val(CUR_HP) - old;
        client::buffer_hurt_msg msg;
        fill_role_data(msg.mutable_rd());
        msg.set_hp(s);
        msg.set_cur_hp(get_attr(CUR_HP));
        get_scene()->screen_broadcast(*this, msg, false);
    }

    uint32 cur = get_attr().get_val(CUR_HP) > hp ? get_attr().get_val(CUR_HP) - hp : 0;
    mutable_attr()->set_val(CUR_HP, cur);
    if (cur == 0) {
        mutable_status()->set_dead();
    }

    return cur;
}

void scene_role::use_passive_skill()
{
	uint32 skill_belong = 0;
	if (this->get_role_type() == RTE_PET){
		uint32 cur_belong = this->get_role_id() + 10;
		if (cur_belong >= SB_BEAUTY_MIN && cur_belong <= SB_BEAUTY_MAX){
			skill_belong = cur_belong;
		}
	}

    std::vector<skill*> vs;
    this->mutable_skill()->get_skill_by_type(ST_PASSIVE, vs,skill_belong);
    for (size_t i = 0; i < vs.size(); ++i) {
        if (vs[i] == NULL) {
            continue;
        }

        const config_skill* pcs = vs[i]->get_config_skill();
        if (pcs == NULL || !get_skill().check_mount_skill(get_attr(MOUNTS_ID), pcs)) {
            continue;
        }

		if (this->get_role_type() == RTE_USER ){
			if (vs[i]->get_skill_belong_type() >= SB_BEAUTY_MIN && vs[i]->get_skill_belong_type() <= SB_BEAUTY_MAX){
				continue;
			}
		}

        if (pcs->get_attack_time() != 0) {
            if (get_attack_count() == 0 ||
                get_attack_count() % pcs->get_attack_time() != 0) {
                    continue;
            }
        }

		if (vs[i]->get_skill_belong_type() >= SB_BEAUTY_MIN && vs[i]->get_skill_belong_type() <= SB_BEAUTY_MAX){

		}else{
			uint32 rand = mtrandom::rand_int32(10000u);
			if (rand > pcs->get_trigger_probability()) {
				continue;
			}
		}

        //printf("%s:%d, PASSIVE %d\n", __FILE__, __LINE__, vs[i]->get_skill_id());
        this->mutable_skill()->use_skill(vs[i]->get_skill_id(), get_attr(ATTACK_SPEED));
        use_skill(vs[i]);
        use_attack_trigger_skill();
        add_attack_count();
    }
}

void scene_role::use_attack_trigger_skill()
{
    std::vector<skill*> vs;
    mutable_skill()->get_skill_by_type(ST_ATK_TIRGGER, vs);

	std::vector<skill*> vs_ar;
	vs_ar.clear();
	mutable_skill()->get_skill_by_belong(SB_ARCHER, vs_ar);

    for (size_t i = 0; i < vs.size(); ++i) {
        const config_skill* pcs = vs[i]->get_config_skill();
        if (pcs == NULL) {
            continue;
        }
		if (pcs->get_skills_belonging() == SB_ARCHER){
			// 
			continue;
		}else{
			if (pcs->get_attack_time() != 0) {
				if (get_attack_count() == 0 ||
					get_attack_count() % pcs->get_attack_time() != 0) {
						continue;
				}
			}

			uint32 rand = mtrandom::rand_int32(TEN_THOUSAND);
			if (rand > pcs->get_trigger_probability()) {
				continue;
			}

			use_skill(vs[i]);

			if (vs_ar.size() != 0){
				bow_attack_skill();
			}
		}
    }


}

void  scene_role::bow_attack_skill()
{
	add_bow_attack_count();
	send_bow_attack_count();
	use_bow_skill();
}

void scene_role::use_bow_skill()
{
	std::vector<skill*> vs_ar;
	vs_ar.clear();
	mutable_skill()->get_skill_by_belong(SB_ARCHER, vs_ar);

	// 弓箭技能使用规则：
	uint32 cur_skill_count = 0;
	cur_skill_count = vs_ar.size();
	if (cur_skill_count != 0 && get_bow_attack_count() != 0){
		uint32 interval_count = get_attr(BOW_SPEED);
		if (interval_count != 0){
			if (get_bow_attack_count() % interval_count == 0){
				//uint32 use_base = get_bow_attack_count() / interval_count;
				//// 算出使用的编号：
				//bool is_around = false;
				//uint32 mod_vale =  cur_skill_count % use_base;
				//if (mod_vale == 0){
				//	mod_vale = cur_skill_count;
				//	is_around = true;
				//}else{
				//	mod_vale -=(cur_skill_count-1);
				//}
				uint32 mod_vale = get_attr(BOW_INDEX);
				//bool is_around = false;
				//if (mod_vale == 0){
				//	mod_vale = cur_skill_count;
				//	is_around = true;
				//}

				for (size_t j = 0; j < vs_ar.size(); ++j) {
					if (vs_ar[j]->get_ser_index() == mod_vale){
						use_skill(vs_ar[j]);
						if (mod_vale >= cur_skill_count){
							mutable_attr()->set_val(BOW_INDEX,1);
						}else{
							mutable_attr()->set_val(BOW_INDEX,mod_vale +1);
						}
					}
				}
			}
		}
	}
}

void scene_role::move(const config_effect* pce, std::vector<scene_role*>& enemies, effect_result& rsp)
{
    if (pce == NULL) {
        return ;
    }

    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return ;
    }

    map_postion p;
    uint32 tenms = time_utils::tenms();
    const std::vector<int>& veot = pce->get_effect_object_type();
    map_postion atk_pos = get_scene_pos();
    for (size_t k = 0; k < veot.size(); ++k) {
        switch (veot[k]) {
        case EOT_ENEMY: {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (enemies[j]->check_immunity_effect(pce)) {
                    continue;
                }

                map_postion fix_pos;
                if (pce->get_repel_point() == RP_DEF) {
                    fix_pos = enemies[j]->get_scene_pos();
                } else {
                    fix_pos = atk_pos;
                }
                //                 ROLE_FACING rf = role_facing(atk_pos, enemies[j]->get_scene_pos());
                //                 psi->calc_pos_to_pos(enemies[j]->get_scene_pos(), rf, psi->calc_cell_length(pce->get_move_range()), p);
                atk_pos.direction_point(enemies[j]->get_scene_pos(), psi->calc_cell_length(pce->get_move_range()), p, &fix_pos);
                p.x = min((uint32)max(0, p.x), psi->get_map().get_config_map()->get_real_width());
                p.y = min((uint32)max(0, p.y), psi->get_map().get_config_map()->get_real_height());
                enemies[j]->check_path_block(p);
                //client::role_pos_msg rpm;
                //enemies[j]->set_scene_pos(p);
                //enemies[j]->fill_role_pos_msg(rpm);
                //get_scene()->screen_broadcast(*this, rpm, false);
                //enemies[j]->change_cur_pos(p);
                enemies[j]->move_to_pos(p, MT_FORCE);
                enemies[j]->set_force_speed(pce->get_move_speed());
                enemies[j]->set_last_move_tick(tenms);

                client::attack_collision_data* acd = rsp.mutable_fd(j)->mutable_acd();
                acd->set_move_speed(enemies[j]->get_force_speed());
                acd->mutable_pos()->set_x(p.x);
                acd->mutable_pos()->set_y(p.y);
                acd->mutable_pos()->set_z(p.z);

                attack_effect(pce, enemies[j]);
            }
            break;
        }
        case EOT_FRIEND: {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (enemies[j]->check_immunity_effect(pce)) {
                    continue;
                }

                map_postion fix_pos;
                if (pce->get_repel_point() == RP_DEF) {
                    fix_pos = enemies[j]->get_scene_pos();
                } else {
                    fix_pos = atk_pos;
                }

                atk_pos.direction_point(enemies[j]->get_scene_pos(), psi->calc_cell_length(pce->get_move_range()), p, &fix_pos);
                p.x = min((uint32)max(0, p.x), psi->get_map().get_config_map()->get_real_width());
                p.y = min((uint32)max(0, p.y), psi->get_map().get_config_map()->get_real_height());
                enemies[j]->check_path_block(p);
//                 enemies[j]->change_cur_pos(p);
                enemies[j]->move_to_pos(p, MT_FORCE);
                enemies[j]->set_force_speed(pce->get_move_speed());
                enemies[j]->set_last_move_tick(tenms);

                client::fight_data* pfd = rsp.add_fd();
                enemies[j]->fill_role_data(pfd->mutable_dst_rd());
                pfd->set_fight_type(client::fight_data_TYPE_attk_pos);
                client::attack_collision_data* acd = pfd->mutable_acd();
                acd->set_move_speed(enemies[j]->get_force_speed());
                acd->mutable_pos()->set_x(p.x);
                acd->mutable_pos()->set_y(p.y);
                acd->mutable_pos()->set_z(p.z);

                attack_effect(pce, enemies[j]);
            }
            break;
        }
        case EOT_OWN: {
            psi->calc_pos_to_pos(get_scene_pos(), get_facing(), psi->calc_cell_length(pce->get_move_range()), p);
            //atk_pos.direction_point(get_scene_pos(), psi->calc_cell_length(pce->get_move_range()), p);
            p.x = min((uint32)max(0, p.x), psi->get_map().get_config_map()->get_real_width());
            p.y = min((uint32)max(0, p.y), psi->get_map().get_config_map()->get_real_height());
            check_path_block(p);
//             this->change_cur_pos(p);
            this->move_to_pos(p, MT_FORCE);
            this->set_force_speed(pce->get_move_speed());
            this->set_last_move_tick(tenms);

            client::fight_data* fd = rsp.add_fd();
            fill_role_data(fd->mutable_dst_rd());
            fd->set_fight_type(client::fight_data_TYPE_attk_pos);
            client::attack_collision_data* acd = fd->mutable_acd();
            acd->set_move_speed(get_force_speed());
            acd->mutable_pos()->set_x(p.x);
            acd->mutable_pos()->set_y(p.y);
            acd->mutable_pos()->set_z(p.z);

            attack_effect(pce, this);
            break;
        }
        default: break;
        }
    }
}

void scene_role::call(const config_effect* pce)
{
    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return ;
    }

    std::map<uint64, scene_role*>& pets = get_pets();
    std::map<uint64, scene_role*> tp = get_pets();
    const std::vector<int>& v = pce->get_summon_monster_list();
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] == 0) {
            continue;
        }

        const origin_npc* n = GAME_MAP_MGR->get_npc(v[i]);
        if (n == NULL) {
            continue;
        }

        map_postion mp = get_scene_pos();;
        switch (pce->get_call_monster_performance()) {
        case SP_FACE: {
            get_scene()->calc_role_cell_to_map_postion(this, 1, mp.x, mp.y);
            break;
        }
        case SP_ROUND: {
            uint32 length = get_scene()->calc_cell_length(pce->get_call_monster_round_radius());
            get_scene()->calc_pos_to_pos(get_scene_pos(), (ROLE_FACING)mtrandom::rand_int32(NORTHWEST), length, mp);
            break;
        }
        case SP_RANDOM: {
            uint32 length = get_scene()->calc_cell_length(pce->get_call_monster_round_radius());
            if (mtrandom::rand_int32(100) < 50) {
                mp.x -= mtrandom::rand_int32(length);
            } else {
                mp.x += mtrandom::rand_int32(length);
            }

            if (mtrandom::rand_int32(100) < 50) {
                mp.y -= mtrandom::rand_int32(length);
            } else {
                mp.y += mtrandom::rand_int32(length);
            }

            break;
        }
        default: break;
        }

        psi->fix_role_pos(mp);
        if (!tp.empty()) {
            std::map<uint64, scene_role*>::iterator i = tp.begin();
            bool c = false;
            while (i != tp.end()) {
                if (i->second->get_role_id() != n->get_role_id() || i->second->get_role_type() != RTE_PET) {
                    ++i;
                    continue;
                }

                c = true;
                if (i->second->get_status().is_dead()) {
                    psi->leave_scene(i->second);
                    pets.erase(i->second->get_inst_id());
                    i->second->set_inst_id(psi->generate_pet_id());
                    i->second->set_born_pos(get_scene_pos());
                    i->second->set_scene_pos(mp);
                    i->second->on_reset(true);
                    psi->enter_scene(i->second);
                    psi->screen_push_role(i->second);
                    i->second->change_cur_pos(mp);
                    add_pet(i->second);
                }

                break;
            }

            if (c) {
                tp.erase(i);
                continue;
            }
        }

        scene_pet* sp = new scene_pet(*n, NULL, psi->generate_pet_id(), this);
        add_pet(sp);
        sp->set_role_id(n->get_role_id());
        sp->set_born_pos(get_scene_pos());
        sp->set_scene_pos(mp);
        psi->enter_scene(sp);
        psi->screen_push_pet(sp);
        sp->change_cur_pos(mp);
    }
}

bool scene_role::rest_pet(uint32 pet_id, bool is_ectype)
{
    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return false;
    }

    std::map<uint64, scene_role*>& pet_map = get_pets();
    if (pet_map.empty()) {
        return false;
    }

    std::map<uint64, scene_role*>::iterator it = pet_map.begin();
    for (; it != pet_map.end(); ++it) {
        if (it->second->get_role_id() == pet_id) {
            if (is_ectype && it->second->get_role_type() == RTE_PET) {
                scene_pet* pet_obj = static_cast<scene_pet*>(it->second);
                uint32 cur_hp = it->second->get_attr(CUR_HP);
				uint32 max_hp = it->second->get_attr(MAX_HP);
                if (pet_obj->get_pet() != NULL) {
                    pet_obj->get_pet()->set_cur_hp(cur_hp);
					pet_obj->get_pet()->set_max_hp(max_hp);
                }
            }

            psi->leave_scene(it->second);
            delete_pet(it->second);
            break;
        }
    }

    return true;
}

bool scene_role::cure_pet(uint32 pet_id)
{
	std::map<uint64, scene_role*>& pet_map_ = get_pets();
	if (pet_map_.size() == 0) {
		return false;
	}

	std::map<uint64, scene_role*>::iterator it = pet_map_.begin();
	for (; it != pet_map_.end(); ++it) {
		if (it->second->get_role_id() == pet_id && it->second->get_role_type() == RTE_PET) {
			scene_pet* sp = static_cast<scene_pet*>(it->second);
			if (sp->get_pet() == NULL) {
				continue;
			}

			break;
		}
	}

	return true;
}

bool scene_role::pet_attr_change(uint32 pet_id,uint32 opt_type)
{
	std::map<uint64, scene_role*>& pet_map_ = get_pets();
	if (pet_map_.size() == 0) {
		return false;
	}

	std::map<uint64, scene_role*>::iterator it = pet_map_.begin();
	for (; it != pet_map_.end(); ++it) {
		if (it->second->get_role_id() == pet_id && it->second->get_role_type() == RTE_PET) {
			scene_pet* sp = static_cast<scene_pet*>(it->second);
			if (sp->get_pet() == NULL) {
				continue;
			}

			sp->on_pet_attr_change(pet_id,opt_type);
			break;
		}
	}

	return true;
}

bool scene_role::pet_skill_effect_up(uint32 pet_id,uint32 skill_id,uint32 upgrade_level)
{
	std::map<uint64, scene_role*>& pet_map_ = get_pets();
	if (pet_map_.size() == 0) {
		return false;
	}

	std::map<uint64, scene_role*>::iterator it = pet_map_.begin();
	for (; it != pet_map_.end(); ++it) {
		if (it->second->get_role_id() == pet_id && it->second->get_role_type() == RTE_PET) {
			scene_pet* sp = static_cast<scene_pet*>(it->second);
			if (sp->get_pet() == NULL) {
				continue;
			}

			sp->on_skill_effect_up(pet_id,skill_id,upgrade_level);
			break;
		}
	}

	return true;
}

void scene_role::collision(const config_skill* pcs, std::vector<scene_role*>& vsr, effect_result& rsp)
{
    if (pcs == NULL) {
        return ;
    }

    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return ;
    }

    uint32 tenms = time_utils::tenms();
    map_postion p;
    if (!vsr.empty()) {
        set_facing(role_facing(get_scene_pos(), vsr[0]->get_scene_pos()));
    }
    psi->calc_pos_to_pos(get_scene_pos(), get_facing(), psi->calc_cell_length(pcs->get_collision_distance()), p);
    p.x = min((uint32)max(0, p.x), psi->get_map().get_config_map()->get_real_width());
    p.y = min((uint32)max(0, p.y), psi->get_map().get_config_map()->get_real_height());
    check_path_block(p);
    move_to_pos(p, MT_FORCE);
    set_force_speed(pcs->get_collision_speed_param());
    set_last_move_tick(tenms);
    client::fight_data* fd = rsp.add_fd();
    fill_role_data(fd->mutable_dst_rd());
    fd->set_fight_type(client::fight_data_TYPE_attk_pos);
    client::attack_collision_data* acd = fd->mutable_acd();
    acd->set_move_speed(this->get_force_speed());
    acd->mutable_pos()->set_x(p.x);
    acd->mutable_pos()->set_y(p.y);
    acd->mutable_pos()->set_z(p.z);
//    amtsvr_log("user (%d, %d)\n", p.x, p.y);

//      client::role_pos_msg rpm;
//      this->set_scene_pos(p);
//      this->fill_role_pos_msg(rpm);
//      get_scene()->screen_broadcast(*this, rpm, false);

    for (size_t i = 0; i < vsr.size(); ++i) {
        if (vsr[i]->check_immunity_skill(pcs)) {
            continue;
        }

        if (pcs->get_object_max_level() != 0) {
            if (vsr[i]->get_attr().level() > pcs->get_object_max_level()) {
                continue;
            }
        } else if (get_attr().level() <= vsr[i]->get_attr().level()) {
            continue;
        }

        psi->calc_pos_to_pos(vsr[i]->get_scene_pos(), get_facing(), psi->calc_cell_length(pcs->get_collision_distance()), p);
        p.x = min((uint32)max(0, p.x), psi->get_map().get_config_map()->get_real_width());
        p.y = min((uint32)max(0, p.y), psi->get_map().get_config_map()->get_real_height());
        if (vsr[i]->get_force_speed() != 0) {
            continue;
        }

        check_path_block(p);
        vsr[i]->move_to_pos(p, MT_FORCE);
        vsr[i]->set_force_speed(pcs->get_collision_speed_param());
        vsr[i]->set_last_move_tick(tenms);
        client::attack_collision_data* acd = rsp.mutable_fd(i)->mutable_acd();
        acd->set_move_speed(vsr[i]->get_force_speed());
        acd->mutable_pos()->set_x(p.x);
        acd->mutable_pos()->set_y(p.y);
        acd->mutable_pos()->set_z(p.z);

//         client::role_pos_msg rpm;
//         vsr[i]->set_scene_pos(p);
//         vsr[i]->fill_role_pos_msg(rpm);
//         get_scene()->screen_broadcast(*vsr[i], rpm, false);

//        amtsvr_log("npc (%d, %d)\n", p.x, p.y);
    }
}

void scene_role::flash(const config_effect* pce)
{
    if (pce == NULL) {
        return ;
    }

    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return ;
    }

    int move_range;
    map_postion pos;
    move_range = pce->get_move_range() > 0 ? pce->get_move_range() : 5;
    psi->calc_pos_to_pos(get_scene_pos(), get_facing(), psi->calc_cell_length(move_range), pos);
    psi->fix_pos(pos);
    cell_mgr& c  = psi->get_cell();
    cell_pos now = psi->to_cell_pos(get_scene_pos());
    cell_pos dst = psi->to_cell_pos(pos);
    do {
        if (c.is_empty(dst)) {
            flash_move(psi->to_map_postion(dst));
            break;
        } else {
            dst = c.step(dst, now);
        }
    } while (dst != now);
}

void scene_role::swap(std::vector<scene_role*>& enemies)
{
    if (!enemies.empty()) {
        swap_postion(enemies[0]);
    }
}

void scene_role::clear_buffer(const config_effect* pce, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends)
{
    std::vector<scene_role*> rs;
    const std::vector<int>& veot = pce->get_effect_object_type();
    for (size_t k = 0; k < veot.size(); ++k) {
        switch (veot[k]) {
        case EOT_OWN: {
            if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_clear_buff_probability()) {
                break;
            }

            rs.push_back(this);
            break;
                      }
        case EOT_FRIEND: {
            for (size_t j = 0; j < friends.size(); ++j) {
                if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_clear_buff_probability()) {
                    continue;
                }

                rs.push_back(this);
            }
            break;
                         }
        case EOT_ENEMY: {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_clear_buff_probability()) {
                    continue;
                }

                rs.push_back(enemies[j]);
            }
            break;
                        }
        default: break;
        }
    }

    if (rs.empty()) {
        return ;
    }

    for (size_t i = 0; i < rs.size(); ++i) {
        rs[i]->mutable_buffer()->del_buf(pce->get_clear_useful_buff_level(), pce->get_clear_harmful_buff_level());
        rs[i]->mutable_buffer()->del_buf(pce->get_clear_buff_list());
    }
}

void scene_role::fall_from_mounts(const config_effect* pce, std::vector<scene_role*>& enemies)
{
    uint32 cur_tick = time_utils::tenms();
    for (size_t index = 0; index < enemies.size(); ++index) {
        if (enemies[index]->get_last_mount_tick() + pce->get_duration_tick() > cur_tick) {
            continue;
        }

        if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_buff_probability()) {
            continue;
        }

        enemies[index]->fall_from_mounts(cur_tick);
        attack_effect(pce, enemies[index]);
    }
}

void scene_role::fall_from_mounts(uint32 tick)
{
    this->set_last_mount_tick(tick);
}

void scene_role::calculate_attr()
{
    int32 hp = get_attr().get_val(CUR_HP);
    int32 mp = get_attr().get_val(CUR_MP);
    scene_role_attr ra(get_attr());
    reset_original_attr();
    is_change_attr  = true;
    mutable_buffer()->calculate_attr(mutable_attr());
    mutable_attr()->clear_modify_attr();

    // 和原数据比较记录改变值
    for (uint32 i = MAX_HP; i < get_attr().field_count() && i <= MOVE_SPEED; ++i) {
        ROLE_ATTRIBUTE index = (ROLE_ATTRIBUTE)i;
        if (ra.get_val(index) != get_attr().get_val(index)) {
            mutable_attr()->set_val(index, get_attr().get_val(index));
        }
    }

    hp = (int32)get_attr().get_val(CUR_HP) - hp;
    mp = (int32)get_attr().get_val(CUR_MP) - mp;
    if (get_scene() != NULL && (hp != 0 || mp != 0 || get_attr().get_val(CUR_HP) == 0)) {
        client::buffer_hurt_msg msg;
        fill_role_data(msg.mutable_rd());
        if (hp != 0) {
            msg.set_hp(hp);
        }

        if (mp != 0) {
            msg.set_mp(mp);
        }

        if (get_attr().get_val(CUR_HP) == 0) {
            if (!get_status().is_dead() && !has_dead()) {
                scene_role* sr = get_hatest_role();
                if (sr != NULL) {
                    sr->on_kill(this);
                } else {
                    this->on_kill(this);
                }

                on_killed(sr);
            }

            mutable_status()->set_dead();
            msg.set_dead(true);
        }

        if (msg.has_hp() || msg.has_mp()) {
            msg.set_cur_hp(get_attr().get_val(CUR_HP));
            get_scene()->screen_broadcast(*this, msg, false);
        }
    }
}

void scene_role::calculate_status()
{
    mutable_status()->normalize();
    mutable_buffer()->calculate_status(mutable_status());

    if (get_status().maimed()) {
        mutable_attr()->set_val(ATTACK_MIN, 1);
        mutable_attr()->set_val(ATTACK_MAX, 1);
    }

    if (get_status().injury()) {
        mutable_attr()->set_val(DEFENSE_MAX, 1);
        mutable_attr()->set_val(DEFENSE_MIN, 1);
    }

    if (get_status().maimed() || get_status().injury()) {
        send_client_change_attr();
    }
}

void scene_role::set_camp(uint32 id)
{
    _camp_id = id;
	mutable_attr()->set_val(ROLE_CAMP_ID, id);
    std::map<uint64, scene_role*>& pets = get_pets();
    std::map<uint64, scene_role*>::iterator i = pets.begin();
    for ( ; i != pets.end(); ++i) {
        if (i->second == NULL) {
            continue;
        }
		if (i->second->get_status().is_dead()) {
			continue;
		}
        i->second->set_camp(id);
    }
}

void scene_role::set_team_id(uint32 id)
{
    _team_id = id;
	mutable_attr()->set_val(ROLE_TEAM_ID, id);
    std::map<uint64, scene_role*>& pets = get_pets();
    std::map<uint64, scene_role*>::iterator i = pets.begin();
    for ( ; i != pets.end(); ++i) {
        if (i->second == NULL) {
            continue;
        }
		if (i->second->get_status().is_dead()) { 
			continue;
		}
        i->second->set_team_id(id);
    }
}


void scene_role::set_attack_mode(uint32 mode)
{
	if((uint32)get_status().atk_mode() == mode)
	{
		return ;
	}
	mutable_status()->set_val(client::RSE_STATUS_ATTKMODE, mode);
	std::map<uint64, scene_role *> &pets = get_pets();
	std::map<uint64, scene_role *>::iterator i;
	for(i = pets.begin(); i != pets.end(); ++i)
	{
		if(i->second == NULL) 
		{
			continue;
		}
		if(i->second->get_status().is_dead()) 
		{
			continue;
		}
		i->second->set_attack_mode(mode);
	}
}

void scene_role::set_family_id(uint32 id)
{
    mutable_attr()->set_val(FAMILY_ID, id);
    std::map<uint64, scene_role*>& pets = get_pets();
    std::map<uint64, scene_role*>::iterator i = pets.begin();
    for ( ; i != pets.end(); ++i) {
        if (i->second == NULL) {
            continue;
        }
		if (i->second->get_status().is_dead()) {
			continue;
		}
        i->second->set_family_id(id);
    }
}

bool scene_role::update_buffer(uint32 tick, uint32& effect_count)
{
    mutable_buffer()->update(tick, effect_count);
    return get_buffer().is_change();
}

void scene_role::screen_broadcast_change_buf()
{
    client::buffer_list bl;
    if (mutable_buffer()->fill_change_buffer_list(bl) > 0 && get_scene() != NULL) {
        fill_role_data(bl.mutable_rd());
        get_scene()->screen_broadcast(*this, bl, false);
    }
}

bool scene_role::on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    if (is_one_sec) {
        on_update_one_second(tick, user_num);
    }

    if (is_fiv_sec) {
        on_update_fiv_second(tick, user_num);
    }

    if (is_ten_sec) {
        on_update_ten_second(tick, user_num);
    }

    if (is_one_minute) {
        on_update_one_minute(tick, user_num);
    }

    if (tick > get_100ms_tick()) {
        set_100ms_tick(tick + 10);
        on_update_100ms(tick, user_num);
    }

    set_last_update_tick(tick);

    return true;
}

bool scene_role::on_update_100ms(uint32 tick, uint32 user_num)
{
    return on_update_effect(tick, user_num);
}

bool scene_role::on_update_one_second(uint32 tick, uint32 user_num)
{
    return true;
}

bool scene_role::on_update_fiv_second(uint32 tick, uint32 user_num)
{
    if (get_scene() == NULL || get_status().is_dead() || get_attr(CUR_HP) >= get_attr(MAX_HP) || get_attr(RECOVER) == 0) {
        return true;
    }

    const uint32 RECOVER_TICK = 1500;
    if (tick - get_last_recover_tick() > RECOVER_TICK || get_last_recover_tick() == 0) {
        set_last_recover_tick(tick);
        mutable_attr()->add_val(CUR_HP, get_attr(RECOVER));
        send_client_change_attr();

        client::recover_hp_msg msg;
        fill_role_data(msg.mutable_rd());
        msg.set_hp(get_attr(RECOVER));
        get_scene()->screen_broadcast(*this, msg, false);
    }

    return true;
}

int scene_role::on_region_change(uint32 old, uint32 now)
{
//     if (old != now) {
//         on_leave_region(old);
//         on_enter_region(now);
//     }

    set_region(now);

    return 0;
}

int scene_role::on_region_check(const region_data& r)
{
    if (get_scene() == NULL) {
        return -1;
    }

    return 0;
}

int scene_role::on_leave_region(const region_data& r)
{
    return 0;
}

int scene_role::on_enter_region(const region_data& r)
{
    if (get_scene() == NULL) {
        return -1;
    }

    return 0;
}

int scene_role::on_move_stop(uint32 tick)
{
    if (get_scene() == NULL) {
        return -1;
    }

    /*client::role_postion_msg msg;
      msg.set_x(get_scene_pos().x);
      msg.set_y(get_scene_pos().y);
      msg.set_z(get_scene_pos().z);
      msg.set_timestamp(tick);
      fill_role_data(msg.mutable_rd());
      get_scene()->screen_broadcast(*this, msg, false);*/

    on_move_end();

    return 0;
}

int scene_role::on_move_end()
{
    set_force_speed(0);

    return 0;
}

int scene_role::add_effect(const config_effect* effect, client::role_data* caster /*= NULL*/)
{
     return mutable_buffer()->add_buf(effect, caster);
}

int scene_role::add_effect(uint32 effect_id, uint32 effect_times /* = 1 */)
{
    const config_effect* effect = CONFMGR->get_config_effect(effect_id);
    if (effect == NULL) {
        return -1;
    }

    if (effect->get_is_buff()) {
        mutable_buffer()->add_buf(effect);
    }

    return 0;
}

int scene_role::remove_effect(uint32 effect_id)
{
    const config_effect* effect = CONFMGR->get_config_effect(effect_id);
    if (effect == NULL) {
        return -1;
    }

    return mutable_buffer()->del_buf(effect);
}

bool scene_role::on_update_effect(uint32 tick, uint32 user_num)
{
    uint32 effect_count = 0;
    bool ret = update_buffer(tick, effect_count);
    if (get_buffer().is_broadcast()) {
        screen_broadcast_change_buf();
    }

    if (!ret) {
        return ret;
    }

    if (effect_count == 0 || get_scene() == NULL) {
        return ret;
    }

    if (!get_buffer().is_trigger()) {
        return ret;
    }

    calculate_attr();
    calculate_status();

    std::vector<client::trigger_skill>& ts = mutable_buffer()->get_skills();
    for (size_t i = 0; i < ts.size(); ++i) {
        skill* ps = mutable_skill()->get_ai_skill(ts[i].skill_id());
        if (ps == NULL) {
            continue;
        }

        use_skill(ps);
    }

    get_scene()->screen_broadcast_modify_role_msg(*this);

    return ret;
}

void scene_role::send_client_attr()
{
    client::update_role_attr_msg msg;
    client::role_msg* rm = msg.add_rm();
    fill_role_data(rm->mutable_rd());
    fill_role_attr(rm->mutable_attr());
    send_client(msg);
}

void scene_role::broadcast_pet_attr()
{
	scene_inst *si = get_scene();
	if(si == NULL) {
		return;
	}

    std::map<uint64, scene_role*>& pets = get_pets();
	std::map<uint64, scene_role *>::iterator it ;
	for(it = pets.begin(); it != pets.end(); ++it) {
		si->screen_broadcast_modify_role_msg(*it->second, false);
	}
}

void scene_role::broadcast_pet_attr(scene_role *sr)
{
	scene_inst *si = get_scene();
	if(NULL == si) {
		return;
	}

	si->screen_broadcast_modify_role_msg(*sr, false);
}

void scene_role::send_client_change_attr()
{
    if (get_scene() == NULL) {
        return;
    }

    get_scene()->screen_broadcast_modify_role_msg(*this);
}

bool scene_role::check_path_block(map_postion& p)
{
    cell_pos now = get_scene()->to_cell_pos(get_scene_pos());
    cell_pos end = get_scene()->to_cell_pos(p);
    cell_pos cp;
    bool block = false;
    cell_mgr& c = get_scene()->get_cell();
    do {
        cp = c.step(now, end);
        if (c.is_block(cp)) {
            block = true;
            break;
        }
        now = cp;
    } while (cp != end);

    p = get_scene()->to_map_postion(now);

    return block;
}

void scene_role::on_attr_change(ROLE_ATTRIBUTE type, uint64 old_val, uint64 new_val)
{
#ifdef __LIB_MINI_SERVER__
    //if (type == CUR_HP && old_val != new_val) {
    //    static uint64 old = 0;
    //    if (old != old_val) {
    //        old = old_val;
    //    } else {
    //        int b = 0;
    //        ++b;
    //    }

    //    if (get_role_type() == RTE_PET) {
    //        amtsvr_log("pet:%u, hp old:%llu, now:%llu\n", get_role_id(), old_val, new_val);
    //    }

    //    if (get_role_type() == RTE_USER) {
    //        amtsvr_log("user:%u, hp old:%llu, now:%llu\n", get_role_id(), old_val, new_val);
    //    }
    //}

    //if (type == MAP_ID && (new_val == 0 || old_val == 0)) {
    //    amtsvr_log("user:%u, map old:%llu, now:%llu\n", get_role_id(), old_val, new_val);
    //}

#endif // __LIB_MINI_SERVER__

    //if (type == CUR_HP && get_scene() != NULL /*&& new_val > old_val*/) {
    //    client::role_attr_change_msg msg;
    //    fill_role_data(msg.mutable_rd());
    //    msg.set_type(type);
    //    msg.set_old(old_val);
    //    msg.set_now(new_val);
    //    get_scene()->screen_broadcast(*this, msg, false);
    //}

//     if (type == MAX_HP && old_val != 0 && new_val > old_val) {
//         amtsvr_log("user:%u, hp old:%llu, now:%llu\n", get_role_id(), old_val, new_val);
//     }

    if (type == CUR_HP && old_val != new_val && get_scene() != NULL) {
        get_scene()->on_role_attr_change(this);
    }
}

void scene_role::on_status_change(uint32 index, uint32 val)
{
//#ifdef __LIB_MINI_SERVER__
//    if (get_role_type() == RTE_USER) {
//        amtsvr_log("index:%u now:%u\n", index, val);
//    }
//#endif // __LIB_MINI_SERVER__
}

int scene_role::send_center(struct amtsvr_context* ctx, google::protobuf::Message& msg)
{
    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        return -1;
    }

    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL) {
        MSG_HANDLER_POOL->push(mh);
        return -2;
    }

    new_msg->CopyFrom(msg);
    mh->msg = new_msg;
    amtsvr_sendname(ctx, CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));

    return 0;
}

int scene_role::send_center(google::protobuf::Message& msg)
{
    if (get_scene() == NULL) {
        return -1;
    }

    return send_center(get_scene()->get_ctx(), msg);
}

int scene_role::send_log(google::protobuf::Message& msg)
{
    return send_center(msg);
}

bool scene_role::has_halo()
{
    if (_halos == NULL) {
        return false;
    }

    return _halos->has_halo();
}

halo_mgr* scene_role::halo_mgr_instance()
{
    if (_halos != NULL) {
        return _halos;
    }

    _halos = new halo_mgr;
    return _halos;
}

void scene_role::halo_mgr_release()
{
    if (_halos != NULL) {
        delete _halos;
        _halos = NULL;
    }
}

void scene_role::add_halo(const config_skill* pcs, const config_effect* pce, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends)
{
    const std::vector<int>& veot = pce->get_effect_object_type();
    for (size_t k = 0; k < veot.size(); ++k) {
        switch (veot[k]) {
        case EOT_ENEMY: {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_buff_probability()) {
                    continue;
                }

                enemies[j]->add_halo(this, pcs, pce);
            }
            break;
                        }
        case EOT_OWN: {
            if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_buff_probability()) {
                break;
            }

            this->add_halo(this, pcs, pce);
            break;
                      }
        case EOT_FRIEND: {
            for (size_t j = 0; j < friends.size(); ++j) {
                if (mtrandom::rand_int32(TEN_THOUSAND) > pce->get_buff_probability()) {
                    continue;
                }

                friends[j]->add_halo(this, pcs, pce);
            }
                         }
        default: break;
        }
    }
}

void scene_role::add_halo(scene_role* pcaster, const config_skill* pcs, const config_effect* pce)
{
    assert(pcaster != NULL);
    assert(pcs != NULL);
    assert(pce != NULL);
    if (get_scene() == NULL) {
        return ;
    }

    client::role_data rd;
    pcaster->fill_role_data(rd);
    int ret = mutable_buffer()->add_buf(pce, &rd);
    if (ret == 1) {
        client::buffer_immune_msg msg;
        this->fill_role_data(msg.mutable_target());
        msg.mutable_caster()->CopyFrom(rd);
        msg.set_id(pce->get_effect_id());
        get_scene()->screen_broadcast(*this, msg, false);
        return ;
    }

    uint32 tick = time_utils::tenms();
    halo_mgr* phm = halo_mgr_instance();
    phm->insert(this, get_scene()->calc_cell_length(pcs->get_round_radius()), pce, tick);
    phm->move_update(tick);
}

void scene_role::del_halo(scene_role *sr, const config_effect *pce)
{
	if(_halos == NULL || sr == NULL || pce == NULL) {
		return;
	}

    halo_mgr_instance()->erase(pce);
    mutable_buffer()->del_buf(pce);
    screen_broadcast_change_buf();
}

void scene_role::role_in(scene_role* psr, uint32 tick)
{
    if (!has_halo()) {
        return ;
    }

    _halos->role_in(psr, tick);
}

void scene_role::role_out(scene_role* psr, uint32 tick)
{
    if (!has_halo()) {
        return ;
    }

    _halos->role_out(psr, tick);
}

scene_role* scene_role::get_hatest_role()
{
    if (get_scene() == NULL) {
        return NULL;
    }
    
    uint64      hatest_role = 0;
    scene_role* sr          = NULL;
    while ((hatest_role = hate_man::get_hate_role()) != 0) {
        sr = get_scene()->get_scene_role(hate_man::get_hate_role());
        if (sr != NULL) {
            return sr;
        }

        hate_man::del_hate_role(hatest_role);
    }

    return NULL;
}

void scene_role::add_counter_role(scene_role *sr, uint32 hate)
{
	if(NULL == sr)
	{
		return ;
	}
	if(sr->get_role_type() != RTE_PET)
	{
		_counter_list[sr->get_inst_id()] += hate;
	}
	else
	{
		scene_role *master = sr->get_master();
		if(master == NULL)
		{
			return ;
		}
		uint64 inst_id = master->get_inst_id();
		_counter_list[inst_id] += hate;
		_counter_list[sr->get_inst_id()] = _counter_list[inst_id];
	}
}

void scene_role::del_counter_role(scene_role *sr)
{
	if(sr == NULL)
	{
		return;
	}
	map<uint64, uint32>::iterator it = _counter_list.find(sr->get_inst_id());
	if(it == _counter_list.end())
	{
		return ;
	}
	_counter_list.erase(it);
}

void scene_role::clear_counter_list() 
{ 
	_counter_list.clear(); 
}

scene_role* scene_role::get_counter_role()
{
	scene_role *sr = NULL;
	uint32 hate = 0;
	map<uint64, uint32>::iterator i = _counter_list.begin();
	while(i != _counter_list.end())
	{
		scene_role *tmp = get_scene()->get_scene_role(i->first);
		if(sr == NULL || sr->get_status().is_dead() || sr->get_role_type() == RTE_PET)
		{
			_counter_list.erase(i++);
			continue;
		}
		if(i->second > hate)
		{
			sr = tmp;
			hate = i->second;
		}
		++i;
	}
	return sr;
}

bool scene_role::is_counter_role(scene_role *sr)
{
	if(sr == NULL)
	{
		return false;
	}
	if(_counter_list.find(sr->get_inst_id()) != _counter_list.end())
	{
		return true;
	}
	if(sr->get_role_type() == RTE_PET)
	{
		scene_role *master = sr->get_master();
		if(master == NULL)
		{
			return false;
		}
		return _counter_list.find(master->get_inst_id()) != _counter_list.end();
	}
	else
	{
		map<uint64, scene_role *> &pets = sr->get_pets();
		map<uint64, scene_role *>::iterator it = pets.begin();
		for(; it != pets.end(); ++it)
		{
			if(it->second == NULL || it->second->get_scene() == NULL || it->second->get_status().is_dead())
			{
				continue;
			}
			if(_counter_list.find(it->first) != _counter_list.end())
			{
				return true;
			}
		}
	}
	return false;
}

void  scene_role::send_bow_attack_count()
{
	if (get_role_type() != RTE_USER){
		return;
	}

	std::vector<skill*> vs_ar;
	vs_ar.clear();
	mutable_skill()->get_skill_by_belong(SB_ARCHER, vs_ar);
	if (vs_ar.size() == 0){
		return;
	}

	if (get_attr(BOW_INDEX) == 0){
		set_attr(BOW_INDEX,1);
	}

	client::bows_skill_fight_rsp rsp_msg;
	rsp_msg.set_attack_count(get_bow_attack_count());
	rsp_msg.set_skill_index(get_attr(BOW_INDEX));
	send_client(rsp_msg);
}

bool scene_role::check_immunity_effect(const config_effect* effect)
{
    return false;
}

bool scene_role::check_immunity_skill(const config_skill* skill)
{
    return false;
}
