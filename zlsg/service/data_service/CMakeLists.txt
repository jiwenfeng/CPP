aux_source_directory (loader   DATA_LOADER)
aux_source_directory (sqlmgr   DATA_SQLMGR)
aux_source_directory (writer   DATA_WRITER)

add_library (data_service ${DATA_LOADER}
                          ${DATA_SQLMGR}
                          ${DATA_WRITER})
