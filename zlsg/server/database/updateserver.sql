/*
Navicat MySQL Data Transfer

Source Server         : 217
Source Server Version : 50173
Source Host           : 192.168.23.217:3306
Source Database       : updateserver

Target Server Type    : MYSQL
Target Server Version : 50173
File Encoding         : 65001

Date: 2015-09-16 21:43:35
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_cdn
-- ----------------------------
DROP TABLE IF EXISTS `t_cdn`;
CREATE TABLE `t_cdn` (
  `pf` varchar(255) NOT NULL,
  `cdn` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`pf`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_cdn
-- ----------------------------
INSERT INTO `t_cdn` VALUES ('qq', 'http://1234567');

-- ----------------------------
-- Table structure for t_client_version
-- ----------------------------
DROP TABLE IF EXISTS `t_client_version`;
CREATE TABLE `t_client_version` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `type` varchar(255) NOT NULL,
  `version` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_client_version
-- ----------------------------
INSERT INTO `t_client_version` VALUES ('1', 'apk', '1');

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
) ENGINE=MyISAM AUTO_INCREMENT=12315 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_files
-- ----------------------------
INSERT INTO `t_files` VALUES ('3', 'cndw', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/cndw/zlsg.apk', '21544');
INSERT INTO `t_files` VALUES ('4', 'android', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/res_21764.zip', '21764');
INSERT INTO `t_files` VALUES ('5', 'android', 'android', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/lib_21764.zip', '21764');
INSERT INTO `t_files` VALUES ('6', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22496.zip', '22496');
INSERT INTO `t_files` VALUES ('2', 'qq', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/zlsg.apk', '23093');
INSERT INTO `t_files` VALUES ('28', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22927.zip', '22927');
INSERT INTO `t_files` VALUES ('13', 'android', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/zlsg.apk', '21980');
INSERT INTO `t_files` VALUES ('14', 'qq', 'android', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/libgame_23011.zip', '23011');
INSERT INTO `t_files` VALUES ('15', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22497.zip', '22497');
INSERT INTO `t_files` VALUES ('17', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22593.zip', '22593');
INSERT INTO `t_files` VALUES ('20', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22844.zip', '22844');
INSERT INTO `t_files` VALUES ('21', 'qq', 'windows', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/monster_id.txt', '8');
INSERT INTO `t_files` VALUES ('22', 'qq', 'windows', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/monster_id.txt', '2');
INSERT INTO `t_files` VALUES ('12314', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_23031.zip', '23031');
INSERT INTO `t_files` VALUES ('12313', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_23010.zip', '23010');
INSERT INTO `t_files` VALUES ('26', 'qq', 'windows', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/l.zip', '9');
INSERT INTO `t_files` VALUES ('27', 'qq', 'windows', 'exe', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/l.zip', '12');

-- ----------------------------
-- Table structure for t_files_check
-- ----------------------------
DROP TABLE IF EXISTS `t_files_check`;
CREATE TABLE `t_files_check` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `pf` varchar(255) NOT NULL,
  `sys` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `url` varchar(255) NOT NULL,
  `version` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=12315 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_files_check
-- ----------------------------
INSERT INTO `t_files_check` VALUES ('1', 'test', 'windows', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/windows/test/config11', '12');
INSERT INTO `t_files_check` VALUES ('16', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22532.zip', '22532');
INSERT INTO `t_files_check` VALUES ('3', 'cndw', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/cndw/zlsg.apk', '21544');
INSERT INTO `t_files_check` VALUES ('4', 'android', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/res_21764.zip', '21764');
INSERT INTO `t_files_check` VALUES ('5', 'android', 'android', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/lib_21764.zip', '21764');
INSERT INTO `t_files_check` VALUES ('6', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22496.zip', '22496');
INSERT INTO `t_files_check` VALUES ('2', 'qq', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/zlsg.apk', '23093');
INSERT INTO `t_files_check` VALUES ('28', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22927.zip', '22927');
INSERT INTO `t_files_check` VALUES ('13', 'android', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/zlsg.apk', '21980');
INSERT INTO `t_files_check` VALUES ('14', 'qq', 'android', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/libgame_23011.zip', '23011');
INSERT INTO `t_files_check` VALUES ('15', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22497.zip', '22497');
INSERT INTO `t_files_check` VALUES ('17', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22593.zip', '22593');
INSERT INTO `t_files_check` VALUES ('18', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22719.zip', '22719');
INSERT INTO `t_files_check` VALUES ('19', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22786.zip', '22786');
INSERT INTO `t_files_check` VALUES ('20', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22844.zip', '22844');
INSERT INTO `t_files_check` VALUES ('21', 'qq', 'windows', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/monster_id.txt', '8');
INSERT INTO `t_files_check` VALUES ('22', 'qq', 'windows', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/monster_id.txt', '2');
INSERT INTO `t_files_check` VALUES ('12314', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_23031.zip', '23031');
INSERT INTO `t_files_check` VALUES ('12313', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_23010.zip', '23010');
INSERT INTO `t_files_check` VALUES ('26', 'qq', 'windows', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/l.zip', '9');
INSERT INTO `t_files_check` VALUES ('27', 'qq', 'windows', 'exe', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/l.zip', '1');

-- ----------------------------
-- Table structure for t_files_update
-- ----------------------------
DROP TABLE IF EXISTS `t_files_update`;
CREATE TABLE `t_files_update` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `pf` varchar(255) NOT NULL,
  `sys` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `url` varchar(255) NOT NULL,
  `version` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=12329 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_files_update
-- ----------------------------
INSERT INTO `t_files_update` VALUES ('3', 'cndw', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/cndw/zlsg.apk', '21544');
INSERT INTO `t_files_update` VALUES ('4', 'android', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/res_21764.zip', '21764');
INSERT INTO `t_files_update` VALUES ('5', 'android', 'android', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/lib_21764.zip', '21764');
INSERT INTO `t_files_update` VALUES ('6', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22496.zip', '22496');
INSERT INTO `t_files_update` VALUES ('2', 'qq', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/zlsg.apk', '23093');
INSERT INTO `t_files_update` VALUES ('28', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22927.zip', '22927');
INSERT INTO `t_files_update` VALUES ('13', 'android', 'android', 'apk', 'http://192.168.23.217:9800/uploads/filesmgr/android/android/zlsg.apk', '21980');
INSERT INTO `t_files_update` VALUES ('14', 'qq', 'android', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/libgame_23011.zip', '23011');
INSERT INTO `t_files_update` VALUES ('15', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22497.zip', '22497');
INSERT INTO `t_files_update` VALUES ('17', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22593.zip', '22593');
INSERT INTO `t_files_update` VALUES ('20', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_22844.zip', '22844');
INSERT INTO `t_files_update` VALUES ('21', 'qq', 'windows', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/monster_id.txt', '8');
INSERT INTO `t_files_update` VALUES ('22', 'qq', 'windows', 'lib', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/monster_id.txt', '2');
INSERT INTO `t_files_update` VALUES ('12314', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_23031.zip', '23031');
INSERT INTO `t_files_update` VALUES ('12313', 'qq', 'android', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/android/qq/lite_23010.zip', '23010');
INSERT INTO `t_files_update` VALUES ('26', 'qq', 'windows', 'res', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/l.zip', '9');
INSERT INTO `t_files_update` VALUES ('27', 'qq', 'windows', 'exe', 'http://192.168.23.217:9800/uploads/filesmgr/windows/qq/l.zip', '12');

-- ----------------------------
-- Procedure structure for files_check
-- ----------------------------
DROP PROCEDURE IF EXISTS `files_check`;
DELIMITER ;;
CREATE DEFINER=`abel`@`%` PROCEDURE `files_check`()
BEGIN
	#Routine body goes here...
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
		select '1' as ret;
	truncate table t_files_check;
	insert into t_files_check select * from t_files;
	insert into t_files_check select * from t_files_update;
	select '0' as ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for files_update
-- ----------------------------
DROP PROCEDURE IF EXISTS `files_update`;
DELIMITER ;;
CREATE DEFINER=`abel`@`%` PROCEDURE `files_update`()
BEGIN
	#Routine body goes here...
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
		select '1' as ret;
	truncate table t_files;
	insert into t_files select * from t_files_update;
	select '0' as ret;
END
;;
DELIMITER ;
