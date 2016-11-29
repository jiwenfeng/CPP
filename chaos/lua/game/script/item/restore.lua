--恢复类道具

clsItem = BASEITEM.clsBaseItem:Inherit()


--使用物品
function clsItem:Use(obj, num)
	if obj:GetRoleType() ~= "user" then
		return false, "主将才能使用该道具。"
	end
	local info = BASEITEM.GetItemById(self:GetId())
	local restore = info.num * num
	if info.kind == 1 then
		obj:SetVigour(restore)
	end
	return true
end