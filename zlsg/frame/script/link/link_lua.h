#ifndef __TEST_LUA_H__
#define __TEST_LUA_H__

struct lua_State;
struct game_service_t;
class scene_user;

class link_lua {
public:
    link_lua();
    ~link_lua();

public:
    void init();
    void release();

public:
    void link(scene_user* psu);
    int  call();

public:

public: // reg_pb
    void bind_reg();
    void init_reg_pb();
    void do_reg();

public: // game
    void bind_game();
    void init_game_pb();
    void do_pb(game_service_t* pgs, const std::string& name, const std::string& buf, client::server_head* svr_head);

public:
    void do_test(game_service_t* pgs, unsigned int role_id, unsigned int client_id, const std::string& script);

public: // center

public:
    inline lua_State* get_l() const { return _l; }
    inline void set_l(lua_State* val) { _l = val; }

private:
    lua_State* _l;
};

#endif // __TEST_LUA_H__