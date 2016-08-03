#include "config_legion.h"
#include "family_mgr.h"
#include "config_flag.h"
family_mgr::family_mgr()
{
}

family_mgr::~family_mgr()
{
    map<uint32, family*>::iterator i = _family.begin();
    for (; i != _family.end(); ++i) {
        delete i->second;
    }
    _family.clear();
    
    i = _remove.begin();
    for (; i != _remove.end(); ++i) {
        delete i->second;
    }
    _remove.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int family_mgr::init(struct cmq_t* l)
{
    init_family_fsid();

    CUR* cur = (CUR*)CONFMGR->get_family_head();
    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_family* tf = static_cast<mysql::tb_family*>(p);
        cur = (CUR*)cur->get_next();
        if (tf == NULL) {
            continue;
        }

        if (set_family_fsid(*tf) != 0) {
            delete tf;
            continue;
        }

        if (tf->status() != 0) {
            family* f = new family(tf);
            _family.insert(pair<uint32, family*>(tf->id(), f));
        } else {
            delete tf;
        }
    }

    _member_mgr.init(l);
    _log_mgr.init(l);
    _skill_mgr.init(l);
    _record_mgr.init(l);
    _domain_mgr.init(l);
	_diplomacy_mgr.init(l);
    map<uint32, family*>::iterator i = _family.begin();
    while (i != _family.end()) {
        if (_member_mgr.member_num(i->first) == 0) {
            if (i->second != NULL) {
                i->second->remove();
                _remove.insert(pair<uint32, family*>(i->second->get()->id(), i->second));
            }
            _family.erase(i++);
        } else {
            ++i;
        }
    }
        
    return 0;
}

int family_mgr::init_family_fsid()
{
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = CONFMGR->get_gdata_max_role_head();
    while (cur != NULL){
        const mysql::sp_get_gdata_max_role* sggmr = static_cast<const mysql::sp_get_gdata_max_role*>(cur->get_val().get_msg());
        cur = cur->get_next();
        family_fsid& ffs = _fsids[sggmr->svr_id()];
        ffs.set(sggmr->svr_id(), 0);
    }

    return 0;
}

int family_mgr::set_family_fsid(const mysql::tb_family& tf)
{
    uint32 sfid = tf.id();
    uint32 svr_id = GET_SVR_ID(sfid);
    map<uint32, family_fsid>::iterator it = _fsids.find(svr_id);
    if (it == _fsids.end()){
        return -1;
    }

    uint32 max_fid = it->second.get_family_id();
    uint32 fid = GET_FAMILY_ID(sfid);
    if (fid > max_fid){
        it->second.set(svr_id, fid);
    }

    return 0;
}

int family_mgr::save(amtsvr_context* ctx)
{
    map<uint32, family*>::iterator i = _family.begin();
    for (; i != _family.end(); ++i) {
        i->second->save(ctx);
    }

    i = _remove.begin();
    for (; i != _remove.end(); ++i) {
        i->second->save(ctx);
        delete i->second;
    }
    _remove.clear();

    _member_mgr.save(ctx);
    _log_mgr.save(ctx);
    _skill_mgr.save(ctx);
    _record_mgr.save(ctx);
    _domain_mgr.save(ctx);
	_diplomacy_mgr.save(ctx);

    return 0;
}

int family_mgr::family_list(uint32 city_family,client::family_list_req* req, client::family_list_rsp& rsp)
{
    map<uint32, family*>::iterator i = _family.begin();
    for (; i != _family.end(); ++i) {
		client::family_info* info = rsp.add_family();
        i->second->family_info(info);
		if (i->second->get_family_id() == city_family){
			info->set_king(true);
		}
    }

    return 0;
}

int family_mgr::family_title_list(client::family_title_list_rsp& rsp)
{
	 map<uint32, family*>::iterator i = _family.begin();
	 for (; i != _family.end(); ++i) {
		 client::family_title_info* info = rsp.add_info();
		 i->second->family_title_info(info);
	 }

	return 0;
}

int family_mgr::family_title(uint32 family_id ,client::family_title_list_rsp& rsp)
{
	 map<uint32, family*>::iterator i = _family.find(family_id);
	 if (i == _family.end()) {
		 return -1;
	 }

	 client::family_title_info* info = rsp.add_info();
	 i->second->family_title_info(info);

	 return 0;
}

bool family_mgr::check_family(uint32 family_id,uint32 role_id)
{
	map<uint32, family*>::iterator i = _family.find(family_id);
	if (i == _family.end()) {
		printf("family_mgr::check_family ERRO!!!!!!!!!!!!!!");
		return false;
	}

	uint32 cur_family_id = 0;
	cur_family_id = _member_mgr.family_id(role_id);

	if (cur_family_id == 0 || cur_family_id != i->first){
		printf("family_mgr::check_family ERRO!!!!!!!!!!!!!!");
		return false;
	}

	return true;
}

int family_mgr::family_info(client::family_req* req, client::family_rsp& rsp)
{
    for (int size = 0; size < req->family_id_size(); ++size) {
        map<uint32, family*>::iterator i = _family.find(req->family_id(size));
        if (i == _family.end()) {
            continue;
        }
        
        client::family_info* fi = rsp.add_fi();
        i->second->family_info(fi);
        fi->set_cur_member_num(_member_mgr.member_num(req->family_id(size)));
    }

    return 0;
}

int family_mgr::family_info(uint32 family_id, client::donate_family_rsp& rsp)
{
    client::family_info* fi = rsp.mutable_info();
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return -1;
    }
    
    i->second->family_info(fi);
    fi->set_cur_member_num(_member_mgr.member_num(family_id));

    return 0;
}


bool family_mgr::generate_family_id(uint32 svr_id, uint32& fid)
{
    if (_fsids.size() == 1 && _fsids.find(0) != _fsids.end()) {
        _fsids[svr_id].set(svr_id, 0);
    }

    map<uint32, family_fsid>::iterator it = _fsids.find(svr_id);
    if (it == _fsids.end()){
        return false;
    }

    fid = it->second.get();
    if (fid == 0){
        return false;
    }

    return true;
}

