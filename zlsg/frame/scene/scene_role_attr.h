#ifndef __SCENE_ROLE_ATTR_H__
#define __SCENE_ROLE_ATTR_H__

#include "scene_define.h"

class scene_role;

class scene_role_attr : public client::role_attr
{
public:
    scene_role_attr();
    scene_role_attr(const scene_role_attr& rhs);
    virtual ~scene_role_attr();

public:
    scene_role_attr& operator=(const scene_role_attr& rhs);

public:
    int init(scene_role* pr);

public:
    inline void set_modify_attr(scene_role_attr* m) { modify_attr = m; }
    void clear_modify_attr();

public:
    bool fill_broadcast_attr(const client::role_attr& attr, client::role_attr& broadcast_attr) const;
    bool fill_modify_attr(client::role_attr& attr, client::role_attr& broadcast_attr, bool& need_broadcast);
    void fill_all_attr(client::role_attr& attr);

public:
    uint32 get_val(ROLE_ATTRIBUTE index) const;
    uint64 get_val_64(ROLE_ATTRIBUTE index) const;
    uint64 get_val_(ROLE_ATTRIBUTE index) const;

    void set_val(ROLE_ATTRIBUTE index, uint32 val);
    void set_val_64(ROLE_ATTRIBUTE index, uint64 val);
    void set_val_(ROLE_ATTRIBUTE index, uint64 val);

    void add_val(ROLE_ATTRIBUTE index, uint32 val);
    void add_val64(ROLE_ATTRIBUTE index, uint64 val);

    void sub_val(ROLE_ATTRIBUTE index, uint32 val);
    void sub_val64(ROLE_ATTRIBUTE index, uint64 val);

    void multi_val(ROLE_ATTRIBUTE index, float val);

    void divide_val(ROLE_ATTRIBUTE index, uint32 val);

public:
    inline uint32 field_count() const { return _des->field_count(); }

protected:
    const google::protobuf::Reflection* _ref;
    const google::protobuf::Descriptor* _des;

    scene_role_attr* modify_attr;
    bool change_attr;

    scene_role* _pr;
};


#endif
