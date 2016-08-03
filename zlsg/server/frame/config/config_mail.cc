#include "cfg_parser.h"
#include "config_mail.h"

config_mail::config_mail(void)
{
}

config_mail::~config_mail(void)
{
}

config_mail_mgr::config_mail_mgr(TTableGroup_s* p)
        : _mail(p)
{
    init(p);
}


config_mail_mgr::~config_mail_mgr(void)
{
    clear();
}

int config_mail_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _mail = p;
    _table = _mail->GetTable("t_mail");
    if (_table == NULL) {
        printf("not found mail\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "ID"         ) == 0) {
            continue;
        }

        config_mail* p = new config_mail;
        p->mail_id                         = _table->GetField            (i, "ID"              );
		p->key							   = _table->GetFieldString      (i, "Key"             );
		p->content_value				   = _table->GetFieldString      (i, "Value"           );
		p->brod_times				       = _table->GetField            (i,"BrodTimes"        );

		_cp.insert(std::pair<int32, config_mail*>(p->mail_id,p));
    }

    return 0;
}

int config_mail_mgr::clear()
{
    for (std::map<uint32, config_mail*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_mail* config_mail_mgr::get_config_mail(uint32 mail_id) const
{
    std::map<uint32, config_mail*>::const_iterator itr = _cp.find(mail_id);
	if (itr != _cp.end()){
		return itr->second;
	}
	

	return NULL;
}
