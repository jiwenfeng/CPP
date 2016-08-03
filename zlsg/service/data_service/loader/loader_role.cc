#include "player_role_status.h"
#include "player_role_skill_mgr.h"
#include "player_role_raid_mgr.h"
#include "player_role_auto_raid.h"
#include "player_role_props.h"
#include "player_role_mail.h"
#include "player_role_task.h"
#include "player_role_label_mgr.h"
#include "player_role_vip.h"
#include "player_role_explore.h"
#include "player_role_statis_mgr.h"
#include "player_role_set.h"
#include "player_role_buffer.h"
#include "player_role_order_mgr.h"
#include "operate_log.h"
#include "player_role_ext_attr.h"
#include "player_role_mounts.h"
#include "player_role_potential.h"
#include "player_role_pet.h"
#include "player_role_cd_cooldown.h"
#include "player_role_treasure.h"
#include "player_role_mission.h"
#include "player_role_vip.h"
#include "player_role_lottery.h"
#include "player_role_black_shop.h"
#include "player_role_levels.h"
#include "player_role_gem.h"
#include "player_role_bows.h"
#include "player_role_cycle.h"
#include "player_role_devil.h"
#include "loader_role.h"

loader_role::loader_role()
{
    _role  = NULL;
    _crole = NULL;
    step   = LPSE_INIT;
}

loader_role::~loader_role()
{
    uint32 size = req_list.size();
    for (uint32 i = 0; i < size; ++i) {
        delete req_list[i];
    }
    
    req_list.clear();

    if (_role != NULL) {
        delete _role;
    }

    if (_crole != NULL) {
        delete _crole;
    }
}

int loader_role::load(struct amtsvr_context * ctx, uint32 role_id)
{
    add_step();
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_player_role %u 0", role_id);

    GET_READ_SQLMGR_NAME(role_id);
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::proc_sqlmgr_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    if (rsp == NULL){
        return -1;
    }

    int session = -2;
    switch(step)
    {
    case LPSE_ROLE:
        {
            session = on_role_rsp(ctx, rsp);
        }
        break;
    case LPSE_STATUS:
        {
            session = on_status_rsp(ctx, rsp);
        }
        break;
    case LPSE_SKILL:
        {
            session = on_skill_rsp(ctx, rsp);
        }
        break;
    case LPSE_PROPS:
        {
            session = on_props_rsp(ctx, rsp);
        }
        break;
    case LPSE_MAIL:
        {
            session = on_mail_rsp(ctx, rsp);
        }
        break;
    case LPSE_TASK:
        {
            session = on_task_rsp(ctx, rsp);
        }
        break;
    case LPSE_PET:
        {
            session = on_pet_rsp(ctx, rsp);
        }
        break;
    case LPSE_LABEL:
        {
            session = on_label_rsp(ctx, rsp);
        }
        break;
    case LPSE_SET:
        {
            session = on_set_rsp(ctx, rsp);
        }
        break;
    case LPSE_EXPLORE:
        {
            session = on_explore_rsp(ctx, rsp);
        }
        break;
    case LPSE_EXPLORE_DETAIL:
        {
            session = on_explore_detail_rsp(ctx, rsp);
        }
        break;
    case LPSE_ROLE_SET:
        {
            session = on_role_set_rsp(ctx, rsp);
        }
        break;
    case LPSE_ROLE_BUFFER:
        {
            session = on_role_buffer_rsp(ctx, rsp);
        }
        break;
    case LPSE_ROLE_RAID:
        {
            session = on_role_raid_rsp(ctx, rsp);
        }
        break;
    case LPSE_ROLE_ORDER:
        {
            session = on_role_order_rsp(ctx, rsp);
        }
        break;
    case LPSE_ROLE_STATIS:
        {
            session = on_role_statis_rsp(ctx, rsp);
        }
        break;
    case LPSE_ROLE_EXT_ATTR:
        {
            session = on_role_ext_attr_rsp(ctx, rsp);
        }
        break;
    case LPSE_ROLE_MOUNTS:
        {
            session = on_role_mounts_rsp(ctx, rsp);
        }
        break;
	case LPSE_ROLE_POTENTIAL:
		{
			session = on_role_potential_rsp(ctx,rsp);
		}
		break;
    case LPSE_ROLE_AUTO_RAID: 
		{
			session = on_role_auto_raid_rsp(ctx, rsp); 
		}
		break;
	case LPSE_ROLE_CD_COOLDOWN:
		{
			session = on_role_cd_cooldown_rsp(ctx, rsp); 
		}
		break;
	case LPSE_ROLE_TREASURE:
		{
			session = on_role_treasure_rsp(ctx, rsp);
		}
		break;
	case LPSE_ROLE_TARGET:
		{
			session = on_role_target_rsp(ctx, rsp);
		}
		break;
	case LPSE_ROLE_VIP:
		{
			session = on_role_vip_rsp(ctx, rsp);
		}
		break;
	case LPSE_ROLE_LOTTERY:
		{
			session = on_role_lottery_rsp(ctx, rsp);
		}
		break;
	case LPSE_ROLE_BLACK_SHOP:
		{
			session = on_role_black_shop_rsp(ctx, rsp);
		}
		break;
	case LPSE_ROLE_LEVELS:
		{
			session = on_role_levels_rsp(ctx, rsp);
		}
		break;
	case LPSE_ROLE_GEMS:
		{
			session = on_role_gem_rsp(ctx,rsp);
		}
		break;
	case LPSE_ROLE_BOWS:
		{
			session = on_role_bows_rsp(ctx,rsp);
		}
		break;
	case LPSE_ROLE_CYCLE:
		{
			session = on_role_cycle_rsp(ctx, rsp);
		}
		break;
	case LPSE_ROLE_DEVIL:
		{
			session = on_role_devil_rsp(ctx, rsp);
		}
		break;
    default :
        {
            for (int i = 0; i < rsp->pb_count; ++i) {
                if (rsp->pb_ctx[i].pb_msg != NULL) {
                    delete rsp->pb_ctx[i].pb_msg;
                }
            }
        }
        break;
    }

    return session;
}

