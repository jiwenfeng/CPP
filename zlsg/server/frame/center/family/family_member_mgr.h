#ifndef __FAMILY_MEMBER_MGR_H__
#define __FAMILY_MEMBER_MGR_H__

#include <vector>
#include "comm_define.h"
#include "client.family.pb.h"
#include "family_member.h"

struct manager_info
{
	uint32 role_id;
	uint32 official_id;
	manager_info(uint32 role_id, uint32 official_id):role_id(role_id), official_id(official_id)
	{
	}
};

struct family_role_key {
    uint32 _family_id;
    uint32 _role_id;

    family_role_key(uint32 family_id, uint32 role_id)
        : _family_id(family_id)
        , _role_id(role_id)
    {
    }
};

struct family_role_value {
    uint32 _permissions;// Ȩ��
    uint32 _add_by;
    uint32 _time;
    uint32 _strength;
	uint32 _dragon_count;
	uint32 _tiger_count;
	uint32 _rosefinch_count;
	uint32 _tortoise_count;
	uint32 _money;
	uint32 _total_donate;

    family_role_value(uint32 permission, uint32 add_by)
        : _permissions(permission)
        , _add_by(add_by)
    {
        _time = (uint32)time(NULL);
        _strength = 0;
		_dragon_count = 0;
		_tiger_count = 0;
		_rosefinch_count = 0;
		_tortoise_count = 0;
		_money = 0;
		_total_donate = 0;
    }
    
    family_role_value(uint32 permission, uint32 add_by, uint32 time, uint32 strength,uint32 dragon_count,uint32 tiger_count,uint32 rosefinch_count,uint32 tortoise_count,uint32 money,uint32 total_donate)
        : _permissions(permission)
        , _add_by(add_by)
        , _time(time)
        , _strength(strength)
		, _dragon_count(dragon_count)
		, _tiger_count(tiger_count)
		, _rosefinch_count(rosefinch_count)
		, _tortoise_count(tortoise_count)
		, _money(money)
		, _total_donate(total_donate)
    {
    }
};

typedef std::map<uint32, family_role_value>       role_permissions;
typedef std::pair<uint32, family_role_value>      pair_role_permissions;// key Ϊ��ɫid
typedef std::map<uint32, role_permissions*>       family_role;// ���ų�Ա�����ݿ�key �Ǿ���ID 
typedef std::pair<uint32, role_permissions*>      pair_family_role;
typedef std::map<family_role_key, family_member*> map_family_member;
typedef std::multimap<uint32, family_member*>     map_role_family_member;// key Ϊ��ɫid
typedef std::pair<uint32, family_member*>         pair_role_family_member;

class family_member_mgr
{
public:
    family_member_mgr();
    virtual ~family_member_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);
    
