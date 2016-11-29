 --引擎退出时的清理函数
function signal_exit()
	FIGHT.ShutDownFinishAllFight()
	USER.ShutDownSaveAll()
--	STATS.SaveStatTbl()
--	STATS.SaveOnlineTimeTbl()
	UNION.ShutdownSaveAllUnion()
	PHYLE.SaveAllPhyleData()
--	OFFICIAL.SaveAllOfficialData()
	ARENA.SaveArenaUserData()
	RANK.SaveRankData("real")
	TRIBE.SaveAllTribeData()
	CITY.SaveAllCityData()
	WILDMINE.SaveAllWildMine()
	WILDCHEST.SaveAllWildMonster()
	local function CB(Ret, Msg)
		print("shutdown now~~", Ret, Msg)
		lengine.shutdown(0);
	end
	DATA.NormalWrite("data/base/.shutdown", os.date("%F %T"), CB)
end


