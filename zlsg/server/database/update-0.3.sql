DROP TABLE IF EXISTS `tb_center_notice`;
CREATE TABLE `tb_center_notice` (
`msg_id`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`content`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' ,
`start_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`close_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`interval_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`is_over`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`brocast_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
PRIMARY KEY (`msg_id`)
)ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;

INSERT INTO `tb_version` (`id`, `database`, `server`, `client`, `time`) VALUES ('3', '0.3', '0.1', '0.1', '2015-09-15 16:50:00')