/*
Navicat MySQL Data Transfer

Source Server         : 217
Source Server Version : 50173
Source Host           : 192.168.23.217:3306
Source Database       : updateserver

Target Server Type    : MYSQL
Target Server Version : 50173
File Encoding         : 65001

Date: 2015-07-17 14:34:21
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_files
-- ----------------------------
DROP TABLE IF EXISTS `t_files`;
CREATE TABLE `t_files` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `pf` varchar(255) NOT NULL,
  `sys` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `url` varchar(255) NOT NULL,
  `version` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