int loader_role::on_role_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    for (int i = 0; i < rsp->pb_count; ++i) {
        mysql::tb_player_role* tmp = static_cast<mysql::tb_player_role*>(rsp->pb_ctx[i].pb_msg);
        if (_role == NULL) {// && tmp->player_id() == _player->id()){
            _role = new player_role(tmp);
        } else {
            delete rsp->pb_ctx[i].pb_msg;
        }
    }

    if (_role == NULL) {
        if (rsp->ret == 0) { // 此用户无角色
            end_step();
            return 0;
        }
        return -1;
    }

    add_step();
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_status %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_status_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_role_status().attach(static_cast<mysql::tb_role_status*>(rsp->pb_ctx[0].pb_msg));
    } else {
        const config_birth* cb = CONFMGR->get_birth(_role->get_db_player_role()->profession());
        if (cb == NULL) {
            return -1;
        }

        _role->get_role_status().create(_role->get_role_id(), cb, _role->get_db_player_role()->create_time());
    }

    for (int i = 1; i < rsp->pb_count; ++i) {
        delete rsp->pb_ctx[i].pb_msg;
    }

    add_step();
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_skill %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_skill_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    _role->get_role_skill_mgr().attach_role_skill(_role->get_role_id(),rsp->pb_count, rsp->pb_ctx);

    add_step();
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_props %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_props_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    _role->attach_role_props(rsp->pb_count, rsp->pb_ctx);

    add_step();
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_mail %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_mail_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_role_mails().set_db_mail(static_cast<mysql::tb_role_mail*>(rsp->pb_ctx[0].pb_msg));
    }

    for (int i = 1; i < rsp->pb_count; ++i) {
        delete rsp->pb_ctx[i].pb_msg;
    }

    add_step();
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_task %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_task_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    _role->get_tasks().attach_role_task(rsp->pb_count, rsp->pb_ctx);

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_pet %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_pet_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    _role->get_pets().attach_role_pet(rsp->pb_count, rsp->pb_ctx);

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_label %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_label_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_role_label_mgr().attach_role_label(rsp->pb_count, rsp->pb_ctx);
    }

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_center_set %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_set_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_crole == NULL) {
        _crole = new center_role;
    }

    if (rsp->pb_count > 0) {
        _crole->attach_role_set(static_cast<mysql::tb_role_center_set*>(rsp->pb_ctx[0].pb_msg));
    }

    for (int i = 1; i < rsp->pb_count; ++i) {
        delete rsp->pb_ctx[i].pb_msg;
    }

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_explore %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_explore_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_role_explore().attach_role_explore(rsp->pb_count, rsp->pb_ctx);
    }
    
    add_step();
        
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_explore_detail %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_explore_detail_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_role_explore().attach_role_explore_detail(rsp->pb_count, rsp->pb_ctx);
    }
    
    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_set %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_set_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0){
        _role->get_set().attach_role_set(rsp->pb_count, rsp->pb_ctx);
    }
    
    add_step();
    
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_buffer %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_buffer_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_buffer().attach_role_buffer(rsp->pb_count, rsp->pb_ctx);
    }

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_raid %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_raid_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_raid_mgr().attach_role_raid(rsp->pb_count, rsp->pb_ctx);
    }

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_order %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_order_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_order_mgr().attach_role_order(rsp->pb_count, rsp->pb_ctx);
    }

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_statis %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_statis_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->get_role_statis_mgr().attach_role_statis(rsp->pb_count, rsp->pb_ctx);
    }

    add_step();
    
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_ext_attr %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}


