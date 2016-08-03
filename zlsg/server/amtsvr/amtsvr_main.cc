#include "stdafx.h"
#include "amtsvr.h"
#include "amtsvr_init.h"
#include "amtsvr_imp.h"
#include "amtsvr_env.h"
#include "luacompat52.h"

#include "define.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#else
#include <windows.h>
#endif

#include <time.h>
#include <fstream>

bool GAME_EXIT = false;

static int
optint(const char *key, int opt) {
    const char * str = amtsvr_getenv(key);
    if (str == NULL) {
        char tmp[20];
        sprintf(tmp,"%d",opt);
        amtsvr_setenv(key, tmp);
        return opt;
    }
    return strtol(str, NULL, 10);
}

static const char *
optstring(const char *key,const char * opt) {
    const char * str = amtsvr_getenv(key);
    if (str == NULL) {
        if (opt) {
            amtsvr_setenv(key, opt);
        }
        return opt;
    }
    return str;
}


static void
_init_env(lua_State *L) {
    lua_pushglobaltable(L);
    lua_pushnil(L);  /* first key */
    while (lua_next(L, -2) != 0) {
        int keyt = lua_type(L, -2);
        if (keyt != LUA_TSTRING) {
            fprintf(stderr, "Invalid config table\n");
            exit(1);
        }
        const char * key = lua_tostring(L,-2);
        if (lua_type(L,-1) == LUA_TBOOLEAN) {
            int b = lua_toboolean(L,-1);
            amtsvr_setenv(key,b ? "true" : "false" );
        } else {
            const char * value = lua_tostring(L,-1);
            if (value == NULL) {
                fprintf(stderr, "Invalid config table key = %s\n", key);
                exit(1);
            }
            amtsvr_setenv(key,value);
        }
        lua_pop(L,1);
    }
    lua_pop(L,1);
}

static void 
_ignore(int signum)
{
#ifndef _WIN32
    struct sigaction sig;
    sig.sa_handler = SIG_IGN;
    sig.sa_flags = 0;

    sigemptyset(&sig.sa_mask);
    sigaction(signum, &sig, NULL);
#else
    printf("ignore signum:%d\n", signum);
#endif
}

int write_close_time();

int sig_lock = 0;

static void 
    _on_sigusr1(int signum)
{
    char* p = NULL;
    *p = 1;
    return ;
}

static void 
_on_sigusr2(int signum)
{
    int ret = 1;
    do {
        ret = __sync_lock_test_and_set(&sig_lock, 1);
        if (ret != 0) {
            usleep(10);
        }
    } while (ret != 0);

    if (GAME_EXIT) {
        __sync_lock_release(&sig_lock);
        return ;
    }

    write_close_time();
    GAME_EXIT = true;
    printf("game_exit signum:%d\n", signum);

    __sync_lock_release(&sig_lock);

    //sleep(90);
    //amtsvr_command(NULL, "ABORT", NULL);
}


#ifndef _WIN32

typedef void (*sighandler)(int);
sighandler LHSignal(int nSigNum, sighandler handler)
{
    struct sigaction saNew, saOld;

    saNew.sa_handler = handler;
    sigemptyset(&saNew.sa_mask);
    if (SIG_DFL != handler && SIG_IGN != handler)
        sigaddset(&saNew.sa_mask, nSigNum);

    saNew.sa_flags = 0;
    if (SIGALRM == nSigNum)
    {
#ifdef SA_INTERRUPT
        saNew.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
        saNew.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(nSigNum, &saNew, &saOld) < 0)
        return SIG_ERR;

    return saOld.sa_handler;
}

