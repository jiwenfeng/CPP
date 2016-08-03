<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Server extends CI_Controller {
    public function __construct() 
    {
        parent::__construct();
        $this->load->helper('url');
        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
    }

    private function _get_pid() {
        $test = "ps aux | grep 'lzsgserver' | grep -v grep | awk '{print $2}'";
        $pid = exec($test);
        return $pid;
    }

    private function output() {
        $pid = $this->_get_pid();
        if ($pid != "") {
            $status = "运行中...";
        } else {
            $status = "已关闭!!!";
        }

        $data['status'] = $status;
        $data['pid']    = $pid;
        $this->load->view('server', $data);
    }

    public function index() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->output();
    }

    public function start() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $pid = $this->_get_pid();
        if ($pid == "") {
            $test = "cd /home/zlsg;./restart.sh >/dev/null &";
            exec($test);
        }

        //$test = "cd /home/rockpile/server/bin;./restart.sh >/dev/null &";
        //$this->output->set_output($test);

        /*exec($test,$array);
        $this->output->set_output(json_encode($array));*/
        /*$fp = popen($test, "r");  //popen打一个进程通道

        while (!feof($fp)) {      //从通道里面取得东西
            $out = fgets($fp, 4096);
            $this->output->set_output($out);         //打印出来
        }

        pclose($fp);*/

        /*$array = array(
            array("pipe", "r"),   //标准输入
            array("pipe", "w"),   //标准输出内容
            array("pipe", "w")    //标准输出错误
        );

        $fp = proc_open($test, $array, $pipes);   //打开一个进程通道
        $this->output->set_output(stream_get_contents($pipes[1]));    //为什么是$pipes[1]，因为1是输出内容
        proc_close($fp);*/

        sleep(10);

        //$this->output();
        redirect('/server/index', 'refresh');
    }

    public function close() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/class/Util.class.php';
        $req = new inner\close_server();
        $req->set_unixtime(0);

        $data   = Util::Pack("inner.close_server", $req->SerializeToString());
        $result = Util::writeAndSend($data);

        //$this->output->set_output($req->toJson());
        sleep(10);

        redirect('/server/index', 'refresh');
        //$this->output();
    }

    public function status() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }


        $this->load->view('server_status');
    }

    public function server_list() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->load->database('server_list');
        $ret = $this->db->get('server_list');
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret->result_array()));
    }

    private function _rsp($ret, $info, $data) {
        $rsp = array(
            'ret' => $ret,
            'info' => $info,
            'data' => $data
        );
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function remote_on() {
        $key  = $this->input->get_post('key');
        $time = $this->input->get_post('time');
        $id   = $this->input->get_post('id');
        $path = $this->input->get_post('path');

        if ($key == md5('r0ckpi1e@server' . $path . $time . $id)) {
            return $this->remote_on_helper($path);
        }

        return 'key error';
    }

    private function remote_on_req($id, $path, $url) {
        $time = time();
        $key  = md5('r0ckpi1e@server' . $path . $time . $id);
        $arg = array(
            'key' => $key,
            'time' => $time,
            'id' => $id,
            'path' => $path,
        );
        $url_arg = http_build_query($arg, NULL, '&');
        $path = $url . '/backdoor/index.php?/server/remote_on?' . $url_arg;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $path);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        $ret = curl_exec($ch);
        curl_close($ch);

        return $ret;
    }

    private function remote_on_helper($path) {
        if (is_dir($path)) {
            $shell = "cd $path;./restart.sh > /dev/null &";
            $data = exec($shell);
            $data = 'server on';
        } else {
            $data = 'server_list path error';
        }

        return $data;
    }

    public function on() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }


        $id = $this->input->get_post('id');
        if (!$id) {
            $this->_rsp(1, 'ID error', $id);
            //return ;
        }

        $this->load->database('server_list');
        $ret = $this->db->get_where('server_list', array('id' => $id));

        $data = "";
        foreach ($ret->result_array() as $row) {
            $path = $row['path'];
            $pid = $this->_is_on($path);
            if ($pid == "") {
                if (is_dir($path)) {
                    $shell = "cd $path;./restart.sh > /dev/null &";
                    $data = exec($shell);
                } else {
                    $data = $this->remote_on_req($id, $path, $row['web']);
                }

                break;
            }
        }

        $this->_rsp(0, 'success', $data);
    }

    private function _is_on($path) {
        $test = "cat $path/server.pid";
        $pid = exec($test);
        return $pid;
    }

    public function off() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id = $this->input->get_post('id');
        if (!$id) {
            $this->_rsp(1, 'ID error', $id);
            //return ;
        }

        $this->load->database('server_list');
        $ret = $this->db->get_where('server_list', array('id' => $id));

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';

        foreach ($ret->result_array() as $row) {
            $req = new inner\close_server();
            $req->set_unixtime(0);

            $data   = $this->backdoor_lib->pack("inner.close_server", $req->SerializeToString());
            $result = $this->backdoor_lib->Send($row['ip'], $row['inner_port'], $data);
            break;
        }

        $this->_rsp(0, 'success', $id);
    }

    private function _get_user($row) {
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.player.php';

        $req = new \inner\t_online_req();
        $req->set_unixtime(time());
        $sendStr = $this->backdoor_lib->pack("inner.t_online_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->Send($row['ip'], $row['inner_port'], $sendStr);
        if (!$result) {
            return array('ret' => false, 'data' => 0);
        }

        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\t_online_rsp();
        $rsp->ParseFromString($result);

        return array('ret' => true, 'data' => $rsp->allcount());
    }

    public function get() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id = $this->input->get_post('id');
        if (!$id) {
            $id = 0;
            $this->_rsp(1, 'ID error', $id);
            //return ;
        }

        $this->load->database('server_list');
        $ret = $this->db->get_where('server_list', array('id' => $id));


        $data = array();

        foreach ($ret->result_array() as $row) {
            $path = $row['path'];
            $pid = $this->_is_on($path);

            $a = $this->_get_user($row);

            $data['user'] = $a['data'];
            if ($a['ret']) {
                $data['status'] = '运行中... ';
                if ($pid != "") {
                    $data['status'] .= 'pid: ' . $pid;
                }
            } else {
                $data['status'] = '关闭';
            }

            break;
        }

        $this->_rsp(0, 'success', $data);
    }

    private function get_role_data($role, $tb) {
        $data = array();
        $ret = $this->db->get_where($tb, array('role_id' => $role->role_id));
        foreach ($ret->result_array() as $row) {
            array_push($data , $row);
        }

        return $data;
    }

    private function set_role_data($role_id, $max_role_id, $data, $tb) {
        if (!$data || !isset($data[$role_id])) {
            echo $tb . " $role_id " . '</br>';
            return ;
        }

        foreach ($data[$role_id] as $row) {
            $row['role_id'] = $max_role_id;
            $this->db->insert($tb, $row);
        }
    }

    public function remote_copy_roles() {
        $openid              = $this->input->get_post('oid'                );
        $all                 = $this->input->get_post('all'                );
        $roles               = unserialize(base64_decode($this->input->get_post('roles'              )));
        $role_status         = unserialize(base64_decode($this->input->get_post('role_status'        )));
        $role_skill          = unserialize(base64_decode($this->input->get_post('role_skill'         )));
        $role_props          = unserialize(base64_decode($this->input->get_post('role_props'         )));
        $role_mail           = unserialize(base64_decode($this->input->get_post('role_mail'          )));
        $role_task           = unserialize(base64_decode($this->input->get_post('role_task'          )));
        $role_pet            = unserialize(base64_decode($this->input->get_post('role_pet'           )));
        $role_label          = unserialize(base64_decode($this->input->get_post('role_label'         )));
        $role_center_set     = unserialize(base64_decode($this->input->get_post('role_center_set'    )));
        $role_explore        = unserialize(base64_decode($this->input->get_post('role_explore'       )));
        $role_explore_detail = unserialize(base64_decode($this->input->get_post('role_explore_detail')));
        $role_set            = unserialize(base64_decode($this->input->get_post('role_set'           )));
        $role_buffer         = unserialize(base64_decode($this->input->get_post('role_buffer'        )));
        $role_raid           = unserialize(base64_decode($this->input->get_post('role_raid'          )));
        $role_order          = unserialize(base64_decode($this->input->get_post('role_order'         )));
        $role_statis         = unserialize(base64_decode($this->input->get_post('role_statis'        )));
        $role_ext_attr       = unserialize(base64_decode($this->input->get_post('role_ext_attr'      )));
        $role_mounts         = unserialize(base64_decode($this->input->get_post('role_mounts'        )));
        $role_potential      = unserialize(base64_decode($this->input->get_post('role_potential'     )));
        $role_auto_raid      = unserialize(base64_decode($this->input->get_post('role_auto_raid'     )));
        $role_cd_cooldown    = unserialize(base64_decode($this->input->get_post('role_cd_cooldown'   )));
        $role_treasure       = unserialize(base64_decode($this->input->get_post('role_treasure'      )));
        $role_target         = unserialize(base64_decode($this->input->get_post('role_target'        )));
        $role_vip            = unserialize(base64_decode($this->input->get_post('role_vip'           )));
        $role_lottery        = unserialize(base64_decode($this->input->get_post('role_lottery'       )));
        $role_black_shop     = unserialize(base64_decode($this->input->get_post('role_black_shop'    )));
        $role_levels         = unserialize(base64_decode($this->input->get_post('role_levels'        )));
        $role_gem            = unserialize(base64_decode($this->input->get_post('role_gem'           )));
        $role_bows           = unserialize(base64_decode($this->input->get_post('role_bows'          )));
        $role_cycle          = unserialize(base64_decode($this->input->get_post('role_cycle'         )));
        $role_devil          = unserialize(base64_decode($this->input->get_post('role_devil'         )));

        $this->load->database('game');
        $tb_player      = 'tb_player';
        $tb_player_role = 'tb_player_role';
        $tb_role_status = 'tb_role_status';
        $tb_role_skill  = 'tb_role_skill';
        $tb_role_props  = 'tb_role_props';
        $tb_role_mail = 'tb_role_mail';
        $tb_role_task = 'tb_role_task';
        $tb_role_pet = 'tb_role_pet';
        $tb_role_label = 'tb_role_label';
        $tb_role_center_set = 'tb_role_center_set';
        $tb_role_explore = 'tb_role_explore'; 
        $tb_role_explore_detail = 'tb_role_explore_detail';
        $tb_role_set = 'tb_role_set';
        $tb_role_buffer = 'tb_role_buffer';
        $tb_role_raid = 'tb_role_raid';
        $tb_role_order = 'tb_role_order';
        $tb_role_statis = 'tb_role_statis';
        $tb_role_ext_attr = 'tb_role_ext_attr';
        $tb_role_mounts = 'tb_role_mounts';
        $tb_role_potential = 'tb_role_potential';
        $tb_role_auto_raid = 'tb_role_auto_raid';
        $tb_role_cd_cooldown = 'tb_role_cd_cooldown';
        $tb_role_treasure = 'tb_role_treasure';
        $tb_role_target = 'tb_role_target';
        $tb_role_vip = 'tb_role_vip';
        $tb_role_lottery = 'tb_role_lottery';
        $tb_role_black_shop = 'tb_role_black_shop';
        $tb_role_levels = 'tb_role_levels';
        $tb_role_gem = 'tb_role_gem';
        $tb_role_bows = 'tb_role_bows';
        $tb_role_cycle = 'tb_role_cycle';
        $tb_role_devil = 'tb_role_devil';

        $ret = $this->db->query("select max(role_id) as role_id from $tb_player_role");
        $max_role_id = 0;
        foreach ($ret->result() as $row) {
            $max_role_id = $row->role_id;
        }

        $ret = $this->db->query("select max(id) as player_id from $tb_player");
        $max_player_id = 0;
        foreach ($ret->result() as $row) {
            $max_player_id = $row->player_id;
        }

        //var_dump($all);
        //var_dump($roles);
        //var_dump($role_props);
        //return ;

        for ($index = 0; $index < $all; ++$index) {
            ++$max_player_id;
            $player['id']     = $max_player_id;
            $player['openid'] = $openid . '_' . $index;
            $ret = $this->db->insert($tb_player, $player);
            if (!$ret) {
                var_dump($ret);
                continue;
            }

            foreach ($roles as $role) {
                $role_id = $role['role_id'];
                ++$max_role_id;
                $role['role_id']   = $max_role_id;
                $role['player_id'] = $max_player_id;
                $role['role_name'] = $role['role_name'] . "_$index";
                $this->db->insert($tb_player_role, $role);

                $this->set_role_data($role_id, $max_role_id, $role_status, $tb_role_status);
                $this->set_role_data($role_id, $max_role_id, $role_skill, $tb_role_skill);
                $this->set_role_data($role_id, $max_role_id, $role_props, $tb_role_props);
                $this->set_role_data($role_id, $max_role_id, $role_mail, $tb_role_mail);
                $this->set_role_data($role_id, $max_role_id, $role_task, $tb_role_task);
                $this->set_role_data($role_id, $max_role_id, $role_pet, $tb_role_pet);
                $this->set_role_data($role_id, $max_role_id, $role_label, $tb_role_label);
                $this->set_role_data($role_id, $max_role_id, $role_center_set, $tb_role_center_set);
                $this->set_role_data($role_id, $max_role_id, $role_explore, $tb_role_explore);
                $this->set_role_data($role_id, $max_role_id, $role_explore_detail, $tb_role_explore_detail);
                $this->set_role_data($role_id, $max_role_id, $role_set, $tb_role_set);
                $this->set_role_data($role_id, $max_role_id, $role_buffer, $tb_role_buffer);
                $this->set_role_data($role_id, $max_role_id, $role_raid, $tb_role_raid);
                $this->set_role_data($role_id, $max_role_id, $role_order, $tb_role_order);
                $this->set_role_data($role_id, $max_role_id, $role_statis, $tb_role_statis);
                $this->set_role_data($role_id, $max_role_id, $role_ext_attr, $tb_role_ext_attr);
                $this->set_role_data($role_id, $max_role_id, $role_mounts, $tb_role_mounts);
                $this->set_role_data($role_id, $max_role_id, $role_potential, $tb_role_potential);
                $this->set_role_data($role_id, $max_role_id, $role_auto_raid, $tb_role_auto_raid);
                $this->set_role_data($role_id, $max_role_id, $role_cd_cooldown, $tb_role_cd_cooldown);
                $this->set_role_data($role_id, $max_role_id, $role_treasure, $tb_role_treasure);
                $this->set_role_data($role_id, $max_role_id, $role_target, $tb_role_target);
                $this->set_role_data($role_id, $max_role_id, $role_vip, $tb_role_vip);
                $this->set_role_data($role_id, $max_role_id, $role_lottery, $tb_role_lottery);
                $this->set_role_data($role_id, $max_role_id, $role_black_shop, $tb_role_black_shop);
                $this->set_role_data($role_id, $max_role_id, $role_levels, $tb_role_levels);
                $this->set_role_data($role_id, $max_role_id, $role_gem, $tb_role_gem);
                $this->set_role_data($role_id, $max_role_id, $role_bows, $tb_role_bows);
                $this->set_role_data($role_id, $max_role_id, $role_cycle, $tb_role_cycle);
                $this->set_role_data($role_id, $max_role_id, $role_devil, $tb_role_devil);
            }
        }
    }

    public function copy_role_to_remote() {
        $openid = $this->input->get_post('oid');
        $fsid   = $this->input->get_post('fsid');
        $tsid   = $this->input->get_post('tsid');
        $all    = $this->input->get_post('all');
        
        $params = array(
            'oid' => $openid,
            'sid' => $tsid,
            'all' => $all,
        );

        $this->load->database('game');
        $tb_player      = 'tb_player';
        $tb_player_role = 'tb_player_role';
        $tb_role_status = 'tb_role_status';
        $tb_role_skill  = 'tb_role_skill';
        $tb_role_props  = 'tb_role_props';
        $tb_role_mail = 'tb_role_mail';
        $tb_role_task = 'tb_role_task';
        $tb_role_pet = 'tb_role_pet';
        $tb_role_label = 'tb_role_label';
        $tb_role_center_set = 'tb_role_center_set';
        $tb_role_explore = 'tb_role_explore'; 
        $tb_role_explore_detail = 'tb_role_explore_detail';
        $tb_role_set = 'tb_role_set';
        $tb_role_buffer = 'tb_role_buffer';
        $tb_role_raid = 'tb_role_raid';
        $tb_role_order = 'tb_role_order';
        $tb_role_statis = 'tb_role_statis';
        $tb_role_ext_attr = 'tb_role_ext_attr';
        $tb_role_mounts = 'tb_role_mounts';
        $tb_role_potential = 'tb_role_potential';
        $tb_role_auto_raid = 'tb_role_auto_raid';
        $tb_role_cd_cooldown = 'tb_role_cd_cooldown';
        $tb_role_treasure = 'tb_role_treasure';
        $tb_role_target = 'tb_role_target';
        $tb_role_vip = 'tb_role_vip';
        $tb_role_lottery = 'tb_role_lottery';
        $tb_role_black_shop = 'tb_role_black_shop';
        $tb_role_levels = 'tb_role_levels';
        $tb_role_gem   = 'tb_role_gem';
        $tb_role_bows  = 'tb_role_bows';
        $tb_role_cycle = 'tb_role_cycle';
        $tb_role_devil = 'tb_role_devil';

        $players = $this->db->get_where($tb_player, array('openid' => $openid));
        foreach ($players->result_array() as $player) {
            if ($player['svr_id'] != $fsid) {
                continue;
            }

            $roles = $this->db->get_where($tb_player_role, array('player_id' => $player['id']));

            $role_status         =  array();
            $role_skill          =  array();
            $role_props          =  array();
            $role_mail           =  array();
            $role_task           =  array();
            $role_pet            =  array();
            $role_label          =  array();
            $role_center_set     =  array();
            $role_explore        =  array();
            $role_explore_detail =  array();
            $role_set            =  array();
            $role_buffer         =  array();
            $role_raid           =  array();
            $role_order          =  array();
            $role_statis         =  array();
            $role_ext_attr       =  array();
            $role_mounts         =  array();
            $role_potential      =  array();
            $role_auto_raid      =  array();
            $role_cd_cooldown    =  array();
            $role_treasure       =  array();
            $role_target         =  array();
            $role_vip            =  array();
            $role_lottery        =  array();
            $role_black_shop     =  array();
            $role_levels         =  array();
            $role_gem            =  array();
            $role_bows           =  array();
            $role_cycle          =  array();
            $role_devil          =  array();

            foreach ($roles->result() as $role) {
                $role_status        [$role->role_id] = $this->get_role_data($role, $tb_role_status        );
                $role_skill         [$role->role_id] = $this->get_role_data($role, $tb_role_skill         );
                $role_props         [$role->role_id] = $this->get_role_data($role, $tb_role_props         );
                $role_mail          [$role->role_id] = $this->get_role_data($role, $tb_role_mail          );
                $role_task          [$role->role_id] = $this->get_role_data($role, $tb_role_task          );
                $role_pet           [$role->role_id] = $this->get_role_data($role, $tb_role_pet           );
                $role_label         [$role->role_id] = $this->get_role_data($role, $tb_role_label         );
                $role_center_set    [$role->role_id] = $this->get_role_data($role, $tb_role_center_set    );
                $role_explore       [$role->role_id] = $this->get_role_data($role, $tb_role_explore       );
                $role_explore_detail[$role->role_id] = $this->get_role_data($role, $tb_role_explore_detail);
                $role_set           [$role->role_id] = $this->get_role_data($role, $tb_role_set           );
                $role_buffer        [$role->role_id] = $this->get_role_data($role, $tb_role_buffer        );
                $role_raid          [$role->role_id] = $this->get_role_data($role, $tb_role_raid          );
                $role_order         [$role->role_id] = $this->get_role_data($role, $tb_role_order         );
                $role_statis        [$role->role_id] = $this->get_role_data($role, $tb_role_statis        );
                $role_ext_attr      [$role->role_id] = $this->get_role_data($role, $tb_role_ext_attr      );
                $role_mounts        [$role->role_id] = $this->get_role_data($role, $tb_role_mounts        );
                $role_potential     [$role->role_id] = $this->get_role_data($role, $tb_role_potential     );
                $role_auto_raid     [$role->role_id] = $this->get_role_data($role, $tb_role_auto_raid     );
                $role_cd_cooldown   [$role->role_id] = $this->get_role_data($role, $tb_role_cd_cooldown   );
                $role_treasure      [$role->role_id] = $this->get_role_data($role, $tb_role_treasure      );
                $role_target        [$role->role_id] = $this->get_role_data($role, $tb_role_target        );
                $role_vip           [$role->role_id] = $this->get_role_data($role, $tb_role_vip           );
                $role_lottery       [$role->role_id] = $this->get_role_data($role, $tb_role_lottery       );
                $role_black_shop    [$role->role_id] = $this->get_role_data($role, $tb_role_black_shop    );
                $role_levels        [$role->role_id] = $this->get_role_data($role, $tb_role_levels        );
                $role_gem           [$role->role_id] = $this->get_role_data($role, $tb_role_gem           );
                $role_bows          [$role->role_id] = $this->get_role_data($role, $tb_role_bows          );
                $role_cycle         [$role->role_id] = $this->get_role_data($role, $tb_role_cycle         );
                $role_devil         [$role->role_id] = $this->get_role_data($role, $tb_role_devil         );
            }

            $params['roles'                          ] = base64_encode(serialize($roles->result_array()));
            $params['role_status'                    ] = base64_encode(serialize($role_status        ));
            $params['role_skill'                     ] = base64_encode(serialize($role_skill         ));
            $params['role_props'                     ] = base64_encode(serialize($role_props         ));
            $params['role_mail'                      ] = base64_encode(serialize($role_mail          ));
            $params['role_task'                      ] = base64_encode(serialize($role_task          ));
            $params['role_pet'                       ] = base64_encode(serialize($role_pet           ));
            $params['role_label'                     ] = base64_encode(serialize($role_label         ));
            $params['role_center_set'                ] = base64_encode(serialize($role_center_set    ));
            $params['role_explore'                   ] = base64_encode(serialize($role_explore       ));
            $params['role_explore_detail'            ] = base64_encode(serialize($role_explore_detail));
            $params['role_set'                       ] = base64_encode(serialize($role_set           ));
            $params['role_buffer'                    ] = base64_encode(serialize($role_buffer        ));
            $params['role_raid'                      ] = base64_encode(serialize($role_raid          ));
            $params['role_order'                     ] = base64_encode(serialize($role_order         ));
            $params['role_statis'                    ] = base64_encode(serialize($role_statis        ));
            $params['role_ext_attr'                  ] = base64_encode(serialize($role_ext_attr      ));
            $params['role_mounts'                    ] = base64_encode(serialize($role_mounts        ));
            $params['role_potential'                 ] = base64_encode(serialize($role_potential     ));
            $params['role_auto_raid'                 ] = base64_encode(serialize($role_auto_raid     ));
            $params['role_cd_cooldown'               ] = base64_encode(serialize($role_cd_cooldown   ));
            $params['role_treasure'                  ] = base64_encode(serialize($role_treasure      ));
            $params['role_target'                    ] = base64_encode(serialize($role_target        ));
            $params['role_vip'                       ] = base64_encode(serialize($role_vip           ));
            $params['role_lottery'                   ] = base64_encode(serialize($role_lottery       ));
            $params['role_black_shop'                ] = base64_encode(serialize($role_black_shop    ));
            $params['role_levels'                    ] = base64_encode(serialize($role_levels        ));
            $params['role_gem'                       ] = base64_encode(serialize($role_gem           ));
            $params['role_bows'                      ] = base64_encode(serialize($role_bows          ));
            $params['role_cycle'                     ] = base64_encode(serialize($role_cycle         ));
            $params['role_devil'                     ] = base64_encode(serialize($role_devil         ));

            break;
        }

        $db   = $this->load->database('server_list', TRUE);
        $ret  = $db->get_where('server_list', array('id' => $tsid));
        $path = '';
        foreach ($ret->result() as $row) {
            $path = $row->web;
        }

        $path .= '/backdoor/index.php?/server/remote_copy_roles';

        //$fields = json_encode($params);
        //var_dump($fields);
        $ch = curl_init();
        $options = array(
            //CURLOPT_HEADER => false,
            //CURLOPT_RETURNTRANSFER => true,
            //CURLOPT_TIMEOUT => 2,
            CURLOPT_URL => $path,
            //CURLOPT_CUSTOMREQUEST => 'POST',
            CURLOPT_POST => true,
            CURLOPT_POSTFIELDS => http_build_query($params),
            //CURLOPT_POSTFIELDS => $params,
            //CURLOPT_POSTFIELDS => json_encode($params),
        ); 
        @curl_setopt_array($ch, $options); 
        $ret = curl_exec($ch);
        curl_close($ch);
    }

    public function copy_user() {
        $openid = $this->input->get_post('oid');
        $sid    = $this->input->get_post('sid');
        $all    = $this->input->get_post('all');

        $this->load->database('game');
        $tb_player      = 'tb_player';
        $tb_player_role = 'tb_player_role';
        $tb_role_status = 'tb_role_status';
        $tb_role_skill  = 'tb_role_skill';
        $tb_role_props  = 'tb_role_props';
        $tb_role_mail = 'tb_role_mail';
        $tb_role_task = 'tb_role_task';
        $tb_role_pet = 'tb_role_pet';
        $tb_role_label = 'tb_role_label';
        $tb_role_center_set = 'tb_role_center_set';
        $tb_role_explore = 'tb_role_explore'; 
        $tb_role_explore_detail = 'tb_role_explore_detail';
        $tb_role_set = 'tb_role_set';
        $tb_role_buffer = 'tb_role_buffer';
        $tb_role_raid = 'tb_role_raid';
        $tb_role_order = 'tb_role_order';
        $tb_role_statis = 'tb_role_statis';
        $tb_role_ext_attr = 'tb_role_ext_attr';
        $tb_role_mounts = 'tb_role_mounts';
        $tb_role_potential = 'tb_role_potential';
        $tb_role_auto_raid = 'tb_role_auto_raid';
        $tb_role_cd_cooldown = 'tb_role_cd_cooldown';
        $tb_role_treasure = 'tb_role_treasure';
        $tb_role_target = 'tb_role_target';
        $tb_role_vip = 'tb_role_vip';
        $tb_role_lottery = 'tb_role_lottery';
        $tb_role_black_shop = 'tb_role_black_shop';
        $tb_role_levels = 'tb_role_levels';
        $tb_role_gem = 'tb_role_gem';
        $tb_role_bows = 'tb_role_bows';
        $tb_role_cycle = 'tb_role_cycle';
        $tb_role_devil = 'tb_role_devil';

        $ret = $this->db->query("select max(role_id) as role_id from $tb_player_role");
        $max_role_id = 0;
        foreach ($ret->result() as $row) {
            $max_role_id = $row->role_id;
        }

        if ($max_role_id == 0) {
            $this->_rsp(1, 'error role_id', $max_role_id);
            return ;
        }

        $ret = $this->db->query("select max(id) as player_id from $tb_player");
        $max_player_id = 0;
        foreach ($ret->result() as $row) {
            $max_player_id = $row->player_id;
        }

        if ($max_player_id == 0) {
            $this->_rsp(1, 'error player_id', $max_player_id);
            return ;
        }

        $index = 0;
        $players = $this->db->get_where($tb_player, array('openid' => $openid));
        foreach ($players->result_array() as $player) {
            if ($player['svr_id'] != $sid) {
                continue;
            }

            $roles = $this->db->get_where($tb_player_role, array('player_id' => $player['id']));

            $role_status         =  array();
            $role_skill          =  array();
            $role_props          =  array();
            $role_mail           =  array();
            $role_task           =  array();
            $role_pet            =  array();
            $role_label          =  array();
            $role_center_set     =  array();
            $role_explore        =  array();
            $role_explore_detail =  array();
            $role_set            =  array();
            $role_buffer         =  array();
            $role_raid           =  array();
            $role_order          =  array();
            $role_statis         =  array();
            $role_ext_attr       =  array();
            $role_mounts         =  array();
            $role_potential      =  array();
            $role_auto_raid      =  array();
            $role_cd_cooldown    =  array();
            $role_treasure       =  array();
            $role_target         =  array();
            $role_vip            =  array();
            $role_lottery        =  array();
            $role_black_shop     =  array();
            $role_levels         =  array();
            $role_gem            =  array();
            $role_bows           =  array();
            $role_cycle          =  array();
            $role_devil          =  array();

            foreach ($roles->result() as $role) {
                $role_status        [$role->role_id] = $this->get_role_data($role, $tb_role_status        );
                $role_skill         [$role->role_id] = $this->get_role_data($role, $tb_role_skill         );
                $role_props         [$role->role_id] = $this->get_role_data($role, $tb_role_props         );
                $role_mail          [$role->role_id] = $this->get_role_data($role, $tb_role_mail          );
                $role_task          [$role->role_id] = $this->get_role_data($role, $tb_role_task          );
                $role_pet           [$role->role_id] = $this->get_role_data($role, $tb_role_pet           );
                $role_label         [$role->role_id] = $this->get_role_data($role, $tb_role_label         );
                $role_center_set    [$role->role_id] = $this->get_role_data($role, $tb_role_center_set    );
                $role_explore       [$role->role_id] = $this->get_role_data($role, $tb_role_explore       );
                $role_explore_detail[$role->role_id] = $this->get_role_data($role, $tb_role_explore_detail);
                $role_set           [$role->role_id] = $this->get_role_data($role, $tb_role_set           );
                $role_buffer        [$role->role_id] = $this->get_role_data($role, $tb_role_buffer        );
                $role_raid          [$role->role_id] = $this->get_role_data($role, $tb_role_raid          );
                $role_order         [$role->role_id] = $this->get_role_data($role, $tb_role_order         );
                $role_statis        [$role->role_id] = $this->get_role_data($role, $tb_role_statis        );
                $role_ext_attr      [$role->role_id] = $this->get_role_data($role, $tb_role_ext_attr      );
                $role_mounts        [$role->role_id] = $this->get_role_data($role, $tb_role_mounts        );
                $role_potential     [$role->role_id] = $this->get_role_data($role, $tb_role_potential     );
                $role_auto_raid     [$role->role_id] = $this->get_role_data($role, $tb_role_auto_raid     );
                $role_cd_cooldown   [$role->role_id] = $this->get_role_data($role, $tb_role_cd_cooldown   );
                $role_treasure      [$role->role_id] = $this->get_role_data($role, $tb_role_treasure      );
                $role_target        [$role->role_id] = $this->get_role_data($role, $tb_role_target        );
                $role_vip           [$role->role_id] = $this->get_role_data($role, $tb_role_vip           );
                $role_lottery       [$role->role_id] = $this->get_role_data($role, $tb_role_lottery       );
                $role_black_shop    [$role->role_id] = $this->get_role_data($role, $tb_role_black_shop    );
                $role_levels        [$role->role_id] = $this->get_role_data($role, $tb_role_levels        );
                $role_gem           [$role->role_id] = $this->get_role_data($role, $tb_role_gem           );
                $role_bows          [$role->role_id] = $this->get_role_data($role, $tb_role_bows          );
                $role_cycle         [$role->role_id] = $this->get_role_data($role, $tb_role_cycle         );
                $role_devil         [$role->role_id] = $this->get_role_data($role, $tb_role_devil         );
            }

            for ($index = 1; $index < $all; ++$index) {
                ++$max_player_id;
                $player['id']     = $max_player_id;
                $player['openid'] = $openid . '_' . $index;
                $ret = $this->db->insert($tb_player, $player);
                if (!$ret) {
                    continue;
                }

                foreach ($roles->result_array() as $role) {
                    $role_id = $role['role_id'];
                    ++$max_role_id;
                    $role['role_id']   = $max_role_id;
                    $role['player_id'] = $max_player_id;
                    $role['role_name'] = $role['role_name'] . "_$index";
                    $this->db->insert($tb_player_role, $role);

                    $this->set_role_data($role_id, $max_role_id, $role_status, $tb_role_status);
                    $this->set_role_data($role_id, $max_role_id, $role_skill, $tb_role_skill);
                    $this->set_role_data($role_id, $max_role_id, $role_props, $tb_role_props);
                    $this->set_role_data($role_id, $max_role_id, $role_mail, $tb_role_mail);
                    $this->set_role_data($role_id, $max_role_id, $role_task, $tb_role_task);
                    $this->set_role_data($role_id, $max_role_id, $role_pet, $tb_role_pet);
                    $this->set_role_data($role_id, $max_role_id, $role_label, $tb_role_label);
                    $this->set_role_data($role_id, $max_role_id, $role_center_set, $tb_role_center_set);
                    $this->set_role_data($role_id, $max_role_id, $role_explore, $tb_role_explore);
                    $this->set_role_data($role_id, $max_role_id, $role_explore_detail, $tb_role_explore_detail);
                    $this->set_role_data($role_id, $max_role_id, $role_set, $tb_role_set);
                    $this->set_role_data($role_id, $max_role_id, $role_buffer, $tb_role_buffer);
                    $this->set_role_data($role_id, $max_role_id, $role_raid, $tb_role_raid);
                    $this->set_role_data($role_id, $max_role_id, $role_order, $tb_role_order);
                    $this->set_role_data($role_id, $max_role_id, $role_statis, $tb_role_statis);
                    $this->set_role_data($role_id, $max_role_id, $role_ext_attr, $tb_role_ext_attr);
                    $this->set_role_data($role_id, $max_role_id, $role_mounts, $tb_role_mounts);
                    $this->set_role_data($role_id, $max_role_id, $role_potential, $tb_role_potential);
                    $this->set_role_data($role_id, $max_role_id, $role_auto_raid, $tb_role_auto_raid);
                    $this->set_role_data($role_id, $max_role_id, $role_cd_cooldown, $tb_role_cd_cooldown);
                    $this->set_role_data($role_id, $max_role_id, $role_treasure, $tb_role_treasure);
                    $this->set_role_data($role_id, $max_role_id, $role_target, $tb_role_target);
                    $this->set_role_data($role_id, $max_role_id, $role_vip, $tb_role_vip);
                    $this->set_role_data($role_id, $max_role_id, $role_lottery, $tb_role_lottery);
                    $this->set_role_data($role_id, $max_role_id, $role_black_shop, $tb_role_black_shop);
                    $this->set_role_data($role_id, $max_role_id, $role_levels, $tb_role_levels);
                    $this->set_role_data($role_id, $max_role_id, $role_gem, $tb_role_gem);
                    $this->set_role_data($role_id, $max_role_id, $role_bows, $tb_role_bows);
                    $this->set_role_data($role_id, $max_role_id, $role_cycle, $tb_role_cycle);
                    $this->set_role_data($role_id, $max_role_id, $role_devil, $tb_role_devil);
                }
            }
        }
    }
}

/* End of file server.php */
/* Location: ./application/controllers/server.php */
