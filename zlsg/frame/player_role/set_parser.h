#ifndef __SET_PARSER_H__
#define __SET_PARSER_H__

#include "comm_define.h"

#define MAX_SET_BOLB_LEN 66560

#define SHORTCUT_MAX_GRID 19
#define SHORTCUT_MAX_TYPE 2

class set_parser
{
public:
    set_parser();
    virtual ~set_parser();

protected:
    void clear();

    int parse(const string& in);

    int serialize(string& out);

protected:
    client::role_set_info _role_set_info;
};

#endif /* __SET_PARSER_H__ */
