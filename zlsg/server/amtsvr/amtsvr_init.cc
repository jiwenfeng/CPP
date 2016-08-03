#include "stdafx.h"
#include "amtsvr.h"
#include "amtsvr_imp.h"
#include "amtsvr_module.h"

#include "pb_mgr.h"
#include "config_map.h"
#include "config_mgr.h"
#include "game_map_mgr.h"
#include "sql_operator.h"
#include "sql_parser.h"
#include "cfg_cache_mgr.h"

#include "logger_service.h"

#include "sql_mgr.h"
#include "loader.h"
#include "writer.h"

#include "role_service.h"
#include "center_service.h"
#include "game_service.h"
#include "chores_service.h"

#include "gate_service.h"
#include "watchdog_service.h"
#include "sender_service.h"
#include "agent_service.h"

#include "node_service.h"

#include <dirent.h>
#include <sys/stat.h>

pb_mgr*                PBMGR            = new pb_mgr;
config_mgr*            CONFMGR          = new config_mgr;
game_map_mgr*          GAME_MAP_MGR     = new game_map_mgr;
cfg_cache_mgr*         CONFIG_CACHE_MGR = new cfg_cache_mgr;
msg_handler_pool*      MSG_HANDLER_POOL = new msg_handler_pool;
client_pb_mgr*         CLIENT_PB_MGR    = new client_pb_mgr;
uint32                 g_run_time;
std::vector<string>    WATCHDOG_NAMES;
std::vector<string>    NODE_NAMES;
//map<string, char> reward_openids; 

// static const char dynamic_data_name[][128] = {"mysql.sp_get_gdata_max_role","mysql.sp_load_all_user_smy","mysql.sp_load_offline_user", "mysql.tb_rank_center", "mysql.tb_task_log", "mysql.tb_money_log", 
//     "mysql.tb_center_arena", "mysql.tb_arena_record", "mysql.tb_arena_reward", "mysql.tb_family", "mysql.tb_family_member", "mysql.tb_family_log", "mysql.tb_family_skill", "mysql.tb_family_record", 
//     "mysql.tb_family_domain", "mysql.tb_siege_warfare", "mysql.tb_mail_center", "mysql.tb_role_friends", "mysql.tb_gamesvr", "mysql.tb_promo_list_of_names", "mysql.tb_land_raid", "mysql.tb_land_raid_tax",
//     "mysql.tb_raid_center", "mysql.tb_godcraft_center", "mysql.tb_mate", "mysql.tb_sect", "mysql.tb_role_farm", "mysql.tb_saparatism_info"};
// 
static const char static_data_name[][128] = {{0}}; /*{"mysql.sp_get_gdata", "mysql.tb_role_init", "mysql.tb_skill", "mysql.tb_goods", "mysql.tb_goods_detail", "mysql.tb_effects", "mysql.tb_equip_set_sys",
                                                   "mysql.tb_trade_center", "mysql.tb_npc_store", "mysql.tb_npc", "mysql.tb_npc_in_scene", "mysql.tb_soul","mysql.tb_spare_skill_init",
                                                   "mysql.tb_soul_doom", "mysql.tb_trans", "mysql.tb_pill", "mysql.tb_pill_fire", "mysql.tb_refine", "mysql.tb_cmpst", "mysql.tb_cmpst_prdct",
                                                   "mysql.tb_cmpst_prdct_detail", "mysql.tb_sys_reward", "mysql.tb_sys_reward_detail", "mysql.tb_achieve_code", "mysql.tb_achieve", "mysql.tb_raid",
                                                   "mysql.tb_internal_activity", "mysql.tb_server_map_br", "mysql.tb_map", "mysql.tb_map_scene_br", "mysql.tb_npc_switcher", "mysql.tb_battle", "mysql.tb_scene",
                                                   "mysql.tb_scene_bindata", "mysql.tb_switcher", "mysql.tb_region", "mysql.tb_region_relive", "mysql.tb_map_scene_br", "mysql.tb_npc_refresh_time", "mysql.tb_npc_skill", "mysql.tb_world_lose",
                                                   "mysql.tb_raid_lose", "mysql.tb_normal_lose", "mysql.tb_raid_detail", "mysql.tb_battle_actvt_time", "mysql.tb_battle_group", "mysql.tb_battle_reward",
                                                   "mysql.tb_hot", "mysql.tb_hot_actvt_time", "mysql.tb_npc_in_master_cave_scene", "mysql.tb_sys_para_config", "mysql.tb_sys_func_config", "mysql.tb_group_relation", "mysql.tb_npc_war",
                                                   "mysql.tb_equip_set_sys","mysql.tb_equip_set_num","mysql.tb_equip_set_num_attr","mysql.tb_target","mysql.tb_task","mysql.tb_task_chain","mysql.tb_task_limit","mysql.tb_task_lose",
                                                   "mysql.tb_task_reward","mysql.tb_task_reward_detail","mysql.tb_task_targ_answ","mysql.tb_task_targ_attr","mysql.tb_task_targ_clt_event","mysql.tb_task_targ_coll","mysql.tb_task_targ_deliver",
                                                   "mysql.tb_task_targ_equip_oprt","mysql.tb_task_targ_esc","mysql.tb_task_targ_exec","mysql.tb_task_targ_guard","mysql.tb_task_targ_hand_equip","mysql.tb_task_targ_hand_props",
                                                   "mysql.tb_task_targ_kill","mysql.tb_task_targ_seek","mysql.tb_task_targ_skill","mysql.tb_task_targ_soul","mysql.tb_task_targ_talk","mysql.tb_task_targ_vessel","mysql.tb_task_targ_xlqy",
                                                   "mysql.tb_store", "mysql.tb_syspet_init","mysql.tb_equip_reset", "mysql.tb_equip_sublime", "mysql.tb_pulse", "mysql.tb_equip_advance_setting", "mysql.tb_raid_batch_npc", 
                                                   "mysql.tb_sys_message_tpl", "mysql.tb_promo_cfg", "mysql.tb_sys_label", "mysql.tb_aircraft", "mysql.tb_verse", "mysql.tb_farm_set"
                                                   };*/