int family_mgr::create_family(client::create_family_req* req, const std::string create_name,client::create_family_rsp& rsp, uint32& family_id,uint32 create_power)
{
    client::create_family_rsp_result ret = client::create_family_rsp_result_success;

	uint32 has_family_id = 0;
	has_family_id = _member_mgr.family_id(req->svr_head().role_id());
	if (has_family_id != 0){
		rsp.set_ret(client::create_family_rsp_result_have_limit);
		return -1;
	}
	
	string s_1 = req->name();
	const size_t pos = s_1.find(" ");
	if (pos != std::string::npos){
		rsp.set_ret(client::create_family_rsp_result_name_limit);
		return -1;
	}

	string s_2 = req->flag_name();
	const size_t pos_2 = s_2.find(" ");
	if (pos_2 != std::string::npos){
		rsp.set_ret(client::create_family_rsp_result_name_limit);
		return -1;
	}

	// 检测军团名字
    if (req->name().length() < 1|| req->flag_name().length() > 18 || check_name(req->name())) {
        ret = client::create_family_rsp_result_name_limit;
    }
	// 检测战旗名字
    if (req->flag_name().length() < 1 || req->flag_name().length() > 18 || check_flag_name(req->flag_name())) {
        ret = client::create_family_rsp_result_flag_limit;
    }


    if (ret != client::create_family_rsp_result_success) {
        rsp.set_ret(ret);
        rsp.set_family_id(0);
        return -1;
    }

    if (!generate_family_id(req->svr_head().svr_id(), family_id)) {
        rsp.set_ret(client::create_family_rsp_result_name_limit);
        rsp.set_family_id(0);
        return -1;
    }

    uint32 role_id = req->svr_head().role_id();
	
    family* f = new family;
    f->create(family_id, req->type(), req->name(), req->flag_id(), req->flag_name(), req->svr_head().role_id(),create_name);

    rsp.set_ret(client::create_family_rsp_result_success);
    rsp.set_family_id(family_id);
    _family.insert(pair<uint32, family*>(family_id, f));
    _member_mgr.add_role(role_id, family_id, role_id, CHIEF);
	// 更新info 当前人数
	uint32 cur_member = _member_mgr.member_num(family_id);
	f->set_cur_member_num(cur_member);
	// 更新info 当前战斗力
	add_family_info_power(family_id,role_id,create_power);
	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
		_log_mgr.insert(family_id, FAMILY_CREATE, role_id, family_id,req->name(),log_id);
	}

	reset_family_power(family_id);

    return 0;
}
// 申请入团
int family_mgr::apply_family(client::apply_family_req* req, client::apply_family_rsp& rsp,uint32 fight_power,client::approval_family_rsp& rsp_msg)
{
    uint32 family_id = req->family_id();
    rsp.set_family_id(family_id);
	rsp_msg.set_ret(approval_family_rsp_result_reject_join);
	rsp_msg.set_family_id(family_id);
	rsp_msg.set_type(1);
	rsp_msg.set_role_id(req->svr_head().role_id());

    family* f = get_family(family_id);

    if (f == NULL || f->get() == NULL) {
		rsp.set_ret(client::apply_family_rsp_result_not_family);
		return -1;
    }

	family* apply_info = get_family_by_role_id(req->svr_head().role_id());
	
	if (apply_info != NULL){
		rsp.set_ret(client::apply_family_rsp_result_have_family);
		return -1;
	}

	if (_member_mgr.is_already_apply(family_id,req->svr_head().role_id()) == 0){
		rsp.set_ret(client::apply_family_rsp_result_already_apply);
		return -1;
	}

	uint32 apply_limit = CONFMGR->get_basic_value(10076);

    if (_member_mgr.get_apply_count(f->get_family_id()) >= apply_limit) {
        rsp.set_ret(client::apply_family_rsp_result_apply_count);
        return -2;
    }

    mysql::tb_family* tf = f->get();
	if (tf == NULL){
		rsp.set_ret(client::apply_family_rsp_result_not_family);
		return -1;
	}

	const config_legion* base_info = CONFMGR->get_config_legion_mgr().get_config_legion(tf->lvl());
	if (base_info == NULL){
		rsp.set_ret(client::apply_family_rsp_result_not_family);
		return -1;
	}

    if ( _member_mgr.member_num(family_id) >= static_cast<uint32>(base_info->get_member_limit())) {
        rsp.set_ret(client::apply_family_rsp_result_full_family);
        return -3;
    }
    
	// 自动成为成员
	if (f->get_approval() == 1){
		_member_mgr.auto_add(req->svr_head().role_id(),family_id,req->svr_head().role_id());
		uint32 member_num = _member_mgr.member_num(req->family_id());
		f->set_cur_member_num(member_num);
		add_family_info_power(family_id,req->svr_head().role_id(),fight_power);
		reset_family_power(family_id);
		uint32 log_id = 0;
		if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
			_log_mgr.insert(family_id,FAMILY_INVITE_MEMBER,req->svr_head().role_id(),family_id,f->get_name(),log_id);
		}
		rsp_msg.set_ret(approval_family_rsp_result_success);
		rsp_msg.set_family_id(family_id);
		rsp_msg.set_role_id(req->svr_head().role_id());
		rsp_msg.set_type(0);
	}
	else{
		_member_mgr.apply(req->svr_head().role_id(), req->family_id(), req->svr_head().role_id());
	}
    rsp.set_ret(client::apply_family_rsp_result_success);
    
    return 0;
}
// 查看审核列表
int family_mgr::apply_family_list(client::apply_family_list_req* req, client::apply_family_list_rsp& rsp)
{
	if (!check_family(req->family_id(),req->svr_head().role_id())){
		return -1;
	}

    rsp.set_family_id(req->family_id());
    _member_mgr.fill_member_apply(req->family_id(), rsp);

    return 0;
}
// 自己申请的军团列表
int family_mgr::apply_family_list_self(client::apply_family_list_self_req* req, client::apply_family_list_self_rsp& rsp)
{
    _member_mgr.fill_member_apply_self(req->svr_head().role_id(), rsp);

    return 0;
}
// 审批处理
int family_mgr::approval_family(client::approval_family_req* req, client::approval_family_rsp& rsp,uint32 fight_power)
{
    rsp.set_family_id(req->family_id());
    rsp.set_role_id(req->role_id());
    rsp.set_type(req->type());

	if (!check_family(req->family_id(),req->svr_head().role_id())){
		rsp.set_ret(client::approval_family_rsp_result_permissions);
		return -1;
	}

    family* f = get_family(req->family_id());
    if (req->family_id() == 0 || f == NULL) {
        rsp.set_ret(client::approval_family_rsp_result_permissions);
        return -1;    
    }
	
	if (_member_mgr.get_permissions(req->family_id(), req->svr_head().role_id()) < VICE_CHIEF){
		rsp.set_ret(client::approval_family_rsp_result_permissions);
		return -1;
	}

    if (req->role_id() == req->svr_head().role_id()) {
        rsp.set_ret(client::approval_family_rsp_result_have_family);
        return -2;
    }

    if (_member_mgr.family_id(req->role_id()) != 0) {
        rsp.set_ret(client::approval_family_rsp_result_have_family);
        return -3;
    }

    mysql::tb_family* tf = f->get();
	if (tf == NULL){
		rsp.set_ret(client::approval_family_rsp_result_full_family);
		return -4;
	}

	const config_legion* base_info = CONFMGR->get_config_legion_mgr().get_config_legion(tf->lvl());
	if (base_info == NULL){
		rsp.set_ret(client::approval_family_rsp_result_full_family);
		return -4;
	}

    if (tf == NULL || _member_mgr.member_num(req->family_id()) >= static_cast<uint32>(base_info->get_member_limit())) {
        rsp.set_ret(client::approval_family_rsp_result_full_family);
        return -4;
    }
    
	// 允许
    _member_mgr.join(req->family_id(), req->role_id());

	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
		_log_mgr.insert(req->family_id(), FAMILY_INVITE_MEMBER, req->role_id(), req->family_id(),f->get_name(),log_id);
	}

	uint32 cur_member = _member_mgr.member_num(req->family_id());
	f->set_cur_member_num(cur_member);
	add_family_info_power(req->family_id(),req->role_id(),fight_power);
	reset_family_power(req->family_id());

    rsp.set_ret(client::approval_family_rsp_result_success);

    return 0;
}

int family_mgr::reject_family(client::approval_family_req* req, client::approval_family_rsp& rsp)
{
	if (!check_family(req->family_id(),req->svr_head().role_id())){
		return -1;
	}
	rsp.set_family_id(req->family_id());
	rsp.set_role_id(req->role_id());
	rsp.set_type(req->type());

	family* f = get_family(req->family_id());
	if (req->family_id() == 0 || f == NULL) {
		rsp.set_ret(client::approval_family_rsp_result_permissions);
		return -1;    
	}

	if (_member_mgr.get_permissions(req->family_id(), req->svr_head().role_id()) < VICE_CHIEF){
		rsp.set_ret(client::approval_family_rsp_result_permissions);
		return -1;
	}

	if (req->role_id() == req->svr_head().role_id()) {
		rsp.set_ret(client::approval_family_rsp_result_have_family);
		return -2;
	}

	_member_mgr.remove(req->family_id(), req->role_id());

	rsp.set_ret(client::approval_family_rsp_result_success);

	return 0;
}
// 邀请进军团
int family_mgr::invite_family(client::invite_family_req* req, client::invite_family_rsp& rsp)
{
    uint32 self_id   = req->svr_head().role_id();
    uint32 family_id = req->family_id();
    uint32 role_id = req->role_id();
    rsp.set_family_id(family_id);
    rsp.set_role_id(role_id);

	if (!check_family(req->family_id(),req->svr_head().role_id())){
		rsp.set_ret(client::invite_family_rsp_result_null_family);
		return -1;
	}
    if (_member_mgr.family_id(role_id) != 0) {
        rsp.set_ret(client::invite_family_rsp_result_has_family);
        return -2;
    }

    family* f = get_family(family_id);
    if (f == NULL || f->get() == NULL) {
        rsp.set_ret(client::invite_family_rsp_result_null_family);
        return -1;
    }

	const config_legion* base_info = CONFMGR->get_config_legion_mgr().get_config_legion(f->lvl());
	if (base_info == NULL){
		rsp.set_ret(client::invite_family_rsp_result_null_family);
		return -1;
	}

    if (_member_mgr.member_num(family_id) >= static_cast<uint32>(base_info->get_member_limit())) {
        rsp.set_ret(client::invite_family_rsp_result_full_family);
        return -3;
    }

    if (_member_mgr.get_permissions(family_id, role_id) != INVITE) {
        _member_mgr.invite(self_id, family_id, role_id);
    }

    rsp.set_ret(client::invite_family_rsp_result_success);

    return 0;
}