public:
	// ���ݾ���ID ��ȡ���г�Ա��Ȩ�޵�����
    role_permissions*  get_role_permissions(uint32 family_id);
	// ���ݾ���ID���ɫID��ȡĳ����Ա��Ȩ�޵�����
    family_role_value* get_family_role(uint32 family_id, uint32 role_id);
    // ���ĳ�������µĳ�Ա
    int add_role(uint32 add_by, uint32 family_id, uint32 role_id, FAMILY_MEMBER_TYPE type);
	// ���������Ա
    int apply(uint32 add_by, uint32 family_id, uint32 role_id);
	// ֱ�ӳ�Ϊ��Ա
	int auto_add(uint32 add_by, uint32 family_id, uint32 role_id);
	// ��������Ա
    int invite(uint32 add_by, uint32 family_id, uint32 role_id);
	// ���������������еĳ�Ա���Ϊ������ʽ��Ա ���Ȩ��
    int join(uint32 family_id, uint32 role_id);
	// �޸ĳ�ԱȨ��
    int modify_role_permissions(uint32 family_id, uint32 role_id, uint32 permissions);
	// ��Ա����ʱ��
	uint32 get_join_time(uint32 family_id, uint32 role_id);
	// �Ƴ�ĳ�����ŵ�ĳ�����
    int remove(uint32 family_id, uint32 role_id);
	// �Ƴ�ĳ�����ţ����ڽ�ɢĳ�����ŵĲ�����
    int remove(uint32 family_id);
	// ��ȡ���ŵ���ʽ��Ա����
    uint32 member_num(uint32 family_id);
	// ���ݾ���ID��ȡ���ų���ID
    uint32 chief_id(uint32 family_id);
	// ���ݾ���ID��ȡ�����ų���ID
	uint32 vice_chief_id(uint32 family_id);

	uint32 permission_id(uint32 family_id,uint32 permission_value);
	// ��ȡ���ų�ԱȨ�����ϵ�ID
    uint32 official_id(uint32 family_id, std::vector<uint32>& official_id);
	// �������ID��ȡ����ID
    uint32 family_id(uint32 role_id);
	// ���ݾ���ID��ȡ��ʽ��Ա���������ų��ȣ���ID���ݼ���
    uint32 member_id(uint32 family_id, std::vector<uint32>& member_id);
	// ��ȡĳ������ĳ���˵�Ȩ������
    uint32 get_permissions(uint32 family_id, uint32 role_id);

	int is_already_apply(uint32 family_id, uint32 role_id);
	// ��ȡĳ�����ŵ���ʿ�ų�����
    uint32 elite_num(uint32 family_id);
    // ��ȡĳ�����ŵ��ͽ��ų�����
    uint32 elder_num(uint32 family_id);
	// ��ȡĳ�����ŵ���ʦ�ų�����
	uint32 tianshi_num(uint32 family_id);
    // ��ȡĳ�����ŵĸ����ų�����
    uint32 vice_chief_num(uint32 family_id);
	// ��ȡĳ������ĳ��Ȩ�޵�����
    uint32 permissions_num(uint32 permissions, uint32 family_id);
	// ��ȡ���������ľ�����
    uint32 get_apply_count(uint32 role_id);
	// ��ȡĳ�����ŵĹ�����Ա ID��Ȩ���ǳ�Ա���ϵ�ְ��
    int get_family_manager(uint32 family_id, std::vector<uint32>& manager);
	// ��ȡĳ�����ŵĹ�����Ա ID �� ����Ȩ�ޣ�Ȩ���ǳ�Ա���ϵ�ְ��
    int get_family_manager(uint32 family_id, std::vector<struct manager_info>& manager);
	// ȡĳ�����ŵĹ�����Ա ID��Ȩ���Ǿ�Ӣ���ϵ�ְ��
    int get_family_manager_add(uint32 family_id, std::vector<uint32>& manager);
	// ��ȡĳ������ĳ���˵��Ƽ��˵�ID
    uint32 get_referrer(uint32 family_id, uint32 role_id);
	// ����ĳ������ĳ���˵�ս����ֵ
    uint32 set_strength(uint32 family_id, uint32 role_id, uint32 strength);

    uint32 get_strength(uint32 family_id);

    uint32 get_strength(uint32 family_id, uint32 role_id);

	uint32 set_dragon_count(uint32 family_id, uint32 role_id, uint32 dragon_count);

	uint32 set_tiger_count(uint32 family_id, uint32 role_id, uint32 tiger_count);

	uint32 set_rosefinch_count(uint32 family_id, uint32 role_id, uint32 rosefinch_count);

	uint32 set_tortoise_count(uint32 family_id, uint32 role_id, uint32 dragon_count);

	uint32 set_money(uint32 family_id, uint32 role_id, uint32 money);
	
	uint32 set_total_donate(uint32 family_id, uint32 role_id, uint32 total);

	uint32 get_member_total_donate(uint32 family_id, uint32 role_id);
public:
    int fill_member_invite(uint32 role_id, client::invite_family_list_rsp& rsp);
    int fill_member_apply(uint32 family_id, client::apply_family_list_rsp& rsp);
    int fill_member_apply_self(uint32 role_id, client::apply_family_list_self_rsp& rsp);
    int fill_member(uint32 family_id, client::family_member_list_rsp& rsp);
	int fill_member(uint32 family_id, vector<uint32> &v);
    int fill_role(uint32 role_id, client::family_self_rsp& rsp);
	int fill_member_donate_info(uint32 family_id, client::donate_list_rsp& rsp_msg);
   
private:
    map_role_family_member _map_role_family_member;
    family_role            _family_role;
    multimap<uint32, family_member*> _remove;
};

#endif /* __FAMILY_MEMBER_MGR_H__ */
