dofile("base/extend.lua")
dofile("base/class.lua")
dofile("base/import.lua")
dofile("base/dump.lua")
dofile("base/clean.lua")
dofile("base/scene.lua")


---------------------------------------------- 开启新服务器时需要定制的参数 ---------------------------
_G.SERVER_ID = 1			--服务器编号
_G.SERVER_START_TIME = os.time({year = 2013, month = 3, day = 15, hour= 12, min = 0, sec = 0})			--开服时间

--端口号
_G.PORT_CONNECT_SERVER = 21000	--游戏服务器登陆端口
_G.PORT_POLICYSERVER = 1 + PORT_CONNECT_SERVER				---权限策略服务器端口
_G.PORT_CONNECT_SAVED = 2 + PORT_CONNECT_SERVER				--用于saved服务器的数据交互
_G.PORT_DEBUG = 8 + PORT_CONNECT_SERVER						--调试端口
_G.PORT_BACKUP_MASTER = 10 + PORT_CONNECT_SERVER    		--主数据库端口
_G.PORT_BACKUP_SLAVE = 11 + PORT_CONNECT_SERVER    			--从数据库端口
_G.PORT_CONNECT_INFO = 3 + PORT_CONNECT_SERVER				--获取统计信息
_G.PORT_GM_SERVER = 5 + PORT_CONNECT_SERVER					--gm 端口
_G.GAME_ROOT_PATH = "/home/public/chaos" .. SERVER_ID		--游戏根目录

--------------------------------------------------------------------------------------------------------

--全局变量

_G.SERVER_ROLE_MAX_LV = 100
_G.SYSTEM_LOGIN_STATE = false				--玩家是否可以登录(默认为false，服务器启动之后转换为true)

_G.USER_ID_ENDING = "1"		--玩家标识符（玩家id = 玩家编号 + 4位服务器编号 + 1位标识符）
_G.NPC_ID_ENDING = "0"		--npc标识符（npcId = npc编号 + 4位服务器编号 + 1位标识符）
_G.HERO_ID_ENDING = "2"		--武将标识符（heroId = hero编号 + 4位服务器编号 + 1位标识符）
_G.GODHERO_ID_ENDING = "3"	--神将标识符
_G.PET_ID_ENDING = "4"		--本命精灵标识符



--判断是否为玩家id
function IsUserId(charId)
	if not charId then
		return false
	end
	return string.sub(charId, -1, -1) == USER_ID_ENDING
end
--由角色id查询服务器编号
function GetServerIndexById(charId)
	return tonumber(string.sub(charId, -5, -2))
end

function GetSystemLoginState()
	return _G.SYSTEM_LOGIN_STATE
end

function SetSystemLoginState(state)
	_G.SYSTEM_LOGIN_STATE = state
end





_G.SYSTEM_USERNAME = "system"
_G.SYSTEM_NAME = "系统" --
_G.SYSTEM_HEADID = 1  --系统头像
_G.SYSTEM_ID = "system"  --系统身份

_G.COUNT_RESET_HOUR = 0			--每日计数重置时间点
--_G.MONEY_TO_INGOT = 10			--钱币、元宝兑换比率
--[[
_G.G_TITLE_COUNT = 0  --称号总数
_G.SPEED_UP_BASE_PREGOLD = 1				--加速计费基本费用
_G.SPEED_UP_BASE_TIME = 5 * 60				--加速计费基本单位

_G.MANOR_MONTH_RENT = 10  --RMB
_G.CASTLE_MONTH_RENT = 20  --RMB
_G.EXCHANGE_RATIO = 1/10   --RMB/GOLD  金币，人民币兑换比例
]]


--base
CALLOUT = Import("base/call_out.lua")
SOCKET = Import("base/socket.lua")
DATA = Import("base/data.lua")
JSON = Import("base/json.lua")
HTTP = Import("base/http.lua")
TIME = Import("base/time.lua")
--protocol
PROTOCOL = Import("protocol/protocol.lua")

--common
DEBUG = Import("common/debug.lua")
TRACE = require("common/trace")
ERROR = Import("common/error.lua")
MONEY = Import("common/money.lua")
DBMANAGE = Import("common/db_manage.lua")
SETTING = Import("common/setting.lua")
NAME = Import("common/name.lua")

--item
BASEITEM = Import("item/baseitem.lua")
SUIT = Import("item/suit.lua")
ITEMMALL = Import("item/itemMall.lua")

--rank
BASERANK = Import("rank/clsBaseRank.lua")
RANK = Import("rank/rank.lua")