int family_mgr::invite_family_list(client::invite_family_list_req* req, client::invite_family_list_rsp& rsp)
{
    _member_mgr.fill_member_invite(req->svr_head().role_id(), rsp);
    
    return 0;
}
// 同意别人邀请我进军团
int family_mgr::agree_family(client::agree_family_req* req, client::agree_family_rsp& rsp, uint32& referrer,uint32 fight_power)
{
    rsp.set_ret(client::agree_family_rsp_result_success);
    rsp.set_family_id(req->family_id());
    rsp.set_role_id(req->svr_head().role_id());
    rsp.set_type(req->type());

	//if (!check_family(req->family_id(),req->svr_head().role_id())){
	//	rsp.set_ret(client::agree_family_rsp_result_has_family);
	//	return -1;
	//}

	uint32 family_id = _member_mgr.family_id(req->svr_head().role_id());
	if (family_id != 0) {
		if (family_id != req->family_id()) {
			_member_mgr.remove(req->family_id(), req->svr_head().role_id());
		}

		rsp.set_ret(client::agree_family_rsp_result_has_family);
		return -1;
	} 

    referrer = _member_mgr.get_referrer(req->family_id(), req->svr_head().role_id());
    
    if (req->type() == 0) {
		family* f = get_family(req->family_id());
        _member_mgr.modify_role_permissions(req->family_id(), req->svr_head().role_id(), MEMBER);
		uint32 log_id = 0;
		if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
			 _log_mgr.insert(req->family_id(), FAMILY_INVITE_MEMBER, req->svr_head().role_id(), req->family_id(),f->get_name(),log_id);
		}

		// 更新军团人数
		uint32 cur_member = _member_mgr.member_num(req->family_id());
		f->set_cur_member_num(cur_member);
		// 添加军团的总战斗力
		add_family_info_power(req->family_id(),req->svr_head().role_id(),fight_power);
		reset_family_power(req->family_id());
    } else {
        _member_mgr.remove(req->family_id(), req->svr_head().role_id());
    }
        
    return 0;
}

int family_mgr::secede_family(client::secede_family_to_center* req, client::secede_family_rsp& rsp,uint32 fight_power)
{
	// 检测是否存在这个军团
	family* f = get_family(req->family_id());
	if (f == NULL || f->get() == NULL) {
		rsp.set_result_type(SECEDE_FAMILY_TYPE_FAILD);
		return -1;
	}
	// 检测是否军团长
	uint32 chief_idx = _member_mgr.chief_id(req->family_id());
	if (chief_idx == req->role_id()){
		rsp.set_result_type(SECEDE_FAMILY_TYPE_LEADER_FAILD);
		return -1;
	}

	_member_mgr.remove(req->family_id(), req->role_id());
	// 记录某人退出某军团
	//_log_mgr.insert(req->family_id(),FAMILY_QUIT_MEMBER,req->role_id(),req->family_id(),f->get_name());

	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->server_id(),log_id)){
		_log_mgr.insert(req->family_id(),FAMILY_QUIT_MEMBER,req->role_id(),req->family_id(),f->get_name(),log_id);
	}

	rsp.set_result_type(SECEDE_FAMILY_TYPE_SUCCESS);

	uint32 cur_member = _member_mgr.member_num(req->family_id());
	f->set_cur_member_num(cur_member);
	del_family_info_power(req->family_id(),fight_power);
	reset_family_power(req->family_id());
	return 0;
}

int family_mgr::expel_member(client::expel_member_req* req, client::expel_member_rsp& rsp)
{
	// 检测是否存在这个军团
	family* f = get_family(req->family_id());
	if (f == NULL || f->get() == NULL) {
		rsp.set_result_type(EXPEL_MEMBER_RESULT_FAMILY_FAILD);
		return -1;
	}

	family_role_value* member_info = _member_mgr.get_family_role(req->family_id(),req->svr_head().role_id());
	family_role_value* expel_info = _member_mgr.get_family_role(req->family_id(),req->evictee_id());
	if (member_info == NULL || expel_info == NULL){
		rsp.set_result_type(EXPEL_MEMBER_RESULT_MEMBER_FAILD);
		return -1;
	}

	// 副军团以下的不能T人
	if (member_info->_permissions  < VICE_CHIEF)
	{
		rsp.set_result_type(EXPEL_MEMBER_RESULT_PERMISSION);
		return -1;
	}

	if (member_info->_permissions == VICE_CHIEF){
		if (expel_info->_permissions > MEMBER){
			rsp.set_result_type(EXPEL_MEMBER_RESULT_PERMISSION);
			return -1;
		}
	}
	family_role_value* member_data = _member_mgr.get_family_role(req->family_id(),req->evictee_id());
	if (member_data == NULL){
		return -1;
	}

	uint32 evictee_power = member_data->_strength;
	_member_mgr.remove(req->family_id(),req->evictee_id());
	rsp.set_result_type(EXPEL_MEMBER_RESULT_SUCCEED);
	uint32 cur_member = _member_mgr.member_num(req->family_id());
	f->set_cur_member_num(cur_member);
	
	del_family_info_power(req->family_id(),evictee_power);
	// 删除该玩家的贡献记录数据
	uint32 log_id = 0;
	// 记录T人
	//_log_mgr.insert(req->family_id(),FAMILY_KCIK_MEMBER,req->svr_head().role_id(),req->evictee_id(),f->get_name());
	if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
		_log_mgr.insert(req->family_id(),FAMILY_KCIK_MEMBER,req->svr_head().role_id(),req->evictee_id(),f->get_name(),log_id);
	}
	return 0;
}

// 修改公告
int family_mgr::modify_family_post(client::modify_family_post_req* req, client::modify_family_post_rsp& rsp)
{
    map<uint32, family*>::iterator i = _family.find(req->family_id());
    if (i == _family.end()) {
        return -1;
    }
	// 只有军团长能修改公告
	uint32 chief_id = _member_mgr.chief_id(req->family_id());
	if (chief_id != req->svr_head().role_id()){
		return -1;
	}

	if (req->has_post()){
		if (req->post().length() > 90){
			rsp.set_ret(modify_family_post_rsp_result_too_long);
			return -1;
		}
	}

    rsp.set_family_id(req->family_id());
    rsp.set_ret(client::modify_family_post_rsp_result_success);
                                   
    if (req->has_post()) {
        i->second->set_post(req->post());
        rsp.set_post(req->post());
		uint32 log_id = 0;
		if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
			_log_mgr.insert(req->family_id(), FAMILY_MODIFY_POST, req->svr_head().role_id(), req->family_id(),i->second->get_name(),log_id);
		}
        //_log_mgr.insert(req->family_id(), FAMILY_MODIFY_POST, req->svr_head().role_id(), req->family_id(),i->second->get_name());
    }

    if (req->has_qq()) {
        i->second->set_qq(req->qq());
        rsp.set_qq(req->qq());
    }

    if (req->has_yy()) {
        i->second->set_yy(req->yy());
        rsp.set_yy(req->yy());
    }
    
    return 0;
}

int family_mgr::family_member_list(client::family_member_list_req* req, client::family_member_list_rsp& rsp)
{
    rsp.set_family_id(req->family_id());
    _member_mgr.fill_member(req->family_id(), rsp);

    return 0;
}

int family_mgr::family_member_list(uint32 family_id, vector<uint32> &v)
{
	return _member_mgr.fill_member(family_id, v);
}

