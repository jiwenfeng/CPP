cmake_minimum_required (VERSION 2.6)
cmake_policy (SET CMP0015 NEW)

project (LZSG)

set (LZSG_VERSION_MAJOR 1)
set (LZSG_VERSION_MINOR 0)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_VERBOSE_MAKEFILE on) #显示打印语句
#set(CMAKE_CXX_COMPILER "/home/rockpile/software/build/Debug+Asserts/bin/clang++")
#set(CMAKE_CXX_COMPILER "/opt/gcc-5.0.0/bin/g++")
#set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wunused -Wall -ggdb -gstabs+ -g -D_cplusplus -D__INNER_DEBUG__PACKAGE_")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -D_cplusplus -D__INNER_DEBUG__PACKAGE_ -D__TEST_LUA__")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

configure_file (
    "${PROJECT_SOURCE_DIR}/LZSGConfig.h.in"
    "${PROJECT_BINARY_DIR}/LZSGConfig.h"
    )

set (AMTSVR_PATH ${PROJECT_SOURCE_DIR}/amtsvr)

set (FRAME_PATH ${PROJECT_SOURCE_DIR}/frame)
set (BASE_PATH ${FRAME_PATH}/base)
set (PB_PATH ${FRAME_PATH}/pb)
set (PB_CLIENT_PATH ${FRAME_PATH}/pb/client)
set (PB_CONFIG_PATH ${FRAME_PATH}/pb/config)
set (PB_DATA_PATH ${FRAME_PATH}/pb/data)
set (PB_INNER_PATH ${FRAME_PATH}/pb/inner)
set (PB_SERVER_PATH ${FRAME_PATH}/pb/server)
set (FRAME_CENTER_PATH ${FRAME_PATH}/center)
set (FRAME_SCRIPT_PATH ${FRAME_PATH}/script)
set (FRAME_SCRIPT_LINK_PATH ${FRAME_SCRIPT_PATH}/link)
set (CENTER_ARENA_PATH   ${FRAME_CENTER_PATH}/arena)
set (CENTER_BOSS_PATH    ${FRAME_CENTER_PATH}/boss)
set (CENTER_CACHE_PATH   ${FRAME_CENTER_PATH}/cache)
set (CENTER_CHAT_PATH    ${FRAME_CENTER_PATH}/chat)
set (CENTER_EXPLORE_PATH ${FRAME_CENTER_PATH}/explore)
set (CENTER_FAMILY_PATH  ${FRAME_CENTER_PATH}/family)
set (CENTER_FRIENDS_PATH ${FRAME_CENTER_PATH}/friends)
set (CENTER_MAIL_PATH    ${FRAME_CENTER_PATH}/mail)
set (CENTER_MATE_PATH    ${FRAME_CENTER_PATH}/mate)
set (CENTER_POLL_ACTIVITY_PATH    ${FRAME_CENTER_PATH}/poll_activity)
set (CENTER_RAID_PATH    ${FRAME_CENTER_PATH}/raid)
set (CENTER_RANDNPC_PATH ${FRAME_CENTER_PATH}/randnpc)
set (CENTER_RANK_PATH    ${FRAME_CENTER_PATH}/rank)
set (CENTER_SECT_PATH    ${FRAME_CENTER_PATH}/sect)
#set (CENTER_SIEGE_PATH   ${FRAME_CENTER_PATH}/siege)
set (CENTER_TEAM_PATH    ${FRAME_CENTER_PATH}/team)
set (CENTER_TIMER_PATH   ${FRAME_CENTER_PATH}/timer)
set (CENTER_CITY_PATH   ${FRAME_CENTER_PATH}/city)
set (CENTER_CYCLE_PATH   ${FRAME_CENTER_PATH}/cycle)
set (CENTER_DEVIL_PATH	 ${FRAME_CENTER_PATH}/devil)
set (CENTER_NOTICE_PATH	 ${FRAME_CENTER_PATH}/notice)
set (CENTER_ILLEGAL_WORD_PATH ${FRAME_CENTER_PATH}/illegal_word)
set (FRAME_CHORES_PATH ${FRAME_PATH}/chores)
set (CHORES_FARM_PATH ${FRAME_CHORES_PATH}/farm)
set (CONFIG_PATH ${FRAME_PATH}/config)
set (NAVMESH_PATH ${FRAME_PATH}/navmesh)
set (SCENE_PATH ${FRAME_PATH}/scene)
set (PLAYER_ROLE_PATH ${FRAME_PATH}/player_role)

