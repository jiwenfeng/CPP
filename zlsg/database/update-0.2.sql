ALTER TABLE `tb_player_role`
ADD COLUMN `equip_power`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `bow_speed`;

INSERT INTO `tb_version` (`id`, `database`, `server`, `client`, `time`) VALUES ('2', '0.2', '0.1', '0.1', '2015-09-11 17:54:00')