int family_mgr::modify_family_member(client::modify_family_member_req* req, client::modify_family_member_rsp& rsp,uint32 fight_power,string role_name,uint32& be_change_id)
{
    rsp.set_family_id(req->family_id());
    rsp.set_role_id(req->role_id());
    rsp.set_type(req->type());

	if (!check_family(req->family_id(),req->svr_head().role_id())){
		return -1;
	}

	map<uint32, family*>::iterator i = _family.find(req->family_id());
	if (i == _family.end()) {
		return -1;
	}

    if (req->role_id() != req->svr_head().role_id()) {
        uint32 src_title = get_family_title(req->svr_head().role_id(), req->family_id());
        uint32 dst_title = get_family_title(req->role_id(), req->family_id());
        if (src_title == 0 || dst_title == 0 || (src_title < req->type() && req->type() != 0) || src_title <= dst_title || src_title < VICE_CHIEF) {
            rsp.set_ret(client::modify_family_member_rsp_result_permissions);
            return -1;
        }
    }
    
    if (req->type() != 0 && req->type() <= CHIEF) {
        //if (!check_permissions_num(req->family_id(), req->type())) {// 检测是否可以设置该头衔，头衔的空位够不够
        //    rsp.set_ret(client::modify_family_member_rsp_result_permissions);
        //    return -2;
        //}
		
		be_change_id = _member_mgr.permission_id(req->family_id(),req->type());

        if (req->role_id() == req->svr_head().role_id()) {
            rsp.set_ret(client::modify_family_member_rsp_result_failure);
            return -2;
        }
		
        int ret = _member_mgr.modify_role_permissions(req->family_id(), req->role_id(), req->type());
        if (ret == 0 && req->type() == CHIEF) {// 任命req->role_id() 为军团长
            _member_mgr.modify_role_permissions(req->family_id(), req->svr_head().role_id(), MEMBER);
			// 设置军团长
            set_chief(req->family_id(), req->role_id(),role_name);
        }else if (ret == 0 && be_change_id != 0){
			_member_mgr.modify_role_permissions(req->family_id(), be_change_id, MEMBER);
        }
    } else {
        if (req->role_id() == get_chief(req->family_id())) {
            rsp.set_ret(client::modify_family_member_rsp_result_failure);
            return -3;
        }
        // 踢req->role_id() 出军团
        _member_mgr.remove(req->family_id(), req->role_id());
        //reset_family_power(req->family_id());
    }

    rsp.set_ret(client::modify_family_member_rsp_result_success);

	if (req->type() == 0){
		reset_family_power(req->family_id());
	}

	//_log_mgr.insert(req->family_id(),FAMILY_PROMOTION,req->role_id(),req->type(),i->second->get_name());
	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
		_log_mgr.insert(req->family_id(),FAMILY_PROMOTION,req->role_id(),req->type(),i->second->get_name(),log_id);
	}
    return 0;
}

int family_mgr::disband_family(client::disband_family_req* req, client::disband_family_rsp& rsp)
{
    uint32 family_id = 0;//req->family_id();
    if (family_id == 0) {
        family_id = _member_mgr.family_id(req->svr_head().role_id());
    }

	if (family_id != req->family_id()){
		rsp.set_ret(client::disband_family_rsp_result_family_id_err);
		return -1;
	}

    rsp.set_family_id(family_id);
    uint32 chief = get_chief(family_id);
    if (chief != req->svr_head().role_id()) {
        rsp.set_ret(client::disband_family_rsp_result_permissions);
        return -1;
    }

	// 移除跟他有关系的数据
	_diplomacy_mgr.update_diplomacy_relation(family_id);

    remove(family_id);


    rsp.set_ret(disband_family_rsp_result_success);
 
    return 0;
}

void family_mgr::mod_family_money(uint32 family_id, uint32 num)
{
	if(family_id == 0)
	{
		return;
	}
	map<uint32, family *>::iterator i = _family.find(family_id);
	if(i == _family.end())
	{
		return;
	}
	
	i->second->set_money(num);
}

int family_mgr::donate_family(client::set_family_money* req,client::set_donate_consume& rsp_msg)
{
    uint32 family_id = 0;
    if (family_id == 0) {
        family_id = _member_mgr.family_id(req->role_id());
    }
    
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return -1;
    }

	const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(i->second->lvl());
	if (legion_info == NULL){
		return -1;
	}
	// 军团获得的捐献值
	uint32 donate_value = 0;
    FAMILY_LOG_TYPE type;
    switch (req->type()) {
    case DONATE_TYPE_COIN: // 铜币
		{
			const std::map<int,int>& donation_coin_map_ = legion_info->get_donation_5();
			std::map<int,int>::const_iterator it_coin = donation_coin_map_.find(req->money());
			if (it_coin == donation_coin_map_.end()){
				return -1;
			}

			donate_value = static_cast<uint32>(it_coin->second);
			type = FAMILY_COIN_DONATE; 
			_member_mgr.set_money(family_id,req->role_id(),req->money());
			i->second->add_money(req->money());
			
		}
		break;
    case DONATE_TYPE_DRAGON: // 青龙令
		{
			const std::map<int,int>& donation_dragon_map_ = legion_info->get_donation_1();
			std::map<int,int>::const_iterator it_dragon = donation_dragon_map_.find(50145001);
			if (it_dragon == donation_dragon_map_.end()){
				return -1;
			}

			donate_value = static_cast<uint32>(it_dragon->second) * req->money();
			type = FAMILY_DRAGON_DONATE; 
			_member_mgr.set_dragon_count(family_id,req->role_id(),req->money());
			i->second->set_dragon_count(i->second->get()->dragon_count() + req->money());
		}
		break;
    case DONATE_TYPE_TIGER:// 白虎令
		{
			const std::map<int,int>& donation_tiger_map_ = legion_info->get_donation_4();
			std::map<int,int>::const_iterator it_tiger = donation_tiger_map_.find(50145004);
			if (it_tiger == donation_tiger_map_.end()){
				return -1;
			}

			donate_value = static_cast<uint32>(it_tiger->second) * req->money();
			type = FAMILY_TIGER_DONATE; 
			_member_mgr.set_tiger_count(family_id,req->role_id(),req->money());
			i->second->set_tiger_count(i->second->get()->tiger_count()+ req->money());
		}
		break;
	case DONATE_TYPE_ROSEFINCH:// 朱雀令
		{
			const std::map<int,int>& donation_rosefinch_map_ = legion_info->get_donation_2();
			std::map<int,int>::const_iterator it_rosefinch = donation_rosefinch_map_.find(50145002);
			if (it_rosefinch == donation_rosefinch_map_.end()){
				return -1;
			}

			donate_value = static_cast<uint32>(it_rosefinch->second) * req->money();
			type = FAMILY_ROSEFINCH_DONATE; 
			_member_mgr.set_rosefinch_count(family_id,req->role_id(),req->money());
			i->second->set_rosefinch_count(i->second->get()->rosefinch_count() + req->money());
		}
		break;
	case DONATE_TYPE_TORROISE: // 玄武令
		{
			const std::map<int,int>& donation_torroise_map_ = legion_info->get_donation_3();
			std::map<int,int>::const_iterator it_torroise = donation_torroise_map_.find(50145003);
			if (it_torroise == donation_torroise_map_.end()){
				return -1;
			}

			donate_value = static_cast<uint32>(it_torroise->second) * req->money();
			type = FAMILY_TORROISE_DONATE; 
			_member_mgr.set_tortoise_count(family_id,req->role_id(),req->money());
			i->second->set_tortoise_count(i->second->get()->tortoise_count() + req->money());
		}
		break;
    default:
		{
			const std::map<int,int>& donation_coin_map_ = legion_info->get_donation_5();
			std::map<int,int>::const_iterator it_coin = donation_coin_map_.find(req->money());
			if (it_coin == donation_coin_map_.end()){
				return -1;
			}

			donate_value = static_cast<uint32>(it_coin->second);
			type = FAMILY_COIN_DONATE; 
			i->second->add_money(req->money());
		}
		break;
    }

	_member_mgr.set_total_donate(family_id,req->role_id(),donate_value);

    uint32 total_donate = _member_mgr.get_member_total_donate(family_id,req->role_id());
	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->server_id(),log_id)){
		_log_mgr.insert(family_id, type, req->role_id(), donate_value,i->second->get_name(),log_id);
	}
	// 捐献记录插入数据
    //_log_mgr.insert(family_id, type, req->role_id(), donate_value,i->second->get_name());
   
	//捐献完后返回game去扣除相关东西
	rsp_msg.set_type(req->type());
	rsp_msg.set_count(req->money());
	rsp_msg.set_contr_value(donate_value);
	rsp_msg.set_is_success(true);
	rsp_msg.set_total_value(total_donate);
	rsp_msg.set_role_id(req->role_id());
	rsp_msg.set_money(req->money());
    return 0;
}

int family_mgr::upgrade_family(client::upgrade_family_req* req, client::upgrade_family_rsp& rsp)
{
	map<uint32, family*>::iterator i = _family.find(req->family_id());
	if (i == _family.end()) {
		return -1;
	}

	if (!check_family(req->family_id(),req->svr_head().role_id())){
		return -1;
	}

    int ret = i->second->upgrade();

    if (ret == 0) {
        rsp.set_ret(client::upgrade_family_rsp_result_success);
		//_log_mgr.insert(req->family_id(),FAMILY_UPGRATE_FLAG,req->family_id(),i->second->lvl(),i->second->get_name());
		uint32 log_id = 0;
		if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
			_log_mgr.insert(req->family_id(),FAMILY_UPGRATE_FLAG,req->family_id(),i->second->lvl(),i->second->get_name(),log_id);
		}
    } else {
        rsp.set_ret(client::upgrade_family_rsp_result_money_limit);
    }

	client::family_info* info = rsp.mutable_info();
	i->second->family_info(info);

    return 0;
}

