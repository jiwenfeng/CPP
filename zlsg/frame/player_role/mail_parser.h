#ifndef __MAIL_PARSER_H__
#define __MAIL_PARSER_H__

#include "comm_define.h"

#define MAIL_TITLE_LEN 64        // 标题最大长度
#define MAIL_CONTENT_LEN 768     // 内容最大长度
#define MAX_MAIL_BOLB_LEN 66560  // 邮件缓存最大长度 

enum ROLE_MAIL_INFO_FLAG
{
    RMIF_NORMAL = 0,
    RMIF_DELETE = 1,
};

class role_mail_info : public client::mail_info
{
public:
    role_mail_info()
    {
        flag = RMIF_NORMAL;
    }

    role_mail_info(const client::mail_info& rhs) : client::mail_info(rhs)
    {
        flag = RMIF_NORMAL;
    }

    virtual ~role_mail_info()
    {

    }

    inline void remove(){flag = RMIF_DELETE;}

    inline bool is_normal(){return flag == RMIF_NORMAL;}

protected:
    ROLE_MAIL_INFO_FLAG flag;
};

class mail_parser
{
public:
    mail_parser();

    virtual ~mail_parser();

protected:
    int parse_mail(const string& in);     // 将DB中的数据，解析成邮件PB

    int serialize_mail(string& out); // 将邮件PB序列化到DB数据

	bool is_past_time_mail(role_mail_info& mail_info);
	
    void clear();

public:
	inline const vector<role_mail_info>& get_mail_list() { return mail_list ;}
protected:
    vector<role_mail_info> mail_list;
};

#endif
