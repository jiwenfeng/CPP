// 将游戏配置生成PB批量文件
#include <iostream>
#include <fstream>

#include "sql_parser.h"
#include "pb_mgr.h"

using namespace std;
pb_mgr*        PBMGR = new pb_mgr;

#define MAX_CFG_TYPES 2000
static char cfg_type_names[MAX_CFG_TYPES][128];

sql_parser g_parser;
database db;

int read_cfgs(const char* type_name)
{
    char file_name[256] = {0};
    snprintf(file_name, sizeof(file_name), "%s.cfg", type_name);

    uint32 success_count = 0;
    ifstream infile(file_name,ifstream::binary);
    infile.seekg(0, ifstream::end);
    std::streamoff size = infile.tellg();
    infile.seekg(0);

    if (size > 0){
        char* buffer = new char[(unsigned int)size];
        infile.read (buffer, size);
        long offset = 0;
        
        while (offset < size){
            google::protobuf::Message* pb = create_msg(type_name);
            if (pb == NULL){
                break;
            }

            int pb_len = *(int*)(buffer+offset);
            offset += sizeof(int);

            if (!pb->ParseFromArray((const void*)(buffer + offset), pb_len)) { 
		        delete pb;
                break;
            } else {
                ++success_count;
            }
            offset += pb_len;
        }

        delete[] buffer;
    }

    infile.close();

    cout<<"read "<<type_name<<" "<<success_count<<endl;
    return 0;
}

int load_cfgs(char* _type_name)
{
	const char* sql = NULL;
	char* pos = strchr(_type_name, '|');
	if (pos != NULL){
		_type_name = pos+1;
		pos = strchr(_type_name, '|');
		if (pos != NULL){
			*pos = '\0';
		}
	} 
	
	char type_name[256] = {0};
	snprintf(type_name, sizeof(type_name), "mysql.%s", _type_name);
	sql = PBMGR->get_querysql(type_name);
    if (sql == NULL){
        cout<<"get "<<type_name<<" querysql failed"<<endl;
        return -1;
    }

    size_t rsp_len;
    struct cmq_svr_msg_t* msg = g_parser.gets(type_name, sql, db, rsp_len);
    if (msg == NULL){
        cout<<cout<<"get "<<type_name<<" cmq_svr_msg_t failed"<<endl;
        return -2;
    }

    uint32 buf_size = 50*1024*1024;
    uint32 offset = 0;
    char* buf = new char[buf_size];

    int succ_count = 0;
    for (int i = 0; i < msg->pb_count; ++i){
        google::protobuf::Message* pb = msg->pb_ctx[i].pb_msg;
        int pb_len = pb->ByteSize();
        *(int*)(buf + offset) = pb_len;
        offset += sizeof(int);

        pb->SerializeToArray((void*)(buf + offset), pb_len);

        //pb->Clear();
        //pb->ParseFromArray((const void*)(buf + offset), pb_len);

        offset += pb_len;

        delete pb;

        ++succ_count;
    }
    free(msg);

	char file_name[256] = {0};
	{    
		snprintf(file_name, sizeof(file_name), "%s.pb", type_name);
		ofstream outfile (file_name, ofstream::binary);
		outfile.write(buf, offset);
		outfile.close();
	}

	{
		snprintf(file_name, sizeof(file_name), "%s.cfg", type_name);
		ofstream outfile (file_name, ofstream::binary);
		outfile.write(buf, offset);
		outfile.close();
	}
	

    delete[] buf;

	read_cfgs(type_name);
	
    return succ_count;
}

int get_type_names(const char* file_name)
{
	int count = 0;
	ifstream type_file(file_name);
	while(type_file.good() && count < MAX_CFG_TYPES){
		type_file.getline(cfg_type_names[count++], 128);
	}
	type_file.close();

	return count;
}

int main(int argc, char* argv[])
{
	if (argc < 2){
		return -1;
	}
	int cfg_size = get_type_names(argv[1]);

    const char* db_name = "BLCX";
    if (argc > 2){
        db_name = argv[2];
    }

    db.set_param("192.168.1.242", "root", "!Q@W#E$R%T^Y", db_name);
    if (db.open() != 0){
        cout<<"db.open() failed\n"<<endl;
        return -1;
    }

    for (int i = 0; i < cfg_size; ++i){
        int count = load_cfgs(cfg_type_names[i]);
        if (count >= 0){
            cout<<"load "<<cfg_type_names[i]<<" success, count:"<<count<<endl;
        } else {
            cout<<"load "<<cfg_type_names[i]<<" failed, error:"<<count<<endl;
        }
    }
    db.close();

    return 0;
}

