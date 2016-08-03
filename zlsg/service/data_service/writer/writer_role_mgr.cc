#include "writer_role_mgr.h"
#include "writer.h"

writer_role_mgr::writer_role_mgr() : cdw(DEFAULT_WRITE_NUM)
{
    
}

writer_role_mgr::~writer_role_mgr()
{

}

int writer_role_mgr::push(uint32 type, uint32 key1, uint32 key2, google::protobuf::Message* msg)
{
    if (msg == NULL) {
        return -1;
    }
   
    CACHE_KEY key(key1, key2);
    int ret = cdw.push(msg->GetTypeName().c_str(), key, msg, (int)type);
    if (ret != 0) {
        return -2;
    }
    
    return 0;
}

int writer_role_mgr::write()
{
    return cdw.write();
}

int writer_role_mgr::write_all()
{
    return cdw.write_all();
}

int writer_role_mgr::close_all()
{
    return cdw.close_all();
}

