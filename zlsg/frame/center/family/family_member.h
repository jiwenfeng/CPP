#ifndef __FAMILY_MEMBER_H__
#define __FAMILY_MEMBER_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"

enum FAMILY_MEMBER_TYPE {         // 成员类型
    APPLY           = 0x00000000, // 验证中
    INVITE          = 0x00000001, // 邀请中
    MEMBER          = 0x00000002, // 成员
    ELITE           = 0x00000004, // 术士团长
    ELDER           = 0x00000008, // 猛将团长
	TIANSHI         = 0x00000010, // 天师团长
    VICE_CHIEF      = 0x00000020, // 副军团长
    CHIEF           = 0x00000040, // 军团长
    
    FOUNDER         = 0xFFFFFFFE, // 创始人
    MYSTERY         = 0xFFFFFFFF, // 神秘人
};

class family_member : public pb_writer
{
public:
    family_member(mysql::tb_family_member* tfm = NULL);
    family_member(uint32 family_id, uint32 role_id, uint32 permissions, uint32 time, uint32 add_by);
    virtual ~family_member();

public:
    int save(amtsvr_context* ctx);

    mysql::tb_family_member* get() { return _member; }
    
public:
    uint32 role_id();

    uint32 family_id();

    uint32 add_by();

    uint32 permissions();

    uint32 time();
    
    int set(uint32 family_id, uint32 role_id, uint32 permissions, uint32 timem, uint32 add_by);

    int set_permissions(uint32 permissions);

    int set_time(uint32 time);

    int set_strength(uint32 strength);

	int set_dragon_count(uint32 dragon_count);

	int set_tiger_count(uint32 tiger_count);

	int set_rosefinch_count(uint32 rosefinch_count);

	int set_tortoise_count(uint32 tortoise_count);

	int set_money(uint32 money);

	int set_total_donate(uint32 total_donate);

    int remove();

private:
    mysql::tb_family_member* _member;
    bool change;
};

#endif /* __FAMILY_MEMBER_H__ */
