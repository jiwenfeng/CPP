#include "friend_parser.h"

friend_parser::friend_parser()
{

}

friend_parser::~friend_parser()
{
	clear();
}

int friend_parser::parse_friend(const string& in)
{
	clear();

	char* ptr = (char*)in.c_str();
	uint32 len = in.size();
	uint32 offset = 0;

	while (offset < len) {
		uint16 msg_len = *(uint16*)(ptr + offset);
		offset += sizeof(uint16);

		client::friend_info fi;
		if (!fi.ParseFromArray(ptr + offset, msg_len)) {
			return 1;
		}

		offset += msg_len;

		if (map_fi.count(fi.role_id()) > 0){
			continue;
		}

		map_fi[fi.role_id()] = fi;
	}

	return 0;
}

int friend_parser::serialize_friend(string& out)
{
    out = "";
	char buf[MAX_FRIEND_BLOB_LEN];
	uint32 offset = 0;

	map<uint32, client::friend_info>::iterator it = map_fi.begin();
	for (; it != map_fi.end(); ++it){
		client::friend_info& fi = it->second;
		uint32 msg_len = fi.ByteSize();
		if (offset + sizeof(uint16) + msg_len > MAX_FRIEND_BLOB_LEN){
			break;
		}

		*(uint16*)(buf + offset) = (uint16)msg_len;
		offset += sizeof(uint16);

		fi.SerializeToArray((void*)(buf + offset), msg_len);
		offset += msg_len;
	}

	out.append(buf, offset);

	return 0;
}

void friend_parser::clear()
{
	
}
