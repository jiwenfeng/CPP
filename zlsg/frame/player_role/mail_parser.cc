#include "mail_parser.h"

mail_parser::mail_parser()
{
}

mail_parser::~mail_parser()
{
    clear();
}

void mail_parser::clear()
{
    mail_list.clear();
}

int mail_parser::parse_mail(const string& in)
{
    clear();

    char* ptr = (char*)in.c_str();
    uint32 len = in.size();
    uint32 offset = 0;
    int count = 0;

    while (offset < len){
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        role_mail_info mi;
        if (!mi.ParseFromArray(ptr + offset, msg_len)){
            break;
        }

        offset += msg_len;

        mail_list.push_back(mi);
        ++count;
        client::mail_head* mh = mail_list[count-1].mutable_mh();
        mh->set_mail_id(count);
    }

    return 0;
}
// 存邮件数据
int mail_parser::serialize_mail(string& out)
{
    char buf[MAX_MAIL_BOLB_LEN];
    uint32 offset = 0;
    int count = 0;

    uint32 size = mail_list.size();
    for (uint32 i = 0; i < size; ++i){
        role_mail_info& mi = mail_list[i];
        if (!mi.is_normal()){
            continue;
        }
		// 检测邮件的过期时间
		if (!is_past_time_mail(mi)){
			continue;
		}
        uint32 msg_len = mi.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_MAIL_BOLB_LEN){
            break;
        }

        *(uint16*)(buf + offset) = (uint16)msg_len;
        offset += sizeof(uint16);
        
        mi.SerializeToArray((void*)(buf + offset), msg_len);
        offset += msg_len;
        
        ++count;
        
    }
    out.append(buf, offset);

    return count;
}

bool mail_parser::is_past_time_mail(role_mail_info& mail_info)
{
	uint32 cur_time = (uint32)time(NULL);
	uint32 mail_time = mail_info.mh().past_time();
	if (cur_time >= mail_time){
		return false;
	}

	return true;
}