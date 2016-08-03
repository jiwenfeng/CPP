#ifndef __FAMILY_H__
#define __FAMILY_H__

#include "define.h"
#include "pb_mgr.h"
#include "client.family.pb.h"
#include "pb_writer.h"

class family : public pb_writer
{
public:
    family(mysql::tb_family* tf = NULL);
    virtual ~family();

public:
    mysql::tb_family* get() { return _family; };

    uint32 get_id()
    {
        if (_family == NULL) {
            return 0;
        }
        
        return _family->id();
    }

    uint32 get_chief()
    {
        if (_family == NULL) {
            return 0;
        }
        
        return _family->chief_id();
    }


	uint32 get_flag_id()
	{
		if(_family == NULL)
		{
			return 1;
		}
		return _family->flag_id();
	}

    const string& get_name()
    {
        if (_family == NULL) {
            static string ret = "";
            return ret;
        }
        
        return _family->name();
    }

    inline uint32 get_belong_sect() const {return (_family != NULL ? _family->belong_sect() : 0);} 

    int save(amtsvr_context* ctx);

public:
    int create(uint32 id, client::create_family_req_use type, const string& name, uint32 flag_id, const string& flag_name, uint32 chief_id,const std::string create_name);
    
    int family_info(client::family_info* fi);

	int family_title_info(client::family_title_info* title_info);

    int set_post(const string& post);
	// 设置自动同意申请0自动 1不自动
	int set_approval(uint32 approval);

	int get_approval();

    int set_qq(uint32 qq);

    int set_yy(uint32 yy);

    int add_money(uint32 money);

    int set_money(uint32 money);

	int set_dragon_count(uint32 count);

	int set_rosefinch_count(uint32 count);

	int set_tiger_count(uint32 count);

	int set_tortoise_count(uint32 count);

	int get_money();

	string get_family_name();

	uint32 get_family_id();

    int upgrade();

    int set_boss_time(uint32 boss_time);

    uint32 lvl();

    int remove();

    int set_update_tag();

	int set_flag_id(uint32 flag_id);

	int set_flag_name(string flag_name);

	int set_cur_member_num(uint32 member_count);

	int set_family_power(uint32 power_value);

public:
    static const uint32 OP_TAG_DOMAIN_RING      = 0x00000001;
    static const uint32 OP_TAG_INFERNO_STRUCK   = 0x00000002;
    static const uint32 OP_TAG_SECT_BATTLE_RING = 0x00000004;

private:
    static const uint32 MAX_FAMILY_LVL = 5;
    
private:
    mysql::tb_family* _family;
    bool change;
};

#endif /*__FAMILY_H__ */
