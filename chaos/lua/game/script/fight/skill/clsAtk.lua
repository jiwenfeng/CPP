--攻击技能

local clsSkill = BASESKILL.clsSkill:Inherit()


--GetSkillEffect(fighter, skillId, skillLv, targetType, targetPos, self)

--需要额外处理：
--	buffer
-- 	miss中断连击
-- 士气值影响攻击伤害

function clsSkill:GetClass()
	return "atk"
end

function clsSkill:GetSkillEffect(fightObj, attacker, targetType, targetPos, isLinked)
	local range = self:GetAttackRange(targetPos)
	local lvInfo = self:GetLvInfo()
	local buff = {}
	for k, v in ipairs(range) do
		local target = fightObj:GetOneFighter(targetType, v)
		if target then
			local deadRound = target:IsDead()
			if not deadRound or (isLinked and deadRound == fightObj:GetRound()) then
				local action = fightObj:CalAttackFlag(attacker, target, self:IsNonMiss())
				if action == "miss" or action == "block" then
					table.insert(buff, {type = targetType, pos = v, action = action})
				else
					local atk = attacker:GetPhyscAtk(self:GetMethod(), lvInfo.num, lvInfo.rate)
					local def = target:GetPhyscDef()
					local rate = 0
					for _, v in ipairs(lvInfo.buffer) do
						if v.id == "20" then
							rate = rate + v.rate
						end
					end
					def = def * (1 - rate)
					if attacker:GetAtkMode() == "magic" then
						atk = attacker:GetMagicAtk(self:GetMethod(), lvInfo.num, lvInfo.rate)					
						def = target:GetMagicDef()
						local rate = attacker:GetMagicDefIgnore()
						def = def * (1 - rate)
					end
					local downInfo = target:IsStateExist("atk_down")
					if downInfo then
						atk = atk - downInfo.num
					end
					local correct = fightObj:CalCorrectPoint(attacker:GetIdentity(), target:GetIdentity())		
					local damage = fightObj:CalBaseDamage("skill", atk, def, correct, attacker:GetLv(), target:GetLv(), action)
					damage = fightObj:CalFinalDamage(attacker, target, damage)
					table.insert(buff, {type = targetType, pos = v, damage = damage, action = action})
				end
			end
		end
	end
	return buff
end

return clsSkill
