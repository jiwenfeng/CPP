#ifndef __SCENE_ROLE_STATUS_H__
#define __SCENE_ROLE_STATUS_H__

#include "scene_define.h"

class scene_role;

class scene_role_status : public client::role_status
{
public:
    scene_role_status();
    scene_role_status(const scene_role_status& rhs);

    virtual ~scene_role_status();

public:
    int init(mysql::tb_role_status* s, scene_role* psr);

    void normalize();

    void set_modify_status(scene_role_status* srs);

    bool fill_modify_status(client::role_status* srs);
    bool pop_modify_status(client::role_status& srs);
    void pop_all_status(client::role_status& srs);

    void reset();

public:
    void set_val(uint32 index, uint32 val);

    void set_dead();
    void set_relive();
    void set_attack_mode(uint8 attk_type, uint8 attk_mode);
    void set_attack_mode(const client::role_status_attack_mode m);
    void set_guard_start(uint32 quality);
    void set_guard_end();
    void set_raid(bool raid = true);

    bool client_set_attack_mode(uint8 attk_type, uint8 attk_mode, const client::role_status_attack_mode set_type);

    inline bool is_dead     () const { return alive   () == client::role_status_alive_status_status_dead; }
    inline bool is_meditate () const { return meditate(); }
    inline bool is_raid     () const { return streanth(); }
    inline bool is_move     () const { return giddy   (); }
    inline bool is_pk       () const { return fight   (); }
    inline bool is_use_skill() const { return silent  (); }

private:
    int init();

protected:
    const google::protobuf::Reflection* _ref;
    const google::protobuf::Descriptor* _desc;

    scene_role_status* _modify_status;
    bool               _change_status;
    scene_role*        _psr;
};

#endif // __SCENE_ROLE_STATUS_H__
