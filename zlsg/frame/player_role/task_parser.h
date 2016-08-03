#ifndef __TASK_PARSER_H__
#define __TASK_PARSER_H__

#include "comm_define.h"
#include "client.task.pb.h"
#include "client.open.pb.h"

#define MAX_TASK_BLOB_LEN 200000
#define MAX_TASK_NUM      1000

class task_parser
{
public:
    task_parser();
    virtual ~task_parser();

protected:
    int parse_acpt_task(const std::string& in);
    int parse_comp_task(const std::string& in);
    int parse_open(const std::string& in);

    int serialize_acpt_task(std::string& out);
    int serialize_comp_task(std::string& out);
    int serialize_open(std::string& out);

    void clear();

public:
    inline const bool is_task_completed(uint32 task_id) const { return comp_tasks.count(task_id) > 0; }
    inline const bool is_task_accepted(uint32 task_id)  const { return acpt_tasks.count(task_id) > 0; }
    inline const std::map<uint32, client::task_accepted>&  get_accepted_tasks()  const { return acpt_tasks; }
    inline const std::map<uint32, client::task_completed>& get_completed_tasks() const { return comp_tasks; }
    inline const std::map<uint32, client::open_info>& get_opens() const { return _opens; }
    inline std::map<uint32, client::task_accepted>*  mutable_accepted_tasks()  { return &acpt_tasks; }
    inline std::map<uint32, client::task_completed>* mutable_completed_tasks() { return &comp_tasks; }
    inline std::map<uint32, client::open_info>* mutable_opens() { return &_opens; }

protected:
    std::map<uint32, client::task_accepted>  acpt_tasks;     // 已接取未完成的任务
    std::map<uint32, client::task_completed> comp_tasks;     // 已经完成的任务
    std::map<uint32, client::open_info>      _opens;
};

#endif // __TASK_PARSER_H__
