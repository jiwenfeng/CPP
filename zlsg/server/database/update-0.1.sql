INSERT INTO `tb_version` (`id`, `database`, `server`, `client`, `time`) VALUES ('1', '0.1', '0.1', '0.1', '2015-09-11 17:00:01')

ALTER TABLE `tb_player_role`
CHANGE COLUMN `bow_speed` `frozen_addition_ratio`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `attack_speed`,
ADD COLUMN `bow_index`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `bow_id`,
ADD COLUMN `cavalry_index`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `bow_index`;
ADD COLUMN `bow_speed`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `cavalry_index`;
ADD COLUMN `equip_power`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `bow_speed`;

ALTER TABLE `tb_role_skill`
ADD COLUMN `smy_id`  int(11) NOT NULL AUTO_INCREMENT AFTER `effect_id`,
DROP PRIMARY KEY,
ADD PRIMARY KEY (`smy_id`, `role_id`);

ALTER TABLE `tb_role_skill`
MODIFY COLUMN `smy_id`  int(11) NOT NULL AFTER `effect_id`,
DROP PRIMARY KEY,
ADD PRIMARY KEY (`role_id`, `smy_id`);

CREATE TABLE `tb_center_devil` (
  `id` int(10) unsigned NOT NULL,
  `role_id` int(10) unsigned NOT NULL,
  `time` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

ALTER TABLE `tb_activity_open`
ADD COLUMN `reward_flag`  int(11) UNSIGNED NOT NULL AFTER `open_type`,
ENGINE=InnoDB;

ALTER TABLE `server_list`
ADD COLUMN `start_time`  datetime NULL AFTER `open`;

ALTER TABLE `server_list`
ADD COLUMN `share_info`  varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT '' AFTER `start_time`;

CREATE TABLE `t_cdn` (
`pf`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL ,
`cdn`  varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci NULL ,
PRIMARY KEY (`pf`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=utf8 COLLATE=utf8_general_ci;

CREATE TABLE `t_group` (
`id`  int NOT NULL ,
'group_id' int NOT NULL,
`backdoor`  varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=utf8 COLLATE=utf8_general_ci
;

CREATE TABLE `t_promotions` (
`id`  int(11) UNSIGNED NOT NULL ,
`name`  varchar(255) NOT NULL ,
`type`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`start_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`duration_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`end_time`  int(11) NOT NULL ,
`loop_space`  int(11) NOT NULL ,
`state`  int(11) NOT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=utf8 COLLATE=utf8_general_ci;


