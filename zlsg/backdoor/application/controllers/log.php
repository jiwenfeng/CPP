<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

/**
 * 日志接口
 */

class Log extends CI_Controller {

	function __construct() {
		parent::__construct();
		ini_set('memory_limit', '512M');
		$this->config->load('backdoor', TRUE);
		$game_db_name = $this->config->item('game_db_name', 'backdoor');
		$this->load->database($game_db_name);
		$this->load->model('backdoor_db', 'game_db');
		$this->load->library('backdoor_lib');
	}

	private $key = '3y789fyf8hf34fu0suivjcxv89la98s2';
	private $content_type = 'application/json;charset=utf-8';
	private $base_path = '/home/rockpile/7788/server/bin/log/';
	//private $base_path = '../../socket/bin/log/';

	static public $log_name = array(
		'client.buy_vip_req',
		'client.pick_prop_req',
		'client.drop_prop',
		'client.sort_pack',
		'client.enhanced_equip',

		'client.equip_props_req',
		'client.use_props_req',
		'client.move_props_req',
		'client.melting_equip_req',
		'client.pick_up_props_req',
		'client.upgrade_equip_req',
		'client.baptize_req',
		'client.baptize_save_req',
		
		
		'client.welfare_online_req',
		'client.online_gifts_req',
		'client.get_gifts_req',
		'client.vip_day_premiums_req',

		'client.accept_task_req',
		'client.submit_task_req',
		'client.talk_task_req',

		'client.enter_raid_req',
		'client.mini_server_req',
		'client.auto_raid_req',
		'client.exit_raid_req',
		'client.team_raid_enter_cmd',

		'client.upgrade_mounts_req',
		'client.upgrade_mounts_skill_req',
		'client.ride_mounts_req',

		'client.act_potential_req',

		'client.jump_scene',

		'client.pet_fight_req',
		'client.pet_rest_req',
		'client.pet_upgrade_req',
		'client.pet_rename_req',
		'client.pet_rest_or_fight_req',
		'client.pet_fight_mode_change_req',
		'client.pet_recruit_req',
		'client.pet_lingering_req',
		'client.magic_upgrade_req',
		
		'client.auth_rsp',

		'client.boss_info_req',

		'client.siege_status',
		'client.siege_enter_req',
		'client.siege_personal_info_req',
		'client.siege_revive_req',

		'client.shop_item_list_req',
		'client.black_shop_req',
		'client.black_shop_reflesh_req',
		'client.buy_goods_req',

		'client.query_req',

		'client.skill_point_time_req',
		'client.role_skill_effect_upgrade_req',

		'client.arena_fight_begin',
		'client.arena_fight_ret',
		'client.arena_speed_up_req',
		'client.arena_buy_challenge_count',
		'client.arena_daily_reward',

		'client.target_prize_req',
	);

	static public $log_type_name = array(
		'1' => "物品",
		'2' => "福利",
		'3' => "任务",
		'4' => "副本",
		'5' => "坐骑",
		'6' => "心法",
		'7' => "场景",
		'8' => "宠物",
		'9' => "登陆",
		'10' => '神将',
		'11' => '王都争霸',
		'12' => '商店',
		'13' => '查询',
		'14' => '技能',
		'15' => '竞技场',
	);

	static public $log_id = array(
		'client.buy_vip_req'              => '1',
		'client.pick_prop_req'            => '1',
		'client.drop_prop'                => '1',
		'client.sort_pack'                => '1',
		'client.move_prop'                => '1',
		'client.enhanced_equip'           => '1',
		'client.refining_prop_req'        => '1', 
		'client.equip_props_req'          => '1',
		'client.use_props_req'            => '1',
		'client.move_props_req'           => '1',
		'client.melting_equip_req'        => '1',
		'client.pick_up_props_req'        => '1',
		'client.upgrade_equip_req'        => '1',
		'client.baptize_req'        	  => '1',
		'client.baptize_save_req'         => '1',

		'client.welfare_online_req'       => '2',
		'client.online_gifts_req'         => '2',
		'client.get_gifts_req'            => '2',
		'client.vip_day_premiums_req'     => '2',

		'client.accept_task_req'          => '3',
		'client.submit_task_req'          => '3',
		'client.talk_task_req'            => '3',

		'client.enter_raid_req'           => '4',
		'client.mini_server_req'          => '4',
		'client.auto_raid_req'            => '4',
		'client.exit_raid_req'            => '4',
		'client.team_raid_enter_cmd'      => '4',

		'client.upgrade_mounts_req'       => '5',
		'client.upgrade_mounts_skill_req' => '5',
		'client.ride_mounts_req'          => '5',

		'client.act_potential_req'        => '6',

		'client.jump_scene'               => '7',

		'client.pet_fight_req'            => '8',
		'client.pet_rest_req'             => '8',
		'client.pet_upgrade_req'          => '8',
		'client.pet_rename_req'           => '8',
		'client.pet_rest_or_fight_req'    => '8',
		'client.pet_fight_mode_change_req'=> '8',
		'client.pet_recruit_req'          => '8',
		'client.pet_lingering_req'		  => '8',
		'client.magic_upgrade_req'		  => '8',
		
		'client.auth_rsp'                 => '9',

		'client.boss_info_req'            => '10',

		'client.siege_status'             => '11',
		'client.siege_enter_req'          => '11',
		'client.siege_personal_info_req'  => '11',
		'client.siege_revive_req'         => '11',

		'client.shop_item_list_req'       => '12',
		'client.black_shop_req'           => '12',
		'client.black_shop_reflesh_req'   => '12',
		'client.buy_goods_req'            => '12',

		'client.query_req'                => '13',

		'client.skill_point_time_req'     => '14',
		'client.role_skill_effect_upgrade_req' => '14',

		'client.arena_fight_begin'		  => '15',
		'client.arena_fight_ret'		  => '15',
		'client.arena_speed_up_req'		  => '15',
		'client.arena_buy_challenge_count'	=> '15',
		'client.arena_daily_reward'			=> '15',

		'client.target_prize_req'			=> '16',
	);

