#ifndef __PLAYER_ROLE_MAIL_H__
#define __PLAYER_ROLE_MAIL_H__

#include "mail_parser.h"

#define MAX_SEND_ATTACH_MENT_NUM 5 // ���5������

class player_role_mail : public mail_parser, public pb_writer
{
public:
    player_role_mail(mysql::tb_role_mail* m = NULL);

    virtual ~player_role_mail();

    int set_db_mail(mysql::tb_role_mail* m);

    int add_mail(uint32 role_id, const client::mail_info& mi);

    int save_mail(amtsvr_context* ctx);

public:
	// ����ʼ��б�����
    void proc_mail_list_req(client::mail_list_rsp& rsp);
	// �����ʼ�����
    bool proc_mail_open_req(uint32 mail_id, client::mail_open_rsp& rsp);
	// ���ɾ���ʼ�������Ϣ����
    void proc_mail_remove_req(client::remove_mail_req& rmr, client::remove_mail_rsp& rsp);
	// ��⸽���Ƿ����
	bool check_attachment_time_out(uint32 mail_id);
	// ��ȡ����
    int proc_extract_attachment(uint32 mail_id, vector<client::props_info>& pis,std::vector<client::mail_attr>& attr);
	// �����������״̬
	bool set_is_take_status(uint32 mail_id);
	
	bool has_take(uint32 mail_id);

protected:
    role_mail_info* get_mail(uint32 mail_id);

protected:
    mysql::tb_role_mail* db_role_mail;   
};


#endif
