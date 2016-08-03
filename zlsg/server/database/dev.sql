/*
Navicat MySQL Data Transfer

Source Server         : 217
Source Server Version : 50173
Source Host           : 192.168.23.217:3306
Source Database       : dev

Target Server Type    : MYSQL
Target Server Version : 50173
File Encoding         : 65001

Date: 2015-10-09 16:33:13
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_login
-- ----------------------------
DROP TABLE IF EXISTS `t_login`;
CREATE TABLE `t_login` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sid` varchar(255) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `did` varchar(255) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡å”¯ä¸€æ€§ID',
  `accountName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶å¸å·',
  `platform` varchar(255) NOT NULL DEFAULT '' COMMENT 'å¹³å°æ ‡è¯†ï¼ˆå¦‚91,UCç­‰ï¼‰',
  `channel_id` varchar(255) NOT NULL DEFAULT '' COMMENT 'æ¸ é“IDï¼Œå¹¿å‘ŠæŠ•æ”¾æ‰“åŒ…æ—¶ç”¨åˆ°',
  `appVersion` varchar(255) NOT NULL DEFAULT '' COMMENT 'èŽ·å–åº”ç”¨çš„ç‰ˆæœ¬å·',
  `appType` varchar(255) NOT NULL DEFAULT '' COMMENT 'iphoneã€ipadã€android',
  `device` varchar(255) NOT NULL DEFAULT '' COMMENT 'èŽ·å–çš„è®¾å¤‡çš„æœºåž‹',
  `nm` varchar(255) NOT NULL DEFAULT '' COMMENT 'è”ç½‘æ–¹å¼å¦‚ï¼šwifi',
  `mno` varchar(255) NOT NULL DEFAULT '' COMMENT 'èŽ·å–è®¾å¤‡çš„è¿è¥å•† å¦‚ï¼šç§»åŠ¨',
  `screen` varchar(255) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡çš„å±å¹•åˆ†è¾¨çŽ‡',
  `os` varchar(255) NOT NULL DEFAULT '' COMMENT 'æ“ä½œç³»ç»Ÿ',
  `mac` varchar(255) NOT NULL DEFAULT '' COMMENT 'iosçš„MACåœ°å€,ä¸å¸¦:å·',
  `ip` varchar(255) NOT NULL DEFAULT '' COMMENT 'ç™»å½•IP',
  `Createtime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'ç™»å½•æ—¶é—´',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_login
-- ----------------------------

-- ----------------------------
-- Table structure for t_moneylog
-- ----------------------------
DROP TABLE IF EXISTS `t_moneylog`;
CREATE TABLE `t_moneylog` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sid` varchar(255) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `did` varchar(255) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡å”¯ä¸€æ€§ID',
  `accountName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶å¸å·',
  `playerName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶è§’è‰²',
  `rootId` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'æ¶ˆè€—çš„å¤§ç±»ï¼ˆå¯¹æ¸¸æˆæœ‰ç›¸ä¼¼æ€§è´¨çš„ä¸åŒæ¶ˆè€—ç±»åž‹å½’ç±»ï¼‰',
  `typeId` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'æ¶ˆè€—çš„å°ç±»',
  `oldValue` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'æ¶ˆè€—å‰è¯¥çŽ©å®¶çš„é‡‘å¸å€¼',
  `newValue` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'æ¶ˆè€—åŽè¯¥çŽ©å®¶çš„é‡‘å¸å€¼',
  `gold` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'æ¶ˆè€—çš„é‡‘å¸æ•°ï¼ˆå¦‚æœ‰oldvalueå’ŒnewValueï¼Œè¯¥å­—æ®µå¯ç•¥ï¼‰',
  `goldTicke` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'æ‰©å±•å­—æ®µï¼ˆå¦‚æœ‰æ¶ˆè€—èµ é€çš„ç¤¼åˆ¸ç­‰ï¼‰',
  `goodsId` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'è´­ä¹°çš„é“å…·id',
  `num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'è´­ä¹°ç‰©å“çš„æ•°é‡',
  `createDate` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'äº‹ä»¶å‘ç”Ÿæ—¶é—´',
  `roleId` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `rootId` (`rootId`) USING HASH,
  KEY `typeId` (`typeId`) USING HASH,
  KEY `createDate` (`createDate`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of t_moneylog
-- ----------------------------

-- ----------------------------
-- Table structure for t_moneylogconfig
-- ----------------------------
DROP TABLE IF EXISTS `t_moneylogconfig`;
CREATE TABLE `t_moneylogconfig` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `rootid` int(10) unsigned NOT NULL,
  `rootname` varchar(255) NOT NULL DEFAULT '',
  `typeid` int(10) unsigned NOT NULL,
  `typename` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=108 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of t_moneylogconfig
-- ----------------------------
INSERT INTO `t_moneylogconfig` VALUES ('1', '1', 'VIP购买', '1', '购买体力');
INSERT INTO `t_moneylogconfig` VALUES ('2', '1', 'VIP购买', '2', '购买铜钱');
INSERT INTO `t_moneylogconfig` VALUES ('3', '1', 'VIP购买', '3', '重置精英副本次数');
INSERT INTO `t_moneylogconfig` VALUES ('4', '1', 'VIP购买', '4', '购买竞技场挑战次数');
INSERT INTO `t_moneylogconfig` VALUES ('5', '1', 'VIP购买', '5', '购买技能点次数');
INSERT INTO `t_moneylogconfig` VALUES ('6', '1', 'VIP购买', '6', '重置坐骑技能升级次数');
INSERT INTO `t_moneylogconfig` VALUES ('7', '1', 'VIP购买', '7', '重置沙场百战');
INSERT INTO `t_moneylogconfig` VALUES ('8', '1', 'VIP购买', '8', '重置魔化神将');
INSERT INTO `t_moneylogconfig` VALUES ('101', '2', '竞技场', '101', '清除挑战冷却时间');
INSERT INTO `t_moneylogconfig` VALUES ('102', '3', '宝箱', '102', '自动购买道具');
INSERT INTO `t_moneylogconfig` VALUES ('103', '4', '提醒', '103', '离线挂机');
INSERT INTO `t_moneylogconfig` VALUES ('104', '5', '活动购买', '104', '购买活动限时物品');
INSERT INTO `t_moneylogconfig` VALUES ('105', '6', '军团消耗', '106', '创建军团');
INSERT INTO `t_moneylogconfig` VALUES ('106', '7', '商城', '106', '元宝商城购买');
INSERT INTO `t_moneylogconfig` VALUES ('107', '8', '坐骑', '80000', '坐骑技能升级');

-- ----------------------------
-- Table structure for t_newtask
-- ----------------------------
DROP TABLE IF EXISTS `t_newtask`;
CREATE TABLE `t_newtask` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sid` varchar(255) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `did` varchar(255) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡å”¯ä¸€æ€§ID',
  `accountName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶å¸å·',
  `playerName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶è§’è‰²',
  `taskId` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'ä»»åŠ¡id',
  `status` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'ä»»åŠ¡çŠ¶æ€ï¼ˆ0ï¼šæŽ¥æ”¶ï¼Œ1ï¼šå®Œæˆï¼Œ2ï¼šå¤±è´¥ï¼‰',
  `quality` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'ä»»åŠ¡å®Œæˆå“è´¨',
  `createDate` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'äº‹ä»¶å‘ç”Ÿæ—¶é—´',
  PRIMARY KEY (`id`),
  KEY `createDate` (`createDate`) USING BTREE,
  KEY `taskId` (`taskId`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_newtask
-- ----------------------------

-- ----------------------------
-- Table structure for t_newtaskconfig
-- ----------------------------
DROP TABLE IF EXISTS `t_newtaskconfig`;
CREATE TABLE `t_newtaskconfig` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `taskid` int(10) unsigned NOT NULL,
  `taskname` varchar(255) NOT NULL DEFAULT '',
  `taskOrder` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1043802 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of t_newtaskconfig
-- ----------------------------
INSERT INTO `t_newtaskconfig` VALUES ('1000001', '1000001', '天下大势', '1');
INSERT INTO `t_newtaskconfig` VALUES ('1000301', '1000301', '上好武器', '2');
INSERT INTO `t_newtaskconfig` VALUES ('1000601', '1000601', '战事告急', '3');
INSERT INTO `t_newtaskconfig` VALUES ('1000901', '1000901', '小试牛刀', '4');
INSERT INTO `t_newtaskconfig` VALUES ('1001201', '1001201', '骑兵袭扰', '5');
INSERT INTO `t_newtaskconfig` VALUES ('1001301', '1001301', '狙击骑兵', '6');
INSERT INTO `t_newtaskconfig` VALUES ('1001401', '1001401', '军情紧急', '7');
INSERT INTO `t_newtaskconfig` VALUES ('1001501', '1001501', '地公归尘', '8');
INSERT INTO `t_newtaskconfig` VALUES ('1001801', '1001801', '的卢马', '9');
INSERT INTO `t_newtaskconfig` VALUES ('1001901', '1001901', '的卢马', '10');
INSERT INTO `t_newtaskconfig` VALUES ('1002101', '1002101', '相助孙权', '11');
INSERT INTO `t_newtaskconfig` VALUES ('1002401', '1002401', '纾困之策', '12');
INSERT INTO `t_newtaskconfig` VALUES ('1002701', '1002701', '火烧粮仓', '13');
INSERT INTO `t_newtaskconfig` VALUES ('1003001', '1003001', '破敌中军', '14');
INSERT INTO `t_newtaskconfig` VALUES ('1003301', '1003301', '乘胜追击', '15');
INSERT INTO `t_newtaskconfig` VALUES ('1003601', '1003601', '追击残兵', '16');
INSERT INTO `t_newtaskconfig` VALUES ('1003901', '1003901', '探听消息', '17');
INSERT INTO `t_newtaskconfig` VALUES ('1004201', '1004201', '怒斩天公', '18');
INSERT INTO `t_newtaskconfig` VALUES ('1004301', '1004301', '锻造要术', '19');
INSERT INTO `t_newtaskconfig` VALUES ('1004501', '1004501', '锻造要术', '20');
INSERT INTO `t_newtaskconfig` VALUES ('1004801', '1004801', '逆贼董卓', '21');
INSERT INTO `t_newtaskconfig` VALUES ('1005101', '1005101', '七星宝刀', '22');
INSERT INTO `t_newtaskconfig` VALUES ('1005401', '1005401', '营救献帝', '23');
INSERT INTO `t_newtaskconfig` VALUES ('1006301', '1006301', '离间董吕', '24');
INSERT INTO `t_newtaskconfig` VALUES ('1006601', '1006601', '洛阳秘宫', '25');
INSERT INTO `t_newtaskconfig` VALUES ('1006701', '1006701', '技能升级', '26');
INSERT INTO `t_newtaskconfig` VALUES ('1006801', '1006801', '技能升级', '27');
INSERT INTO `t_newtaskconfig` VALUES ('1006901', '1006901', '再探秘宫', '28');
INSERT INTO `t_newtaskconfig` VALUES ('1007501', '1007501', '解救貂蝉', '29');
INSERT INTO `t_newtaskconfig` VALUES ('1007801', '1007801', '有美相伴', '30');
INSERT INTO `t_newtaskconfig` VALUES ('1008101', '1008101', '缓兵之计', '31');
INSERT INTO `t_newtaskconfig` VALUES ('1008401', '1008401', '缓兵之计', '32');
INSERT INTO `t_newtaskconfig` VALUES ('1008701', '1008701', '紫电怒弩', '33');
INSERT INTO `t_newtaskconfig` VALUES ('1009001', '1009001', '营救曹操', '34');
INSERT INTO `t_newtaskconfig` VALUES ('1009101', '1009101', '熔炼之道', '35');
INSERT INTO `t_newtaskconfig` VALUES ('1009201', '1009201', '熔炼之道', '36');
INSERT INTO `t_newtaskconfig` VALUES ('1009301', '1009301', '猛攻秘宫', '37');
INSERT INTO `t_newtaskconfig` VALUES ('1009601', '1009601', '橙色武器', '38');
INSERT INTO `t_newtaskconfig` VALUES ('1009701', '1009701', '熔炼精髓', '39');
INSERT INTO `t_newtaskconfig` VALUES ('1009801', '1009801', '强化精髓', '40');
INSERT INTO `t_newtaskconfig` VALUES ('1009901', '1009901', '击杀董卓', '41');
INSERT INTO `t_newtaskconfig` VALUES ('1010201', '1010201', '功亏一篑', '42');
INSERT INTO `t_newtaskconfig` VALUES ('1010501', '1010501', '衣带诏', '43');
INSERT INTO `t_newtaskconfig` VALUES ('1010801', '1010801', '乌合之军', '44');
INSERT INTO `t_newtaskconfig` VALUES ('1011101', '1011101', '立马扬威', '45');
INSERT INTO `t_newtaskconfig` VALUES ('1011301', '1011301', '克扣军粮', '46');
INSERT INTO `t_newtaskconfig` VALUES ('1011401', '1011401', '克扣军粮', '47');
INSERT INTO `t_newtaskconfig` VALUES ('1012001', '1012001', '冲锋骑', '48');
INSERT INTO `t_newtaskconfig` VALUES ('1012301', '1012301', '联军大营', '49');
INSERT INTO `t_newtaskconfig` VALUES ('1012601', '1012601', '坐骑升阶', '50');
INSERT INTO `t_newtaskconfig` VALUES ('1012701', '1012701', '坐骑升阶', '51');
INSERT INTO `t_newtaskconfig` VALUES ('1012901', '1012901', '击败袁绍', '52');
INSERT INTO `t_newtaskconfig` VALUES ('1013201', '1013201', '回报献帝', '53');
INSERT INTO `t_newtaskconfig` VALUES ('1013501', '1013501', '精锐部队', '54');
INSERT INTO `t_newtaskconfig` VALUES ('1013801', '1013801', '以战养战', '55');
INSERT INTO `t_newtaskconfig` VALUES ('1014101', '1014101', '以战养战', '56');
INSERT INTO `t_newtaskconfig` VALUES ('1014401', '1014401', '温酒斩华雄', '57');
INSERT INTO `t_newtaskconfig` VALUES ('1014701', '1014701', '温酒斩华雄', '58');
INSERT INTO `t_newtaskconfig` VALUES ('1014801', '1014801', '竞技场', '59');
INSERT INTO `t_newtaskconfig` VALUES ('1014831', '1014831', '竞技场', '60');
INSERT INTO `t_newtaskconfig` VALUES ('1014901', '1014901', '精英考验', '61');
INSERT INTO `t_newtaskconfig` VALUES ('1014931', '1014931', '精英考验', '62');
INSERT INTO `t_newtaskconfig` VALUES ('1014961', '1014961', '黄金宝箱', '63');
INSERT INTO `t_newtaskconfig` VALUES ('1015001', '1015001', '三英战吕布', '64');
INSERT INTO `t_newtaskconfig` VALUES ('1015101', '1015101', '强大军团', '65');
INSERT INTO `t_newtaskconfig` VALUES ('1015301', '1015301', '会盟之意', '66');
INSERT INTO `t_newtaskconfig` VALUES ('1015601', '1015601', '杀退伏兵', '67');
INSERT INTO `t_newtaskconfig` VALUES ('1015901', '1015901', '飘雪射手', '68');
INSERT INTO `t_newtaskconfig` VALUES ('1016001', '1016001', '四门金锁阵', '69');
INSERT INTO `t_newtaskconfig` VALUES ('1016151', '1016151', '尚香感恩', '70');
INSERT INTO `t_newtaskconfig` VALUES ('1016201', '1016201', '大将魏延', '71');
INSERT INTO `t_newtaskconfig` VALUES ('1016501', '1016501', '探听消息', '72');
INSERT INTO `t_newtaskconfig` VALUES ('1016801', '1016801', '解救孙权', '73');
INSERT INTO `t_newtaskconfig` VALUES ('1017101', '1017101', '了解行踪', '74');
INSERT INTO `t_newtaskconfig` VALUES ('1017401', '1017401', '大将黄祖', '75');
INSERT INTO `t_newtaskconfig` VALUES ('1017701', '1017701', '突破铁桥', '76');
INSERT INTO `t_newtaskconfig` VALUES ('1018001', '1018001', '孙坚之死', '77');
INSERT INTO `t_newtaskconfig` VALUES ('1018301', '1018301', '斩刘表', '78');
INSERT INTO `t_newtaskconfig` VALUES ('1018601', '1018601', '老将黄忠', '79');
INSERT INTO `t_newtaskconfig` VALUES ('1018901', '1018901', '孙坚之死', '80');
INSERT INTO `t_newtaskconfig` VALUES ('1019201', '1019201', '黄巾骑兵', '81');
INSERT INTO `t_newtaskconfig` VALUES ('1019501', '1019501', '巨锤兵', '82');
INSERT INTO `t_newtaskconfig` VALUES ('1019801', '1019801', '了解战事', '83');
INSERT INTO `t_newtaskconfig` VALUES ('1020101', '1020101', '黄巾精锐', '84');
INSERT INTO `t_newtaskconfig` VALUES ('1020401', '1020401', '黄巾妖书', '85');
INSERT INTO `t_newtaskconfig` VALUES ('1020701', '1020701', '夺下码头', '86');
INSERT INTO `t_newtaskconfig` VALUES ('1021001', '1021001', '古之恶来', '87');
INSERT INTO `t_newtaskconfig` VALUES ('1021301', '1021301', '黄巾护法', '88');
INSERT INTO `t_newtaskconfig` VALUES ('1021601', '1021601', '陷阵骠骑', '89');
INSERT INTO `t_newtaskconfig` VALUES ('1021901', '1021901', '拼死抵抗', '90');
INSERT INTO `t_newtaskconfig` VALUES ('1022201', '1022201', '招揽之策', '91');
INSERT INTO `t_newtaskconfig` VALUES ('1022501', '1022501', '哼哈二将', '92');
INSERT INTO `t_newtaskconfig` VALUES ('1022801', '1022801', '徐州惊变', '93');
INSERT INTO `t_newtaskconfig` VALUES ('1023101', '1023101', '联络郭嘉', '94');
INSERT INTO `t_newtaskconfig` VALUES ('1023401', '1023401', '断枪', '95');
INSERT INTO `t_newtaskconfig` VALUES ('1023701', '1023701', '制胜之策', '96');
INSERT INTO `t_newtaskconfig` VALUES ('1024001', '1024001', '击败赵云', '97');
INSERT INTO `t_newtaskconfig` VALUES ('1024301', '1024301', '冲锋骑', '98');
INSERT INTO `t_newtaskconfig` VALUES ('1024601', '1024601', '克敌制胜', '99');
INSERT INTO `t_newtaskconfig` VALUES ('1024901', '1024901', '黑面杀神', '100');
INSERT INTO `t_newtaskconfig` VALUES ('1025201', '1025201', '击败张飞', '101');
INSERT INTO `t_newtaskconfig` VALUES ('1025501', '1025501', '突入徐州', '102');
INSERT INTO `t_newtaskconfig` VALUES ('1025801', '1025801', '拜访刘备', '103');
INSERT INTO `t_newtaskconfig` VALUES ('1026101', '1026101', '积极备战', '104');
INSERT INTO `t_newtaskconfig` VALUES ('1026401', '1026401', '清剿敌军', '105');
INSERT INTO `t_newtaskconfig` VALUES ('1026701', '1026701', '击败关羽', '106');
INSERT INTO `t_newtaskconfig` VALUES ('1027001', '1027001', '提升战力', '107');
INSERT INTO `t_newtaskconfig` VALUES ('1027301', '1027301', '陶谦', '108');
INSERT INTO `t_newtaskconfig` VALUES ('1027601', '1027601', '援助孙策', '109');
INSERT INTO `t_newtaskconfig` VALUES ('1027901', '1027901', '接应周瑜', '110');
INSERT INTO `t_newtaskconfig` VALUES ('1028201', '1028201', '先锋黄盖', '111');
INSERT INTO `t_newtaskconfig` VALUES ('1028501', '1028501', '破敌之策', '112');
INSERT INTO `t_newtaskconfig` VALUES ('1028801', '1028801', '出其不意', '113');
INSERT INTO `t_newtaskconfig` VALUES ('1029101', '1029101', '击退伏兵', '114');
INSERT INTO `t_newtaskconfig` VALUES ('1029401', '1029401', '山林烽烟', '115');
INSERT INTO `t_newtaskconfig` VALUES ('1029501', '1029501', '提升实力', '117');
INSERT INTO `t_newtaskconfig` VALUES ('1029701', '1029701', '生擒太史慈', '116');
INSERT INTO `t_newtaskconfig` VALUES ('1030001', '1030001', '进攻丹阳关', '118');
INSERT INTO `t_newtaskconfig` VALUES ('1030301', '1030301', '截断军需', '118');
INSERT INTO `t_newtaskconfig` VALUES ('1030401', '1030401', '决战之计', '118');
INSERT INTO `t_newtaskconfig` VALUES ('1030501', '1030501', '击杀于麋', '118');
INSERT INTO `t_newtaskconfig` VALUES ('1030504', '1030504', '击杀刘繇', '118');
INSERT INTO `t_newtaskconfig` VALUES ('1030601', '1030601', '曹操之危', '119');
INSERT INTO `t_newtaskconfig` VALUES ('1030901', '1030901', '杀入重围', '120');
INSERT INTO `t_newtaskconfig` VALUES ('1031201', '1031201', '直捣内城', '121');
INSERT INTO `t_newtaskconfig` VALUES ('1031501', '1031501', '击败袁绍', '122');
INSERT INTO `t_newtaskconfig` VALUES ('1031601', '1031601', '问计孙权', '122');
INSERT INTO `t_newtaskconfig` VALUES ('1031801', '1031801', '许褚接应', '123');
INSERT INTO `t_newtaskconfig` VALUES ('1032101', '1032101', '阴谋重重', '124');
INSERT INTO `t_newtaskconfig` VALUES ('1032401', '1032401', '破阵', '125');
INSERT INTO `t_newtaskconfig` VALUES ('1032701', '1032701', '鸿门宴', '126');
INSERT INTO `t_newtaskconfig` VALUES ('1033001', '1033001', '紧追不放', '127');
INSERT INTO `t_newtaskconfig` VALUES ('1033301', '1033301', '提升实力', '128');
INSERT INTO `t_newtaskconfig` VALUES ('1033601', '1033601', '悍勇虎卫', '129');
INSERT INTO `t_newtaskconfig` VALUES ('1033901', '1033901', '进攻废墟', '130');
INSERT INTO `t_newtaskconfig` VALUES ('1034001', '1034001', '提升实力', '128');
INSERT INTO `t_newtaskconfig` VALUES ('1034201', '1034201', '营救曹操', '131');
INSERT INTO `t_newtaskconfig` VALUES ('1034501', '1034501', '袁术称帝', '132');
INSERT INTO `t_newtaskconfig` VALUES ('1034801', '1034801', '急先锋', '133');
INSERT INTO `t_newtaskconfig` VALUES ('1035101', '1035101', '击破骑兵', '134');
INSERT INTO `t_newtaskconfig` VALUES ('1035401', '1035401', '猛将纪灵', '135');
INSERT INTO `t_newtaskconfig` VALUES ('1035701', '1035701', '击败纪灵', '136');
INSERT INTO `t_newtaskconfig` VALUES ('1036001', '1036001', '反击包围', '137');
INSERT INTO `t_newtaskconfig` VALUES ('1036301', '1036301', '共商大计', '138');
INSERT INTO `t_newtaskconfig` VALUES ('1036601', '1036601', '共商大计', '139');
INSERT INTO `t_newtaskconfig` VALUES ('1036901', '1036901', '风雷策士', '140');
INSERT INTO `t_newtaskconfig` VALUES ('1037201', '1037201', '江东小霸王', '141');
INSERT INTO `t_newtaskconfig` VALUES ('1037501', '1037501', '难言之隐', '142');
INSERT INTO `t_newtaskconfig` VALUES ('1037801', '1037801', '难言之隐', '143');
INSERT INTO `t_newtaskconfig` VALUES ('1038101', '1038101', '狙击败兵', '144');
INSERT INTO `t_newtaskconfig` VALUES ('1038401', '1038401', '禁卫统领', '145');
INSERT INTO `t_newtaskconfig` VALUES ('1038701', '1038701', '提升实力', '146');
INSERT INTO `t_newtaskconfig` VALUES ('1039001', '1039001', '歼灭袁术', '147');
INSERT INTO `t_newtaskconfig` VALUES ('1039101', '1039101', '爱妃大乔', '147');
INSERT INTO `t_newtaskconfig` VALUES ('1039301', '1039301', '称帝之因', '148');
INSERT INTO `t_newtaskconfig` VALUES ('1039601', '1039601', '急攻山道', '149');
INSERT INTO `t_newtaskconfig` VALUES ('1039901', '1039901', '蛟龙战士', '150');
INSERT INTO `t_newtaskconfig` VALUES ('1040201', '1040201', '精锐之师', '151');
INSERT INTO `t_newtaskconfig` VALUES ('1040501', '1040501', '策士陈宫', '152');
INSERT INTO `t_newtaskconfig` VALUES ('1040801', '1040801', '陷阵勇士', '153');
INSERT INTO `t_newtaskconfig` VALUES ('1041101', '1041101', '了解军情', '154');
INSERT INTO `t_newtaskconfig` VALUES ('1041401', '1041401', '连营之火', '155');
INSERT INTO `t_newtaskconfig` VALUES ('1041701', '1041701', '提升实力', '156');
INSERT INTO `t_newtaskconfig` VALUES ('1042001', '1042001', '击败貂蝉', '157');
INSERT INTO `t_newtaskconfig` VALUES ('1042301', '1042301', '巨斧悍将', '158');
INSERT INTO `t_newtaskconfig` VALUES ('1042601', '1042601', '破营', '159');
INSERT INTO `t_newtaskconfig` VALUES ('1042901', '1042901', '问计曹操', '160');
INSERT INTO `t_newtaskconfig` VALUES ('1043201', '1043201', '击败陈宫', '161');
INSERT INTO `t_newtaskconfig` VALUES ('1043501', '1043501', '英雄末路', '162');
INSERT INTO `t_newtaskconfig` VALUES ('1043801', '1043801', '风轻云淡', '162');

-- ----------------------------
-- Table structure for t_offline
-- ----------------------------
DROP TABLE IF EXISTS `t_offline`;
CREATE TABLE `t_offline` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sid` varchar(255) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `did` varchar(255) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡å”¯ä¸€æ€§ID',
  `accountName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶å¸å·',
  `playerName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶è§’è‰²',
  `playerLevel` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å½“å‰çŽ©å®¶ç­‰çº§',
  `ip` varchar(255) NOT NULL DEFAULT '' COMMENT 'ç”¨æˆ·ç™»å½•ip',
  `onlineTime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'çŽ©å®¶è¯¥æ¬¡ç™»å½•åœ¨çº¿æ—¶é•¿ï¼ˆsocketè¿žæŽ¥æ—¶é•¿ï¼‰',
  `historyOnline` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'çŽ©å®¶åŽ†å²ç´¯è®¡åœ¨çº¿æ—¶é•¿',
  `createDate` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'äº‹ä»¶å‘ç”Ÿæ—¶é—´',
  PRIMARY KEY (`id`),
  KEY `createDate` (`createDate`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_offline
-- ----------------------------

-- ----------------------------
-- Table structure for t_online
-- ----------------------------
DROP TABLE IF EXISTS `t_online`;
CREATE TABLE `t_online` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT 'è‡ªå¢žid',
  `sid` varchar(255) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `allcount` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'å…¨æœè¯¥æ—¶åˆ»åœ¨çº¿æ•°',
  `cashcount` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'å……å€¼è¿‡çš„çŽ©å®¶åœ¨çº¿æ•°',
  `createDate` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'æ—¥å¿—æ—¶é—´ï¼ˆå»ºè®®æ¯5åˆ†é’Ÿçºªå½•ä¸€æ¬¡ï¼‰',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_online
-- ----------------------------

-- ----------------------------
-- Table structure for t_pay_jiayou
-- ----------------------------
DROP TABLE IF EXISTS `t_pay_jiayou`;
CREATE TABLE `t_pay_jiayou` (
  `orderid` varchar(255) NOT NULL,
  `username` varchar(255) DEFAULT NULL,
  `gameid` varchar(255) DEFAULT NULL,
  `roleid` varchar(255) DEFAULT NULL,
  `serverid` varchar(255) DEFAULT NULL,
  `paytype` varchar(255) DEFAULT NULL,
  `amount` varchar(255) DEFAULT NULL,
  `paytime` varchar(255) DEFAULT NULL,
  `attach` varchar(255) DEFAULT NULL,
  `sign` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`orderid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of t_pay_jiayou
-- ----------------------------

-- ----------------------------
-- Table structure for t_pay_paojiao
-- ----------------------------
DROP TABLE IF EXISTS `t_pay_paojiao`;
CREATE TABLE `t_pay_paojiao` (
  `orderNo` varchar(255) NOT NULL,
  `uid` varchar(255) DEFAULT NULL,
  `price` varchar(255) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `remark` varchar(255) DEFAULT NULL,
  `subject` varchar(255) DEFAULT NULL,
  `gameId` varchar(255) DEFAULT NULL,
  `payTime` varchar(255) DEFAULT NULL,
  `ext` varchar(255) DEFAULT NULL,
  `sign` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`orderNo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of t_pay_paojiao
-- ----------------------------

-- ----------------------------
-- Table structure for t_recharge
-- ----------------------------
DROP TABLE IF EXISTS `t_recharge`;
CREATE TABLE `t_recharge` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sid` varchar(255) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `did` varchar(255) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡å”¯ä¸€æ€§ID',
  `accountName` varchar(255) NOT NULL DEFAULT '' COMMENT 'å……å€¼å¸å·',
  `playerName` varchar(255) NOT NULL DEFAULT '' COMMENT 'å……å€¼è§’è‰²',
  `playerLevel` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å……å€¼æ—¶çš„çŽ©å®¶ç­‰çº§',
  `transactionId` varchar(100) NOT NULL DEFAULT '' COMMENT 'è®¢å•å·',
  `currency` varchar(10) NOT NULL DEFAULT '' COMMENT 'è´§å¸ç±»åž‹ï¼šä¸€èˆ¬ä¸ºå¤§å†™ç®€å†™ï¼Œå¦‚RMB',
  `inpourType` varchar(50) NOT NULL DEFAULT '' COMMENT 'æ”¯ä»˜æ–¹å¼ï¼šå¦‚æ”¯ä»˜å®ã€ç½‘é“¶ç­‰',
  `addcoin` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT 'å¥—é¤ç±»åž‹ï¼šå¦‚æžœæ¸¸æˆæœ‰å……å€¼å¥—é¤æ ‡è®°',
  `money` decimal(10,2) unsigned NOT NULL DEFAULT '0.00' COMMENT 'å……å€¼é‡‘é¢ï¼ˆQå¸ã€å¹³å°å¸ã€rmbï¼‰',
  `gold` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å……å€¼èŽ·å¾—é‡‘å¸',
  `exchangeType` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT 'å……å€¼ç±»åž‹(0/1æ­£å¸¸å……å€¼,2.è¡¥å¿é‡‘å¸å¡(å……å€¼å¤±è´¥å®¢æœè¡¥å¿ï¼Œå¯å½“ä½œæ­£å¸¸å……å€¼) 3.æ™ºèƒ½é‡‘å¸å¡(æ´»åŠ¨å¥–åŠ±æˆ–è€…æ‰£é™¤é‡‘å¸ï¼Œå¯ä»¥æœ‰æ­£è´Ÿ) 4.å†…éƒ¨æµ‹è¯•(ç»™å†…éƒ¨äººå‘˜å‘æ”¾é‡‘å¸)',
  `depay` decimal(10,2) unsigned NOT NULL DEFAULT '0.00' COMMENT 'æ‰‹ç»­è´¹(ç½‘é“¶æ‰‹ç»­è´¹ç­‰)',
  `gDepay` decimal(10,2) unsigned NOT NULL DEFAULT '0.00' COMMENT 'æ¸¸æˆæ‰‹ç»­è´¹(å¦‚ï¼šè…¾è®¯æŠµæ‰£åˆ¸å……å€¼)',
  `state` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT 'çŠ¶æ€ï¼šå¦‚æœ‰å¤±è´¥çš„æƒ…å†µï¼Œç”¨äºŽåŒºåˆ†',
  `createDate` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å……å€¼æ—¶é—´ï¼ˆç§’æ—¶é—´æˆ³ï¼‰',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_recharge
-- ----------------------------

-- ----------------------------
-- Table structure for t_upgradelog
-- ----------------------------
DROP TABLE IF EXISTS `t_upgradelog`;
CREATE TABLE `t_upgradelog` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sid` varchar(255) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `did` varchar(255) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡å”¯ä¸€æ€§ID',
  `accountName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶å¸å·',
  `playerName` varchar(255) NOT NULL DEFAULT '' COMMENT 'çŽ©å®¶è§’è‰²',
  `oldLevel` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å‡çº§å‰ç­‰çº§',
  `newLevel` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å‡çº§åŽç­‰çº§',
  `sucess` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '1ï¼šæˆåŠŸï¼Œ0å¤±è´¥',
  `upgradeType` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å‡çº§ç±»åž‹(1:çŽ©å®¶ç­‰çº§,2:åéª‘ç­‰çº§,3:å® ç‰©ç­‰çº§â€¦)',
  `params` varchar(255) NOT NULL DEFAULT '' COMMENT 'æ¶ˆè€—é‡‘å¸å’Œé“å…·æƒ…å†µï¼ˆæ²¡æœ‰å¯ä»¥ä¸è®°å½•ï¼‰è§æ³¨â‘ ',
  `createDate` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'äº‹ä»¶å‘ç”Ÿæ—¶é—´',
  PRIMARY KEY (`id`),
  KEY `createDate` (`createDate`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_upgradelog
-- ----------------------------

-- ----------------------------
-- Table structure for tb_activity_open
-- ----------------------------
DROP TABLE IF EXISTS `tb_activity_open`;
CREATE TABLE `tb_activity_open` (
  `activity_id` int(11) unsigned NOT NULL DEFAULT '0',
  `open_type` int(11) unsigned NOT NULL DEFAULT '0',
  `reward_flag` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`activity_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;

-- ----------------------------
-- Records of tb_activity_open
-- ----------------------------

-- ----------------------------
-- Table structure for tb_activity_record
-- ----------------------------
DROP TABLE IF EXISTS `tb_activity_record`;
CREATE TABLE `tb_activity_record` (
  `activity_id` int(11) unsigned NOT NULL DEFAULT '0',
  `act_record` blob NOT NULL,
  PRIMARY KEY (`activity_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_activity_record
-- ----------------------------

-- ----------------------------
-- Table structure for tb_arena_record
-- ----------------------------
DROP TABLE IF EXISTS `tb_arena_record`;
CREATE TABLE `tb_arena_record` (
  `role_id` int(11) unsigned NOT NULL,
  `status` int(11) NOT NULL,
  `log` blob NOT NULL,
  PRIMARY KEY (`role_id`),
  KEY `ID` (`role_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_arena_record
-- ----------------------------

-- ----------------------------
-- Table structure for tb_arena_robot
-- ----------------------------
DROP TABLE IF EXISTS `tb_arena_robot`;
CREATE TABLE `tb_arena_robot` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `config_id` int(10) unsigned NOT NULL,
  `fighting_power` int(10) unsigned NOT NULL,
  `attr` blob NOT NULL,
  `pet_id` int(10) NOT NULL,
  `g_attr` blob NOT NULL,
  `standby` int(11) NOT NULL,
  `b_attr` blob NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_arena_robot
-- ----------------------------

-- ----------------------------
-- Table structure for tb_center_arena
-- ----------------------------
DROP TABLE IF EXISTS `tb_center_arena`;
CREATE TABLE `tb_center_arena` (
  `rank` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `type` int(11) DEFAULT NULL,
  `history` int(11) unsigned NOT NULL,
  PRIMARY KEY (`rank`),
  KEY `rank_id` (`rank`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_center_arena
-- ----------------------------

-- ----------------------------
-- Table structure for tb_center_cycle
-- ----------------------------
DROP TABLE IF EXISTS `tb_center_cycle`;
CREATE TABLE `tb_center_cycle` (
  `level` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `tick` int(11) unsigned NOT NULL,
  PRIMARY KEY (`level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_center_cycle
-- ----------------------------

-- ----------------------------
-- Table structure for tb_center_cycle_role
-- ----------------------------
DROP TABLE IF EXISTS `tb_center_cycle_role`;
CREATE TABLE `tb_center_cycle_role` (
  `role_id` int(10) unsigned NOT NULL,
  `helped_count` int(10) unsigned NOT NULL,
  `timestamp` int(10) NOT NULL,
  `passed_levels` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_center_cycle_role
-- ----------------------------

-- ----------------------------
-- Table structure for tb_center_devil
-- ----------------------------
DROP TABLE IF EXISTS `tb_center_devil`;
CREATE TABLE `tb_center_devil` (
  `id` int(10) unsigned NOT NULL,
  `role_id` int(10) unsigned NOT NULL,
  `time` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_center_devil
-- ----------------------------

-- ----------------------------
-- Table structure for tb_center_notice
-- ----------------------------
DROP TABLE IF EXISTS `tb_center_notice`;
CREATE TABLE `tb_center_notice` (
  `msg_id` int(11) unsigned NOT NULL DEFAULT '0',
  `content` varchar(255) NOT NULL DEFAULT '',
  `start_time` int(11) unsigned NOT NULL DEFAULT '0',
  `close_time` int(11) unsigned NOT NULL DEFAULT '0',
  `interval_time` int(11) unsigned NOT NULL DEFAULT '0',
  `is_over` int(11) unsigned NOT NULL DEFAULT '0',
  `brocast_time` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`msg_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_center_notice
-- ----------------------------

-- ----------------------------
-- Table structure for tb_city_attack
-- ----------------------------
DROP TABLE IF EXISTS `tb_city_attack`;
CREATE TABLE `tb_city_attack` (
  `id` int(11) NOT NULL,
  `family_id` int(11) unsigned NOT NULL DEFAULT '0',
  `occupy_time` int(11) unsigned NOT NULL DEFAULT '0',
  `title` varchar(255) NOT NULL,
  `mod_title_time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_city_attack
-- ----------------------------

-- ----------------------------
-- Table structure for tb_city_welfare
-- ----------------------------
DROP TABLE IF EXISTS `tb_city_welfare`;
CREATE TABLE `tb_city_welfare` (
  `id` int(10) unsigned NOT NULL,
  `family_id` int(10) unsigned NOT NULL,
  `official_1` int(10) unsigned DEFAULT NULL,
  `official_2` int(10) unsigned DEFAULT NULL,
  `official_3` int(10) unsigned DEFAULT NULL,
  `official_4` int(10) unsigned DEFAULT NULL,
  `official_5` int(10) unsigned DEFAULT NULL,
  `official_6` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_city_welfare
-- ----------------------------

-- ----------------------------
-- Table structure for tb_family
-- ----------------------------
DROP TABLE IF EXISTS `tb_family`;
CREATE TABLE `tb_family` (
  `id` int(11) unsigned NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂID',
  `name` varchar(64) NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂÃ¥ÂÂÃ¥Â­â€”',
  `flag_id` int(11) unsigned NOT NULL DEFAULT '0',
  `flag_name` varchar(64) NOT NULL,
  `lvl` int(11) unsigned NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂÃ§Â­â€°Ã§ÂºÂ§',
  `chief_id` int(11) unsigned NOT NULL COMMENT 'Ã¦â€”ÂÃ©â€¢Â¿',
  `chief_name` varchar(64) NOT NULL DEFAULT '',
  `deputy_id` int(11) unsigned NOT NULL,
  `post` varchar(1024) NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂÃ¥â€¦Â¬Ã¥â€˜Å ',
  `money` int(11) unsigned NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂÃ¨Âµâ€žÃ©â€¡â€˜',
  `power` int(11) unsigned NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂÃ¦Ë†ËœÃ¦â€“â€”Ã¥Å â€º',
  `domain` int(11) unsigned NOT NULL DEFAULT '0',
  `king` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `emperor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `cur_member_num` int(11) unsigned NOT NULL COMMENT 'Ã¥Â½â€œÃ¥â€°ÂÃ¦Ë†ÂÃ¥â€˜ËœÃ¤Â¸ÂªÃ¦â€¢Â°',
  `approval` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `activity` int(11) unsigned NOT NULL DEFAULT '0',
  `qq` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'QQÃ§Â¾Â¤Ã¥ÂÂ·',
  `yy` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'YYÃ§Â¾Â¤Ã¥ÂÂ·',
  `boss_time` int(11) unsigned NOT NULL DEFAULT '0',
  `call_boss_time` int(11) unsigned NOT NULL DEFAULT '0',
  `op_tag` int(11) unsigned NOT NULL DEFAULT '0',
  `op_tag_time` int(11) unsigned NOT NULL DEFAULT '0',
  `status` int(11) unsigned NOT NULL DEFAULT '0',
  `belong_sect` int(11) unsigned NOT NULL DEFAULT '1',
  `create_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'å†›å›¢åˆ›å»ºæ—¶é—´',
  `dragon_count` int(11) unsigned NOT NULL DEFAULT '0',
  `tiger_count` int(11) unsigned NOT NULL DEFAULT '0',
  `rosefinch_count` int(11) unsigned NOT NULL DEFAULT '0',
  `tortoise_count` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`belong_sect`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_family
-- ----------------------------

-- ----------------------------
-- Table structure for tb_family_diplomacy
-- ----------------------------
DROP TABLE IF EXISTS `tb_family_diplomacy`;
CREATE TABLE `tb_family_diplomacy` (
  `family_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `target_id` int(11) unsigned NOT NULL DEFAULT '0',
  `relation_type` int(11) unsigned NOT NULL DEFAULT '0',
  `apply_time` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`family_id`,`target_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_family_diplomacy
-- ----------------------------

-- ----------------------------
-- Table structure for tb_family_domain
-- ----------------------------
DROP TABLE IF EXISTS `tb_family_domain`;
CREATE TABLE `tb_family_domain` (
  `scene_id` int(11) unsigned NOT NULL,
  `family_id` int(11) unsigned NOT NULL,
  `time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`scene_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_family_domain
-- ----------------------------

-- ----------------------------
-- Table structure for tb_family_log
-- ----------------------------
DROP TABLE IF EXISTS `tb_family_log`;
CREATE TABLE `tb_family_log` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `family_id` int(11) unsigned NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂId',
  `time` int(11) unsigned NOT NULL COMMENT 'Ã¦â€”Â¶Ã©â€”Â´',
  `type` int(11) unsigned NOT NULL COMMENT 'Ã§Â±Â»Ã¥Å¾â€¹',
  `key` int(11) unsigned NOT NULL COMMENT 'Ã¥Ââ€˜Ã¨ÂµÂ·',
  `value` int(11) unsigned NOT NULL COMMENT 'Ã¤Âºâ€¹Ã¨Â¿Â¹',
  `family_name` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`,`family_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_family_log
-- ----------------------------

-- ----------------------------
-- Table structure for tb_family_member
-- ----------------------------
DROP TABLE IF EXISTS `tb_family_member`;
CREATE TABLE `tb_family_member` (
  `family_id` int(11) unsigned NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂId',
  `role_id` int(11) unsigned NOT NULL COMMENT 'Ã¨Â§â€™Ã¨â€°Â²Id',
  `permissions` int(11) unsigned NOT NULL COMMENT 'Ã¦ÂÆ’Ã©â„¢Â',
  `time` int(11) unsigned NOT NULL COMMENT 'Ã¥Å Â Ã¥â€¦Â¥Ã¦â€”Â¶Ã©â€”Â´',
  `add_by` int(11) unsigned NOT NULL COMMENT 'Ã¦Â·Â»Ã¥Å Â Ã¨â‚¬â€¦Ã¨Â§â€™Ã¨â€°Â²Id',
  `strength` int(11) unsigned NOT NULL,
  `dragon_count` int(11) unsigned NOT NULL DEFAULT '0',
  `tiger_count` int(11) unsigned NOT NULL DEFAULT '0',
  `rosefinch_count` int(11) unsigned NOT NULL DEFAULT '0',
  `tortoise_count` int(11) unsigned NOT NULL DEFAULT '0',
  `money` int(11) unsigned NOT NULL DEFAULT '0',
  `total_donate` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`family_id`,`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_family_member
-- ----------------------------

-- ----------------------------
-- Table structure for tb_family_record
-- ----------------------------
DROP TABLE IF EXISTS `tb_family_record`;
CREATE TABLE `tb_family_record` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `type` int(11) unsigned NOT NULL,
  `killer_family` int(11) unsigned NOT NULL COMMENT 'Ã¥Â®Â¶Ã¦â€”ÂId',
  `killer` int(11) unsigned NOT NULL COMMENT 'Ã¦Ë†ËœÃ¦â€“â€”Ã§Â±Â»Ã¥Å¾â€¹',
  `slain_family` int(11) unsigned NOT NULL COMMENT 'Ã¥â€¡Â»Ã¦Ââ‚¬Ã¨â‚¬â€¦',
  `slain_title` int(11) unsigned NOT NULL COMMENT 'Ã¨Â¢Â«Ã¥â€¡Â»Ã¦Ââ‚¬Ã¨â‚¬â€¦',
  `slain` int(11) unsigned NOT NULL COMMENT 'Ã¨Â¢Â«Ã¦Ââ‚¬Ã¨â‚¬â€¦Ã¨ÂÅ’Ã¤Â½Â',
  PRIMARY KEY (`id`,`killer_family`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_family_record
-- ----------------------------

-- ----------------------------
-- Table structure for tb_family_skill
-- ----------------------------
DROP TABLE IF EXISTS `tb_family_skill`;
CREATE TABLE `tb_family_skill` (
  `family_id` int(11) unsigned NOT NULL,
  `skills` blob NOT NULL,
  PRIMARY KEY (`family_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_family_skill
-- ----------------------------

-- ----------------------------
-- Table structure for tb_gamesvr
-- ----------------------------
DROP TABLE IF EXISTS `tb_gamesvr`;
CREATE TABLE `tb_gamesvr` (
  `svr_id` int(11) unsigned NOT NULL DEFAULT '0',
  `svr_name` varchar(32) NOT NULL DEFAULT '',
  `player_port` int(11) unsigned NOT NULL DEFAULT '0',
  `inner_port` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`svr_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_gamesvr
-- ----------------------------

-- ----------------------------
-- Table structure for tb_mail_center
-- ----------------------------
DROP TABLE IF EXISTS `tb_mail_center`;
CREATE TABLE `tb_mail_center` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `mail` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_mail_center
-- ----------------------------

-- ----------------------------
-- Table structure for tb_mate
-- ----------------------------
DROP TABLE IF EXISTS `tb_mate`;
CREATE TABLE `tb_mate` (
  `role_id` int(11) unsigned NOT NULL,
  `mate_role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `today_times` int(11) unsigned NOT NULL DEFAULT '0',
  `last_time` int(11) unsigned NOT NULL DEFAULT '0',
  `miss_times` int(11) unsigned NOT NULL DEFAULT '0',
  `total_times` int(11) unsigned NOT NULL DEFAULT '0',
  `depth_time` int(11) unsigned NOT NULL DEFAULT '0',
  `advance_depth` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`),
  KEY `role_id` (`role_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_mate
-- ----------------------------

-- ----------------------------
-- Table structure for tb_player
-- ----------------------------
DROP TABLE IF EXISTS `tb_player`;
CREATE TABLE `tb_player` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `svr_id` int(11) unsigned NOT NULL DEFAULT '0',
  `openid` varchar(32) NOT NULL DEFAULT '',
  `last_login_time` int(11) unsigned NOT NULL DEFAULT '0',
  `create_time` int(11) unsigned NOT NULL DEFAULT '0',
  `platform_id` int(11) unsigned NOT NULL DEFAULT '0',
  `sid` varchar(64) NOT NULL DEFAULT '' COMMENT 'æœåŸŸåå”¯ä¸€æ ‡è¯†ç¬¦',
  `did` varchar(64) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡å”¯ä¸€æ€§ID',
  `platform` varchar(64) NOT NULL DEFAULT '' COMMENT 'å¹³å°æ ‡è¯†ï¼ˆå¦‚91,UCç­‰ï¼‰',
  `channel_id` varchar(64) NOT NULL DEFAULT '' COMMENT 'æ¸ é“IDï¼Œå¹¿å‘ŠæŠ•æ”¾æ‰“åŒ…æ—¶ç”¨åˆ°',
  `appVersion` varchar(64) NOT NULL DEFAULT '' COMMENT 'èŽ·å–åº”ç”¨çš„ç‰ˆæœ¬å·',
  `appType` varchar(64) NOT NULL DEFAULT '' COMMENT '(iphoneã€ipadã€android)',
  `device` varchar(64) NOT NULL DEFAULT '' COMMENT 'èŽ·å–çš„è®¾å¤‡çš„æœºåž‹',
  `nm` varchar(64) NOT NULL DEFAULT '' COMMENT 'è”ç½‘æ–¹å¼å¦‚ï¼šwifi',
  `mno` varchar(64) NOT NULL DEFAULT '' COMMENT 'èŽ·å–è®¾å¤‡çš„è¿è¥å•† å¦‚ï¼šç§»åŠ¨',
  `screen` varchar(64) NOT NULL DEFAULT '' COMMENT 'è®¾å¤‡çš„å±å¹•åˆ†è¾¨çŽ‡',
  `os` varchar(64) NOT NULL DEFAULT '' COMMENT 'æ“ä½œç³»ç»Ÿ',
  `mac` varchar(64) NOT NULL DEFAULT '' COMMENT 'iosçš„MACåœ°å€,ä¸å¸¦:å·',
  `check` int(11) unsigned NOT NULL,
  `check_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `openid` (`openid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_player
-- ----------------------------

-- ----------------------------
-- Table structure for tb_player_role
-- ----------------------------
DROP TABLE IF EXISTS `tb_player_role`;
CREATE TABLE `tb_player_role` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `system_id` int(11) unsigned NOT NULL DEFAULT '0',
  `player_id` int(11) unsigned NOT NULL DEFAULT '0',
  `role_name` varchar(64) NOT NULL DEFAULT '',
  `status` int(11) unsigned NOT NULL DEFAULT '0',
  `sex` int(11) unsigned NOT NULL DEFAULT '0',
  `profession` int(11) unsigned NOT NULL DEFAULT '0',
  `experience` bigint(20) unsigned NOT NULL DEFAULT '0',
  `level` int(11) unsigned NOT NULL DEFAULT '0',
  `tael` int(11) unsigned NOT NULL DEFAULT '0',
  `ingot` int(11) unsigned NOT NULL DEFAULT '0',
  `gift_money` int(11) unsigned NOT NULL DEFAULT '0',
  `is_suspended_account` int(11) unsigned NOT NULL DEFAULT '0',
  `partner_id` int(11) unsigned NOT NULL DEFAULT '0',
  `family_id` int(11) unsigned NOT NULL DEFAULT '0',
  `family_position` int(11) unsigned NOT NULL DEFAULT '0',
  `can_be_raised` int(11) unsigned NOT NULL DEFAULT '0',
  `family_level` int(11) unsigned NOT NULL DEFAULT '0',
  `can_join_family` int(11) unsigned NOT NULL DEFAULT '0',
  `can_be_transmit` int(11) unsigned NOT NULL DEFAULT '0',
  `pk_model` int(11) unsigned NOT NULL DEFAULT '0',
  `pk_value` int(11) unsigned NOT NULL DEFAULT '0',
  `military_strategy` int(11) unsigned NOT NULL DEFAULT '0',
  `map_id` int(11) unsigned NOT NULL DEFAULT '0',
  `x_pos` int(11) unsigned NOT NULL DEFAULT '0',
  `y_pos` int(11) unsigned NOT NULL DEFAULT '0',
  `siege_reputation` int(11) unsigned NOT NULL DEFAULT '0',
  `cross_svr_war_reputation` int(11) unsigned NOT NULL DEFAULT '0',
  `max_hp` int(11) unsigned NOT NULL DEFAULT '0',
  `cur_hp` int(11) unsigned NOT NULL DEFAULT '0',
  `absorb_hp` int(11) unsigned NOT NULL DEFAULT '0',
  `max_mp` int(11) unsigned NOT NULL DEFAULT '0',
  `cur_mp` int(11) unsigned NOT NULL DEFAULT '0',
  `absorb_mp` int(11) unsigned NOT NULL DEFAULT '0',
  `attack_min` int(11) unsigned NOT NULL DEFAULT '0',
  `attack_max` int(11) unsigned NOT NULL DEFAULT '0',
  `defense_min` int(11) unsigned NOT NULL DEFAULT '0',
  `defense_max` int(11) unsigned NOT NULL DEFAULT '0',
  `hit_point` int(11) unsigned NOT NULL DEFAULT '0',
  `dodge` int(11) unsigned NOT NULL DEFAULT '0',
  `stability` int(11) unsigned NOT NULL DEFAULT '0',
  `recover` int(11) unsigned NOT NULL DEFAULT '0',
  `attack_speed` int(11) unsigned NOT NULL DEFAULT '0',
  `frozen_addition_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `poisoning_addition_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `palsy_addition_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `dizziness_addition_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `frozen_resistance_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `poisoning_resistance_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `palsy_resistance_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `dizziness_resistance_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `output_hurt_promote_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `suffer_hurt_reduce_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `ignore_defense_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `rebound_hurt_ratio` int(11) unsigned NOT NULL DEFAULT '0',
  `exp_coefficient` int(11) unsigned NOT NULL DEFAULT '0',
  `move_speed` int(11) unsigned NOT NULL DEFAULT '0',
  `armor_id` int(11) unsigned NOT NULL DEFAULT '0',
  `weapon_id` int(11) unsigned NOT NULL DEFAULT '0',
  `mounts_id` int(11) unsigned NOT NULL DEFAULT '0',
  `backpack_size` int(11) unsigned NOT NULL DEFAULT '0',
  `warehouse_size` int(11) unsigned NOT NULL DEFAULT '0',
  `physical_power` int(11) unsigned NOT NULL DEFAULT '0',
  `fighting_power` int(11) unsigned NOT NULL DEFAULT '0',
  `anger` int(11) unsigned NOT NULL DEFAULT '0',
  `create_time` int(11) unsigned NOT NULL DEFAULT '0',
  `online_time` int(11) unsigned NOT NULL DEFAULT '0',
  `offline_time` int(11) unsigned NOT NULL DEFAULT '0',
  `update_time` int(11) unsigned NOT NULL DEFAULT '0',
  `history_online` int(11) unsigned NOT NULL,
  `melting` int(11) unsigned NOT NULL DEFAULT '0',
  `potential_share_max` int(11) unsigned NOT NULL DEFAULT '0',
  `potential_accumulation` int(11) unsigned NOT NULL DEFAULT '0',
  `skill_point` int(11) unsigned NOT NULL DEFAULT '0',
  `popularity` int(11) unsigned NOT NULL DEFAULT '0',
  `skill_point_time` int(11) unsigned NOT NULL DEFAULT '0',
  `vip_level` int(11) unsigned NOT NULL DEFAULT '0',
  `team_id` int(11) unsigned NOT NULL DEFAULT '0',
  `camp_id` int(11) unsigned NOT NULL DEFAULT '0',
  `strategy` int(11) unsigned NOT NULL DEFAULT '0',
  `store_weapon` int(11) unsigned NOT NULL DEFAULT '0',
  `bow_id` int(11) unsigned NOT NULL DEFAULT '0',
  `bow_index` int(11) unsigned NOT NULL DEFAULT '0',
  `cavalry_index` int(11) unsigned NOT NULL DEFAULT '0',
  `bow_speed` int(11) unsigned NOT NULL DEFAULT '0',
  `equip_power` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_player_role
-- ----------------------------

-- ----------------------------
-- Table structure for tb_promotions
-- ----------------------------
DROP TABLE IF EXISTS `tb_promotions`;
CREATE TABLE `tb_promotions` (
  `id` int(11) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `type` int(11) unsigned NOT NULL DEFAULT '0',
  `start_time` int(11) unsigned NOT NULL DEFAULT '0',
  `duration_time` int(11) unsigned NOT NULL DEFAULT '0',
  `end_time` int(11) unsigned NOT NULL DEFAULT '0',
  `loop_time` int(11) unsigned NOT NULL DEFAULT '0',
  `state` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_promotions
-- ----------------------------

-- ----------------------------
-- Table structure for tb_raid_center
-- ----------------------------
DROP TABLE IF EXISTS `tb_raid_center`;
CREATE TABLE `tb_raid_center` (
  `raid_id` int(11) unsigned NOT NULL COMMENT 'Ã¥â€°Â¯Ã¦Å“Â¬Id',
  `clearance_sec` int(11) unsigned NOT NULL COMMENT 'Ã¦Å“â‚¬Ã¥Â¿Â«Ã¦Â¸â€¦Ã©â„¢Â¤Ã¦â€”Â¶Ã©â€”Â´',
  `clearance_role_id` int(11) unsigned NOT NULL COMMENT 'Ã¦Å“â‚¬Ã¥Â¿Â«Ã¦Â¸â€¦Ã©â„¢Â¤Ã¨Â§â€™Ã¨â€°Â²',
  PRIMARY KEY (`raid_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_raid_center
-- ----------------------------

-- ----------------------------
-- Table structure for tb_rank
-- ----------------------------
DROP TABLE IF EXISTS `tb_rank`;
CREATE TABLE `tb_rank` (
  `rank_type` int(11) NOT NULL,
  `update_time` int(11) NOT NULL,
  `rank_list` blob NOT NULL,
  PRIMARY KEY (`rank_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_rank
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_auto_raid
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_auto_raid`;
CREATE TABLE `tb_role_auto_raid` (
  `role_id` int(11) unsigned NOT NULL,
  `info` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_role_auto_raid
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_black_shop
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_black_shop`;
CREATE TABLE `tb_role_black_shop` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `black_shops` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_role_black_shop
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_bows
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_bows`;
CREATE TABLE `tb_role_bows` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `bow_id` int(11) unsigned NOT NULL DEFAULT '0',
  `advance_num` int(11) unsigned NOT NULL DEFAULT '0',
  `advance_time` int(11) unsigned NOT NULL DEFAULT '0',
  `advance_bless` int(11) unsigned NOT NULL DEFAULT '0',
  `bow_level` int(11) unsigned NOT NULL DEFAULT '0',
  `bow_exp` int(11) unsigned NOT NULL DEFAULT '0',
  `bow_power` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_bows
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_buffer
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_buffer`;
CREATE TABLE `tb_role_buffer` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `buffers` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_buffer
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_cd_cooldown
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_cd_cooldown`;
CREATE TABLE `tb_role_cd_cooldown` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `cds` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_cd_cooldown
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_center_set
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_center_set`;
CREATE TABLE `tb_role_center_set` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `team_auto_agree` int(11) unsigned NOT NULL DEFAULT '1',
  `team_auto_join` int(11) unsigned NOT NULL DEFAULT '1',
  `team_min_lvl` int(11) unsigned NOT NULL DEFAULT '30',
  `team_max_lvl` int(11) unsigned NOT NULL DEFAULT '100',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_center_set
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_cycle
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_cycle`;
CREATE TABLE `tb_role_cycle` (
  `role_id` int(10) unsigned NOT NULL,
  `reset_time` int(10) unsigned NOT NULL,
  `cur_level` int(10) unsigned NOT NULL,
  `top_level` int(10) unsigned NOT NULL,
  `passed_levels` blob NOT NULL COMMENT '{level:{"time":time, "tick":tick, "alone":alone, "freeze":freeze}}',
  `helpers` blob NOT NULL COMMENT '{role_id:time}',
  PRIMARY KEY (`role_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_role_cycle
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_devil
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_devil`;
CREATE TABLE `tb_role_devil` (
  `role_id` int(10) unsigned NOT NULL,
  `stage_info` blob NOT NULL,
  `raid_info` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_devil
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_explore
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_explore`;
CREATE TABLE `tb_role_explore` (
  `role_id` int(11) unsigned NOT NULL COMMENT 'Ã§Å½Â©Ã¥Â®Â¶Id',
  `num` int(11) unsigned NOT NULL COMMENT 'Ã¦Å½Â¢Ã©â„¢Â©Ã§Å¡â€žÃ¦Â¬Â¡Ã¦â€¢Â°',
  `money` int(11) unsigned NOT NULL COMMENT 'Ã¦Å½Â¢Ã©â„¢Â©Ã§â€Â¨Ã§Å¡â€žÃ©â€™Â±',
  `success_ratio` int(11) unsigned NOT NULL COMMENT 'Ã¦Ë†ÂÃ¥Å Å¸Ã§Å½â€¡',
  `failure_money` int(11) unsigned NOT NULL COMMENT 'Ã¥Â¤Â±Ã¨Â´Â¥Ã§â€Â¨Ã§Å¡â€žÃ©â€™Â±',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_explore
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_explore_detail
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_explore_detail`;
CREATE TABLE `tb_role_explore_detail` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `role_id` int(11) unsigned NOT NULL,
  `time` int(11) unsigned NOT NULL,
  `place_id` int(11) unsigned NOT NULL,
  `goods_id` int(11) unsigned NOT NULL,
  `goods_num` int(11) unsigned NOT NULL,
  `goods_quality` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`,`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_explore_detail
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_ext_attr
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_ext_attr`;
CREATE TABLE `tb_role_ext_attr` (
  `role_id` int(11) unsigned NOT NULL,
  `src_type` int(11) unsigned NOT NULL DEFAULT '0',
  `attr` blob NOT NULL,
  PRIMARY KEY (`role_id`,`src_type`),
  KEY `role_id` (`role_id`) USING BTREE,
  KEY `src_type` (`src_type`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_ext_attr
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_farm
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_farm`;
CREATE TABLE `tb_role_farm` (
  `role_id` int(11) unsigned NOT NULL,
  `farm` blob NOT NULL,
  PRIMARY KEY (`role_id`),
  KEY `ID` (`role_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_farm
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_friends
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_friends`;
CREATE TABLE `tb_role_friends` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `friends` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_friends
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_gem
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_gem`;
CREATE TABLE `tb_role_gem` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `gems` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_gem
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_label
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_label`;
CREATE TABLE `tb_role_label` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Ã¨Â§â€™Ã¨â€°Â²Id',
  `label_code` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Ã§Â§Â°Ã¥ÂÂ·Id',
  `use_state` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Ã¤Â½Â¿Ã§â€Â¨Ã§Å Â¶Ã¦â‚¬Â',
  `effective_state` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Ã¦â€¢Ë†Ã¦Å¾Å“Ã§Å Â¶Ã¦â‚¬Â',
  `end_date` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Ã¨Â¶â€¦Ã¦â€”Â¶Ã¦â€”Â¶Ã©â€”Â´',
  PRIMARY KEY (`role_id`,`label_code`),
  KEY `role_id` (`role_id`) USING BTREE,
  KEY `label_code` (`label_code`) USING BTREE,
  KEY `use_state` (`use_state`) USING BTREE,
  KEY `end_date` (`end_date`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_label
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_levels
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_levels`;
CREATE TABLE `tb_role_levels` (
  `role_id` int(10) unsigned NOT NULL,
  `time` int(10) unsigned NOT NULL,
  `new_level` int(10) unsigned NOT NULL,
  `old_level` int(10) unsigned NOT NULL,
  `new_power` int(10) unsigned NOT NULL,
  `old_power` int(10) unsigned NOT NULL,
  `cur_level` int(10) NOT NULL,
  `levels` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_role_levels
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_lottery
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_lottery`;
CREATE TABLE `tb_role_lottery` (
  `role_id` int(11) unsigned NOT NULL,
  `num` int(11) unsigned NOT NULL,
  `gold_time` int(11) unsigned NOT NULL,
  `bronze_time` int(11) unsigned NOT NULL,
  `props_list` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_role_lottery
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_mail
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_mail`;
CREATE TABLE `tb_role_mail` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `mail` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_mail
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_mounts
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_mounts`;
CREATE TABLE `tb_role_mounts` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `mount_id` int(11) unsigned NOT NULL DEFAULT '0',
  `advance_num` int(11) unsigned NOT NULL DEFAULT '0',
  `advance_time` int(11) unsigned NOT NULL DEFAULT '0',
  `advance_bless` int(11) unsigned NOT NULL DEFAULT '0',
  `skill_upgrade_num` int(11) unsigned NOT NULL DEFAULT '0',
  `mount_level` int(11) unsigned NOT NULL DEFAULT '0',
  `mount_exp` int(11) unsigned NOT NULL DEFAULT '0',
  `mount_power` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_mounts
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_order
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_order`;
CREATE TABLE `tb_role_order` (
  `role_id` int(11) unsigned NOT NULL,
  `store_id` int(11) unsigned NOT NULL,
  `count` int(11) unsigned NOT NULL,
  `type` int(11) unsigned NOT NULL,
  `time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`role_id`,`store_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_order
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_pet
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_pet`;
CREATE TABLE `tb_role_pet` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `pets` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_pet
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_potential
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_potential`;
CREATE TABLE `tb_role_potential` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `potentials` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_potential
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_props
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_props`;
CREATE TABLE `tb_role_props` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `pack` blob NOT NULL,
  `depot` blob NOT NULL,
  `equip` blob NOT NULL,
  `explore` blob NOT NULL,
  `task` blob NOT NULL,
  `created_date` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_props
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_raid
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_raid`;
CREATE TABLE `tb_role_raid` (
  `role_id` int(11) unsigned NOT NULL,
  `raid_id` int(11) unsigned NOT NULL,
  `star_num` int(11) unsigned NOT NULL DEFAULT '0',
  `fast_tick` int(11) unsigned NOT NULL DEFAULT '0',
  `last_complete_time` int(11) unsigned NOT NULL DEFAULT '0',
  `complete_count` int(11) unsigned NOT NULL DEFAULT '0',
  `create_time` int(11) unsigned NOT NULL DEFAULT '0',
  `auto_time` int(11) unsigned NOT NULL DEFAULT '0',
  `elite_star_num` int(11) unsigned NOT NULL DEFAULT '0',
  `elite_fast_tick` int(11) unsigned NOT NULL DEFAULT '0',
  `elite_last_complete_time` int(11) unsigned NOT NULL DEFAULT '0',
  `elite_complete_count` int(11) unsigned NOT NULL DEFAULT '0',
  `elite_create_time` int(11) unsigned NOT NULL DEFAULT '0',
  `elite_auto_time` int(11) unsigned NOT NULL DEFAULT '0',
  `elite_enter_count` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`,`raid_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_raid
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_recharge
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_recharge`;
CREATE TABLE `tb_role_recharge` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `openid` varchar(32) NOT NULL DEFAULT '',
  `openkey` varchar(64) NOT NULL,
  `pf` varchar(32) NOT NULL DEFAULT '',
  `ts` int(11) NOT NULL,
  `payitem` varchar(128) NOT NULL DEFAULT '',
  `token` varchar(64) NOT NULL DEFAULT '',
  `billno` varchar(64) NOT NULL DEFAULT '',
  `zoneid` int(11) NOT NULL,
  `providetype` int(11) NOT NULL,
  `amt` int(11) NOT NULL,
  `payamt_coins` int(11) NOT NULL,
  `pubacct_payamt_coins` int(11) NOT NULL,
  `state` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_recharge
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_set
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_set`;
CREATE TABLE `tb_role_set` (
  `role_id` int(11) unsigned NOT NULL,
  `shortcut` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_set
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_skill
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_skill`;
CREATE TABLE `tb_role_skill` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `skill_id` int(11) unsigned NOT NULL DEFAULT '0',
  `skill_level` int(11) unsigned NOT NULL DEFAULT '0',
  `skill_grid_index` int(11) unsigned NOT NULL DEFAULT '0',
  `skillful` int(11) NOT NULL,
  `studied` int(11) unsigned NOT NULL DEFAULT '0',
  `end_cd_time` int(11) unsigned NOT NULL DEFAULT '0',
  `created_date` int(11) unsigned NOT NULL DEFAULT '0',
  `status` int(11) unsigned NOT NULL DEFAULT '0',
  `effect_id` int(11) unsigned NOT NULL DEFAULT '0',
  `smy_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`role_id`,`smy_id`),
  KEY `role_id` (`role_id`) USING BTREE,
  KEY `smy_id` (`smy_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_skill
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_statis
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_statis`;
CREATE TABLE `tb_role_statis` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `statis_type` int(11) unsigned NOT NULL DEFAULT '0',
  `update_type` int(11) unsigned NOT NULL,
  `val` int(11) unsigned NOT NULL DEFAULT '0',
  `last_time` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`,`statis_type`),
  KEY `role_id` (`role_id`) USING BTREE,
  KEY `statis_type` (`statis_type`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_statis
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_status
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_status`;
CREATE TABLE `tb_role_status` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `pos_map_id` int(11) unsigned NOT NULL DEFAULT '0',
  `pos_scene_id` int(11) unsigned NOT NULL DEFAULT '0',
  `pos_x` int(11) unsigned NOT NULL DEFAULT '0',
  `pos_y` int(11) unsigned NOT NULL DEFAULT '0',
  `pos_z` int(11) unsigned NOT NULL DEFAULT '0',
  `tmp_map_id` int(11) unsigned NOT NULL DEFAULT '0',
  `tmp_scene_id` int(11) unsigned NOT NULL DEFAULT '0',
  `tmp_x` int(11) unsigned NOT NULL DEFAULT '0',
  `tmp_y` int(11) unsigned NOT NULL DEFAULT '0',
  `tmp_z` int(11) unsigned NOT NULL DEFAULT '0',
  `mditation` int(11) unsigned NOT NULL DEFAULT '0',
  `riding` int(11) unsigned NOT NULL DEFAULT '0',
  `findpath` int(11) unsigned NOT NULL DEFAULT '0',
  `fight` int(11) unsigned NOT NULL DEFAULT '0',
  `auto_play` int(11) unsigned NOT NULL DEFAULT '0',
  `pvp_mode` int(11) unsigned NOT NULL DEFAULT '0',
  `attk_mode` int(11) unsigned NOT NULL DEFAULT '0',
  `alive` int(11) unsigned NOT NULL DEFAULT '1',
  `online` int(11) unsigned NOT NULL DEFAULT '0',
  `red_goods_num` int(11) unsigned NOT NULL DEFAULT '0',
  `blue_goods_num` int(11) unsigned NOT NULL DEFAULT '0',
  `complete_task_num` int(11) unsigned NOT NULL DEFAULT '0',
  `abandon_task_num` int(11) unsigned NOT NULL DEFAULT '0',
  `receve_task_num` int(11) unsigned NOT NULL DEFAULT '0',
  `is_package_full` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_furious` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_hurted` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_week` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_giddy` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_faint` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_fired` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_posion` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_blood` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_injury` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_cursed` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_blind` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_maimed` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_silent` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_disarm` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_slowly` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_dec_harm` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_rej_harm` int(11) unsigned NOT NULL DEFAULT '0',
  `buf_turn_body` int(11) unsigned NOT NULL DEFAULT '0',
  `strength` int(11) unsigned NOT NULL DEFAULT '0',
  `status` int(11) unsigned NOT NULL DEFAULT '0',
  `created_date` int(11) unsigned NOT NULL DEFAULT '0',
  `tmp_scene_inst_id` int(11) unsigned NOT NULL DEFAULT '0',
  `tmp_scene_inst_time` int(11) unsigned NOT NULL DEFAULT '0',
  `spa_action_times` int(11) unsigned NOT NULL DEFAULT '0',
  `spa_last_time` int(11) unsigned NOT NULL DEFAULT '0',
  `arena_last_chall_time` int(11) unsigned NOT NULL DEFAULT '0',
  `arena_has_times` int(11) unsigned NOT NULL DEFAULT '0',
  `arena_buy_times` int(11) unsigned NOT NULL DEFAULT '0',
  `test_rewards` int(11) unsigned NOT NULL DEFAULT '0',
  `unlimited_fly` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`),
  UNIQUE KEY `role_id` (`role_id`) USING BTREE,
  KEY `online` (`online`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_status
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_target
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_target`;
CREATE TABLE `tb_role_target` (
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  `list` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_role_target
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_task
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_task`;
CREATE TABLE `tb_role_task` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `acpt_task` blob NOT NULL,
  `comp_task` blob NOT NULL,
  `fresh_task` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_task
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_task_chain
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_task_chain`;
CREATE TABLE `tb_role_task_chain` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `chain_id` int(11) unsigned NOT NULL DEFAULT '0',
  `succ_times` int(11) unsigned NOT NULL DEFAULT '0',
  `submit_index` int(11) unsigned NOT NULL DEFAULT '0',
  `created_date` int(11) unsigned NOT NULL DEFAULT '0',
  `cnt` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`,`chain_id`),
  KEY `role_id` (`role_id`) USING BTREE,
  KEY `chain_id` (`chain_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_task_chain
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_treasure
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_treasure`;
CREATE TABLE `tb_role_treasure` (
  `role_id` int(11) unsigned NOT NULL DEFAULT '0',
  `treasures` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_role_treasure
-- ----------------------------

-- ----------------------------
-- Table structure for tb_role_vip
-- ----------------------------
DROP TABLE IF EXISTS `tb_role_vip`;
CREATE TABLE `tb_role_vip` (
  `role_id` int(11) unsigned NOT NULL,
  `vip_lv` int(11) NOT NULL,
  `vip_exp` int(11) unsigned NOT NULL DEFAULT '0',
  `month_timeout` int(11) unsigned NOT NULL DEFAULT '0',
  `list` blob NOT NULL,
  `buy_record` blob NOT NULL,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_role_vip
-- ----------------------------

-- ----------------------------
-- Table structure for tb_version
-- ----------------------------
DROP TABLE IF EXISTS `tb_version`;
CREATE TABLE `tb_version` (
  `id` int(11) unsigned NOT NULL,
  `database` varchar(256) NOT NULL DEFAULT '',
  `server` varchar(256) NOT NULL DEFAULT '',
  `client` varchar(256) NOT NULL DEFAULT '',
  `time` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of tb_version
-- ----------------------------
INSERT INTO `tb_version` VALUES ('1', '0.5', '0.1', '0.1', '2015-09-24 16:52:44');

-- ----------------------------
-- Procedure structure for sp_find_role
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_find_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_find_role`(`id` int)
BEGIN
	#Routine body goes here...
	DECLARE _o_id VARCHAR(32) DEFAULT "";
	DECLARE _p_id int         DEFAULT 0;
	DECLARE _name VARCHAR(64) DEFAULT "";

	SELECT role_name, player_id from tb_player_role WHERE role_id = id into _name, _p_id;
	SELECT openid from tb_player WHERE tb_player.id=_p_id into _o_id;
	SELECT _o_id as 'å¸å·', _name as 'åå­—', _p_id as 'player_id', id as 'id';

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_find_role_by_name
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_find_role_by_name`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_find_role_by_name`(`name` varchar(64))
BEGIN
	#Routine body goes here...
	DECLARE _o_id VARCHAR(32) DEFAULT "";
	DECLARE _p_id int         DEFAULT 0;
	DECLARE _id   int         DEFAULT 0;

	SELECT role_id, player_id from tb_player_role WHERE role_name = name into _id, _p_id;
	SELECT openid from tb_player WHERE tb_player.id=_p_id into _o_id;
	SELECT _o_id as 'å¸å·', name as 'åå­—', _p_id as 'player_id', _id as 'id';

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_get_gdata
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_gdata`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_get_gdata`()
BEGIN
	DECLARE max_lvl     	INT(11) DEFAULT 1;
	DECLARE spare_num 		INT(11) DEFAULT 0;
	DECLARE svr_open_time   INT(11) DEFAULT 0;
	DECLARE joint_active_flag   INT(11) DEFAULT 0;
	DECLARE svr_merge_time  INT(11) DEFAULT 0;
	DECLARE activity_flag INT(11) DEFAULT 0;
	DECLARE first_pay_flag INT(11) DEFAULT 0;

	SELECT COUNT(*) FROM tb_player_role_spare WHERE STATUS = 1  LIMIT 1  INTO spare_num;
    IF spare_num > 0
    THEN
        SELECT MAX(LEVEL)  FROM tb_player_role_spare  WHERE STATUS = 1 LIMIT 1  INTO max_lvl;	 
		IF max_lvl = NULL
		THEN
		    SET  max_lvl = 1;
       	END IF;
    END IF;
	
	SELECT UNIX_TIMESTAMP(DATE(active_date)) FROM tb_system WHERE STATUS =1 LIMIT 1 INTO svr_open_time;
	if svr_open_time = NULL
	THEN 
	    SET svr_open_time = 0;
	END IF;
	
	SELECT joint_active_flag, UNIX_TIMESTAMP(DATE(joint_suit_date)) FROM tb_system WHERE STATUS =1 LIMIT 1 INTO joint_active_flag, svr_merge_time;
    IF ROW_COUNT() = 0
    THEN
        SET joint_active_flag = 0;
    END IF;

	SELECT activity_flag FROM tb_system_activity WHERE STATUS =1 LIMIT 1 INTO activity_flag;
    IF ROW_COUNT() = 0
    THEN
        SET activity_flag = 0;
    END IF;

    SELECT first_pay_flag FROM tb_system WHERE STATUS =1 LIMIT 1 INTO first_pay_flag;
    IF ROW_COUNT() = 0
    THEN
        SET first_pay_flag = 0;
    END IF;
    
	
	SELECT max_lvl,spare_num,svr_open_time,joint_active_flag,svr_merge_time,activity_flag,first_pay_flag;
	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_get_gdata_max_role
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_gdata_max_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_get_gdata_max_role`()
BEGIN
	DECLARE count_role INT(11) DEFAULT 0;
	
	SELECT count(*) FROM tb_player_role INTO count_role;
	IF count_role = NULL
	THEN
	    SET count_role = 0;
	END IF;
	
	IF count_role > 0 
	THEN
	    SELECT system_id as svr_id, MAX(role_id) as max_role_id FROM tb_player_role group by system_id;
	ELSE
		SELECT 0 as svr_id, 0 as max_role_id; 
    END IF;
	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_get_player_role
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_player_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_get_player_role`(IN id INT(11))
BEGIN
	select role_id, system_id, player_id from tb_player_role where player_id = id;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_load_all_user_smy
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_all_user_smy`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_load_all_user_smy`()
select tb_player_role.role_id,tb_player_role.role_name,tb_player_role.status,tb_player_role.profession,tb_player_role.fighting_power,tb_player_role.level, tb_player_role.weapon_id,tb_player_role.armor_id ,tb_role_mounts.mount_level,tb_role_mounts.mount_power, tb_player_role.vip_level, tb_player_role.offline_time, IFNULL(tb_role_mounts.mount_level, 0),IFNULL(tb_role_mounts.mount_power, 0) from tb_player_role left join tb_role_mounts on tb_player_role.role_id = tb_role_mounts.role_id
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_load_offline_user
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_offline_user`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_load_offline_user`()
BEGIN
    SELECT role_id,player_id FROM tb_player_role where status=1 order by update_time desc limit 1000;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_login_auth
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_auth`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_login_auth`(IN  p_openid VARCHAR(64) charset utf8, IN p_yel_vip INT(11), IN p_server_id INT(11), IN p_agent_id INT(11),  IN p_client_id INT(11), IN platform_id INT(11))
BEGIN
	DECLARE v_user_count  INT(11) DEFAULT 0;
	DECLARE v_user_id     INT(11) DEFAULT 0;
	DECLARE v_role_count  INT(11) DEFAULT 0;
	DECLARE v_role_id     INT(11) DEFAULT 0;
	DECLARE v_prof        INT(11) DEFAULT 0;
	DECLARE v_sex         INT(11) DEFAULT 0;
	DECLARE v_max_id      INT(11) DEFAULT 0;
	DECLARE v_now         INT(11) DEFAULT 0;
	DECLARE v_name        VARCHAR(64) DEFAULT '';
	DECLARE v_max_sin_id  INT(11) DEFAULT 0;
  DECLARE v_level       INT(11) DEFAULT 0;
  DECLARE v_fighting_power INT(11) DEFAULT 0;

	START TRANSACTION;
	
	select count(*) from tb_player where openid=p_openid and svr_id=p_server_id into v_user_count;
	set v_now = UNIX_TIMESTAMP(now());
	
	if v_user_count = 0
	then
	    set v_max_sin_id = 1;
		select max(id) from tb_player where svr_id=p_server_id into v_max_id;
		if v_max_id > 0
		then
		    set v_max_sin_id = (v_max_id&1048575)+1;
		end if;
		set v_max_id=((p_server_id<<20)|v_max_sin_id);
		insert into tb_player values(v_max_id,p_server_id,p_openid, v_now, v_now, platform_id);
		select id from tb_player where openid=p_openid into v_user_id;
		
	end if;
	
	if v_user_count = 1
	then
	    update tb_player set last_login_time=v_now where openid=p_openid and svr_id=p_server_id;
	    select id from tb_player where openid=p_openid and svr_id=p_server_id into v_user_id;
		select count(*) FROM tb_player_role where player_id=v_user_id and system_id=p_server_id into v_role_count;
		if v_role_count > 0
		then
			select role_id,profession,sex,role_name,level,fighting_power FROM tb_player_role where player_id=v_user_id and system_id=p_server_id limit 1 into v_role_id,v_prof,v_sex,v_name,v_level,v_fighting_power;
		end if;
	end if;
	
	COMMIT;
	
	select v_user_id,v_role_id,v_prof,v_sex,v_name,p_openid,p_yel_vip,p_server_id,p_agent_id,p_client_id,v_level,v_fighting_power;
	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_login_auths
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_auths`;
DELIMITER ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `sp_login_auths`(IN  p_openid VARCHAR(64) charset utf8, IN p_yel_vip INT(11), IN p_server_id INT(11), IN p_agent_id INT(11),  IN p_client_id INT(11), IN platform_id INT(11))
BEGIN
	DECLARE v_user_count  INT(11) DEFAULT 0;
	DECLARE v_user_id     INT(11) DEFAULT 0;
	DECLARE v_role_count  INT(11) DEFAULT 0;
	DECLARE v_role_id     INT(11) DEFAULT 0;
	DECLARE v_prof        INT(11) DEFAULT 0;
	DECLARE v_sex         INT(11) DEFAULT 0;
	DECLARE v_max_id      INT(11) DEFAULT 0;
	DECLARE v_now         INT(11) DEFAULT 0;
	DECLARE v_name        VARCHAR(64) DEFAULT '';
	DECLARE v_max_sin_id  INT(11) DEFAULT 0;
  DECLARE v_level       INT(11) DEFAULT 0;
  DECLARE v_fighting_power INT(11) DEFAULT 0;

	START TRANSACTION;
	
	select count(*) from tb_player where openid=p_openid and svr_id=p_server_id into v_user_count;
	set v_now = UNIX_TIMESTAMP(now());
	
	if v_user_count = 0
	then
	    set v_max_sin_id = 1;
		select max(id) from tb_player where svr_id=p_server_id into v_max_id;
		if v_max_id > 0
		then
		    set v_max_sin_id = (v_max_id&1048575) + 1;
		end if;
		set v_max_id = ((p_server_id << 20) | v_max_sin_id);
		insert into tb_player values(v_max_id, p_server_id, p_openid, v_now, v_now, platform_id, '', '', '', '', '', '', '', '', '', '', '', '', 0, 0);
		select id from tb_player where openid=p_openid and svr_id=p_server_id into v_user_id;
	end if;
	
	if v_user_count = 1
	then
	    update tb_player set last_login_time=v_now where openid=p_openid and svr_id=p_server_id;
	    select id from tb_player where openid=p_openid and svr_id=p_server_id into v_user_id;
			select count(*) FROM tb_player_role where player_id=v_user_id and system_id=p_server_id into v_role_count;
	end if;
	
	COMMIT;

	IF v_user_id = 0
	THEN
		select p_openid, p_yel_vip, p_server_id, CAST(p_agent_id AS CHAR), p_client_id, v_user_id;
	ELSE
		select role_id,profession,sex,role_name,level,fighting_power FROM tb_player_role where player_id=v_user_id and system_id=p_server_id union select p_openid, p_yel_vip, p_server_id, CAST(p_agent_id AS CHAR), p_client_id, v_user_id;
	END IF;

END
;;
DELIMITER ;