	static public $log_sub_id = array(
		'client.buy_vip_req'              => '10003',
		'client.pick_prop_req'            => '10004',
		'client.equip_on'                 => '10005',
		'client.equip_off'                => '10006',
		'client.drop_prop'                => '10007',
		'client.sort_pack'                => '10008',
		'client.enhanced_equip'           => '10010',
		'client.refining_prop_req'        => '10011', 
		'client.equip_props_req'          => '10012',
		'client.use_props_req'            => '10013',
		'client.move_props_req'           => '10014',
		'client.melting_equip_req'        => '10015',
		'client.pick_up_props_req'        => '10016',
		'client.upgrade_equip_req'        => '10017',
		'client.baptize_req'        	  => '10018',
		'client.baptize_save_req'         => '10019',

		'client.welfare_online_req'       => '20001',
		'client.online_gifts_req'         => '20002',
		'client.get_gifts_req'            => '20003',
		'client.vip_day_premiums_req'     => '20004',

		'client.accept_task_req'          => '30001',
		'client.submit_task_req'          => '30002',
		'client.talk_task_req'            => '30003',

		'client.enter_raid_req'           => '40001',
		'client.auto_raid_req'            => '40002',
		'client.exit_raid_req'            => '40003',
		'client.mini_server_req'          => '40004',
		'client.team_raid_enter_cmd'      => '40005',

		'client.upgrade_mounts_req'       => '50001',
		'client.upgrade_mounts_skill_req' => '50002',
		'client.ride_mounts_req'          => '50003',

		'client.act_potential_req'        => '60001',

		'client.jump_scene'               => '70001',

		'client.pet_fight_req'            => '80001',
		'client.pet_rest_req'             => '80002',
		'client.pet_upgrade_req'          => '80003',
		'client.pet_rename_req'           => '80004',
		'client.pet_rest_or_fight_req'    => '80005',
		'client.pet_fight_mode_change_req'=> '80006',
		'client.pet_recruit_req'          => '80007',
		'client.pet_lingering_req'        => '80008',
		'client.magic_upgrade_req'        => '80009',

		'client.auth_rsp'                 => '90001',

		'client.boss_info_req'            => '100001',

		'client.siege_status'             => '110001',
		'client.siege_enter_req'          => '110002',
		'client.siege_personal_info_req'  => '110003',
		'client.siege_revive_req'         => '110004',

		'client.shop_item_list_req'       => '120001',
		'client.black_shop_req'           => '120002',
		'client.black_shop_reflesh_req'   => '120003',
		'client.buy_goods_req'            => '120004',

		'client.query_req'                => '130001',

		'client.skill_point_time_req'     => '140001',
		'client.role_skill_effect_upgrade_req' => '140002',

		'client.arena_fight_begin'		=> '150001',
		'client.arena_fight_ret'		=> '150002',
		'client.arena_speed_up_req'		=> '150003',
		'client.arena_buy_challenge_count' => '150004',
		'client.arena_daily_reward'			=> '150005',

		'client.target_prize_req'		=> '160005',
	);

	static public $log_type = array(
		'client.use_prop'                  => '使用物品',
		'client.buy_vip_req'               => '购买VIP',
		'client.pick_prop_req'             => '拾取物品',
		'client.equip_on'                  => '穿上装备',
		'client.equip_off'                 => '卸下装备',
		'client.drop_prop'                 => '丢弃物品',
		'client.sort_pack'                 => '整理背包',
		'client.enhanced_equip'            => '加持装备',
		'client.refining_prop_req'         => '炼制物品', 
		'client.equip_props_req'           => '穿戴装备',
		'client.use_props_req'             => '使用物品',
		'client.move_props_req'            => '脱除物品',
		'client.melting_equip_req'         => '熔炼物品',
		'client.pick_up_props_req'         => '拾取物品',
		'client.upgrade_equip_req'         => '强化装备',
		'client.baptize_req'         	   => '洗练装备',
		'client.baptize_save_req'          => '洗练保存装备',

		'client.welfare_online_req'        => '在线福利',
		'client.online_gifts_req'          => '在线礼包',
		'client.get_gifts_req'             => '在线元宝',
		'client.vip_day_premiums_req'      => 'VIP福利',

		'client.accept_task_req'           => '接受任务',
		'client.submit_task_req'           => '提交任务',
		'client.talk_task_req'             => '对话任务',

		'client.enter_raid_req'            => '进入副本',
		'client.auto_raid_req'             => '扫荡副本',
		'client.exit_raid_req'             => '退出副本',
		'client.mini_server_req'           => '征战副本',
		'client.team_raid_enter_cmd'       => '组队副本',

		'client.upgrade_mounts_req'        => '进阶坐骑',
		'client.upgrade_mounts_skill_req'  => '升级坐骑技能',
		'client.ride_mounts_req'           => '操作坐骑',

		'client.act_potential_req'         => '心法激活',
		'client.jump_scene'                => '跳转场景',
		'client.pet_fight_req'             => '美人出战',
		'client.pet_rest_req'              => '美人休息',
		'client.pet_upgrade_req'           => '美人进阶',
		'client.pet_rename_req'            => '美人改名',
		'client.pet_rest_or_fight_req'     => '美人登场',
		'client.pet_fight_mode_change_req' => '美人切换',
		'client.pet_recruit_req'           => '美人招募',
		'client.pet_lingering_req'		   => '美人缠绵',
		'client.magic_upgrade_req'		   => '神兵升级',

		'client.auth_rsp'                  => '玩家登陆',

		'client.boss_info_req'             => '神将',

		'client.siege_status'              => '王都争霸',
		'client.siege_enter_req'           => '进入王都争霸',
		'client.siege_personal_info_req'   => '获取王都争霸个人信息',
		'client.siege_revive_req'          => '在王都争霸中复活',

		'client.shop_item_list_req'        => '商店物品列表',
		'client.black_shop_req'            => '黑店物品列表',
		'client.black_shop_reflesh_req'    => '黑店刷新',
		'client.buy_goods_req'             => '购买物品',

		'client.query_req'                => '查询数据',

		'client.skill_point_time_req'     => '获取技能点',
		'client.role_skill_effect_upgrade_req' => '升级技能效果',

		'client.arena_fight_begin'		  => '竞技场挑战',
		'client.arena_fight_ret'		  => '竞技场挑战结果',
		'client.arena_speed_up_req'		  => '竞技场冷却加速',
		'client.arena_buy_challenge_count' => '竞技场增加挑战次数',
		'client.arena_daily_reward'			=> '竞技场每日排名奖励',

		'client.target_prize_req'			=> '日常成就',

	);