void RedirectStdIO(const char *szInFile, const char *szOutFile, const char *szErrFile)
{
    int fd;

    if (NULL != szInFile)
    {
        fd = open(szInFile, O_RDONLY | O_CREAT, 0666);
        if (fd > 0)
        {
            // 标准输入重定向
            if (dup2(fd, STDIN_FILENO) < 0)
            {
                //ErrorLog("RedirectStdIO dup2 in");
                exit(EXIT_FAILURE);
            }

            close(fd);
        }
        //else
            //ErrorLogFmt("RedirectStdIO open %s: %s\n", szInFile, strerror(errno));
    }

    if (NULL != szOutFile)
    {
        fd = open(szOutFile, O_WRONLY | O_CREAT | O_APPEND /*| O_TRUNC*/, 0666);
        if (fd > 0)
        {
            // 标准输出重定向 
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                //ErrorLog("RedirectStdIO dup2 out");
                exit(EXIT_FAILURE);
            }

            close(fd);
        }
        //else
            //ErrorLogFmt("RedirectStdIO open %s: %s\n", szOutFile, strerror(errno));
    }

    if (NULL != szErrFile)
    {
        fd = open(szErrFile, O_WRONLY | O_CREAT | O_APPEND /*| O_TRUNC*/, 0666);
        if (fd > 0)
        {
            // 标准错误重定向 
            if (dup2(fd, STDERR_FILENO) < 0)
            {
                //ErrorLog("RedirectIO dup2 error");
                exit(EXIT_FAILURE);
            }

            close(fd);
        }
        //else
            //ErrorLogFmt("RedirectStdIO open %s: %s\n", szErrFile, strerror(errno));
    }
}

#endif // _WIN32

static void 
_daemon()
{
#ifndef _WIN32
//     LHSignal(SIGTTOU, SIG_IGN); 
//     LHSignal(SIGTTIN, SIG_IGN); 
//     LHSignal(SIGTSTP, SIG_IGN); 
//     LHSignal(SIGHUP, SIG_IGN

    //signal(SIGINT , SIG_IGN);
    //signal(SIGHUP , SIG_IGN);
    //signal(SIGQUIT, SIG_IGN);
    //signal(SIGPIPE, SIG_IGN);
    //signal(SIGTTOU, SIG_IGN);
    //signal(SIGTTIN, SIG_IGN);
    //signal(SIGCHLD, SIG_IGN);
    //signal(SIGTERM, SIG_IGN);
    signal(SIGUSR1, _on_sigusr1);
    signal(SIGUSR2, _on_sigusr2);

    _ignore(SIGINT );
    _ignore(SIGHUP );
    _ignore(SIGQUIT);
    _ignore(SIGPIPE);
    _ignore(SIGTTOU);
    _ignore(SIGTTIN);
    _ignore(SIGCHLD);
    _ignore(SIGTERM);

    pid_t pid;
    if ((pid = fork()) != 0) {
        exit(0);
    }

    setsid();

    if ((pid = fork()) != 0) {
        exit(0);
    }

    umask(0);

    const char* LOG_FILE = "./lhsg.log";
    RedirectStdIO("/dev/null", LOG_FILE, LOG_FILE);    //重定向标准输入输出
    getdtablesize();
    int fdTableSize = getdtablesize();
    for (int fd=3; fd<fdTableSize; fd++)
        close(fd);
#endif
}

struct amtsvr_config config;

void set_amtsvr_svr_start_time(unsigned int start_svr_time)
{
    config.start_svr_time = start_svr_time;
}

unsigned int amtsvr_svr_start_time()
{
	return config.start_svr_time;
}

unsigned int amtsvr_svr_start_day(unsigned int now)
{
    if (now <= config.start_svr_time) {
        return 1;
    }

    return (now - config.start_svr_time) / 86400;
}

int write_close_time() {
    std::ofstream ofile;
    ofile.open("./close_server.log");

    const int len = 80;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [len] = {0};

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer, len, "%F %u %T\n", timeinfo);

    ofile << buffer;

    ofile.close();

    return 0;
}

int read_pid(const char* pfile, char* buf) {
    FILE* fd = fopen(pfile, "a+");
    if (fd == NULL) {
        return -1;
    }

    fread(buf, sizeof(buf), sizeof(buf), fd);
    fclose(fd);

    return 0;
}

