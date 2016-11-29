--必杀技

local clsSkill = BASESKILL.clsSkill:Inherit()

function clsSkill:GetClass()
	return "stunt"
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
					local physcAtk = attacker:GetPhyscAtk()
					local magicAtk = attacker:GetMagicAtk()
					local physcDef = target:GetPhyscDef()
					local magicDef = target:GetMagicDef()
					
					local atk = lvInfo.num * (physcAtk + magicAtk) / 2
					local def = (physcDef + magicDef) / 2
					local downInfo = target:IsStateExist("atk_down")
					if downInfo then
						atk = atk - downInfo.num
					end
					damage = atk - def
					local atkLv = attacker:GetLv()
					local defLv = target:GetLv()
					if damage <= 0 then
						damage = math.random(5, 10)
						if atkLv >= defLv then
							damage = damage + (atkLv - defLv)
						end
					else
						damage = damage * math.max((1 + (atkLv - defLv) / SERVER_ROLE_MAX_LV), 0.8)
					end
					table.insert(buff, {type = targetType, pos = v, damage = damage, action = action})
				end
			end
		end
	end
	return buff
end

return clsSkill