	private function get_logs($time, $end_time, &$list) {
		if (!$time || !$end_time) {
			$time = time() - 86400;
			$end_time = time();
		}

		date_default_timezone_set('Asia/Shanghai');
		$date_name = date("Y_m_", $time);
		$day       = date("d", $time);
		$hour      = date('H', $time);
		$end_day   = date("d", $end_time);
		$end_hour  = date('H', $end_time);
		if ($hour > $end_hour) {
			$end_hour = $hour ;
		}

		$index = 0;
		$log_cfg = array();
		/*$jsonfile = file_get_contents('./application/controllers/log_goods.json');
		$log_cfg['goods'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_store.json');
		$log_cfg['store'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_cmpst.json');
		$log_cfg['cmpst'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_reward.json');
		$log_cfg['reward'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_task.json');
		$log_cfg['task'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_npc.json');
		$log_cfg['npc'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_raid.json');
		$log_cfg['raid'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_scene.json');
		$log_cfg['scene'] = json_decode($jsonfile, true);
		$jsonfile = file_get_contents('./application/controllers/log_map.json');
		$log_cfg['map'] = json_decode($jsonfile, true);*/
		$this->config->load('backdoor', TRUE);
		$names = array();
		$accounts = array();
		for ($file_count = 0; $day <= $end_day; ++$hour, ++$file_count) {
			$path = $this->config->item('log_path', 'backdoor');
			if ($hour > 9) {
				$path .= '/bill_' . $date_name . $day . '_' . $hour . '.log';
			} else {
				$path .= '/bill_' . $date_name . $day . '_0' . $hour . '.log';
			}
			if ($hour > 23) {
				$day += 1;
				$hour = 0;
			}

			if (!file_exists($path)) {
				continue ;
			}

			$logs = file($path, FILE_IGNORE_NEW_LINES);
			$max_index = count($logs);
			for ($j = 0; $j < $max_index; ++$index, ++$j) {
				$item = explode('|', $logs[$j]);
				$max_size = count($item);
				if ($max_size < 4 /*|| !isset(self::$log_type[$item[3]])*/) {
					continue;
				}

				$list[$index]['addTime']     = $item[1];
				$description = array();
				for ($i = 4; $i < $max_size; ++$i) {
					$description[$i - 4] = $item[$i];
				}

				if (!isset($names[$item[2]])) {
					$names[$item[2]] =  $this->game_db->id_to_name($item[2]);
				}

				if (!isset($accounts[$item[2]])) {
					$accounts[$item[2]] = $this->game_db->id_to_account($item[2]);
				}

				$list[$index]['eventId']     = isset(self::$log_sub_id[$item[3]]) ? self::$log_sub_id[$item[3]] : 0;
				$list[$index]['id']          = $index;//isset(self::$log_id[$item[3]]) ? self::$log_id[$item[3]] : 0;
				$list[$index]['nickName']    = $names[$item[2]];
				$list[$index]['objectId']    = isset(self::$log_id[$item[3]]) ? self::$log_id[$item[3]] : 0;
				$list[$index]['userId']      = $item[2];
				$list[$index]['userName']    = $accounts[$item[2]];
				$list[$index]['description'] = $this->description($log_cfg, $item[3], $description);
			}
		}
	}