int family_mgr::family_self(client::family_self_req* req, client::family_self_rsp& rsp)
{
    return _member_mgr.fill_role(req->svr_head().role_id(), rsp);
}

int family_mgr::family_log(client::family_log_req* req, client::family_log_rsp& rsp)
{
	if (!check_family(req->family_id(),req->svr_head().role_id())){
		return -1;
	}
	
    _log_mgr.fill_log(req->family_id(), rsp);
    
    return 0;
}

int family_mgr::family_record(client::family_record_req* req, client::family_record_rsp& rsp)
{
	if (!check_family(req->family_id(),req->svr_head().role_id())){
		return -1;
	}

	_record_mgr.fill(req->family_id(), rsp);

	return 0;
}

int family_mgr::modify_family_power(client::modify_family_power* req)
{
    map<uint32, family*>::iterator i = _family.find(req->family_id());
    if (i == _family.end()) {
        return -1;
    }

	if (!check_family(req->family_id(),req->role_id())){
		return -1;
	}

    _member_mgr.set_strength(req->family_id(), req->role_id(), req->new_power());
    //uint32 fight_power = i->second->get()->power();
    //if (fight_power >= 3999999999) {
    //    fight_power = req->new_power();
    //} else if (fight_power >= 999999999) {
    //    // nothing to do diao bao le
    //} else if (fight_power > req->old_power()) {
    //    fight_power -= req->old_power();
    //    fight_power += req->new_power();
    //} else {
    //    fight_power = req->new_power();
    //}
    //
    //i->second->get()->set_power(fight_power);
    //i->second->set_update_tag();
	reset_family_power(req->family_id());
    return 0;
}

int family_mgr::add_family_info_power(uint32 family_id,uint32 role_id,uint32 add_power)
{
	map<uint32, family*>::iterator i = _family.find(family_id);
	if (i == _family.end()) {
		return -1;
	}

	if (!check_family(family_id,role_id)){
		return -1;
	}

	_member_mgr.set_strength(family_id, role_id, add_power);

	//uint32 fight_power = i->second->get()->power();

	//if (fight_power >= 999999999) {
	//	// nothing to do diao bao le
	//} else {
	//	fight_power += add_power;
	//}

	//i->second->get()->set_power(fight_power);
	//i->second->set_update_tag();

	return 0;
}

int family_mgr::del_family_info_power(uint32 family_id,uint32 del_power)
{
	map<uint32, family*>::iterator i = _family.find(family_id);
	if (i == _family.end()) {
		return -1;
	}

	uint32 total_fight_power = i->second->get()->power();

	total_fight_power -= del_power;

	i->second->get()->set_power(total_fight_power);
	i->second->set_update_tag();

	return 0;
}

int family_mgr::family_record_update(client::family_record_update* req)
{
    return _record_mgr.update(req);
}

const string &family_mgr::get_family_name(uint32 family_id)
{
    family* f = get_family(family_id);
    if (f == NULL) 
	{
		static string ret = "";
		return ret;
	}
	return f->get_name();
}
family_role_value*    family_mgr::get_member_info(uint32 family_id,uint32 role_id)
{
	if (!check_family(family_id,role_id)){
		return NULL;
	}

	family_role_value* member_info = _member_mgr.get_family_role(family_id,role_id);

	if (member_info != NULL){

		return member_info;
	}

	return NULL;
}

uint32 family_mgr::get_flag_id(uint32 family_id)
{
    family* f = get_family(family_id);
    if (f == NULL || f->get() == NULL) {
		return 0;
	}

	return f->get()->flag_id();
}

uint32 family_mgr::get_family_level(uint32 family_id)
{
	family* f = get_family(family_id);
	if (f == NULL || f->get() == NULL) {
		return 0;
	}

	return f->get()->lvl();
}

family* family_mgr::get_family(uint32 family_id)
{
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return NULL;
    }

    return i->second;
}

family* family_mgr::get_family_by_role_id(uint32 role_id)
{
	uint32 family_id = 0;
    family_id = get_family_id(role_id);
    return get_family(family_id);
}

int family_mgr::fill_smy_family_base_data(const mysql::sp_load_all_user_smy* smy, client::role_smy_info& rsi)
{
	
	family* family_base_info = get_family_by_role_id(smy->role_id());
	if (family_base_info == NULL){
		return -1;
	}

	family_role_value* member_info = _member_mgr.get_family_role(family_base_info->get_family_id(),smy->role_id());

	if (member_info == NULL){
		return -1;
	}

	rsi.set_family_id(family_base_info->get_family_id());
	rsi.set_family_name(family_base_info->get_family_name());
	rsi.set_family_postion(member_info->_permissions);

	return 0;
}

int   family_mgr::fill_query_family_base(uint32 role_id,client::role_query_info& role_info)
{
	family* family_base_info = get_family_by_role_id(role_id);
	if (family_base_info == NULL){
		return -1;
	}

	family_role_value* member_info = _member_mgr.get_family_role(family_base_info->get_family_id(),role_id);

	if (member_info == NULL){
		return -1;
	}

	role_info.set_family_id(family_base_info->get_family_id());
	role_info.set_family_name(family_base_info->get_family_name());
	role_info.set_family_position(member_info->_permissions);

	return 0;
}

uint32 family_mgr::get_chief(uint32 family_id)
{
	map<uint32, family*>::iterator i = _family.find(family_id);
	if (i == _family.end()) {
		return 0;
	}

    return _member_mgr.chief_id(family_id);
}

const string &family_mgr::get_chief_name(uint32 family_id)
{
	map<uint32, family *>::iterator i = _family.find(family_id);
	if(i == _family.end()) {
		static string str = "";
		return str;
	}
	return i->second->get()->chief_name();
}

void  family_mgr::get_diplomacy_relation(uint32 family_id,uint32 relation_type,vector<uint32>& vec_family_id)
{
	map<uint32, family*>::iterator i = _family.find(family_id);
	if (i == _family.end()) {
		return;
	}

	_diplomacy_mgr.get_relation_family_id(family_id,relation_type,vec_family_id);
}

uint32 family_mgr::clear_famliy_apply_relation()
{
	_diplomacy_mgr.clear_apply_relation();

	return 0;
}

int family_mgr::set_chief(uint32 family_id, uint32 role_id,string role_name)
{
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return -1;
    }
    
	if (!check_family(family_id,role_id)){
		return -1;
	}

    i->second->get()->set_chief_id(role_id);
	i->second->get()->set_chief_name(role_name);
    i->second->set_update_tag();
    
    return 0;
}

int family_mgr::get_member(uint32 family_id, std::vector<uint32>& member)
{
    return _member_mgr.member_id(family_id, member);
}


int family_mgr::get_manager(uint32 family_id, std::vector<struct manager_info>&manager)
{
	return _member_mgr.get_family_manager(family_id, manager);
}

int family_mgr::get_manager(uint32 family_id, std::vector<uint32>& manager)
{
    return _member_mgr.get_family_manager(family_id, manager);
}

int family_mgr::get_manager_add(uint32 family_id, std::vector<uint32>& manager)
{
    return _member_mgr.get_family_manager_add(family_id, manager);
}

uint32 family_mgr::get_family_id(uint32 role_id, uint32* sect_id)
{   
	// 根据角色ID找到军团ID
    uint32 family_id = _member_mgr.family_id(role_id);
    if (family_id == 0) {
        return 0;
    }
    // 根据军团ID找到军团数据
    family* f = get_family(family_id);
    if (f == NULL) {
		// 找不到这个军团，就移除在member里面残留的数据
        _member_mgr.remove(family_id);
        return 0;
    }

    if (sect_id != NULL){
        *sect_id = f->get_belong_sect();
    }

    return f->get_id();
}

uint32 family_mgr::get_sect_id(uint32 role_id)
{
    uint32 family_id = _member_mgr.family_id(role_id);
    if (family_id == 0) {
        return 0;
    }

    family* f = get_family(family_id);
    if (f == NULL) {
        return 0;
    }

    return f->get_belong_sect();
}

int family_mgr::get_family_title_index(uint32 role_id, uint32 family_id)
{
	uint32 pos = get_family_title(role_id, family_id);
	return trans_permission_to_index(pos);
}

uint32 family_mgr::get_member_donate(uint32 family_id,uint32 role_id)
{
	family_role_value* member_info = _member_mgr.get_family_role(family_id,role_id);

	if (member_info == NULL){
		return 0;
	}

	return member_info->_total_donate;
}