/*
static int get_reward_users()
{
FILE * fp;
char * line = NULL;
size_t len = 0;
ssize_t read;

fp = fopen("./ids.txt", "r");
if (fp == NULL){
return 0;
}

char tmp[33] = {0};
while ((read = getline(&line, &len, fp)) != -1) {
if (read != 33){
continue;
}
snprintf(tmp, sizeof(tmp), "%s", line);
reward_openids[tmp] = 0;
}

if (line){
free(line);
}

fclose(fp);

return 0;
}
*/

int get_filename_list(std::vector<std::string>& FileList, const char* sExtend)
{    
    DIR*			pdir     = NULL;
    struct dirent*	pdir_ent = NULL;
    std::string str_tmp;
    std::string::size_type dot_pos;
    std::string sCurDir = "./game_cfg/";
    std::string sSubDir = "";
    vector<string> vDirList;
    vDirList.push_back(sCurDir);
    while (!vDirList.empty()) {
        pdir = opendir(vDirList.back().c_str());
        vDirList.pop_back();
        if (pdir == NULL) {
            return -1;
        }	

        while((pdir_ent = readdir(pdir)) != NULL) {
            str_tmp.clear();
            str_tmp = pdir_ent->d_name;
            if(strcmp(pdir_ent->d_name, ".") == 0 || strcmp(pdir_ent->d_name, "..") == 0 || strcmp(pdir_ent->d_name, ".svn") == 0) {
                continue;
            }

            if(pdir_ent->d_type == DT_DIR) {
                sSubDir = sCurDir +  pdir_ent->d_name + "/";
                vDirList.push_back(sSubDir);
                continue;
            }

            if((dot_pos = str_tmp.find_last_of(".")) == string::npos) {
                continue;
            }

            if (dot_pos >= str_tmp.length()) {
                continue;
            }

            if(str_tmp.substr(dot_pos,str_tmp.length() - dot_pos).compare(sExtend) !=0 ) {
                continue;
            }

            FileList.push_back(str_tmp.substr(0, dot_pos));
        }

        closedir(pdir);
    }

    return 0;
}

//#define PRINT_TREE
string find_file(const char* dir, const char* name)
{
#ifdef PRINT_TREE
    static int level = -1;
    level++;
#endif

    DIR* dirp = opendir(dir);
    if (dirp != NULL) {
        struct dirent* dp;
        while ((dp = readdir(dirp)) != NULL) {
            /*
            ** build filename
            */
            char filename[NAME_MAX + 1] = {0};
            strcpy(filename, dir);
            //strcat(filename, "/");
            strcat(filename, dp->d_name);

            /*
            ** get file type
            */
            struct stat buf;
            if (stat(filename, &buf) == 0) {
#ifdef PRINT_TREE
                for ( int i=0; i<level; i++) printf(" ");
                printf("%s", dp->d_name );
                if (S_ISDIR(buf.st_mode))
                    printf("/");
                printf("\n");
#endif

                if (S_ISREG(buf.st_mode) && strcasecmp(dp->d_name, name) == 0) {
                    closedir(dirp);
#ifdef PRINT_TREE
                    level--;
#endif
                    //printf("dir %s \n", dir);
                    return dir;
                }
                else if (S_ISDIR(buf.st_mode) && strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
                    //printf("filename %s \n", filename);
                    size_t len = strlen(filename);
                    if (len > 1 && filename[len - 1] != '/') {
                        strcat(filename, "/");
                    }
                    string found = find_file(filename, name);
                    //printf("found %s \n", found.c_str());
                    if (!found.empty()) {
                        closedir(dirp);
#ifdef PRINT_TREE
                        level--;
#endif
                        return found;
                    }
                }
            }
        }
        closedir(dirp);
    }
#ifdef PRINT_TREE
    level--;
#endif
    return "";
}