	public function description($log_cfg, $proto_name, $params) {
		$store  = array();//$log_cfg['store'];
		$goods  = array();//$log_cfg['goods'];
		$cmpst  = array();//$log_cfg['cmpst'];
		$reward = array();//$log_cfg['reward'];
		$task   = array();//$log_cfg['task'];
		$npc    = array();//$log_cfg['npc'];
		$raid   = array();//$log_cfg['raid'];
		$map    = array();//$log_cfg['map'];
		$scene  = array();//$log_cfg['scene'];
		switch ($proto_name) {
		case 'client.auth_rsp'             : return '地址为 ' . $params[1] . ' 在 ' . date('Y-m-d H:i:s', $params[2]) . ' 登陆 ' . $params[0] . ' 号服务器';
		case 'client.buy_vip_req'                : return $this->description_buy_vip($params);
		case 'client.pick_prop_req'              : return $this->description_pick_prop($goods, $params);
		case 'client.equip_on'                   : return $this->description_equip_on($goods, $params);
		case 'client.equip_off'                  : return $this->description_equip_off($goods, $params);
		case 'client.drop_prop'                  : return $this->description_drop_prop($goods, $params);
		case 'client.sort_pack'                  : return "整理背包";
		case 'client.enhanced_equip'             : return $this->description_enhanced_equip($goods, $params);
		case 'client.refining_prop_req'          : return $this->description_refining_prop($cmpst, $params);
		case 'client.equip_props_req'            : return $this->_description_equip_props($params);
		case 'client.use_props_req'              : return $this->_description_use_props($params);
		case 'client.move_props_req'             : return $this->_description_move_props($goods, $params);
		case 'client.melting_equip_req'          : return $this->_description_melting_equip($goods, $params);
		case 'client.pick_up_props_req'          : return $this->_description_pick_up_props($goods, $params);
		case 'client.upgrade_equip_req'          : return $this->_description_upgrade_equip($params);
		case 'client.baptize_req'          		 : return $this->_description_baptize($params);
		case 'client.baptize_save_req'           : return $this->_description_baptize_save($params);
		case 'client.welfare_online_req'         : return $this->description_welfare_online($params);
		case 'client.online_gifts_req'           : return $this->description_online_gifts($params);
		case 'client.get_gifts_req'              : return $this->description_get_gifts($reward, $params);
		case 'client.vip_day_premiums_req'       : return "领取VIP每日礼包";
		case 'client.accept_task_req'            : return $this->description_accept_task($task, $params);
		case 'client.submit_task_req'            : return $this->_description_submit_task($task, $params);
		case 'client.talk_task_req'              : return $this->description_talk_task($task, $npc, $params);
		case 'client.enter_raid_req'             : return $this->_description_enter_raid_req($raid, $params);
		case 'client.auto_raid_req'              : return $this->_description_auto_raid_req($raid, $params);
		case 'client.exit_raid_req'              : return "退出副本";//$this->description_exit_raid($raid, $params);
		case 'client.mini_server_req'            : return $this->_description_mini_server_raid($raid, $params);
		case 'client.team_raid_enter_cmd'        : return $this->_description_team_raid_enter($raid, $params);
		case 'client.upgrade_mounts_req'         : return $this->_description_upgrade_mounts($params);
		case 'client.upgrade_mounts_skill_req'   : return $this->_description_upgrade_mounts_skill($params);
		case 'client.ride_mounts_req'            : return $this->_description_ride_mounts($params);
		case 'client.act_potential_req'          : return $this->description_act_potential($params);
		case 'client.jump_scene'                 : return $this->description_jump_scene($map, $scene, $params);

		case 'client.pet_fight_req'              : return $this->description_pet_fight($params);
		case 'client.pet_rest_req'               : return $this->description_pet_rest($params);
		case 'client.pet_upgrade_req'            : return $this->description_pet_upgrade($params);
		case 'client.pet_rename_req'             : return $this->description_pet_rename($params);
		case 'client.pet_rest_or_fight_req'      : return $this->_description_pet_rest_or_fight($params);
		case 'client.pet_fight_mode_change_req'  : return $this->_description_pet_fight_mode_change($params);
		case 'client.pet_recruit_req'            : return $this->_description_pet_recruit($params);
		case 'client.pet_lingering_req'          : return $this->_description_pet_lingering($params);
		case 'client.magic_upgrade_req'          : return $this->_description_magic_upgrade($params);

		case 'client.boss_info_req'              : return $this->_description_boss_info($params);

		case 'client.siege_status'               : return $this->_description_siege_status($params);
		case 'client.siege_enter_req'            : return $this->_description_siege_enter($params);
		case 'client.siege_personal_info_req'    : return $this->_description_siege_personal_info($params);
		case 'client.siege_revive_req'           : return $this->_description_siege_revive($params);

		case 'client.shop_item_list_req'         : return $this->_description_shop_item_list($params);
		case 'client.black_shop_req'             : return $this->_description_black_shop($params);
		case 'client.black_shop_reflesh_req'     : return $this->_description_black_shop_reflesh($params);
		case 'client.buy_goods_req'              : return $this->_description_buy_goods($params);
		case 'client.query_req'                  : return $this->_description_query($params);

		case 'client.skill_point_time_req'       : return $this->_description_skill_point_time($params);
		case 'client.role_skill_effect_upgrade_req' : return $this->_description_skill_effect_upgrade($params);
		case 'client.arena_fight_begin'			 : return $this->_description_arena_fight_begin($params);
		case 'client.arena_fight_ret'			 : return $this->_description_arena_fight_ret($params);
		case 'client.arena_speed_up_req'		 : return $this->_description_arena_speed_up_req($params);
		case 'client.arena_buy_challenge_count'	 : return $this->_description_arena_buy_challenge_count($params);
		case 'client.target_prize_req'			 : return $params[0];
		case 'client.arena_daily_reward'			 : return $this->_description_arena_daily_reward($params);
		default : return $this->_description_default($proto_name, $params);
		}

		return "";
	}

	public function description_buy_mall($store, $goods, $params) {
		$ret = "购买 ";
		$size = count($params);
		for ($i = 0; $i < $size; $i += 3) {
			if ($params[$i + 2] == 1) {
				$ret .= $params[$i + 1] . " 个 " . $goods[$params[$i]];
			} else {
				$ret .= $params[$i + 1] . " 个 " . $goods[$store[$params[$i]]];
			}
		}

		return $ret;
	}

	private function _description_use_props($params) {
		if (count($params) < 4) return '使用物品';
		if(count($params) == 11) {
			if ($params[9] == 5) {
				if ($params[10] == 18) {
					$ret = "消耗物品{$params[0]}，前值{$params[1]}/后值{$params[2]};获得经验{$params[3]}，
					前值{$params[4]}/后值{$params[5]};坐骑等级{$params[6]}，前值{$params[7]}/后值{$params[8]}";
				}
			}
		}elseif (count($params) == 12) {
			if ($params[10] == 5) {
				if ($params[11] == 19) {
					$ret = "美人{$params[0]}：消耗物品{$params[1]}，前值{$params[2]}/后值{$params[3]};获得经验{$params[4]}，
					前值{$params[5]}/后值{$params[6]};美人等级{$params[7]}，前值{$params[8]}/后值{$params[9]}";
				}
			}
		}else{
			$ret = "使用 {$params[1]} 个 格子为 {$params[0]} 物品 状态 {$params[2]} ";
		}

		
		return $ret;
	}

	public function description_buy_vip($params) {
		$ret = "成为 ";
		switch ($params[0]) {
		case 1: return $ret .= "白金仙灵";
		case 2: return $ret .= "钻石仙灵";
		case 3: return $ret .= "至尊仙灵";
		case 4: return $ret .= "临时至尊仙灵";
		case 5: return $ret .= "体验至尊仙灵";
		}

		return $ret;
	}

	public function description_pick_prop($goods, $params) {
		$ret = "拾取1个 " . $goods[$params[1]];
		return $ret;
	}

	public function description_equip_on($goods, $params) {
		if (count($params) < 3) return "佩戴 装备"; 
		$ret = "佩戴 " . $goods[$params[2]];
		return $ret;
	}

	public function description_equip_off($goods, $params) {
		if (count($params) < 3) return "卸下 装备"; 
		$ret = "卸下 " . $goods[$params[2]];
		return $ret;
	}

	public function description_drop_prop($goods, $params) {
		if (count($params) < 3) return "丢弃 物品";
		$ret = "丢弃 " . $params[1] . " 个 " . $goods[$params[2]];
		return $ret;
	}

	private function _description_move_props($goods, $params) {
		if (count($params) < 4) return '脱除装备';
		$ret = "脱除装备{$params[1]}，装备等级{$params[2]}，基础属性变化:{$params[3]};成长值:{$params[4]}";
		if ($params[5] == 0) {
			$ret .= "蓝色"; 
		}elseif ($params[5] == 1) {
			$ret .= "白色"; 
		}elseif ($params[5] == 2) {
			$ret .= "紫色"; 
		}elseif ($params[5] == 3) {
			$ret .= "绿色"; 
		}elseif ($params[5] == 4) {
			$ret .= "橙色"; 
		}

		$ret .= "强化等级: {$params[6]}; 强化属性: {$params[7]}; 洗练属性:{$params[8]};脱除属性变化:{$params[9]}";

		return $ret;
	}


