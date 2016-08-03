aux_source_directory (. AMTSVR)
add_executable (LZSG ${AMTSVR})

target_link_libraries (LZSG 
	agent_service  
	center_service  
	chores_service  
	game_service 
	gate_service  
	logger_service  
	role_service  
	sender_service  
	watch_dog_service
    node_service
	data_service  
	chores 
	player_role  
	config 
	base
	scene  
	center
	pb
	navmesh
    script
	${MYSQL_LIBRARY} 
	m
	rt
	dl
	z
	${LUA_LIBRARY} 
	${SYS_LIBRARY}
    ${PROTO_BUF_LIBRARIES}
    ${LUA_TINKER_LIBRARY})
	
#target_link_libraries (LZSG 
#	${MYSQL_LIBRARY} 
#	${LUA_LIBRARY} 
#	${SYS_LIBRARY}
#   ${PROTO_BUF_LIBRARIES} 
#	${BASE_PATH}/libbase.a 
#	${FRAME_CENTER_PATH}/libcenter.a 
#	${FRAME_CHORES_PATH}/libchores.a 
#	${CONFIG_PATH}/libconfig.a 
#	${NAVMESH_PATH}/libnavmesh.a 
#	${PB_PATH}/libpb.a 
#	${PLAYER_ROLE_PATH}/libplayer_role.a 
#	${SCENE_PATH}/libscene.a 
#	${AGENT_PATH}/libagent_service.a 
#	${CENTER_PATH}/libcenter_service.a 
#	${CHORES_PATH}/libchores_service.a 
#	${DATA_PATH}/libdata_service.a 
#	${GAME_PATH}/libgame_service.a
#	${GATE_PATH}/libgate_service.a 
#	${LOGGER_PATH}/liblogger_service.a 
#	${ROLE_PATH}/librole_service.a 
#	${SENDER_PATH}/libsender_service.a 
#	${WATCHDOG_PATH}/libwatch_dog_service.a)
	
add_dependencies(LZSG pb)  
