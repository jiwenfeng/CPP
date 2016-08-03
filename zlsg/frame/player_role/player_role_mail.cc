#include "player_role_mail.h"

bool get_item_attr(uint32 item_id, uint32 item_count,uint32& attr_type,uint32& attr_value)
{
	switch(item_id)
	{
	case 5001://铜币
		{
			attr_type = TAEL;
		}
		break;
	case 5002:// 元宝
		{
			attr_type = INGOT;
		}
		break;
	case 5003:// 军贡
		{
			attr_type = CAN_JOIN_FAMILY;
		}
		break;
	case 5004:// 威望
		{
			attr_type = POPULARITY;
		}
		break;
	case 5005:// 谋略
		{
			attr_type = STRATEGY;
		}
		break;
	case 5006:// 韬略
		{
			attr_type = MILITARY_STRATEGY;
		}
		break;
	case 5007:// 体力
		{
			attr_type = PHYSICAL_POWER;
		}
		break;
	default:
		return false;
		break;
	}

	attr_value = item_count;

	return true;
}

player_role_mail::player_role_mail(mysql::tb_role_mail* m) : db_role_mail(m)
{
    if (db_role_mail != NULL){
        parse_mail(db_role_mail->mail());   
    }
}


player_role_mail::~player_role_mail()
{
   if (db_role_mail != NULL){
       delete db_role_mail;
       db_role_mail = NULL;
   } 

}

int player_role_mail::set_db_mail(mysql::tb_role_mail* m)
{
    if (m == db_role_mail){
        return -1;
    }

    if (db_role_mail != NULL){
        delete db_role_mail;
        db_role_mail = NULL;
    }

    db_role_mail = m;
    return parse_mail(db_role_mail->mail());
}

int player_role_mail::add_mail(uint32 role_id, const client::mail_info& mi)
{
    if (db_role_mail == NULL){
        db_role_mail = new mysql::tb_role_mail;
        db_role_mail->set_role_id(role_id);
        writer_type = WOTE_INSERT;
    }

    role_mail_info new_mail(mi);
    client::mail_head* mh = new_mail.mutable_mh();
    mh->set_mail_id(mail_list.size() + 1);

    mail_list.push_back(new_mail);

    return mh->mail_id();
}

void player_role_mail::proc_mail_list_req(client::mail_list_rsp& rsp)
{
	rsp.set_update_type(MAIL_LIST_UPDATE_TYPE_ALL);
    uint32 size = mail_list.size();
    for (uint32 i = 0; i < size; ++i){
        role_mail_info& mi = mail_list[i];
        if (!mi.is_normal()){
            continue;
        }

        client::mail_head* mh = mi.mutable_mh();
        mh->set_mail_id(i+1);

        client::mail_head* rsp_mh = rsp.add_mh();
        *rsp_mh = *mh;
    }
}

bool player_role_mail::proc_mail_open_req(uint32 mail_id, client::mail_open_rsp& rsp)
{
    role_mail_info* mi = get_mail(mail_id);
    if (mi == NULL || !mi->is_normal()){
        return false;
    }
	// 设置邮件已阅状态
    mi->mutable_mh()->set_opened(true);

    rsp.set_mail_id(mail_id);
    if (mi->has_mb()){
        *rsp.mutable_mb() = mi->mb();
    }

	for (int i = 0; i < mi->attr_size(); i++){
		client::mail_attr* attr = rsp.add_attr();
		attr->set_attr_type(mi->attr(i).attr_type());
		attr->set_attr_value(mi->attr(i).attr_value());
	}

    for (int i = 0; i < mi->ma_size(); ++i){
		client::props_info* info = rsp.add_ma();
		info->CopyFrom(mi->ma(i));
    }

	return true;
}

void player_role_mail::proc_mail_remove_req(client::remove_mail_req& rmr, client::remove_mail_rsp& rsp)
{
    uint32 size = rmr.mail_id_size();
    uint32 max_mail_id = mail_list.size();
	bool is_attach = true;
	bool is_sys_err = true;
    for (uint32 i = 0; i < size; ++i){
		uint32 mail_id = rmr.mail_id(i);
        if (mail_id == 0 || mail_id > max_mail_id){
            is_sys_err = false;
            continue;
        }

        role_mail_info& mi = mail_list[mail_id - 1];
        if (!mi.is_normal()){
            is_sys_err = false;
            continue;
        }

		if (check_attachment_time_out(mi.mh().mail_id())){
			// 还没过期
			// 有附件无法删除
			if (mi.mh().has_is_take() && !mi.mh().is_take()){
				is_attach  = false;
				continue;
			}
		}

		// 修改标识，保存数据时候根据标识，不保存已经删除的邮件
        mi.remove();
    }

	if (is_attach || is_sys_err){
		rsp.set_ret(client::remove_mail_rsp_REMOVE_RET_RRET_SUCCESS);
	}

	if (!is_attach){
		rsp.set_ret(remove_mail_rsp_REMOVE_RET_RRET_ATTACH_ERR);
	}

	if (!is_sys_err){
		rsp.set_ret(remove_mail_rsp_REMOVE_RET_RRET_SYS_ERR);
	}
}