	private function _description_baptize($params) {
		if (count($params) < 1) return '洗练装备';
		$ret = "装备:{$params[0]}";
		if ($params[1] == 1) {
			$ret .= "消耗铜钱 {$params[2]},前值 {$params[3]} / 后值 {$params[4]};";
		}elseif ($params[1] == 2) {
			$ret .= "消耗一级洗练石 {$params[2]},前值 {$params[3]} / 后值 {$params[4]}";
		}elseif ($params[1] == 3) {
			$ret .= "消耗二级洗练石 {$params[2]},前值 {$params[3]} / 后值 {$params[4]}";
		}

		$ret .= "{$params[5]}";

		return $ret;
	}

	private function _description_upgrade_equip($params) {
		if (count($params) < 1) return '强化装备';
		$ret = "装备:{$params[0]}";
		if ($params[1] == 0) {
			$ret .= "消耗强化石{$params[2]}，前值{$params[3]} / 后值 {$params[4]};消耗铜钱{$params[5]}，前值 {$params[6]} / 后值 {$params[7]};
			强化等级 {$params[8]}，前值 {$params[9]} / 后值 {$params[10]};{$params[11]}";
		}else{
			$ret .= "消耗升星石{$params[2]}，前值{$params[3]} / 后值 {$params[4]};消耗铜钱{$params[5]}，前值 {$params[6]} / 后值 {$params[7]};
			强化等级 {$params[8]}，前值 {$params[9]} / 后值 {$params[10]};{$params[11]}";
		}

		return $ret;
	}

	private function _description_baptize_save($params) {
		if (count($params) < 1) return '洗练保存装备';
		$ret = "保存装备:{$params[0]}，{$params[1]}，{$params[2]}";
		return $ret;
	}

	private function _description_melting_equip(&$goods, &$params) {
		if (count($params) < 1) return '熔炼物品';
		$ret = "熔炼物品 {$params[0]}";
		return $ret;
	}

	private function _description_pick_up_props(&$goods, &$params) {
		if (count($params) < 1) return '拾取物品';
		$ret = "拾取物品 {$params[0]}";
		return $ret;
	}

	public function description_enhanced_equip($goods, $params) {
		if (count($params) < 6) return "加持装备" . $params[1] . "次";
		$ret = "加持 " . $goods[$params[0]] . " " . $params[2] . "次";
		return $ret;
	}

	public function description_refining_prop($cmpst, $params) {
		$ret = "炼制 " . $params[1] . " 个 " . $cmpst[$params[0]];
		if ($params[2] == 0) {
			$ret .= " 成功";
		} else {
			$ret .= " 失败";
		}

		return $ret;
	}

	private function _description_equip_props(&$params) {
		if (count($params) < 4) return '穿戴装备';
		$ret = "穿戴装备{$params[1]}，装备等级{$params[2]}，基础属性变化:{$params[3]};成长值:{$params[4]}";
		if ($params[5] == 0) {
			$ret .= "蓝色"; 
		}elseif ($params[5] == 1) {
			$ret .= "白色"; 
		}elseif ($params[5] == 2) {
			$ret .= "紫色"; 
		}elseif ($params[5] == 3) {
			$ret .= "绿色"; 
		}elseif ($params[5] == 4) {
			$ret .= "橙色"; 
		}

		$ret .= "强化等级:{$params[6]}; 强化属性:{$params[7]}; 洗练属性:{$params[8]};穿戴属性变化:{$params[9]}";

		return $ret;
	}

	public function description_welfare_online($params) {
		$ret = "领取登陆奖励 ";
		if ($params[0] != 0) {
			$ret .= "签到奖励";
		} else if ($params[1] != 0) {
			$ret .= "每周仙禄";
		} else if ($params[2] != 0) {
			$ret .= "闭关潜修";
		}

		if ($params[3] == 0) {
			$ret .= " 成功";
		} else {
			$ret .= " 失败";
		}

		return $ret;
	}

	public function description_online_gifts($params) {
		$ret = "领取 " . $params[0] . " 分钟在线奖励 ";
		if ($params[1] == 0) {
			$ret .= "成功";
		} else {
			$ret .= "失败";
		}

		return $ret;
	}

	public function description_get_gifts($reward, $params) {
		$ret = "领取 " . $reward[$params[0]] . " 礼包奖励 ";
		if ($params[1] == 0) {
			$ret .= "成功";
		} else {
			$ret .= "失败";
		}

		return $ret;
	}

	public function description_accept_task($task, $params) {
		if (count($params) < 3) return "领取任务";
		$ret = "领取 " . $task[$params[0]] . " ";
		if ($params[2] == 1) {
			$ret .= "成功";
		} else {
			$ret .= "失败";
		}

		return $ret;
	}

	public function _description_submit_task(&$task, &$params) {
		if (count($params) < 1) return "提交任务";
		$name = array_key_exists($params[0], $task) ? $task[$params[0]] : $params[0];
		$ret = '提交 ' . $name . ' 任务';
		return $ret;
	}

	public function description_talk_task($task, $npc, $params) {
		if (count($params) < 2) return "对话任务";
		$ret = "做 " . $task[$params[0]] . " 任务与 " . $npc[$params[1]] . " 对话 ";
		return $ret;
	}

	private function _description_enter_raid_req(&$cfg, &$params) {
		if (count($params) < 1) return "进入副本";
		$name = array_key_exists($params[0], $cfg) ? $cfg[$params[0]] : $params[0];
		$ret = "进入 " . $name . " 副本";
		return $ret;
	}

	private function _description_auto_raid_req(&$raid, &$params) {
		if (count($params) < 1) return "扫荡副本";
		$ret = "扫荡 " . $params[0] . " 副本";
		return $ret;
	}

	private function _description_mini_server_raid(&$cfg, &$params) {
		if (count($params) < 3) return '征战副本';
		if ($params[0] == 1)  {
			$name = array_key_exists($params[1], $cfg) ? $cfg[$params[1]] : $params[1];
			$ret = "进入 {$name} 副本 状态码: {$params[2]}";
		} else {
			//$name = array_key_exists($params[1], $cfg) ? $cfg[$params[1]] : $params[1];
			$ret = '退出征战副本';
		}

		return $ret;
	}