int init_cfgs_from_dot_bin_file()
{
    amtsvr_log("\n");
    std::vector<std::string> filenames;
    get_filename_list(filenames, ".bin");
    std::map<std::string, TTableGroup_s*>* cfg = CONFIG_CACHE_MGR->get_cache_cfg();
    if (cfg != NULL) {
        std::map<std::string, TTableGroup_s*>::iterator i = cfg->begin();
        for ( ; i != cfg->end(); ++i) {
            if (i->second != NULL) {
                delete i->second;
            }
        }

        cfg->clear();
        BinTable_s::clear();
    }

    for (size_t i = 0; i < filenames.size(); ++i) {
        if (cfg->find(filenames[i]) != cfg->end()) {
            continue;
        }

        TTableGroup_s* pTableGroup = BinTable_s::GetTalbeGroup(filenames[i]);
        cfg->insert(pair<std::string, TTableGroup_s*>(filenames[i], pTableGroup));
        //printf("cfg:%s\n", filenames[i].c_str());
    }

    return 0;
}

static int init_map_from_dot_data_file()
{
    amtsvr_log("\n");
    std::vector<std::string> filenames;
    get_filename_list(filenames, ".data");
    std::map<std::string, MapConfig_s*>* mm = CONFIG_CACHE_MGR->get_cache_map();
    if (mm != NULL) {
        std::map<std::string, MapConfig_s*>::iterator i = mm->begin();
        for ( ; i != mm->end(); ++i) {
            if (i->second != NULL) {
                delete i->second;
            }
        }
        
        mm->clear();
    }

    for (size_t i = 0; i < filenames.size(); ++i) {
        if (mm->find(filenames[i]) != mm->end()) {
            amtsvr_log("map repeat id = %s\n", filenames[i].c_str());
            continue;
        }

        MapConfig_s* p = new config_map;
        string filepath = find_file("./game_cfg/", (filenames[i] + ".data").c_str()) + filenames[i];
        //printf("filepath %s \n", filepath.c_str());
        if (!p->loadConfig(filepath.c_str())) {
            delete p;
            continue;
        }

        mm->insert(pair<std::string, MapConfig_s*>(filenames[i], p));
        //printf("map:%s\n", filenames[i].c_str());
    }

    return 0;
}

static int init_cfgs_from_file(const char* type_name)
{
    cfg_cache_ctx* ctx = CONFIG_CACHE_MGR->get_cache_ctx(type_name);
    if (ctx == NULL) { 
        return -1;
    }

    char file_name[256] = {0};
    snprintf(file_name, sizeof(file_name), "./game_cfg/%s.cfg", type_name);

    ifstream infile(file_name,ifstream::binary);
    infile.seekg(0, ifstream::end);
    std::streamoff size = infile.tellg();
    infile.seekg(0);

    if (size > 0) {
        char* buffer = new char[(unsigned int)size];
        infile.read (buffer, size);
        long offset = 0;

        while (offset < size) {
            google::protobuf::Message* pb = create_msg(type_name);
            if (pb == NULL){
                break;
            }

            int pb_len = *(int*)(buffer+offset);
            offset += sizeof(int);

            if (!pb->ParseFromArray((const void*)(buffer + offset), pb_len)) { 
                delete pb;
                break;
            }
            offset += pb_len;

            ctx->put(pb);
        }

        delete[] buffer;
    }
    infile.close();

    return 0;
}


static int game_static_data_init()
{
    int size = sizeof(static_data_name)/128;
    for (int i = 0; i < size; ++i){
        init_cfgs_from_file(static_data_name[i]);
    }

    init_cfgs_from_dot_bin_file();
    init_map_from_dot_data_file();

    return 0;
}

static int get_dynamic_data(struct amtsvr_config * config)
{
    database   db;
    db.set_param(config->db_host, config->db_user, config->db_pass, config->db_name, config->db_port);
    if (db.open() != 0) {
        return -1;
    }

    sql_parser sp;
//     int size = sizeof(dynamic_data_name) / 128;
//     for (int i = 0; i < size; ++i) {
//         const char* type_name = dynamic_data_name[i];
    std::map<string, pb_obj*>& pbs = PBMGR->get_cfg_pb_maps();
    for (std::map<string, pb_obj*>::iterator it = pbs.begin(); it != pbs.end(); ++it) {
        const char* sql = PBMGR->get_querysql(it->first);
        if (sql == NULL) {
            continue;
        }

        cfg_cache_ctx* ctx = CONFIG_CACHE_MGR->get_cache_ctx(it->first.c_str());
        if (ctx == NULL) {
            continue;
        }

        size_t rsp_len;
        struct cmq_svr_msg_t* msg = sp.gets(it->first.c_str(), sql, db, rsp_len);
        if (msg == NULL) {
            continue;
        }

        for (int i = 0; i < msg->pb_count; ++i) {
            ctx->put(msg->pb_ctx[i].pb_msg);
        }

        free(msg);
    }

    return 0;
}