uint32 family_mgr::get_family_title(uint32 role_id, uint32 family_id)
{
    return _member_mgr.get_permissions(family_id, role_id);
}

uint32 family_mgr::get_domain_num(uint32 family_id)
{
    return _domain_mgr.get_num(family_id);
}

uint32 family_mgr::get_family_power(uint32 family_id)
{
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return -1;
    }
    
    return i->second->get()->power();
}

uint32 family_mgr::get_family_money(uint32 role_id)
{
	uint32 family_id = get_family_id(role_id);
	map<uint32, family*>::iterator i = _family.find(family_id);
	if (i == _family.end()) {
		return -1;
	}

	 return i->second->get()->money();
}

uint32 family_mgr::get_domain_owner(uint32 scene_id)
{
    return _domain_mgr.get_owner(scene_id);
}

multimap<uint32, family_domain*>& family_mgr::get_domain_scene()
{
    return _domain_mgr.get();
}

uint32 family_mgr::get_op_tag(uint32 family_id)
{
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    uint32 old = i->second->get()->op_tag_time();
    if (time_utils::is_same_day(now, old)) {
        return i->second->get()->op_tag();
    } else {
        return 0;
    }
}

uint32 family_mgr::set_op_tag(uint32 family_id, uint32 op_tag)
{
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    i->second->get()->set_op_tag_time(now);
    i->second->get()->set_op_tag(op_tag);
    i->second->set_update_tag();

    return 0;
}

uint32 family_mgr::get_strength(uint32 family_id, uint32 role_id)
{
    return _member_mgr.get_strength(family_id, role_id);
}

void family_mgr::set_strength(uint32 family_id, uint32 role_id, uint32 strength)
{
    _member_mgr.set_strength(family_id, role_id, strength);
    reset_family_power(family_id);
}

void family_mgr::record_delete_all()
{
    _record_mgr.delete_all();
}

bool family_mgr::check_name(const string& name)
{
    map<uint32, family*>::iterator i = _family.begin();
    for (; i != _family.end(); ++i) {
        mysql::tb_family* p = i->second->get();
        if (p == NULL) {
            continue;
        }

        if (p->name() == name) {
            return true;
        }
    }
    
    return false;
}

bool family_mgr::check_flag_name(const string& name)
{
    map<uint32, family*>::iterator i = _family.begin();
    for (; i != _family.end(); ++i) {
        mysql::tb_family* p = i->second->get();
        if (p == NULL) {
            continue;
        }

        if (p->flag_name() == name) {
            return true;
        }
    }

    return false;
}

bool family_mgr::check_permissions_num(uint32 family_id, uint32 permission)
{
	map<uint32, family*>::iterator i = _family.find(family_id);
	if (i == _family.end()) {
		return false;
	}

	switch (permission) {
	case ELITE:       return 1 >  _member_mgr.elite_num(family_id); 
	case ELDER:       return 1  > _member_mgr.elder_num(family_id);
	case TIANSHI:     return 1 >  _member_mgr.tianshi_num(family_id);
	case VICE_CHIEF:  return 1  > _member_mgr.vice_chief_num(family_id); 
	default:
		return true;
	}

	return false;
}

//int family_mgr::family_monster_boss(client::family_monster_boss_req* req, client::family_monster_boss_rsp& rsp)
//{
//    uint32 family_id = req->family_id();
//    if (family_id == 0) {
//        family_id = _member_mgr.family_id(req->svr_head().role_id());
//    }
//
//    map<uint32, family*>::iterator i = _family.find(family_id);
//    if (i == _family.end()) {
//        return false;
//    }
//
//    uint32 boss_time = i->second->get()->boss_time();
//    uint32 h = (boss_time & 0x00FF0000) >> 16;
//    uint32 m = (boss_time & 0x0000FF00) >> 8;
//    uint32 s = boss_time & 0x000000FF;
//
//    rsp.set_family_id(family_id);
//    client::monster_boss_time* mbt = rsp.mutable_mbt();
//    mbt->set_hour(h);
//    mbt->set_minute(m);
//    mbt->set_second(s);
//
//    return 0;
//}

//int family_mgr::family_monster_boss_set(client::family_monster_boss_set_req* req, client::family_monster_boss_set_rsp& rsp)
//{
//    uint32 family_id = req->family_id();
//    if (family_id == 0) {
//        family_id = _member_mgr.family_id(req->svr_head().role_id());
//    }
//
//    map<uint32, family*>::iterator i = _family.find(family_id);
//    if (i == _family.end()) {
//        return -1;
//    }
//    
//    uint32 h = req->mbt().hour();
//    uint32 m = req->mbt().minute();
//    uint32 s = req->mbt().second();
//    if (h > 23 || m > 60 || s > 60) {
//        return -2;
//    }
//    
//    uint32 boss_time = h << 16 | m << 8 | s;
//    i->second->get()->set_boss_time(boss_time);
//    i->second->set_update_tag();
//
//    rsp.set_family_id(family_id);
//    client::monster_boss_time* mbt = rsp.mutable_mbt();
//    mbt->set_hour(h);
//    mbt->set_minute(m);
//    mbt->set_second(s);
//    
//    return 0;
//}

//int family_mgr::family_monster_boss_call_jump(client::family_monster_boss_call_jump_req* req, client::family_monster_boss_call_jump_rsp& rsp)
//{
//    uint32 family_id = req->family_id();
//    if (family_id == 0) {
//        family_id = _member_mgr.family_id(req->svr_head().role_id());
//    }
//
//    map<uint32, family*>::iterator i = _family.find(family_id);
//    if (i == _family.end()) {
//        return -1;
//    }
//    
//    return 0;
//}

int family_mgr::family_log_update(client::family_log_update* req)
{
    //for (int32 i = 0; i < req->fli_size(); ++i) {
    //    const client::family_log_info& fli = req->fli(i);
    //    //_log_mgr.insert(req->family_id(), fli.type(), fli.key(), fli.value(),fli.family_name());
    //}

    return 0;
}

void family_mgr::remove(uint32 family_id)
{
    map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return ;
    }

    i->second->remove();
    _remove.insert(pair<uint32, family*>(i->second->get()->id(), i->second));
    _family.erase(i);

    _member_mgr.remove(family_id);
    _log_mgr.remove   (family_id);
    _skill_mgr.remove (family_id);
    _record_mgr.remove(family_id);
    _domain_mgr.remove(family_id);
	_diplomacy_mgr.remove(family_id);
}
// 重算军团战力
int family_mgr::reset_family_power(uint32 family_id)
{
    std::map<uint32, family*>::iterator i = _family.find(family_id);
    if (i == _family.end()) {
        return -1;
    }
    
    return reset_family_power(i->second);
}

int family_mgr::reset_family_power(family* f)
{
    if (f == NULL) {
        return -1;
    }

    uint32 strength = _member_mgr.get_strength(f->get_id());
    f->get()->set_power(strength);
    f->set_update_tag();
    
    return 0;
}


#if 0
int family_mgr::fill_family_info(client::load_family_list_rsp &rsp)
{
	map<uint32, family*>::iterator it = _family.begin();
	for(; it != _family.end(); ++it)
	{
		family_camp_info *fci = rsp.add_fci();
		fci->set_family_id(it->first);
		fci->set_money(it->second->get_money());
		fci->set_family_name(it->second->get_name());
		fci->set_flag_id(it->second->get_flag_id());
		fci->set_user_name("");
		fci->set_role_id(it->second->get_chief());
	}
	return 0;
}
#endif

int family_mgr::family_set(client::family_set_req* req, client::family_set_rsp& rsp)
{
	std::map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr != _family.end()){
		if (static_cast<uint32>(itr->second->get_approval()) == req->approval()){
			rsp.set_is_success(1);
			return -1;
		}

		itr->second->set_approval(req->approval());
		rsp.set_is_success(0);
		rsp.set_approval(req->approval());
		return 0;
	}

	rsp.set_is_success(1);
	return -1;
}