	private function _description_team_raid_enter(&$raid, &$params) {
		if (count($params) < 3) return '进入组队副本';
		$ret = "进入组队副本 队长 {$params[0]} 队伍 {$params[1]} 副本 {$params[2]}";
		return $ret;
	}

	private function _description_upgrade_mounts(&$params) {
		$ret = "进阶坐骑";
		if (count($params) == 8) {
			if ($params[7] == 1) {
				$ret .= "消耗 {$params[0]};获得祝福值 {$params[1]},前值 {$params[2]} / 后值 {$params[3]};
				坐骑等阶{$params[4]}，前值{$params[5]}/ 后值{$params[6]};成功";
			}else{
				$ret .= "消耗 {$params[0]};获得祝福值 {$params[1]},前值 {$params[2]} / 后值 {$params[3]};
				坐骑等阶{$params[4]}，前值{$params[5]}/ 后值{$params[6]};失败";
			}
		}

		return $ret;
	}

	private function _description_upgrade_mounts_skill(&$params) {
		if (count($params) < 1) return '升级坐骑技能';
		$ret = "升级坐骑技能 {$params[0]}";
		return $ret;
	}

	private function _description_ride_mounts(&$params) {
		if (count($params) < 1) return '操作坐骑';
		if ($params[0] == 1) {
			$ret = "上坐骑，{$params[1]}";
		}else{
			$ret = "下坐骑，{$params[1]}";
		}

		return $ret;
	}

	public function description_act_potential($params) {
		if (count($params) < 1) return "心法激活";
		if ($params[0] == 0) {
			$ret = "消耗韬略值: {$params[1]}，前值{$params[2]} / 后值{$params[3]}; 激活心法 {$params[4]} 失败;
			获得经验 {$params[5]}，前值 {$params[6]} / 后值 {$params[7]}";
		}else{
			$ret = "消耗韬略值: {$params[1]}，前值{$params[2]} / 后值{$params[3]}; 成功激活心法 {$params[4]}; 获得属性:{$params[5]};
			激活心法技能 {$params[6]};获得经验 {$params[7]}，前值 {$params[8]} / 后值 {$params[9]}; 增加周围玩家经验，{$params[10]}";
		}

		return $ret;
	}

	public function description_jump_scene($map, $scene, $params) {
		$ret = "跳转场景";
		if (count($params) < 11) return $ret;
		if (isset($map[$params[9]]) && isset($scene[$params[10]])) {
			$ret = "跳转到 " . $map[$params[9]] . " 地图中的 " . $scene[$params[10]] . " 场景中的 x " . $params[5] . " y " . $params[6];
		}

		return $ret;
	}

	public function description_pet_fight($params) {
		if (count($params) < 1) return "美人出战";
		return "第 " . $params[0] . " 号位美人出战";
	}

	public function description_pet_rest($params) {
		if (count($params) < 1) return "美人出战";
		return "第 " . $params[0] . " 号位美人休战";
	}

	public function description_pet_upgrade($params) {
		if (count($params) < 2) return "美人进阶";
		return "第 " . $params[0] . " 号位美人进阶";
	}

	public function description_pet_rename($params) {
		if (count($params) < 2) return "美人改名";
		return "第 " . $params[0] . " 号位美人改名为 " . $params[1];
	}

	private function _description_pet_rest_or_fight(&$params) {
		if (count($params) < 2) return '美人登场';
		if ($params[1] > 0) {
			return "美人 {$params[0]} 登场";
		} else {
			return "美人 {$params[0]} 退场";
		}
	}

	private function _description_pet_fight_mode_change(&$params) {
		if (count($params) < 1) return '美人切换';
		return "美人切换 {$params[0]}";
	}

	private function _description_pet_recruit(&$params) {
		if (count($params) < 1) return '美人招募';
		return "美人招募 {$params[0]}";
	}

	private function _description_pet_lingering(&$params) {
		if (count($params) < 1) return '美人缠绵';
		$ret = "美人{$params[0]},消耗物品：{$params[1]}，前值 {$params[2]} / 后值{$params[3]};增加属性：
		{$params[4]}，已缠绵次数：{$params[5]}";

		return $ret;
	}

	private function _description_magic_upgrade(&$params) {
		if (count($params) < 1) return '神兵升级';
		$ret = "美人{$params[0]},消耗物品：{$params[1]}，前值 {$params[2]} / 后值{$params[3]};前值 {$params[4]} 阶{$params[5]}星 / 后值
		{$params[6]}阶 {$params[7]} 星;属性：{$params[8]}";

		return $ret;
	}

	private function _description_boss_info(&$params) {
		if (count($params) < 1) return '查询神将';
		return "查询神将 {$params[0]} 秒后刷新";
	}

	private function _description_siege_status(&$params) {
		if (count($params) < 1) return '王都争霸';
		return "王都争霸状态 {$params[0]}";
	}

	private function _description_siege_enter(&$params) {
		return "进入王都争霸";
	}

	private function _description_siege_personal_info(&$params) {
		if (count($params) < 3) return '获取王都争霸个人信息';
		return "获取王都争霸个人信息 区域 {$params[0]} 经验 {$params[1]} 时间 {$params[2]}";
	}

	private function _description_siege_revive(&$params) {
		return "在王都争霸中复活";
	}

	private function _description_shop_item_list(&$params) {
		if (count($params) < 1) return '商店列表';
		return "商店列表 类型为 {$params[0]}";
	}

	private function _description_black_shop(&$params) {
		if (count($params) < 1) return '黑店列表';
		return "黑店列表 {$params[0]}"; 
	}

	private function _description_black_shop_reflesh(&$params) {
		if (count($params) < 1) return '刷新黑店列表';
		return "刷新黑店列表 {$params[0]}"; 
	}

