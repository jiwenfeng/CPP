
--统计玩家等级数据
function LogAllUserLv()
	local cur = SAVE.DBFind("lcr.user", {})
	local list = {}
	for data in cur:results() do
		list[data.username] = {name = data.name, Lv = data.Lv}
	end
	local function PrintTable(t)
		local buff = {}
		for k, v in pairs(t) do
			table.insert(buff, k .. "\t" .. v.name .. "\t" .. v.Lv .. "\n")
		end
		SAVE.log(table.concat(buff), "userLv" .. os.date("%Y%m%d") .. ".log")
	end
	PrintTable(list)
end

