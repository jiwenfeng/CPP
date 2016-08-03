#ifndef __CONFIG_OPEN_H__
#define __CONFIG_OPEN_H__

#include "cfg_cache_mgr.h"
#include "config_base.h"

class config_open
{
public:
    config_open(void);
    ~config_open(void);

public:
    inline int32 get_func_id       () const { return _func_id       ; }
    inline int32 get_open_type     () const { return _open_type     ; }
    inline int32 get_open_condition() const { return _open_condition; }

public:
    static const int32 OPEN_TYPE_QUEST = 1;
    static const int32 OPEN_TYPE_LEVEL = 2;

public:
    int32                   _func_id;
    int32                   _open_type;
    int32                   _open_condition;
};

class config_open_mgr : public config_base_mgr
{
public:
    config_open_mgr(TTableGroup_s* p = NULL);
    virtual ~config_open_mgr(void);

public:
    virtual int init(TTableGroup_s* p);
    int clear();

public:
    inline const TTable* get_table() const { return _table; }
    inline const std::map<uint32, config_open*>& get_cfg() const { return _cfg; }

    const config_open* get_config_open(uint32 id) const;

public:
    uint32 get_max() const;

private:
    TTableGroup_s* _group;
    TTable*        _table;

private:
    std::map<uint32, config_open*> _cfg;
};

#endif // __CONFIG_OPEN_H__
