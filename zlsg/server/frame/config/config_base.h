#ifndef __CONFIG_BASE_H__
#define __CONFIG_BASE_H__

struct table_index
{
    int _source;
    int _index;
};

struct TTableGroup_s;

class config_base_mgr {
public:
    config_base_mgr() {}
    virtual ~config_base_mgr() {}

public:
    virtual int init(TTableGroup_s* p) = 0;
};

#endif // __CONFIG_BASE_H__