int family_mgr::change_family_flag_id(client::change_family_flag_id_req *req,client::change_family_flag_id_rsp& rsp_msg)
{
	map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr == _family.end()) {
		return -1;
	}

	const config_flag* flag_info = CONFMGR->get_config_flag_mgr().get_flag(req->flag_id());
	if (flag_info == NULL){
		return -1;
	}
	// 检测权限是是否满足
	if (_member_mgr.get_permissions(req->family_id(),req->svr_head().role_id()) != CHIEF){
		rsp_msg.set_result_type(3);
		return -1;
	}
	// 检测资源够不够
	const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(itr->second->lvl());
	if (legion_info == NULL){
		rsp_msg.set_result_type(2);
		return -1;
	}
	
	uint32 pay_value = static_cast<uint32>(legion_info->get_change_flag_pay());
	// 
	const std::map<int,int>& pay_item_list = legion_info->get_change_flag_item();
	if (pay_item_list.size() == 0){
		printf("config_legion get_change_flag_item is null \n");
		rsp_msg.set_result_type(2);
		return -1;
	}
	
	if (static_cast<uint32>(itr->second->get_money()) < pay_value){
		rsp_msg.set_result_type(1);
		return -1;
	}

	std::map<int,int>::const_iterator it = pay_item_list.begin();
	for (;it != pay_item_list.end();++it){
		if (it->first == 50145001){ // 青龙
			if (itr->second->get()->dragon_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(1);
				return -1;
			}
		}
		if (it->first == 50145002){ // 朱雀
			if (itr->second->get()->rosefinch_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(1);
				return -1;
			}
		}
		if (it->first == 50145004){ // 白虎
			if (itr->second->get()->tiger_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(1);
				return -1;
			}
		}
		if (it->first == 50145003){ // 玄武
			if (itr->second->get()->tortoise_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(1);
				return -1;
			}
		}
	}

	// 修改战旗ID
	itr->second->set_flag_id(req->flag_id());
	// 扣钱
	itr->second->set_money(itr->second->get_money() - pay_value);
	const std::map<int,int>& pay_item_ = legion_info->get_change_flag_item();
	std::map<int,int>::const_iterator it_1 = pay_item_.begin();
	for (;it_1 != pay_item_.end();++it_1){
		if (it_1->first == 50145001){ // 青龙
			itr->second->set_dragon_count(itr->second->get()->dragon_count() - it_1->second);
		}
		if (it_1->first == 50145002){ // 朱雀
			itr->second->set_rosefinch_count(itr->second->get()->rosefinch_count() - it_1->second);
		}
		if (it_1->first == 50145004){ // 白虎
			itr->second->set_tiger_count(itr->second->get()->tiger_count() - it_1->second);
		}
		if (it_1->first == 50145003){ // 玄武
			itr->second->set_tortoise_count(itr->second->get()->tortoise_count() - it_1->second);
		}
	}
	
	rsp_msg.set_result_type(0);
	client::family_info* info = rsp_msg.mutable_info();
	itr->second->family_info(info);
	//_log_mgr.insert(req->family_id(),FAMILY_CHANGE_FLAG_ID,req->svr_head().role_id(),req->family_id(),itr->second->get_name());
	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
		_log_mgr.insert(req->family_id(),FAMILY_CHANGE_FLAG_ID,req->svr_head().role_id(),req->family_id(),itr->second->get_name(),log_id);
	}
	return 0;
}

int family_mgr::change_family_flag_name(client::change_family_flag_name_req *req,client::change_family_flag_name_rsp& rsp_msg)
{

	map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr == _family.end()) {
		return -1;
	}

	if (req->flag_name().length() < 1) {
		rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_CAN_NOT_NULL);
		return -1;
	}

	if (req->flag_name().length() > 18){
		rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_TOO_LONG);
		return -1;
	}

	if (check_flag_name(req->flag_name())){
		rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_REPETITION);
		return -1;
	}

	// 检测权限是是否满足
	if (_member_mgr.get_permissions(req->family_id(),req->svr_head().role_id()) != CHIEF){
		rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_PRMISSION_LIMIT);
		return -1;
	}

	const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(itr->second->lvl());
	if (legion_info == NULL){
		return -1;
	}
	// 检测资源够不够
	uint32 pay_value = static_cast<uint32>(legion_info->get_change_flag_name_pay());

	if (static_cast<uint32>(itr->second->get_money()) < pay_value){
		rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_RESOURCE);
		return -1;
	}

	const std::map<int,int>&  flag_name_pay_list_ = legion_info->get_change_flag_name_item();
	if (flag_name_pay_list_.size() == 0){
		rsp_msg.set_result_type(PT_CHANGE_FLAG_NAME_CONFIG_ERRO);
		return -1;
	}

	std::map<int,int>::const_iterator it = flag_name_pay_list_.begin();
	for (;it != flag_name_pay_list_.end(); ++it){
		if (it->first == 50145001){ // 青龙
			if (itr->second->get()->dragon_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_RESOURCE);
				return -1;
			}
		}
		if (it->first == 50145002){ // 朱雀
			if (itr->second->get()->rosefinch_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_RESOURCE);
				return -1;
			}
		}
		if (it->first == 50145004){ // 白虎
			if (itr->second->get()->tiger_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_RESOURCE);
				return -1;
			}
		}
		if (it->first == 50145003){ // 玄武
			if (itr->second->get()->tortoise_count() < static_cast<uint32>(it->second)){
				rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_RESOURCE);
				return -1;
			}
		}
	}
	// 修改战旗名字
	itr->second->set_flag_name(req->flag_name());
	// 扣
	itr->second->set_money(itr->second->get_money() - pay_value);
	const std::map<int,int>&  flag_name_pay_ = legion_info->get_change_flag_name_item();
	std::map<int,int>::const_iterator it_1 = flag_name_pay_.begin();
	for (;it_1 != flag_name_pay_.end();++it_1){
		if (it_1->first == 50145001){ // 青龙
			itr->second->set_dragon_count(itr->second->get()->dragon_count() - it_1->second);
		}
		if (it_1->first == 50145002){ // 朱雀
			itr->second->set_rosefinch_count(itr->second->get()->rosefinch_count() - it_1->second);
		}
		if (it_1->first == 50145004){ // 白虎
			itr->second->set_tiger_count(itr->second->get()->tiger_count() - it_1->second);
		}
		if (it_1->first == 50145003){ // 玄武
			itr->second->set_tortoise_count(itr->second->get()->tortoise_count() - it_1->second);
		}
	}

	rsp_msg.set_result_type(OPT_CHANGE_FLAG_NAME_CHANGE_SUCESS);
	// 扣道具，扣钱
	client::family_info* info = rsp_msg.mutable_info();
	itr->second->family_info(info);
	//_log_mgr.insert(req->family_id(),FAMILY_CHANGE_FLAG_NAME,req->svr_head().role_id(),req->family_id(),itr->second->get_name());
	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
		_log_mgr.insert(req->family_id(),FAMILY_CHANGE_FLAG_NAME,req->svr_head().role_id(),req->family_id(),itr->second->get_name(),log_id);
	}
	return 0;
}

int family_mgr::impeach_chief(client::impeach_chief_req *req,client::impeach_chief_rsp& rsp_msg,string role_name)
{
	map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr == _family.end()) {
		return -1;
	}

	// 检测权限是是否满足
	if (_member_mgr.get_permissions(req->family_id(),req->svr_head().role_id()) == CHIEF){
		rsp_msg.set_result_type(IMPEACH_RESULT_CHIEF_ERRO);
		return -1;
	}

	uint32 cheif_id = _member_mgr.chief_id(req->family_id());

	_member_mgr.modify_role_permissions(req->family_id(), cheif_id, MEMBER);
	_member_mgr.modify_role_permissions(req->family_id(), req->svr_head().role_id(), CHIEF);
	set_chief(req->family_id(), req->svr_head().role_id(),role_name);
	rsp_msg.set_result_type(IMPEACH_RESULT_SUCESS);
	//旧军团长邮件提示（不在线）：“您被弹劾，降为普通成员，愿以后勿负人”
	client::family_info* info = rsp_msg.mutable_info();
	itr->second->family_info(info);
	//_log_mgr.insert(req->family_id(),FAMILY_IMPEACH,req->svr_head().role_id(),cheif_id,itr->second->get_name());
	uint32 log_id = 0;
	if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
		_log_mgr.insert(req->family_id(),FAMILY_IMPEACH,req->svr_head().role_id(),cheif_id,itr->second->get_name(),log_id);
	}

	return 0;
}

int family_mgr::donate_list(client::donate_list_req *req,client::donate_list_rsp& rsp_msg)
{
	map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr == _family.end()) {
		return -1;
	}

	rsp_msg.set_family_id(req->family_id());

	_member_mgr.fill_member_donate_info(req->family_id(),rsp_msg);

	return 0;
}

