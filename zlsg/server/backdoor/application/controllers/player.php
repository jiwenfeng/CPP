<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Player extends CI_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->database('game');
        $this->load->model('backdoor_db', 'game_db');
        $this->load->library('backdoor_lib');
        $this->config->load('backdoor', TRUE);
        $this->backdoor_lib->load_session();
    }

    private function _check_login() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return true;
        }

        return false;
    }

    public function index() {
        if ($this->_check_login()) {
            return ;
        }

        $this->load->view('player');
    }

    public function player_role_to_utf8() {
        return ;

        $table = 'tb_player_role';
        $ret = $this->db->query("set character set 'latin1'");
        $rsp = $this->db->get($table)->result();
        $ret = $this->db->query("set character set 'utf8'");
        foreach ($rsp as $obj) {
            $id = $obj->role_id;
            $name = $obj->role_name;
            $this->db->query("update `$table` SET `role_name`='$name' where (`role_id`='$id')");
        }

        var_dump($table);
    }

    public function city_attack_to_utf8() {
        return ;
        $table = 'tb_city_attack';
        $ret = $this->db->query("set character set 'latin1'");
        $rsp = $this->db->get($table)->result();
        $ret = $this->db->query("set character set 'utf8'");
        foreach ($rsp as $obj) {
            $id = $obj->id;
            $title = $obj->title;
            $this->db->query("update `$table` SET `title`='$title' where (`id`='$id')");
        }

        var_dump($table);
    }

    public function family_to_utf8() {
        return ;
        $table = 'tb_family';
        $ret = $this->db->query("set character set 'latin1'");
        $rsp = $this->db->get($table)->result();
        $ret = $this->db->query("set character set 'utf8'");
        foreach ($rsp as $obj) {
            $id = $obj->id;
            $name = $obj->name;
            $flag = $obj->flag_name;
            $chief = $obj->chief_name;
            $post = $obj->post;
            $this->db->query("update `$table` SET `name`='$name', `flag_name`='$flag', `chief_name` = '$chief', `post`='$post' where (`id`='$id')");
        }

        var_dump($table);
    }

    public function arena_robot_to_utf8() {
        return ;
        $table = 'tb_arena_robot';
        $ret = $this->db->query("set character set 'latin1'");
        $rsp = $this->db->get($table)->result();
        $ret = $this->db->query("set character set 'utf8'");
        foreach ($rsp as $obj) {
            $id = $obj->id;
            $name = $obj->name;
            $this->db->query("update `$table` SET `name`='$name' where (`id`='$id')");
        }

        var_dump($table);
    }

    public function latin1_to_utf8() {
        return ;
        $table = 'tb_newtaskconfig';
        $ret = $this->db->query("set character set 'latin1'");
        $rsp = $this->db->get($table)->result();
        $ret = $this->db->query("set character set 'utf8'");
        foreach ($rsp as $obj) {
            $id = $obj->id;
            $name = $obj->taskname;
            $this->db->query("update `$table` SET `taskname`='$name' where (`id`='$id')");
        }

        var_dump($table);
    }

    public function name()
    {
        $role_name = $this->input->post("role_name");
        $this->output->set_content_type('application/json;charset=utf-8');
        if (!$role_name) {
            $ret = json_encode(array('status' => 0, 'info' => 'role_name can not null', 'data' => null));
            $this->output->set_output($ret);
            return ;
        }

        $this->load->database("game");
        $ret = $this->db->query("set character set 'utf8'");
        $sql = "select tb_player_role.*, tb_player.* from tb_player_role,tb_player where binary role_name like '%$role_name%' and tb_player.id = tb_player_role.player_id";
        var_dump($sql);
        $query = $this->db->query($sql);
        if ($query->num_rows() > 0) {
            $data = array();
            foreach ($query->result_array() as $item) {
                $item['role_name'] = urlencode($item['role_name']);
                $role = array( 'openid' => $item['openid'],
                    'player_id' => $item['player_id'],
                    'role_id' => $item['role_id'],
                    'role_name' => $item['role_name'],
                   );
                array_push($data, $role);
            }

            $ret = urldecode(json_encode(array('status' => 1, 'info' => urlencode('成功'), 'data' => json_encode($data))));
            $this->output->set_output($ret);
        } else {
            $ret = json_encode(array('status' => 0, 'info' => 'role_name error', 'data' => null));
            $this->output->set_output($ret);
        }
    }

    public function role()
    {
        $role_id = $this->input->post("role_id");
        $this->output->set_content_type('application/json;charset=utf-8');
        if (!$role_id) {
            $ret = json_encode(array('status' => 0, 'info' => 'role_id can not null', 'data' => null));
            $this->output->set_output($ret);
            return ;
        }

        $this->load->database("game");
        $sql = "select * from tb_player_role where role_id = '" . $role_id . "'";
        $query = $this->db->query($sql);
        if ($query->num_rows() > 0) {
            foreach ($query->result_array() as $row) {
                $role = $row;
                break;
            }

            $ret = urldecode(json_encode(array('status' => 1, 'info' => urlencode('成功'), 'data' => $role)));
            $this->output->set_output($ret);
        } else {
            $ret = json_encode(array('status' => 0, 'info' => 'role_id error', 'data' => null));
            $this->output->set_output($ret);
        }
    }

    private function _make_query_role($json, &$sql)
    {
        $sql = 'SELECT * FROM tb_player_role ';
        if ($json['user'] && $json['userType'] && strlen($json['user']) > 0) {
            if ($json['userType'] == 0) {
                $sql .= 'WHERE role_id = '   . $json['user'];
            } else if ($json['userType'] == 1) {
                $query = $this->db->query('SELECT * FROM tb_player WHERE openid = "' . $json['user'] . '"');
                foreach ($query->result() as $row) {
                    $sql .= 'WHERE player_id = "' . $row->id . '"';
                    break;
                }

            } else if ($json['userType'] == 2) {
                $sql .= 'WHERE role_name = "' . $json['user'] . '"';
            }
        } else if ($json['regBeginTime'] || $json['regEndTime']) {
            if (strlen($json['regBeginTime']) > 0 && strlen($json['regEndTime']) > 0) {
                $sql .= 'WHERE create_time > ' . $json['regBeginTime'] . ' and create_time <' . $json['regEndTime'];
            } else if (strlen($json['regBeginTime']) > 0) {
                $sql .= 'WHERE create_time > ' . $json['regBeginTime'];
            } else if (strlen($json['regEndTime']) > 0) {
                $sql .= 'WHERE create_time <' . $json['regEndTime'];
            }
        } else if ($json['loginBeginTime'] || $json['loginEndTime']) {
            if (strlen($json['loginBeginTime']) > 0 && strlen($json['loginEndTime']) > 0) {
                $sql .= 'WHERE factor_be_soil >= ' . $json['loginBeginTime'] . ' and factor_be_soil <=' . $json['loginEndTime'];
            } else if (strlen($json['loginBeginTime']) > 0) {
                $sql .= 'WHERE factor_be_soil >= ' . $json['loginBeginTime'];
            } else if (strlen($json['loginEndTime']) > 0) {
                $sql .= 'WHERE factor_be_soil <=' . $json['loginEndTime'];
            }
        }

        if ($json['page'] || $json['pageSize']) {
            $page = 1;
            if (strlen($json['page']) > 0) {
                $page = $json['page'] > 0 ? $json['page'] : 1;
                $page = $json['page'];
            }

            $pageSize = 20;
            if (strlen($json['pageSize']) > 0) {
                $pageSize = $json['pageSize'];
            }

            $start = ($page - 1) * $pageSize;
            //$start = $page * $pageSize;
            $end   = $start + $pageSize;
            $sql  .= ' LIMIT ' . $start . ', ' . $end;
        } else {
            $sql  .= ' LIMIT 0 , 1000';
        }
    }

    public function getPlayerList()
    {
        $this->get_player_list();
    }

    public function get_player_list()
    {
        $ret = $this->backdoor_lib->check_sign($this);
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }

        $param['user']           = $this->input->get_post('user');
        $param['userType']       = $this->input->get_post('userType');
        $param['regBeginTime']   = $this->input->get_post('regBeginTime');
        $param['regEndTime']     = $this->input->get_post('regEndTime');
        $param['loginBeginTime'] = $this->input->get_post('loginBeginTime');
        $param['loginEndTime']   = $this->input->get_post('loginEndTime');
        $param['page']           = $this->input->get_post('page');
        $param['pageSize']       = $this->input->get_post('pageSize');
        $sql = '';
        $this->_make_query_role($param, $sql);
        //$ret = $this->db->query("set character set 'latin1'");
        $query = $this->db->query($sql);
        $total = $query->num_rows();
        $data = array();
        $data['status'] = 1;
        $data['info']   = "0";
        $list = array();
        $this->_make_role_data($query->result(), $list);
        $data['data']['list'] = $list;
        $query = $this->db->query('select count(*) as total from tb_player_role');
        foreach ($query->result() as $row)
        {
            $data['data']['total']  = $row->total;//$total;
        }

        $this->_custom_field_name($data['data']['column']);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($data, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($data));
        }
    }

    public static $profession = array("猛将", "巫神", "术士", "天仙");

    private function _make_role_data ($result, &$list) {
        $i = 0;
        date_default_timezone_set('Asia/Chongqing');
        foreach ($result as $row) {
            $query = $this->db->query('SELECT * FROM tb_player WHERE id = "' . $row->player_id . '"');
            $openid = $row->player_id;
            foreach ($query->result() as $r) {
                $openid = $r->openid;
                break;
            }

            $list[$i]['userID']           = $row->role_id;
            $list[$i]['userName']         = $openid;
            $list[$i]['nickName']         = $row->role_name;
            $list[$i]['createTime']       = date('Y-m-d H:i:s', $row->create_time);
            $list[$i]['loginTime']        = date('Y-m-d H:i:s', $row->online_time);
            $list[$i]['logoutTime']       = date('Y-m-d H:i:s', $row->offline_time);
            $list[$i]['sex']              = $row->sex;
            $list[$i]['profession']       = self::$profession[$row->profession];
            $list[$i]['level']            = $row->level;
            $list[$i]['experience']       = $row->experience;
            $list[$i]['tael']             = $row->tael;
            $list[$i]['ingot']            = $row->ingot;
            $list[$i]['last_update_time'] = date('Y-m-d H:i:s', $row->update_time);
            ++$i;
        }
    }

    private function _custom_field_name(&$colum) {
        $colum['userID']           = '玩家ID';
        $colum['userName']         = '帐号';
        $colum['nickName']         = '角色';
        $colum['createTime']       = '创建时间';
        $colum['loginTime']        = '登陆时间';
        $colum['logoutTime']       = '退出时间';
        $colum['sex']              = '性别';
        $colum['profession']       = '职业';
        $colum['level']            = '等级';
        $colum['experience']       = '经验';
        $colum['tael']             = '铜钱';
        $colum['ingot']            = '元宝';
        $colum['last_update_time'] = '数据更新时间';
    }

    public function online() {
        $time = $this->input->get_post('time');
        /*$ret = $this->backdoor_lib->check_sign($this);
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }*/

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.player.php';

        $req = new \inner\t_online_req();
        $req->set_unixtime(time());
        $sendStr = $this->backdoor_lib->pack("inner.t_online_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        if ($result == FALSE) {
            $ret = array();
            if (!$time) {
                $ret = urldecode(json_encode(array('type' => 1, 'msg' => urlencode('连接游戏服务器失败'), 'data' => NULL)));
            } else {
                $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode('连接游戏服务器失败'), 'data' => NULL)));
            }
            $this->output->set_output($ret);
            return ;
        }

        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\t_online_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();

        if (!$time) {
            $output = array('type' => $tail->status() == 1 ? 0 : 1, 'msg' => $tail->info(), 'data' => $rsp->allcount());
        } else {
            $output = array('result' => $tail->status(), 'info' => $tail->info(), 'data' => $rsp->allcount());
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }  

    public function source() {
        $source = array();
        $this->game_db->user_source($source);
        $output = array('type' => 0, 'msg' => '', 'data' => $source);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }

    public function set_attr() {
        if ($this->_check_login()) {
            return ;
        }

        $role_id = $this->input->get_post('id');
        $index   = $this->input->get_post('index');
        $value   = $this->input->get_post('value');

        if (!$role_id || !$index || !$value) {
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.player.php';

        $req = new \inner\set_attr_req();
        $user = $req->add_user();
        $user->set_id($role_id);
        $req->set_index($index);
        $req->set_value($value);
        $sendStr = $this->backdoor_lib->pack("inner.set_attr_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        if (!$result) {
            return ;
        }

        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\set_attr_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();

        var_dump($rsp);
    }

    public function set_vip() {
        if ($this->_check_login()) {
            return ;
        }

        $role_id = $this->input->get_post('id');
        $lv      = $this->input->get_post('lv');
        $month   = $this->input->get_post('month');

        if (!$role_id || !$lv || !$month) {
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.player.php';

        $req = new \inner\set_vip_req();
        $user = $req->add_user();
        $user->set_id($role_id);
        $req->set_lv($lv);
        $req->set_month($month);
        $sendStr = $this->backdoor_lib->pack("inner.set_vip_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        //$result  = $this->backdoor_lib->send("192.168.23.217", 1289, $sendStr);
        if (!$result) {
            return ;
        }

        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\set_vip_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();

        var_dump($rsp);
    }
}

/* End of file player.php */
/* Location: ./application/controllers/player.php */
