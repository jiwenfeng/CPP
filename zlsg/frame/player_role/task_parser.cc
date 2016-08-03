#include "task_parser.h"

task_parser::task_parser()
{
}

task_parser::~task_parser()
{
    clear();
}

void task_parser::clear()
{
    acpt_tasks.clear();
    comp_tasks.clear();
}

int task_parser::parse_acpt_task(const std::string& in)
{
    char*  ptr    = (char*)in.c_str();
    uint32 len    = in.size();
    uint32 offset = 0;
    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::task_accepted rt;
        if (!rt.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }

        offset += msg_len;

        if (acpt_tasks.find(rt.task_id()) != acpt_tasks.end()) {
            continue;
        }

        acpt_tasks.insert(pair<uint32, client::task_accepted>(rt.task_id(), rt));
    }

    return 0;
}

int task_parser::parse_comp_task(const std::string& in)
{
    char*  ptr    = (char*)in.c_str();
    uint32 len    = in.size();
    uint32 offset = 0;
    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::task_completed cpt;
        if (!cpt.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }
        offset += msg_len;

        if (comp_tasks.find(cpt.task_id()) != comp_tasks.end()) {
            continue;
        }

        comp_tasks.insert(pair<uint32, client::task_completed>(cpt.task_id(), cpt));
    }

    return 0;
}

int task_parser::parse_open(const std::string& in)
{
    char*  ptr    = (char*)in.c_str();
    uint32 len    = in.size();
    uint32 offset = 0;
    while (offset < len) {
        uint16 msg_len = *(uint16*)(ptr + offset);
        offset += sizeof(uint16);

        client::open_info cpt;
        if (!cpt.ParseFromArray(ptr + offset, msg_len)) {
            break;
        }

        offset += msg_len;

        if (_opens.find(cpt.id()) != _opens.end()) {
            continue;
        }

        _opens.insert(pair<uint32, client::open_info>(cpt.id(), cpt));
    }

    return 0;
}

int task_parser::serialize_acpt_task(std::string& out)
{
    int    count                  = 0;
    char   buf[MAX_TASK_BLOB_LEN] = { 0 };
    uint32 offset                 = 0;
    std::map<uint32, client::task_accepted>::iterator it = acpt_tasks.begin();
    for (; it != acpt_tasks.end(); ++it) {
        client::task_accepted& rat = it->second;

        uint32 msg_len = rat.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_TASK_BLOB_LEN){
            break;
        }

        *(uint16*)(buf + offset) = (uint16)msg_len;
        offset += sizeof(uint16);

        rat.SerializeToArray((void*)(buf + offset), msg_len);
        offset += msg_len;

        ++count;
    }

    if (count > 0) {
        out.append(buf, offset);
    }

    return count;
}

int task_parser::serialize_comp_task(std::string& out)
{
    int    count                  = 0;
    char   buf[MAX_TASK_BLOB_LEN] = {0};
    uint32 offset                 = 0;
    std::map<uint32, client::task_completed>::iterator it = comp_tasks.begin();
    for (; it != comp_tasks.end(); ++it) {
        client::task_completed& rct = it->second;
        uint32 msg_len = rct.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_TASK_BLOB_LEN){
            break;
        }

        *(uint16*)(buf + offset) = (uint16)msg_len;
        offset += sizeof(uint16);

        rct.SerializeToArray((void*)(buf + offset), msg_len);
        offset += msg_len;

        ++count;

    }

    if (count > 0){
        out.append(buf, offset);
    }

    return count;
}

int task_parser::serialize_open(std::string& out)
{
    int    count                  = 0;
    char   buf[MAX_TASK_BLOB_LEN] = {0};
    uint32 offset                 = 0;
    std::map<uint32, client::open_info>::iterator it = _opens.begin();
    for (; it != _opens.end(); ++it) {
        client::open_info& rct = it->second;
        uint32 msg_len = rct.ByteSize();
        if (offset + sizeof(uint16) + msg_len > MAX_TASK_BLOB_LEN) {
            break;
        }

        *(uint16*)(buf + offset) = (uint16)msg_len;
        offset += sizeof(uint16);

        rct.SerializeToArray((void*)(buf + offset), msg_len);
        offset += msg_len;

        ++count;
    }

    if (count > 0) {
        out.append(buf, offset);
    }

    return count;
}

