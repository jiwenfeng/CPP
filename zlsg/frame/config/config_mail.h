#ifndef __CONFIG_MAIL_H__
#define __CONFIG_MAIL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"



// �ʼ�������
class config_mail
{
public:
    config_mail();
    ~config_mail();

public:
	// GET
	inline uint32                              get_mail_id()		const { return mail_id				;}
	inline std::string                         get_key()		    const { return key					;}
	inline std::string                         get_content_value()	const { return content_value		;}
	inline uint32                              get_brod_times()	    const { return brod_times		    ;}
public:

public:
	// ģ�������
	uint32                       mail_id                     ; // ΨһID
	std::string	                 key						 ; // һ������
	std::string	                 content_value				 ; // ����
	uint32						 brod_times					 ; // ���Ŵ���
};

class config_mail_mgr : public config_base_mgr
{
public:
    config_mail_mgr(TTableGroup_s* p = NULL);
    ~config_mail_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_mail* get_config_mail(uint32 mail_id) const;

private:
    TTableGroup_s*                            _mail;
    TTable*                                   _table;
    std::map<uint32, config_mail*>            _cp;
};

#endif // __CONFIG_MAIL_H__
