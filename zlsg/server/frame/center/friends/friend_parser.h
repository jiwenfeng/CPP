#ifndef __FRIEND_PARSER_H__
#define __FRIEND_PARSER_H__

#include "comm_define.h"

#define MAX_FRIEND_BLOB_LEN 65560 // �����ѻ��泤��

class friend_parser
{
public:
	friend_parser();

	virtual ~friend_parser();

	inline const std::map<uint32, client::friend_info> get_list() const{return map_fi;}
protected:
	int parse_friend(const string& in);     // ��DB�е����ݣ��������ʼ�PB
	int serialize_friend(string& out); // ���ʼ�PB���л���DB����

	void clear();

protected:
	std::map<uint32, client::friend_info> map_fi;
};


#endif
