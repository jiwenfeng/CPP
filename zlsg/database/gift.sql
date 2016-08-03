/*
Navicat MySQL Data Transfer

Source Server         : 217
Source Server Version : 50173
Source Host           : 192.168.23.217:3306
Source Database       : gift

Target Server Type    : MYSQL
Target Server Version : 50173
File Encoding         : 65001

Date: 2015-09-16 23:12:31
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_gift
-- ----------------------------
DROP TABLE IF EXISTS `t_gift`;
CREATE TABLE `t_gift` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `type` int(11) unsigned NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `num` int(10) unsigned NOT NULL,
  `context` varchar(1024) NOT NULL DEFAULT '',
  `items` varchar(1024) NOT NULL DEFAULT '',
  `start_time` int(11) unsigned NOT NULL,
  `end_time` int(11) unsigned NOT NULL,
  `day` int(11) unsigned NOT NULL,
  `limit_device` int(11) NOT NULL,
  `limit_account` int(11) NOT NULL,
  `limit_use` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift
-- ----------------------------
INSERT INTO `t_gift` VALUES ('19', '0', '我的邀请码', '10', '我的邀请码', '{\"1001\":\"1\",\"1002\":\"1\"}', '1435680000', '1443628800', '0', '0', '0', '10');

-- ----------------------------
-- Table structure for t_gift_code_0
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_0`;
CREATE TABLE `t_gift_code_0` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_0
-- ----------------------------
INSERT INTO `t_gift_code_0` VALUES ('403', '19', '1-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('404', '19', '34173cb38f07f89ddbebc2ac9128303f-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('405', '19', 'c16a5320fa475530d9583c34fd356ef5-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('406', '19', '6364d3f0f495b6ab9dcf8d3b5c6e0b01-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('407', '19', '182be0c5cdcd5072bb1864cdee4d3d6e-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('408', '19', 'e369853df766fa44e1ed0ff613f563bd-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('409', '19', '1c383cd30b7c298ab50293adfecb7b18-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('410', '19', '19ca14e7ea6328a42e0eb13d585e4c22-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('411', '19', 'a5bfc9e07964f8dddeb95fc584cd965d-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('412', '19', 'a5771bce93e200c36f7cd9dfd0e5deaa-0', '0', '0', '0', '1437979807');
INSERT INTO `t_gift_code_0` VALUES ('413', '19', '9600-0', '0', '0', '960', '1442384622');

-- ----------------------------
-- Table structure for t_gift_code_1
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_1`;
CREATE TABLE `t_gift_code_1` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_1
-- ----------------------------
INSERT INTO `t_gift_code_1` VALUES ('1', '19', '9610-1', '0', '0', '961', '1442407237');

-- ----------------------------
-- Table structure for t_gift_code_2
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_2`;
CREATE TABLE `t_gift_code_2` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_2
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_code_3
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_3`;
CREATE TABLE `t_gift_code_3` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_3
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_code_4
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_4`;
CREATE TABLE `t_gift_code_4` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_4
-- ----------------------------
INSERT INTO `t_gift_code_4` VALUES ('1', '19', '1258291412-4', '0', '12', '12582914', '1442386145');

-- ----------------------------
-- Table structure for t_gift_code_5
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_5`;
CREATE TABLE `t_gift_code_5` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_5
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_code_6
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_6`;
CREATE TABLE `t_gift_code_6` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_6
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_code_7
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_7`;
CREATE TABLE `t_gift_code_7` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_7
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_code_8
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_8`;
CREATE TABLE `t_gift_code_8` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_8
-- ----------------------------
INSERT INTO `t_gift_code_8` VALUES ('1', '19', '10485781-8', '0', '1', '1048578', '1439898454');

-- ----------------------------
-- Table structure for t_gift_code_9
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_code_9`;
CREATE TABLE `t_gift_code_9` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gift_id` int(11) unsigned NOT NULL,
  `code` varchar(255) NOT NULL DEFAULT '',
  `state` tinyint(3) unsigned NOT NULL,
  `server` int(11) unsigned NOT NULL,
  `role_id` int(11) unsigned NOT NULL,
  `use_time` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_code_9
-- ----------------------------
INSERT INTO `t_gift_code_9` VALUES ('1', '19', '11490-9', '0', '0', '1149', '1442372309');

-- ----------------------------
-- Table structure for t_gift_use_0
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_0`;
CREATE TABLE `t_gift_use_0` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_0
-- ----------------------------
INSERT INTO `t_gift_use_0` VALUES ('3', '403', '1', '', '831', '1437983552');

-- ----------------------------
-- Table structure for t_gift_use_1
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_1`;
CREATE TABLE `t_gift_use_1` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_1
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_use_2
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_2`;
CREATE TABLE `t_gift_use_2` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_2
-- ----------------------------
INSERT INTO `t_gift_use_2` VALUES ('12', '1', '0', 'pctest123456', '972', '1438742027');

-- ----------------------------
-- Table structure for t_gift_use_3
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_3`;
CREATE TABLE `t_gift_use_3` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_3
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_use_4
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_4`;
CREATE TABLE `t_gift_use_4` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_4
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_use_5
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_5`;
CREATE TABLE `t_gift_use_5` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_5
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_use_6
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_6`;
CREATE TABLE `t_gift_use_6` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_6
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_use_7
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_7`;
CREATE TABLE `t_gift_use_7` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_7
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_use_8
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_8`;
CREATE TABLE `t_gift_use_8` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_8
-- ----------------------------

-- ----------------------------
-- Table structure for t_gift_use_9
-- ----------------------------
DROP TABLE IF EXISTS `t_gift_use_9`;
CREATE TABLE `t_gift_use_9` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `code_id` int(11) unsigned NOT NULL,
  `s` int(11) NOT NULL,
  `dev` varchar(255) NOT NULL,
  `role_id` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_gift_use_9
-- ----------------------------