--unit
JOB = Import("unit/job.lua")
ROLE = Import("unit/role.lua")
CHAR = Import("unit/char.lua")
USER = Import("unit/user.lua")
USERBASE = Import("unit/clsUser.lua")
USEREXTD = Import("unit/user_ext.lua")
CLSHERO = Import("unit/clsHero.lua")
FAMOUSHERO = Import("unit/famousHero.lua")
HERO = Import("unit/hero.lua")
CLSPET = Import("unit/clsPet.lua")
PET = Import("unit/pet.lua")
CLSSTATUS = Import("unit/clsStatus.lua")
STATUS = Import("unit/status.lua")
FRIEND = Import("unit/friend.lua")
GODHERO = Import("unit/godHero.lua")

--fight
CLSFIGHTER = Import("fight/clsFighter.lua")
CLSFIGHT = Import("fight/clsFight.lua")
FIGHT = Import("fight/fight.lua")
FIGHTREC = Import("fight/fightRec.lua")
TESTFIGHT = Import("fight/testFight.lua")
BASESKILL = Import("fight/clsBaseSkill.lua")
SKILL = Import("fight/skill.lua")
FORMATION = Import("fight/formation.lua")

--mission
CLSMISS = Import("mission/clsMission.lua")
DAILYMISS = Import("mission/dailyMission.lua")
MISSKINDACT = Import("mission/missKindAct.lua")
MISSION = Import("mission/mission.lua")
USERMISS = Import("mission/user_mission.lua")

--achieve
ACHV = Import("achieve/achieve.lua")
USERACHV = Import("achieve/user_achv.lua")
TITLE = Import("achieve/title.lua")

--npc
CLSNPC = Import("npc/clsNpc.lua")
CLSFOE = Import("npc/clsFoe.lua")
NPC = Import("npc/npc.lua")

--scene
CLSSCENE = Import("scene/clsScene.lua")
TOWNSCENE = Import("scene/clsTownScene.lua")
COPYSCENE = Import("scene/clsCopyScene.lua")
EMPTYSCENE = Import("scene/clsEmptyScene.lua")
HEROSCENE = Import("scene/clsHeroScene.lua")
UNISECRETSCENE = Import("scene/clsUniSecretScene.lua")
UNIRELICSCENE = Import("scene/clsUniRelicScene.lua")
WORLDBOSSSCENE = Import("scene/clsWorldBossScene.lua")
COLLECTSCENE = Import("scene/clsCollectScene.lua")
SCENE = Import("scene/scene.lua")

--phyle
PHYLESTRUCT = Import("phyle/struct.lua")
MUNITION = Import("phyle/munition.lua")
ALTAR = Import("phyle/altar.lua")
TOTEM = Import("phyle/totem.lua")
CLSPHYLE = Import("phyle/clsPhyle.lua")
PHYLE = Import("phyle/phyle.lua")

--building
UPGRADELIST = Import("building/upgradeList.lua")
STRUCT = Import("building/struct.lua")
TRIBE = Import("building/tribe.lua")
MAINHALL = Import("building/mainHall.lua")
RETREAT = Import("building/retreat.lua")
FURNACE = Import("building/furnace.lua")
MARKET = Import("building/market.lua")
SPARMINE = Import("building/sparMine.lua")
HOUSE = Import("building/house.lua")
ACADEMY = Import("building/academy.lua")
BARRACK = Import("building/barrack.lua")
EMBASSY = Import("building/embassy.lua")
GODALTAR = Import("building/godAltar.lua")

--forge
ENHANCE = Import("forge/enhance.lua")
ENCHASE = Import("forge/enchase.lua")

--module
LOGIN = Import("module/login.lua")
--LOGIN = Import("module/login_web.lua")
EXIT = Import("module/exit.lua")
NEWUSER = Import("module/newuser.lua")
OFFLINE = Import("module/offline.lua")
CHAT = Import("module/chat.lua")
BUFFER = Import("module/buffer.lua")
MAIL = Import("module/mail.lua")
BOURSE = Import("module/bourse.lua")
TRIPOD = Import("module/tripod.lua")
FAIRYDEV = Import("module/fairyDevice.lua")
--OFFICIAL = Import("module/official.lua")
ARENA = Import("module/arena.lua")
CLSTREASURE = Import("module/clsTreasure.lua")
TREASURE = Import("module/treasure.lua")
PRACTICE = Import("module/practice.lua")
TEST = Import("module/test.lua")
MONEYTREE = Import("module/moneyTree.lua")
CLSMONALBUM = Import("module/clsMonAlbum.lua")
MONALBUM = Import("module/monAlbum.lua")
COLLECT = Import("module/collect.lua")

