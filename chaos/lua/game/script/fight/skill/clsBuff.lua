--辅助技能

local clsSkill = BASESKILL.clsSkill:Inherit()

function clsSkill:GetClass()
	return "buff"
end


function clsSkill:GetSkillEffect(fightObj, attacker, targetType, targetPos)
	local range = self:GetAttackRange(targetPos)
	local lvInfo = self:GetLvInfo()
	local buff = {}
	for k, v in ipairs(range) do
		local target = fightObj:GetOneFighter(targetType, v)
		if target and not target:IsDead() then
			local buffer = {}
			for _, v in ipairs(lvInfo.buffer) do
				if math.random() <= v.occur then
					table.insert(buffer, {id = v.id, last = v.last, num = v.num, rate = v.rate})
				end
			end
			table.insert(buff, {type = targetType, pos = v, buffer = buffer})
		end
	end
	return buff
end

return clsSkill