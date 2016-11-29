
--[[this file is prepare for retreate not use right now ]]


--修仙塔副本
clsTowerCopy = CLSCOPY.clsCopy:Inherit()

function clsTowerCopy:__init__()
	Super(clsTowerCopy).__init__(self)
	self.__save = {}
	self.__temp = {}
end

function clsTowerCopy:GetType()
	return "tower"
end