--act
WELFARE	 = Import("act/welfare.lua")
VIP = Import("act/vip.lua")
ACTIVITY = Import("act/activity.lua")
SIGN = Import("act/sign.lua")
FUNCOPEN =Import("act/funcOpen.lua")
RECHARGE = Import("act/recharge.lua")
ONLINEPRIZE = Import("act/onlinePrize.lua")
CLSWORLDBOSS = Import("act/clsWorldBoss.lua")
WORLDBOSS = Import("act/worldBoss.lua")


--UNION
CLSUNION = Import("union/clsUnion.lua")
UNION = Import("union/union.lua")
CLSUNISECRET = Import("union/clsUniSecret.lua")
UNISECRET = Import("union/uniSecret.lua")
CLSUNIRELIC = Import("union/clsUniRelic.lua")
UNIRELIC = Import("union/uniRelic.lua")

--city
CLSCITY = Import("city/clsCity.lua")
CITYFORM = Import("city/cityForm.lua")
CITYFIGHT = Import("city/cityFight.lua")
CITY = Import("city/city.lua")
WILDMINE = Import("city/wildMine.lua")
WILDCHEST = Import("city/wildChest.lua")

--phyleFight
CLSNODE = Import("phyleFight/clsNode.lua")
NODE = Import("phyleFight/node.lua")

--[[
--copy
BASECOPY = Import("copy/clsBaseCopy.lua")
CLSCOPY = Import("copy/clsCopy.lua")
COPY = Import("copy/copy.lua")
USERCOPY = Import("copy/user_copy.lua")
CLSWORLDBOSS = Import("copy/clsWorldBoss.lua")
WORLDBOSS = Import("copy/worldBoss.lua")
CLSTEAM = Import("copy/clsTeam.lua")
CLSTEAMCOPY = Import("copy/clsTeamCopy.lua")
TEAMCOPY = Import("copy/teamCopy.lua")]]


USERCOPY = Import("copy1/user_copy.lua")
CLSCOPY = Import("copy1/clsCopy.lua")
CLSNORMALCOPY = Import("copy1/clsNormalCopy.lua")
CLSTEAMCOPY = Import("copy1/clsTeamCopy.lua")
CLSHEROCOPY = Import("copy1/clsHeroCopy.lua")
CLSTOWERCOPY = Import("copy1/clsTowerCopy.lua")
COPY = Import("copy1/copy.lua")
TEAMCOPY = Import("copy1/teamCopy.lua")

--stats
STATS = Import("stats/stats.lua")
ADMIN = Import("stats/admin.lua")

--防沉迷
ANTIADDICT = Import("act/antiAddict.lua")

--manage
CMD = Import("manage/cmdNumber.lua")
MANAGESVR = Import("manage/manageSvr.lua")
GAMEMANAGE = Import("manage/gameManage.lua")
ITEMINFO = Import("manage/itemInfo.lua")
USERINFO = Import("manage/userInfo.lua")
LOGININFO = Import("manage/loginInfo.lua")
UNIONINFO = Import("manage/unionInfo.lua")
CITYINFO = Import("manage/cityInfo.lua")
PHYLEINFO = Import("manage/phyleInfo.lua")

--初始化服务器设定
function InitServer()
	USERBASE.InitHeroDB()
	RANK.InitAllRank()
	MAIL.InitMailDataBase()
	
	SetSystemLoginState(true)
	print(string.format("Server %d open now!", SERVER_ID))
end

--开启新服定时器
function StartNewServer()
	local timelong = _G.SERVER_START_TIME - os.time()
	if timelong > 0 then
		CALLOUT.CallOut(InitServer, timelong)
		print(string.format("Server %d will open after %d sec.", SERVER_ID, timelong))
	else
		CALLOUT.CallOut(SetSystemLoginState, 5, true)
		print(string.format("Server %d start ok!", SERVER_ID))
	end
end


StartNewServer()


local function MemStat()
	local count = collectgarbage("count")
	local cmd = string.format("top -a | grep main >> %s/saved/script/data/log/%s/memstat.txt", GAME_ROOT_PATH, os.date("%Y%m%d"))
	os.execute(cmd)
	DATA.log(string.format("lua mem: %d k. user online: %d", count, table.size(USER.GetAllOnlineUser())), "memstat.txt")
end

--MemStat