role_mail_info* player_role_mail::get_mail(uint32 mail_id)
{
    uint32 size = mail_list.size();
    if (mail_id == 0 || mail_id > size){
        return NULL;
    }

    return &(mail_list[mail_id - 1]);
}

uint32 get_mail_type_time(uint32 mail_type)
{
	uint32 past_time = 0;

	switch(mail_type)
	{
	case MAIL_TYPE_SYSTEM:// 系统
		{
			past_time = CONFMGR->get_basic_value(10102);
		}
		break;
	case MAIL_TYPE_PLAYER: // 玩家邮件
		{
			past_time = CONFMGR->get_basic_value(10103);
		}
		break;
	case MAIL_TYPE_ADMIN: // 管理员邮件
		{
			past_time = CONFMGR->get_basic_value(10104);
		}
		break;
	case MAIL_TYPE_FAMILY: // 同盟邮件邀请
		{
			past_time = CONFMGR->get_basic_value(10105);
		}
		break;
	case MAIL_TYPE_ARMY: // 其余军团邮件（同盟拒绝通知，敌盟通知）
		{
			past_time = CONFMGR->get_basic_value(10106);
		}
		break;
	default:
		{
			past_time = CONFMGR->get_basic_value(10102);
		}
		break;
	}

	return past_time;
}
// 检测附件是否过期
bool player_role_mail::check_attachment_time_out(uint32 mail_id)
{
	role_mail_info* mi = get_mail(mail_id);
	if (mi == NULL || !mi->is_normal()){
		return false;
	}

	uint32 need_check_time = mi->mh().past_time() - get_mail_type_time(mi->mh().mail_type());

	uint32 now = (uint32)time(NULL);

	if (now >= need_check_time){
		return false;
	}

	return true;
}
// 附件提取操作
int player_role_mail::proc_extract_attachment(uint32 mail_id, vector<client::props_info>& pis, std::vector<client::mail_attr>& attr)
{
    role_mail_info* mi = get_mail(mail_id);
    if (mi == NULL || !mi->is_normal()){
        return -1;
    }

    uint32 size = mi->ma_size();
	std::vector<client::mail_attr> attr_vec;
	attr_vec.clear();
	uint32 attr_type = 0;
	uint32 attr_value = 0;
    for (uint32 i = 0; i < size; ++i){
        const client::props_info& ma = mi->ma(i);

		if (get_item_attr(ma.props_id(),ma.count(),attr_type,attr_value)){
			client::mail_attr attr_info;
			attr_info.set_attr_type(attr_type);
			attr_info.set_attr_value(attr_value);
			attr_vec.push_back(attr_info);
			continue;
		}

        //client::props_info& pi = pis.at(i);
        //pi.CopyFrom(ma);
		pis.push_back(ma);
    }

	uint32 attr_size = mi->attr_size();
	//attr.resize(attr_size);
	for (uint32 j = 0; j < attr_size; ++j){
		//attr[j].set_attr_type(mi->attr(j).attr_type());
		//attr[j].set_attr_value(mi->attr(j).attr_value());
		//attr[j].CopyFrom(mi->attr(j));
		attr.push_back(mi->attr(j));
	}
	
	if (attr_vec.size() != 0){
		for (uint32 k = 0; k < attr_vec.size(); ++k){
			attr.push_back(attr_vec[k]);
		}
	}

    return 0;
}

bool player_role_mail::set_is_take_status(uint32 mail_id)
{
	role_mail_info* mi = get_mail(mail_id);

	if (mi == NULL || !mi->is_normal()){
		return false;
	}

	mi->mutable_mh()->set_is_take(true);

	return true;
}
bool player_role_mail::has_take(uint32 mail_id)
{
	role_mail_info* mi = get_mail(mail_id);

	if (mi == NULL || !mi->is_normal()){
		return false;
	}

	return mi->mh().is_take();
}
int player_role_mail::save_mail(amtsvr_context* ctx)
{
    if (db_role_mail == NULL){
        return -1;
    }

    mysql::tb_role_mail* tmc = new mysql::tb_role_mail(*db_role_mail);
    CACHE_KEY k(tmc->role_id(), 0);
    string out;
    serialize_mail(out);
    tmc->set_mail(out);
    write(ctx, k, tmc);
    writer_type = WOTE_UPDATE;

    return 0;
}