	private function _description_buy_goods(&$params) {
		if (count($params) < 1) return '在商店购买物品';
		$ret = "购买道具 {$params[0]}，数量 {$params[1]}，购买前值 {$params[2]} / 后值 {$params[3]};消耗: ";
		if ($params[4] == 5) {
			$ret .= "元宝";
		}elseif ($params[4] == 13) {
			$ret .= "军贡";
		}elseif ($params[4] == 69) {
			$ret .= "威望";
		}elseif ($params[4] == 74) {
			$ret .= "谋略";
		}else{
			$ret .= "铜钱";
		}

		$ret .= "货币 {$params[5]}，前值 {$params[6]} / 后值 {$params[7]}";

		return $ret; 
	}

	private function _description_query(&$params) {
		if (count($params) < 1) return '查询数据';
		return "查询数据 类型为 {$params[0]}"; 
	}

	private function _description_skill_point_time(&$params) {
		return '获取当前技能点';
	}

	private function _description_skill_effect_upgrade(&$params) {
		if (count($params) < 1) return '升级技能效果';

		if (count($params) == 9) {
			$ret = "美人{$params[0]}，技能:{$params[1]};消耗技能点 {$params[2]},剩余技能点前值 {$params[3]} / 后值 {$params[4]};
			铜钱前值{$params[5]} / 后值 {$params[6]}; 技能效果等级前值 {$params[7]} / 后值 {$params[8]}";
		}else{
			$ret = "升级技能效果 {$params[0]}";
		}

		return $ret;
	}

	private function _description_arena_fight_begin(&$params) {
		return "挑战 {$params[0]}; 挑战次数前值 {$params[1]} / 后值 {$params[2]}";
	}

	private function _description_arena_fight_ret(&$params) {
		$ret = "挑战 {$params[0]}; ";
		if($params[1] == 0) {
			$ret .= "成功";
		} else {
			$ret .= "失败";
		}
		$ret .= ", 获得威望 {$params[2]}, 排名前值 {$params[3]} / 排名后值 {$params[4]}";
		return $ret;
	}

	private function _description_arena_speed_up_req(&$params) {
		return "冷却CD加速, 消耗 {$params[0]} 元宝(前值{$params[1]} / 后值 {$params[2]}";
	}

	private function _description_arena_buy_challenge_count(&$params) {
		return "增加挑战次数(前值 {$params[0]} / 后值 {$params[1]}), 消耗元宝 {$params[2]} (前值 {$params[3]} / 后值 {$params[4]})";
	}

	private function _description_arena_daily_reward(&$params) {
		return "排名 {$params[0]} 发送 ${params[1]}";
	}

	private function _description_default(&$proto_name, &$params) {
		$ret = $proto_name;
		$ret .= ' '; 
		foreach($params as $param) {
			$ret .= $param . ' ';
		}

		return $ret;
	}

