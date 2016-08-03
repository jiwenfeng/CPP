aux_source_directory (client    PB_CLIENT )
aux_source_directory (config    PB_CONFIG )
aux_source_directory (data      PB_DATA   )
aux_source_directory (inner     PB_INNER  )
aux_source_directory (server    PB_SERVER )

add_library (pb      ${PB_CLIENT}
                     ${PB_CONFIG}
                     ${PB_DATA}
                     ${PB_INNER}
                     ${PB_SERVER})