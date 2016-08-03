aux_source_directory (arena   CENTER_ARENA   )
aux_source_directory (boss    CENTER_BOSS    )
aux_source_directory (cache   CENTER_CACHE   )
aux_source_directory (chat    CENTER_CHAT    )
aux_source_directory (explore CENTER_EXPLORE )
aux_source_directory (family  CENTER_FAMILY  )
aux_source_directory (friends CENTER_FRIENDS )
aux_source_directory (mail    CENTER_MAIL    )
aux_source_directory (mate    CENTER_MATE    )
aux_source_directory (poll_activity    CENTER_POLL_ACTIVITY    )
aux_source_directory (raid    CENTER_RAID    )
aux_source_directory (randnpc CENTER_RANDNPC )
aux_source_directory (rank    CENTER_RANK    )
aux_source_directory (sect    CENTER_SECT    )
aux_source_directory (team    CENTER_TEAM    )
aux_source_directory (city	  CENTER_CITY   )
aux_source_directory (cycle   CENTER_CYCLE )
aux_source_directory (devil   CENTER_DEVIL )
aux_source_directory (notice   CENTER_NOTICE )
aux_source_directory (illegal_word CENTER_ILLEGAL_WORD)
add_library (center ${CENTER_ARENA} 
					${CENTER_BOSS} 
					${CENTER_CACHE}
					${CENTER_CHAT}
					${CENTER_EXPLORE}
					${CENTER_FAMILY}
					${CENTER_FRIENDS}
					${CENTER_LABEL}
					${CENTER_MAIL}
					${CENTER_MATE}
					${CENTER_POLL_ACTIVITY}
					${CENTER_RAID}
					${CENTER_RANDNPC}
					${CENTER_RANK}
					${CENTER_SECT}
					${CENTER_SIEGE}
					${CENTER_TEAM}
					${CENTER_CITY}
					${CENTER_CYCLE}
					${CENTER_DEVIL}
					${CENTER_NOTICE}
					${CENTER_ILLEGAL_WORD}
					)