int family_mgr::ally_family(client::ally_family_req *req,client::ally_family_rsp& rsp_msg)
{
	rsp_msg.set_opt_type(req->opt_type());
	rsp_msg.set_result_type(1);
	// 检测是否有这军团
	map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr == _family.end()) {
		return -1;
	}

	string family_name = itr->second->get_name();
	// 检测目标军团存在
	map<uint32, family*>::iterator it = _family.find(req->target_family_id());
	if (it == _family.end()){
		return -1;
	}
	string target_family_name = it->second->get_name();

	FAMILY_LOG_TYPE type;
	type = FAMILY_CREATE;
	// 结盟
	if (req->opt_type() == OPT_FORIGN_TYPE_ALLY){
		type = FAMILY_ALLIANCE;
	}else if (req->opt_type() == OPT_FORIGN_TYPE_HOSTILITY){
		type = FAMILY_ENENY_FAMILY;
	}else if (req->opt_type() == OPT_FORIGN_TYPE_CHECKING){
		// not thing to do
	}else if (req->opt_type() == OPT_FORIGN_TYPE_REJECT){
		// not thing to do
	}else{
		return -1;
	}
	

	bool is_sucess = false;
	if (req->opt_type() == OPT_FORIGN_TYPE_ALLY){
		if (_diplomacy_mgr.operate_family_foreign(req->opt_type(),req->family_id(),req->target_family_id()) &&
			_diplomacy_mgr.operate_family_foreign(req->opt_type(),req->target_family_id(),req->family_id())){
			is_sucess = true;
		}else{
			rsp_msg.set_result_type(1);
			return -1;
		}
	}else if (req->opt_type() == OPT_FORIGN_TYPE_HOSTILITY){
		uint32 hostility_limit = CONFMGR->get_basic_value(10158);
		if (_diplomacy_mgr.get_relation_count(req->family_id(),OPT_FORIGN_TYPE_HOSTILITY) >= hostility_limit){
			rsp_msg.set_result_type(2);
			return -1;
		}

		if (_diplomacy_mgr.operate_family_foreign(req->opt_type(),req->family_id(),req->target_family_id())&&
			_diplomacy_mgr.clean_up_relation_by_hostility(req->target_family_id(),req->family_id())){
			is_sucess = true;
		}
		else{
			rsp_msg.set_result_type(1);
			return -1;
		}
	}else if (req->opt_type() == OPT_FORIGN_TYPE_REJECT){
		if (_diplomacy_mgr.get_relation_type(req->family_id(),req->target_family_id()) != OPT_FORIGN_TYPE_HOSTILITY&&
			_diplomacy_mgr.get_relation_type(req->family_id(),req->target_family_id()) != OPT_FORIGN_TYPE_ALLY){
			if (_diplomacy_mgr.operate_family_foreign(req->opt_type(),req->family_id(),req->target_family_id())){
				is_sucess = true;
			}else{
				rsp_msg.set_result_type(1);
				return -1;
			}
		}else{
			rsp_msg.set_result_type(1);
			return -1;
		}
	}else if (req->opt_type() == OPT_FORIGN_TYPE_CHECKING){
		uint32 ally_limit = CONFMGR->get_basic_value(10157);
		if (_diplomacy_mgr.get_relation_count(req->family_id(),OPT_FORIGN_TYPE_ALLY) >= ally_limit||
			_diplomacy_mgr.get_relation_count(req->target_family_id(),OPT_FORIGN_TYPE_ALLY) >= ally_limit){
				rsp_msg.set_result_type(2);
				return -1;
		}

		if (_diplomacy_mgr.operate_family_foreign(req->opt_type(),req->family_id(),req->target_family_id())){
			is_sucess = true;
		}else{
			rsp_msg.set_result_type(1);
			return -1;
		}
	}

	if (is_sucess){
		rsp_msg.set_result_type(0);
		rsp_msg.set_target_family_id(req->target_family_id());
	}

	if (type == FAMILY_ALLIANCE || type == FAMILY_ENENY_FAMILY){
		// 见闻记录
		uint32 log_id = 0;
		if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
			_log_mgr.insert(req->family_id(),type,req->family_id(),req->target_family_id(),target_family_name,log_id);
		}

		uint32 log_id_2 = 0;
		//_log_mgr.insert(req->family_id(),type,req->family_id(),req->target_family_id(),target_family_name);
		if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id_2)){
			_log_mgr.insert(req->target_family_id(),type,req->family_id(),req->target_family_id(),family_name,log_id_2);
		}
		//_log_mgr.insert(req->target_family_id(),type,req->family_id(),req->target_family_id(),family_name);
	} 

	return 0;
}
// 解除外交关系
int family_mgr::remove_foreign(client::remove_foreign_req *req,client::remove_foreign_rsp& rsp_msg,bool& is_remove_ally)
{
	rsp_msg.set_src_family_id(req->family_id());
	rsp_msg.set_target_family_id(req->target_family_id());
	rsp_msg.set_result_type(1);
	// 检测是否有这军团
	map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr == _family.end()) {
		return -1;
	}

	// 检测目标军团存在
	map<uint32, family*>::iterator it = _family.find(req->target_family_id());
	if (it == _family.end()) {
		return -1;
	}
	uint32 relation_type = _diplomacy_mgr.get_relation_type(req->family_id(),req->target_family_id());
	if (relation_type == 0) {
		return -1;
	}
	uint32 log_id = 0;
	uint32 log_id_2 = 0;
	if (relation_type == OPT_FORIGN_TYPE_ALLY){
		if (_diplomacy_mgr.remove(req->family_id(),req->target_family_id(),is_remove_ally) == 0 && _diplomacy_mgr.remove(req->target_family_id(),req->family_id(),is_remove_ally) == 0){
			rsp_msg.set_result_type(0);
			
			if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
				_log_mgr.insert(req->family_id(),FAMILY_CANCEL_ALLIANCE,req->family_id(),req->target_family_id(),it->second->get_name(),log_id);
			}
			//_log_mgr.insert(req->family_id(),FAMILY_CANCEL_ALLIANCE,req->family_id(),req->target_family_id(),it->second->get_name());
			if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id_2)){
				_log_mgr.insert(req->target_family_id(),FAMILY_CANCEL_ALLIANCE,req->family_id(),req->target_family_id(),itr->second->get_name(),log_id_2);
			}

			//_log_mgr.insert(req->target_family_id(),FAMILY_CANCEL_ALLIANCE,req->family_id(),req->target_family_id(),itr->second->get_name());
		}
	}else  if (relation_type == OPT_FORIGN_TYPE_HOSTILITY){
		if (_diplomacy_mgr.remove(req->family_id(),req->target_family_id(),is_remove_ally) == 0){
			rsp_msg.set_result_type(0);
			if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id)){
				_log_mgr.insert(req->family_id(),FAMILY_ENENY_CANCEL_FAMILY,req->family_id(),req->target_family_id(),it->second->get_name(),log_id);
			}

			//_log_mgr.insert(req->family_id(),FAMILY_ENENY_CANCEL_FAMILY,req->family_id(),req->target_family_id(),it->second->get_name());
			if (_log_mgr.generate_log_id(req->svr_head().svr_id(),log_id_2)){
				_log_mgr.insert(req->target_family_id(),FAMILY_ENENY_CANCEL_FAMILY,req->family_id(),req->target_family_id(),itr->second->get_name(),log_id_2);
			}
			//_log_mgr.insert(req->target_family_id(),FAMILY_ENENY_CANCEL_FAMILY,req->family_id(),req->target_family_id(),itr->second->get_name());
		}
	}
	else {
		return -1;
	}

	return 0;
}

int family_mgr::ally_family_list(client::ally_family_list_req *req,client::ally_family_list_rsp& rsp_msg)
{
	rsp_msg.set_family_id(req->family_id());
	// 检测是否有这军团
	map<uint32, family*>::iterator itr = _family.find(req->family_id());
	if (itr == _family.end()) {
		return -1;
	}

	_diplomacy_mgr.fill_diplomacy_info(req->family_id(),rsp_msg);

	return 0;
}

int family_mgr::trans_permission_to_index(uint32 permission)
{
	if(permission & CHIEF)
	{
		return 1;
	}
	else if(permission & VICE_CHIEF)
	{
		return 3;
	}
	else if(permission & ELDER)
	{
		return 4;
	}
	else if(permission & ELITE)
	{
		return 5;
	}
	else if(permission & TIANSHI)
	{
		return 6;
	}
	return 0;
}

uint32 family_mgr::get_join_time(uint32 family_id, uint32 role_id)
{
	return _member_mgr.get_join_time(family_id, role_id);
}
