--恢复类

local clsSkill = BASESKILL.clsSkill:Inherit()


function clsSkill:GetClass()
	return "restore"
end

function clsSkill:GetSkillEffect(fightObj, attacker, targetType, targetPos)
	local range = self:GetAttackRange(targetPos)
	local lvInfo = self:GetLvInfo()
	local buff = {}
	for k, v in ipairs(range) do
		local target = fightObj:GetOneFighter(targetType, v)
		if target and not target:IsDead() then
			table.insert(buff, {type = targetType, pos = v, damage = -lvInfo.num, action = "restore"})
		end
	end
	return buff
end


return clsSkill