

--在引擎中回调
function CallFromEngineOnEnterLeft(CharId, EnterObjectIdList)
	local Obj = assert(SCENE.GetCharById(CharId))
	if not Obj:IsSceneEnterDone() then
		return
	end
	for _, EnterObjectId in pairs(EnterObjectIdList) do
		if CharId ~= EnterObjectId then
			local EnterObj = SCENE.GetCharById(EnterObjectId)
	--		if (EnterObj) and (not Obj:GetFightObj()) and EnterObj:IsVisible() and EnterObj:CanBeSeenBy(userObj) then
			if EnterObj then
				Obj:AddUserInSight(EnterObjectId)
				if (EnterObj:GetRoleType() ~= "user" or Obj:IsUserInSight(EnterObjectId)) then
					Obj:OnEnterAOI(EnterObj)
				end
			end
		end
	end
end

function CallFromEngineOnEnterRight(EnterCharId, ObjectIdList)
	local EnterObj = assert(SCENE.GetCharById(EnterCharId))
	for _, ObjectId in pairs(ObjectIdList) do
		if EnterCharId ~= ObjectId then
			local Obj = SCENE.GetCharById(ObjectId)
		--	if (Obj) and (not Obj:GetFightObj()) and EnterObj:IsVisible() and EnterObj:CanBeSeenBy(Obj) then
			if Obj and Obj:IsSceneEnterDone() then
				Obj:AddUserInSight(EnterCharId)
				if (EnterObj:GetRoleType() ~= "user" or Obj:IsUserInSight(EnterCharId)) then
					Obj:OnEnterAOI(EnterObj)
				end
			end
		end
	end
end

function CallFromEngineOnLeaveLeft(CharId, LeaveObjectIdList, flag)
	if flag == 1 then return end			--flag为1时表示退出场景，此时不需要处理LeaveObjectIdList离开CharId的视野
	local Obj = assert(SCENE.GetCharById(CharId))
	if not Obj:IsSceneEnterDone() then
		return
	end
	for _, LeaveObjectId in pairs(LeaveObjectIdList) do
		if CharId ~= LeaveObjectId then
			local LeaveObj = SCENE.GetCharById(LeaveObjectId)
			if LeaveObj and (Obj:IsUserInSight(LeaveObjectId) or LeaveObj:GetRoleType() ~= "user") then
				Obj:OnLeftAOI(LeaveObj)
			end
		end
	end
end

function CallFromEngineOnLeaveRight(LeaveCharId, ObjectIdList)
	local LeaveObj = assert(SCENE.GetCharById(LeaveCharId))
	for _, ObjectId in pairs(ObjectIdList) do
		if LeaveCharId ~= ObjectId then
			local Obj = SCENE.GetCharById(ObjectId)
			if Obj and Obj:IsSceneEnterDone() and (Obj:IsUserInSight(LeaveCharId) or LeaveObj:GetRoleType() ~= "user") then
				Obj:OnLeftAOI(LeaveObj)
			end
		end
	end
end