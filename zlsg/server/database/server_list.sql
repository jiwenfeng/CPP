/*
Navicat MySQL Data Transfer

Source Server         : 217
Source Server Version : 50173
Source Host           : 192.168.23.217:3306
Source Database       : server_list

Target Server Type    : MYSQL
Target Server Version : 50173
File Encoding         : 65001

Date: 2015-09-16 15:10:07
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for server_list
-- ----------------------------
DROP TABLE IF EXISTS `server_list`;
CREATE TABLE `server_list` (
  `id` int(11) unsigned NOT NULL,
  `name` varchar(128) NOT NULL,
  `ip` varchar(32) NOT NULL,
  `port` int(11) unsigned NOT NULL,
  `inner_port` int(11) unsigned NOT NULL,
  `status` varchar(128) NOT NULL,
  `path` varchar(255) DEFAULT NULL,
  `revolving_door` varchar(1024) DEFAULT NULL,
  `qq` varchar(32) DEFAULT NULL,
  `mail` varchar(255) DEFAULT NULL,
  `bulletin_board` varchar(2048) DEFAULT NULL,
  `t` varchar(128) NOT NULL DEFAULT '0',
  `web` varchar(255) DEFAULT NULL,
  `open` tinyint(3) unsigned DEFAULT NULL,
  `start_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`,`t`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of server_list
-- ----------------------------
INSERT INTO `server_list` VALUES ('0', '1388服-内网开发', '192.168.23.217', '1388', '1389', '1_1', '/home/zlsg', '', '', '', '', 'test', 'http://192.168.23.217:9800', null, '2015-09-15 10:00:00');
INSERT INTO `server_list` VALUES ('1', '001服-腾讯测试', '119.29.138.236', '1288', '1289', '1_1', '/data/www/lhsg_s1', '8', '6', '2', '《龙皇三国》夏季火热突袭中，邀请伙伴开始三国征战！', 'paojiao', 's1.lhsg.uwan.com', null, '2015-09-08 00:00:00');
INSERT INTO `server_list` VALUES ('1', '001服-腾讯测试', '119.29.138.236', '1288', '1289', '1_1', '/data/www/lhsg_s1', '8', '6', '2', '《龙皇三国》夏季火热突袭中，邀请伙伴开始三国征战！', 'test', 's1.lhsg.uwan.com', null, null);
INSERT INTO `server_list` VALUES ('2', '002服-评测服', '203.195.129.156', '1288', '1289', '1_0', null, null, null, null, 'ss', 'test', null, null, null);
INSERT INTO `server_list` VALUES ('3', '003服-泡椒', '119.29.11.117', '1288', '1289', '1_1', '/data/www/lhsg_ly_s1', '办证 537080702', '537080702', '537080702@qq.com', '耍开心', 'paojiao', 's1.lhsg.ly.uwan.com', null, '2015-09-08 00:00:15');
INSERT INTO `server_list` VALUES ('3', '1288服-内网开发', '192.168.23.217', '1288', '1289', '0_0', '/home/rockpile/server/bin', '4', '4', '4', '', 'test', 'http://192.168.23.217:9800', null, '2015-09-08 00:00:02');
INSERT INTO `server_list` VALUES ('4', '1688服-内网开发', '192.168.22.132', '1288', '1289', '3_0', null, '3', '5', '3', '123', 'test', null, null, null);
INSERT INTO `server_list` VALUES ('11', '1488服-内网开发', '192.168.23.187', '1488', '1489', '0_1', null, '5', '3', '5', null, 'test', null, null, null);
INSERT INTO `server_list` VALUES ('12', '1588服-内网开发', '192.168.23.217', '1588', '1589', '0_0', null, '6', '2', '6', '恭喜你中奖了 请到经理办公室领取奖品http://192.168.23.217:9800/backdoor/index.php?/bb', 'test', 'http://192.168.23.217:9800', null, null);

-- ----------------------------
-- Table structure for t_group
-- ----------------------------
DROP TABLE IF EXISTS `t_group`;
CREATE TABLE `t_group` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_group
-- ----------------------------
INSERT INTO `t_group` VALUES ('1', 'admin');

-- ----------------------------
-- Table structure for t_group_web
-- ----------------------------
DROP TABLE IF EXISTS `t_group_web`;
CREATE TABLE `t_group_web` (
  `id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  `backdoor` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_group_web
-- ----------------------------

-- ----------------------------
-- Table structure for t_user
-- ----------------------------
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `psw` varchar(255) NOT NULL,
  `group` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_user
-- ----------------------------
INSERT INTO `t_user` VALUES ('2', 'admin', '21232f297a57a5a743894a0e4a801fc3', '0');
INSERT INTO `t_user` VALUES ('3', 'test', 'e10adc3949ba59abbe56e057f20f883e', '1');

-- ----------------------------
-- Table structure for t_white_list
-- ----------------------------
DROP TABLE IF EXISTS `t_white_list`;
CREATE TABLE `t_white_list` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '自增索引',
  `open_id` varchar(128) NOT NULL COMMENT 'open_id',
  `group` tinyint(3) unsigned DEFAULT '0' COMMENT '权限',
  `remark` varchar(255) DEFAULT NULL COMMENT '备注',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_white_list
-- ----------------------------
INSERT INTO `t_white_list` VALUES ('1', '1', '1', '测试一下');
INSERT INTO `t_white_list` VALUES ('2', '2', '2', '2');
INSERT INTO `t_white_list` VALUES ('3', '3', '3', '测试三下');
INSERT INTO `t_white_list` VALUES ('6', 'rockpile', '1', '开发测试 ');