	public function makelogcfg() {
		$config['hostname'] = '192.168.1.242';
		$config['username'] = 'root';
		$config['password'] = '!Q@W#E$R%T^Y';
		$config['database'] = 'BLCX';
		$config['dbdriver'] = 'mysql';
		$config['dbprefix'] = '';
		$config['pconnect'] = FALSE;
		$config['db_debug'] = TRUE;
		$config['cache_on'] = FALSE;
		$config['cachedir'] = '';
		$config['char_set'] = 'latin1';
		$config['dbcollat'] = 'utf8_general_ci';
		$DB = $this->load->database($config, TRUE);

		// log_goods
		$query = $DB->query("select * from tb_goods");
		$goods = array();
		foreach ($query->result() as $row) {
			$goods["$row->id"] = $row->name;
		}

		$str = json_encode($goods, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_goods.json', $str);

		// log_store
		$query = $DB->query("select * from tb_store");
		$store = array();
		foreach ($query->result() as $row) {
			$store["$row->id"] = $row->goods_id;
		}

		$str = json_encode($store, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_store.json', $str);

		// log_cmpst
		$query = $DB->query("select * from tb_cmpst");
		$store = array();
		foreach ($query->result() as $row) {
			$store["$row->id"] = $row->name;
		}

		$str = json_encode($store, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_cmpst.json', $str);

		// log_reward
		$query = $DB->query("select * from tb_sys_reward");
		$store = array();
		foreach ($query->result() as $row) {
			$store["$row->id"] = $row->name;
		}

		$str = json_encode($store, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_reward.json', $str);

		// log_task
		$query = $DB->query("select * from tb_task");
		$store = array();
		foreach ($query->result() as $row) {
			$task["$row->id"] = $row->title;
		}

		$str = json_encode($task, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_task.json', $str);

		// log_npc
		$query = $DB->query("select * from tb_npc");
		$store = array();
		foreach ($query->result() as $row) {
			$npc["$row->id"] = $row->name;
		}

		$str = json_encode($npc, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_npc.json', $str);

		// log_raid
		$query = $DB->query("select * from tb_raid");
		$store = array();
		foreach ($query->result() as $row) {
			$raid["$row->id"] = $row->name;
		}

		$str = json_encode($raid, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_raid.json', $str);

		// log_raid
		$query = $DB->query("select * from tb_map");
		$store = array();
		foreach ($query->result() as $row) {
			$map["$row->id"] = $row->name;
		}

		$str = json_encode($map, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_map.json', $str);

		// log_raid
		$query = $DB->query("select * from tb_scene");
		$store = array();
		foreach ($query->result() as $row) {
			$scene["$row->id"] = $row->name;
		}

		$str = json_encode($scene, JSON_UNESCAPED_UNICODE);
		file_put_contents('/www/via/application/controllers/log_scene.json', $str);

		$this->output->set_content_type('application/json;charset=utf-8')->set_output("ok");
	}

	/**
	 * 获取日志类型
	 */
	public function getLogType() {
		$ret = $this->backdoor_lib->check_sign($this);
		if (is_string($ret)) {
			$ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
			$this->output->set_output($ret);
			return ;
		}

		$time = $this->input->get('time');
		//if (Util::customer_check_sign($time, $this->input->get('sign'))) {
		$list = array();
		//$this->get_logs($time, $list);
		foreach (self::$log_name as $name) {
			$id = self::$log_id[$name];
			$list[$id]['rootTypeName'] = self::$log_type_name[$id];
			$list[$id]['subTypeList'][self::$log_sub_id[$name]] = self::$log_type[$name];
		}
		$rsp['status'] = 1;
		$rsp['info']   = "";
		$rsp['data']['list'] = $list;
		$this->output->set_content_type('application/json;charset=utf-8');
		if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
			$this->output->set_output(json_encode($rsp, JSON_UNESCAPED_UNICODE));
		} else {
			$this->output->set_output(json_encode($rsp));
		}
	}

	private $column = array(
		'id'          => 'ID',
		'userId'      => '用户ID',
		'eventId'     => '子类型ID' ,
		'nickName'    => '玩家昵称',
		'description' => '描述',
		'objectId'    => '主类型ID',
		'userName'    => '用户名',
		'addTime'     => '操作时间' ,
	);

	/**
	 * 玩家日志查询
	 */
	public function getLogList() {
		$ret = $this->backdoor_lib->check_sign($this);
		if (is_string($ret)) {
			$ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
			$this->output->set_output($ret);
			//return ;
		}

		$user       = $this->input->get_post('user');
		$user_type  = $this->input->get_post('userType');
		$keywords   = $this->input->get_post('keywords');
		$object_id  = $this->input->get_post('objectId');
		$event_id   = $this->input->get_post('eventId');
		$begin_time = $this->input->get_post('beginTime');
		$end_time   = $this->input->get_post('endTime');
		$page       = $this->input->get_post('page');
		$page_size  = $this->input->get_post('pageSize');
		if (!$page_size) {
			$page_size = 30;
		}

		$list = array();
		$this->get_logs($begin_time, $end_time, $list);
		$user_list = array();
		$i = 0;
		$role_id = 0;
		if ($user_type == 0) {
			$role_id = $user;
		} else if ($user_type == 1) {
			$role_id = 0;//Util::open_id_to_role_id($this->db, $json['user']);
		} else if ($user_type == 2) {
			$role_id = $this->game_db->name_to_id($user);
		}

		$size  = 0;
		$count = 0;
		$start = (($page == 0 ? 1 : $page) - 1) * $page_size;
		foreach ($list as $item) {
			++$size;
			if ($size < $start) {
				continue;
			}

			if ($count >= $page_size) {
				break;
			}


			if ($object_id != 0 && $object_id != $item['objectId']) {
				continue;
			}

			if ($event_id != 0 && $event_id!= $item['eventId']) {
				continue;
			}

			if ($role_id == 0) {
				array_push($user_list, $item);
				++$count;
			} else if ($item['userId'] == $role_id) {
				$user_list[$i] = $item;
				++$i;
				++$count;
			} else {
				//var_dump($item);
			}
		}

		$rsp['data']['total']  = count($list);
		$rsp['data']['list']   = $user_list;
		$rsp['data']['column'] = $this->column;
		$rsp['status']         = 1;
		$rsp['info']           = "";
		$this->output->set_content_type('application/json;charset=utf-8');
		if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
			$this->output->set_output(json_encode($rsp, JSON_UNESCAPED_UNICODE));
		} else {
			$this->output->set_output(json_encode($rsp));
		}
	}

	public function loginLog() {
		$rsp  = array();
		$time = $this->input->get('time');
		$sign = $this->input->get('sign');
		$file = $this->input->get('filename');
		if (!isset($time) || !isset($sign) || !isset($file)) {
			$rsp['result'] = 0;
			$rsp['info']   = '参数错误';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		if (!Util::data_center_check_sign($time, $sign)) {
			$rsp['result'] = 0;
			$rsp['info']   = '校验失败';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		$name = explode('_', $file);
		$logname  = 'bill_';
		$logname .= substr($name[1], 0, 4) . '_' . substr($name[1], 4, 2) . '_' . substr($name[1], 6, 2) . '_';
		$logname .= substr($name[2], 0, 2) . '.log';
		$path = $this->base_path . $logname;
		if (!file_exists($path)) {
			$rsp['result'] = 0;
			$rsp['info']   = '找不到文件';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		$filedata = file($path);
		$str = '';
		foreach ($filedata as $item) {
			if (preg_match("/client\.auth_rsp/", $item) != 1) {
				continue;
			}

			$str .= $item;
		}

		$this->rsp($str);
	}

	public function get_log() {
		$time = $this->input->get('time');
		$sign = $this->input->get('sign');
		$file = $this->input->get('filename');
		if (!isset($time) || !isset($sign) || !isset($file)) {
			$rsp['result'] = 0;
			$rsp['info']   = '参数错误';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		if (!Util::data_center_check_sign($time, $sign)) {
			$rsp['result'] = 0;
			$rsp['info']   = '校验失败';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		$path = $this->base_path . $file;
		if (!file_exists($path)) {
			$rsp['result'] = 0;
			$rsp['info']   = '找不到文件';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		$filedata = file($path);
		$proto = $this->input->get('proto');
		$str = "";
		foreach ($filedata as $item) {
			if (isset($proto) && preg_match("/". $proto . "/", $item) != 1) {
				continue;
			}

			$str .= $item;
		}

		$this->rsp($str);
	}

	public function cfg() {
		$rsp = array();
		$time = $this->input->get('time');
		$sign = $this->input->get('sign');
		$log  = $this->input->get('log');
		if (!isset($time) || !isset($sign) || !isset($log)) {
			$rsp['result'] = 0;
			$rsp['info']   = '参数错误';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		if (!Util::data_center_check_sign($this->input->get('time'), $this->input->get('sign'))) {
			$rsp['result'] = 0;
			$rsp['info']   = '校验失败';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		$path = './application/controllers/' . $log;
		if (!file_exists($path)) {
			$rsp['result'] = 0;
			$rsp['info']   = '找不到文件';
			$rsp['data']   = null;
			$this->rsp(json_encode($rsp, JSON_UNESCAPED_UNICODE));
			return ;
		}

		$jsonfile = file_get_contents($path);
		$this->rsp($jsonfile);
	}

	public function rsp($rsp) {
		$this->output->set_content_type($this->content_type)->set_output($rsp);
	}

	public function task() {
		$this->load->database();
		$sql = "select * from tb_task_log where create_date > 1379001600";
		$query = $this->db->query($sql);
		$i = 0;
		$data = array();
		$list = array();
		foreach ($query->result() as $row) {
			$data[$i] = $row->task_id;
			$list[$row->task_id] = 0;
			++$i;
		}

		foreach ($data as $task) {
			++$list[$task];
		}

		$this->rsp(json_encode($list, JSON_UNESCAPED_UNICODE));
	}
}
