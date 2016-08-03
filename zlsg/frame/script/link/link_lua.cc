extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include "lua_tinker.h"

#include "game_interface.h"
#include "scene_user.h"

#include "link_lua.h"

link_lua::link_lua()
{
    init();
}

link_lua::~link_lua()
{
    release();
}

void link_lua::init()
{
    _l = luaL_newstate();//lua_tinker::lua_context();
    luaL_openlibs(get_l());
    //luaopen_base(_l);
    //luaopen_string(_l);
}

void link_lua::release()
{
    lua_close(_l);
}

void link_lua::link(scene_user* psu)
{
    bind_game();
    lua_tinker::set(get_l(), "psu", psu);
    lua_tinker::dofile(_l, "E:\\zlsg\\code\\zproj\\server\\frame\\script\\src\\test.lua");

    call();
}

int link_lua::call()
{
    return lua_tinker::call<int>(get_l(), "test_func");
}
#include "client.comm.pb.h"
void link_lua::bind_game()
{
    // 继承或重载函数要确定其类型
    typedef int (*send_client) (struct game_service_t* pgs, uint32 client_id, const char* name, const char* bin, uint32 len);
    send_client gsc = game_send_client;
    lua_tinker::def(get_l(), "game_send_client", gsc);

    lua_tinker::class_add<game_service_t>(get_l(), "game_service_t");
    lua_tinker::class_mem<game_service_t>(get_l(), "sim", &game_service_t::sim);

    lua_tinker::class_add<scene_inst_mgr>(get_l(), "scene_inst_mgr");
    lua_tinker::class_def<scene_inst_mgr>(get_l(), "get_user", &scene_inst_mgr::get_user);

    lua_tinker::class_add<client::server_head>(get_l(), "client_server_head");
    lua_tinker::class_def<client::server_head>(get_l(), "svr_id", &client::server_head::svr_id);
    lua_tinker::class_def<client::server_head>(get_l(), "user_id", &client::server_head::user_id);
    lua_tinker::class_def<client::server_head>(get_l(), "role_id", &client::server_head::role_id);
    lua_tinker::class_def<client::server_head>(get_l(), "agent_id", &client::server_head::agent_id);
    lua_tinker::class_def<client::server_head>(get_l(), "client_id", &client::server_head::client_id);
    lua_tinker::class_def<client::server_head>(get_l(), "scenesvr_id", &client::server_head::scenesvr_id);
    lua_tinker::class_def<client::server_head>(get_l(), "client_ip", &client::server_head::client_ip);
    lua_tinker::class_def<client::server_head>(get_l(), "user_name", &client::server_head::user_name);

    typedef uint32 (scene_user::*get_attr) (ROLE_ATTRIBUTE type) const;
    typedef uint32 (scene_user::*get_role_id) () const;
    typedef const char* (scene_user::*get_role_name) () const;
    get_role_id   pget_role_id   = &scene_user::get_role_id;
    get_role_name pget_role_name = &scene_user::get_role_name;
    get_attr      pget_attr      = &scene_user::get_attr;
    lua_tinker::class_add<scene_user>(get_l(), "scene_user");
    lua_tinker::class_def<scene_user>(get_l(), "get_role_id"  , pget_role_id  );
    lua_tinker::class_def<scene_user>(get_l(), "get_role_name", pget_role_name);
    lua_tinker::class_def<scene_user>(get_l(), "get_attr"     , pget_attr     );
}

void link_lua::bind_reg()
{
    lua_tinker::class_add<client_pb_mgr>(get_l(), "client_pb_mgr");
    lua_tinker::class_def<client_pb_mgr>(get_l(), "insert", &client_pb_mgr::insert);
    lua_tinker::set(get_l(), "cpm", CLIENT_PB_MGR);
}

void link_lua::init_reg_pb()
{
    bind_reg();
    lua_tinker::dofile(get_l(), "./lua/reg_service.lua");
}

void link_lua::do_reg()
{
    lua_tinker::call<int>(get_l(), "reg_pb");
}

void link_lua::init_game_pb()
{
    bind_game();
    lua_tinker::dofile(get_l(), "./lua/game_service.lua");
}

void link_lua::do_pb(game_service_t* pgs, const std::string& name, const std::string& buf, client::server_head* svr_head)
{
    lua_pushcclosure(get_l(), lua_tinker::on_error, 0);
    int errfunc = lua_gettop(get_l());

    lua_getglobal(get_l(), "do_pb");

    if(lua_isfunction(get_l(), -1)) {
        lua_tinker::push(get_l(), pgs);
        lua_tinker::push<const std::string&>(get_l(), name);
        lua_tinker::push<const std::string&>(get_l(), buf);
        lua_tinker::push(get_l(), svr_head);
        if(lua_pcall(get_l(), 4, 1, errfunc) != 0) {
            lua_pop(get_l(), 1);
        }
    } else {
        lua_tinker::print_error(get_l(), "lua_tinker::call() attempt to call global `%s' (not a function)", name.c_str());
    }

    lua_remove(get_l(), -2);
    /*int result = */lua_tinker::pop<int>(get_l());

    //int result = lua_tinker::call<int>(get_l(), , pgs, name, buf, len);
    //amtsvr_log("%d\n", result);
}

void link_lua::do_test(game_service_t* pgs, unsigned int role_id, unsigned int client_id, const std::string& script)
{
    bind_game();
    lua_tinker::dofile(get_l(), "./lua/pb_test.lua");

    lua_pushcclosure(get_l(), lua_tinker::on_error, 0);
    int errfunc = lua_gettop(get_l());

    lua_getglobal(get_l(), "do_test");

    if(lua_isfunction(get_l(),-1)) {
        lua_tinker::push(get_l(), pgs);
        lua_tinker::push(get_l(), role_id);
        lua_tinker::push(get_l(), client_id);
        lua_tinker::push<const std::string&>(get_l(), script);
        if(lua_pcall(get_l(), 4, 1, errfunc) != 0) {
            lua_pop(get_l(), 1);
        }
    } else {
        lua_tinker::print_error(get_l(), "lua_tinker::call() attempt to call global `%s' (not a function)", script.c_str());
    }

    lua_remove(get_l(), -2);
    /*int result = */lua_tinker::pop<int>(get_l());
}