int loader_role::on_role_ext_attr_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    _role->get_player_role_ext_attr_mgr().attach(rsp->pb_count, rsp->pb_ctx);

    add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_mounts %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_mounts_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    _role->get_mounts().attach(rsp->pb_count, rsp->pb_ctx);

    add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_potential %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}


int loader_role::on_role_potential_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
	if (_role == NULL) {
		return -1;
	}

	if (rsp->pb_count > 0) {
		_role->get_potentials().attach_role_potential(rsp->pb_count,rsp->pb_ctx);
	}

	add_step();

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "get mysql.tb_role_auto_raid %u 0", _role->get_role_id());
    GET_READ_SQLMGR_NAME(_role->get_role_id());
    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_auto_raid_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
    if (_role == NULL) {
        return -1;
    }

    if (rsp->pb_count > 0) {
        _role->attach_role_auto_raid(rsp->pb_count,rsp->pb_ctx);
    }

    add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_cd_cooldown %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_cd_cooldown_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
	if (_role == NULL) {
		return -1;
	}

	if (rsp->pb_count > 0) {
		_role->get_role_cds().attach_role_cd_cooldown(rsp->pb_count,rsp->pb_ctx);
	}

	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_treasure %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_treasure_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
	if (_role == NULL) {
		return -1;
	}

	if (rsp->pb_count > 0) {
		_role->get_role_treasures().attach_role_treasure(rsp->pb_count,rsp->pb_ctx);
	}

	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_target %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_target_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
	if(_role == NULL)
	{
		return -1;
	}
	if(rsp->pb_count > 0)
	{
		_role->get_player_role_target().attach_role_target(rsp->pb_count, rsp->pb_ctx);
	}
	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_vip %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_vip_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
	if(_role == NULL)
	{
		return -1;
	}
	if(rsp->pb_count > 0)
	{
		_role->get_player_role_vip().attach_role_vip(rsp->pb_count, rsp->pb_ctx);
	}
	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_lottery %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_lottery_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
	if(_role == NULL)
	{
		return -1;
	}
	if(rsp->pb_count > 0)
	{
		_role->get_player_role_lottery().attach_role_lottery(rsp->pb_count, rsp->pb_ctx);
	}
	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_black_shop %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_black_shop_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp)
{
	if(_role == NULL){
		return -1;
	}

	if(rsp->pb_count > 0)
	{
		_role->get_player_role_black_shop().attach_role_black_shop(rsp->pb_count, rsp->pb_ctx);
	}

	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_levels %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}


int loader_role::on_role_levels_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp)
{
	if(_role == NULL){
		return -1;
	}
	if(rsp->pb_count > 0)
	{
		_role->get_player_role_levels().attach_role_levels(rsp->pb_count, rsp->pb_ctx);
	}
	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_gem %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);

}

int loader_role::on_role_gem_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp)
{
	if(_role == NULL){
		return -1;
	}
	if(rsp->pb_count > 0)
	{
		_role->get_player_role_gems().attach_role_gem(rsp->pb_count, rsp->pb_ctx);
	}

	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_bows %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);

}

int loader_role::on_role_bows_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp)
{

	if(_role == NULL){
		return -1;
	}
	if(rsp->pb_count > 0)
	{
		_role->get_player_role_bows().attach(rsp->pb_count, rsp->pb_ctx);
	}

	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_cycle %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_cycle_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp)
{

	if(_role == NULL){
		return -1;
	}
	if(rsp->pb_count > 0)
	{
		_role->get_role_cycle().attach_role_cycle(rsp->pb_count, rsp->pb_ctx);
	}

	add_step();

	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "get mysql.tb_role_devil %u 0", _role->get_role_id());
	GET_READ_SQLMGR_NAME(_role->get_role_id());
	return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, cmd, strlen(cmd)+1);
}

int loader_role::on_role_devil_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp)
{

	if(_role == NULL){
		return -1;
	}

	if(rsp->pb_count > 0)
	{
		_role->get_role_devil().attach_role_devil(rsp->pb_count, rsp->pb_ctx);
	}

	add_step();

	return 0;
}
