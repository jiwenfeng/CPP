DROP TABLE IF EXISTS `tb_promotions`;
CREATE TABLE `tb_promotions` (
`id`  int(11) UNSIGNED NOT NULL ,
`name`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL ,
`type`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`start_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`duration_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`end_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`loop_time`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
`state`  int(11) UNSIGNED NOT NULL DEFAULT 0 ,
PRIMARY KEY (`id`)
)ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;

INSERT INTO `tb_version` (`id`, `database`, `server`, `client`, `time`) VALUES ('4', '0.4', '0.1', '0.1', '2015-09-16 16:40:00');

select max(`database`) from `tb_version`;

ALTER TABLE `tb_player`
ADD COLUMN `check`  int(11) UNSIGNED NOT NULL AFTER `mac`,
ADD COLUMN `check_time`  int(11) UNSIGNED NOT NULL AFTER `check`;

CREATE DEFINER = `zlsg`@`%` PROCEDURE `sp_login_auths`(IN  p_openid VARCHAR(64) charset utf8, IN p_yel_vip INT(11), IN p_server_id INT(11), IN p_agent_id INT(11),  IN p_client_id INT(11), IN platform_id INT(11))
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

END;

REPLACE INTO `tb_version` (`id`, `database`, `server`, `client`, `time`) VALUES ('1', '0.5', '0.1', '0.1', '2015-09-28 11:40:00');
