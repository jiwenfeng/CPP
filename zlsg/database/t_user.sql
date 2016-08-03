/*
Navicat MySQL Data Transfer

Source Server         : 217
Source Server Version : 50173
Source Host           : 192.168.23.217:3306
Source Database       : server_list

Target Server Type    : MYSQL
Target Server Version : 50173
File Encoding         : 65001

Date: 2015-08-18 10:49:47
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_user
-- ----------------------------
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `psw` varchar(255) NOT NULL,
  `group` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_user
-- ----------------------------
INSERT INTO `t_user` VALUES ('2', 'admin', '21232f297a57a5a743894a0e4a801fc3', '0');
INSERT INTO `t_user` VALUES ('3', 'test', 'e10adc3949ba59abbe56e057f20f883e', '1');