static int game_dynamic_data_init(struct amtsvr_config * config)
{
    int ret = get_dynamic_data(config);
    if (ret != 0) {
        return -1;
    }

    return 0;
}

static int game_data_init(struct amtsvr_config * config)
{
    int ret = game_static_data_init();
    if (ret != 0) {
        return -1;
    }

    ret = game_dynamic_data_init(config);
    if (ret != 0) {
        return -2;
    }

    return 0;
}

static int game_module_init()
{
    amtsvr_module_create("logger", (amtsvr_dl_create)logger_create, (amtsvr_dl_init)logger_init, (amtsvr_dl_release)logger_release);

    amtsvr_module_create("sqlmgr", (amtsvr_dl_create)sqlmgr_create, (amtsvr_dl_init)sqlmgr_init, (amtsvr_dl_release)sqlmgr_release);
    amtsvr_module_create("loader", (amtsvr_dl_create)loader_create, (amtsvr_dl_init)loader_init, (amtsvr_dl_release)loader_release);
    amtsvr_module_create("writer", (amtsvr_dl_create)writer_create, (amtsvr_dl_init)writer_init, (amtsvr_dl_release)writer_release);

    amtsvr_module_create("role",   (amtsvr_dl_create)role_create,   (amtsvr_dl_init)role_init,   (amtsvr_dl_release)role_release);
    amtsvr_module_create("center", (amtsvr_dl_create)center_create, (amtsvr_dl_init)center_init, (amtsvr_dl_release)center_release);
    amtsvr_module_create("game",   (amtsvr_dl_create)game_create,   (amtsvr_dl_init)game_init,   (amtsvr_dl_release)game_release);
    amtsvr_module_create("chores",   (amtsvr_dl_create)chores_create,   (amtsvr_dl_init)chores_init,   (amtsvr_dl_release)chores_release);

    amtsvr_module_create("gate",     (amtsvr_dl_create)gate_create,     (amtsvr_dl_init)gate_init,     (amtsvr_dl_release)gate_release);
    amtsvr_module_create("watchdog", (amtsvr_dl_create)watchdog_create, (amtsvr_dl_init)watchdog_init, (amtsvr_dl_release)watchdog_release);
    amtsvr_module_create("agent",    (amtsvr_dl_create)agent_create,    (amtsvr_dl_init)agent_init,    (amtsvr_dl_release)agent_release);
    amtsvr_module_create("client",   (amtsvr_dl_create)sender_create,   (amtsvr_dl_init)sender_init,   (amtsvr_dl_release)sender_release);

    amtsvr_module_create("node", (amtsvr_dl_create)node_create,   (amtsvr_dl_init)node_init,   (amtsvr_dl_release)node_release);
    return 0;
}

int game_env_init(struct amtsvr_config * config)
{
    g_run_time = (uint32)time(NULL);

    //get_reward_users();

    int ret = game_data_init(config);
    if (ret != 0) {
        return -1;
    }

    ret = CONFMGR->init(CONFIG_CACHE_MGR);
    if (ret != 0) {
        return -2;
    }

    ret = GAME_MAP_MGR->init();
    if (ret != 0) {
        return -3;
    }

    ret = game_module_init();
    if (ret != 0) {
        return -4;
    }

    return 0;
}

void game_env_release()
{
    if (PBMGR != NULL) {
        delete PBMGR;
        PBMGR = NULL;
    }

    if (CONFMGR != NULL) {
        delete CONFMGR;
        CONFMGR = NULL;
    }

    if (GAME_MAP_MGR != NULL) {
        delete GAME_MAP_MGR;
        GAME_MAP_MGR = NULL;
    }

    if (CONFIG_CACHE_MGR != NULL) {
        delete CONFIG_CACHE_MGR;
        CONFIG_CACHE_MGR = NULL;
    }

    if (MSG_HANDLER_POOL != NULL) {
        delete MSG_HANDLER_POOL;
        MSG_HANDLER_POOL = NULL;
    }

    if (CLIENT_PB_MGR != NULL) {
        delete CLIENT_PB_MGR;
        CLIENT_PB_MGR = NULL;
    }

    google::protobuf::ShutdownProtobufLibrary();
}

