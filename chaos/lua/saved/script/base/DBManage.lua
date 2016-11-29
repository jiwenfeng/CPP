--数据库维护

local MAIL_OUTDATED_TIMELONG = 30 * 24 * 3600
local FIGHTREC_OUTDATED_TIMELONG = 15 * 24 * 3600

--清理过期邮件
function ClearOutDatedMail()
	local path_mail = "chaos.mail"
	local timelong = os.time() - MAIL_OUTDATED_TIMELONG
	-- gt大于 gte大于等于 lt小于 lte小于等于
	SAVE.DBRemove(path_mail, {time = {["$lt"] = timelong}})
end

--清理过期的战斗录像
function ClearOutDatedFightRec()
	local path_fightRec = "chaos.fightRec"
	local timelong = os.time() - FIGHTREC_OUTDATED_TIMELONG
	SAVE.DBRemove(path_fightRec, {time = {["$lt"] = timelong}})
end
















function __init__()
	CALLOUT.Daily(getfenv(2), 3, 0, ClearOutDatedMail)
	CALLOUT.Daily(getfenv(2), 3, 10, ClearOutDatedFightRec)
end