int write_pid(const char* pfile) {
    FILE* fd = fopen(pfile, "w+");
    if (fd == NULL) {
        return -1;
    }

    //int ret = flock(fd->_file, LOCK_EX | LOCK_NO);
    //if (ret == -1) {
    //    return -2;
    //}

    int pid = getpid();
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%d", pid);
    fwrite(buf, strlen(buf), strlen(buf), fd);

    //ret = flock(fd->_file, LOCK_UN);
    //if (ret == -1) {
    //    return -2;
    //}

    fclose(fd);

    return 0;
}

int check_pid(const char* buf) {
#ifndef _WIN32
    char dirbuf[32] = { 0 };
    snprintf(dirbuf, sizeof(dirbuf), "/proc/%s", buf);
    DIR* dir = opendir(dirbuf);
    if (dir != NULL) {
        return -1;
    }

#endif // _WIN32

    return 0;
}

int check_exit(const char* pfile) {
    if (pfile == NULL) {
        return -1;
    }

    char buf[32] = { 0 };
    if (read_pid(pfile, buf)) {
        return -2;
    }

    if (strlen(buf) != 0 && check_pid(buf) != 0) {
        return -3;
    }

    write_pid(pfile);

    return 0;
}

int main(int argc, char *argv[]) 
{
    const char * config_file = "config";
    if (argc == 1 || (argc > 1 && strcmp("-d", argv[1]) != 0)) {
        _daemon();
    } else {
#ifndef _WIN32
        signal(SIGUSR1, _on_sigusr1);
        signal(SIGUSR2, _on_sigusr2);
#endif
    }

    const char* PID_FILE = "./server.pid";
    if (check_exit(PID_FILE) != 0) {
        // exit(0);
        amtsvr_log("repeat start\n");
        return 0;
    }

    amtsvr_env_init();

    struct lua_State *L = luaL_newstate();
    luaL_openlibs(L);	// link lua lib
    lua_close(L);

    L = luaL_newstate();

    int err = luaL_dofile(L, config_file);
    if (err) {
        fprintf(stderr,"%s\n",lua_tostring(L,-1));
        lua_close(L);
        return 1;
    } 

    _init_env(L);

#ifndef _WIN32
    const char *path = optstring("lua_path","./lualib/?.lua;./lualib/?/init.lua");
    setenv("LUA_PATH",path,1);
    const char *cpath = optstring("lua_cpath","./luaclib/?.so");
    setenv("LUA_CPATH",cpath,1);
#endif

    config.thread             = optint   ("thread"         , 8               );
    config.module_path        = optstring("cpath"          , "./service/?.so");
    config.logger             = optstring("logger"         , NULL            );
    config.db_host            = optstring("db_host"        , "127.0.0.1"     );
    config.db_port            = optint   ("db_port"        , 3306            );
    config.db_name            = optstring("db_name"        , "XXSQ"          );
    config.db_user            = optstring("db_user"        , "abel"          );
    config.db_pass            = optstring("db_pass"        , "0402"          );
    config.client_port        = optstring("client_port"    , "6688|0|0|0|"   );
    config.per_gate_num       = optint   ("per_gate_num"   , 800             );
    config.inner_port         = optint   ("inner_port"     , 6695            );
    config.max_online_user    = optint   ("max_online_user", 3000            );
    config.start_svr_time     = (unsigned int)optint("start_svr_time", (unsigned int)time(NULL));
    config.center_server      = optstring("center_server"  , NULL);
    config.center_server_port = optint   ("center_server_port", 0);

    lua_close(L);

    amtsvr_start(&config);
    printf("%s:%d, amtsvr stop\n", __FILE__, __LINE__);

    game_env_release();
    amtsvr_env_release();
    printf("%s:%d, env release\n", __FILE__, __LINE__);

    //
    printf("%s:%d, server stop\n", __FILE__, __LINE__);
    //exit(0);

    remove(PID_FILE);

    return 0;
}