set (SERVICE_PATH ${PROJECT_SOURCE_DIR}/service)
set (AGENT_PATH ${SERVICE_PATH}/agent_service)
set (CENTER_PATH ${SERVICE_PATH}/center_service)
set (CHORES_PATH ${SERVICE_PATH}/chores_service)
set (DATA_PATH ${SERVICE_PATH}/data_service)
set (LOADER_PATH ${DATA_PATH}/loader)
set (SQLMGR_PATH ${DATA_PATH}/sqlmgr)
set (WRITER_PATH ${DATA_PATH}/writer)
set (GAME_PATH ${SERVICE_PATH}/game_service)
set (GATE_PATH ${SERVICE_PATH}/gate_service)
set (LOGGER_PATH ${SERVICE_PATH}/logger_service)
set (ROLE_PATH ${SERVICE_PATH}/role_service)
set (SENDER_PATH ${SERVICE_PATH}/sender_service)
set (WATCHDOG_PATH ${SERVICE_PATH}/watchdog_service)
set (NODE_PATH ${SERVICE_PATH}/node_service)

include_directories (${AMTSVR_PATH} ${BASE_PATH} ${PB_CLIENT_PATH}
    ${PB_CONFIG_PATH} ${PB_DATA_PATH} ${PB_INNER_PATH} ${PB_SERVER_PATH}
    ${FRAME_CENTER_PATH} ${FRAME_SCRIPT_LINK_PATH} ${CENTER_ARENA_PATH} ${CENTER_BOSS_PATH} ${CENTER_CACHE_PATH}
    ${CENTER_CHAT_PATH} ${CENTER_EXPLORE_PATH} ${CENTER_FAMILY_PATH} 
	${CENTER_FRIENDS_PATH} ${CENTER_MAIL_PATH} ${CENTER_MATE_PATH} ${CENTER_POLL_ACTIVITY_PATH}
	${CENTER_RAID_PATH} ${CENTER_RANDNPC_PATH} ${CENTER_RANK_PATH} ${CENTER_SECT_PATH} 
	${CENTER_SIEGE_PATH} ${CENTER_TEAM_PATH} ${CENTER_TIMER_PATH}
	${CENTER_CITY_PATH} ${CHORES_FARM_PATH} ${CONFIG_PATH} 
	${CENTER_CYCLE_PATH}
	${CENTER_DEVIL_PATH}
	${CENTER_NOTICE_PATH}
	${CENTER_ILLEGAL_WORD_PATH}
	${NAVMESH_PATH}
    ${SCENE_PATH} ${PLAYER_ROLE_PATH} ${AGENT_PATH}
    ${CENTER_PATH} ${CHORES_PATH} ${DATA_PATH} ${GAME_PATH} ${GATE_PATH}
    ${LOGGER_PATH} ${ROLE_PATH} ${SENDER_PATH} ${WATCHDOG_PATH} ${NODE_PATH}
    ${LOADER_PATH} ${SQLMGR_PATH} ${WRITER_PATH} 
    ${LUA_TINKER_INCLUDE_DIR}
    )

#link_directories (${BASE_PATH} ${PB_PATH} ${PB_CLIENT_PATH} ${PB_CONFIG_PATH} 
#    ${PB_DATA_PATH} ${PB_INNER_PATH} ${PB_SERVER_PATH}
#    ${FRAME_CENTER_PATH} ${FRAME_CHORES_PATH} ${CONFIG_PATH} ${NAVMESH_PATH}
#    ${SCENE_PATH} ${PLAYER_ROLE_PATH} ${AGENT_PATH}
#    ${CENTER_PATH} ${CHORES_PATH} ${DATA_PATH} ${GAME_PATH} ${GATE_PATH}
#    ${LOGGER_PATH} ${ROLE_PATH} ${SENDER_PATH} ${WATCHDOG_PATH}
#    ${LOADER_PATH} ${SQLMGR_PATH} ${WRITER_PATH}
#    )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(MYSQL REQUIRED)
find_package(LUA REQUIRED)
find_package(LUA_TINKER REQUIRED)
find_package(SYS REQUIRED)
find_package(PROTO_BUF REQUIRED)
mark_as_advanced (
    MYSQL_INCLUDE_DIR
    MYSQL_LIBRARY
    )

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
#    message("mysql found!")
    include_directories(${MYSQL_INCLUDE_DIR})
endif(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)

add_subdirectory (${BASE_PATH})
add_subdirectory (${FRAME_CENTER_PATH})
add_subdirectory (${FRAME_CHORES_PATH})
add_subdirectory (${FRAME_SCRIPT_PATH})
add_subdirectory (${CONFIG_PATH})
add_subdirectory (${NAVMESH_PATH})
add_subdirectory (${PB_PATH})
add_subdirectory (${PLAYER_ROLE_PATH})
add_subdirectory (${SCENE_PATH})
add_subdirectory (${AGENT_PATH})
add_subdirectory (${CENTER_PATH})
add_subdirectory (${CHORES_PATH})
add_subdirectory (${DATA_PATH})
add_subdirectory (${GAME_PATH})
add_subdirectory (${GATE_PATH})
add_subdirectory (${LOGGER_PATH})
add_subdirectory (${ROLE_PATH})
add_subdirectory (${SENDER_PATH})
add_subdirectory (${WATCHDOG_PATH})
add_subdirectory (${NODE_PATH})
add_subdirectory (${AMTSVR_PATH})
