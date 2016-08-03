<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

/**
 * 玩家封号/禁言接口
 */
class Restict extends CI_Controller {
    function __construct() {
        parent::__construct();
        //require_once dirname(dirname(__FILE__)) . '/include/class/Util.class.php';
        /*require_once '/www/via/application/include/message/pb_message.php';
        require_once '/www/via/application/include/proto/pb2php/pb_proto_inner.comm.php';
        require_once '/www/via/application/include/proto/pb2php/pb_proto_inner.restict.php';*/
        $this->config->load('backdoor', TRUE);
        $game_db_name = $this->config->item('game_db_name', 'backdoor');
        $this->load->database($game_db_name);
        $this->load->model('backdoor_db', 'game_db');
        $this->load->library('backdoor_lib');
    }

    /**
     * 删除封号/禁言
     */
    public function delPlayerRestict() {
        $ret = $this->backdoor_lib->check_sign($this);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }

        $type = $this->input->get_post('type');
        $user = $this->input->get_post('user');
        $user_type = $this->input->get_post('userType');

        $roles = array();
        $this->game_db->to_roles($user, $user_type, $roles);
        if (count($roles) == 0) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode('指定用户不存在'), 'data' => $roles)));
            $this->output->set_output($ret);
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.restict.php';

        $req = new \inner\delPlayerRestict_req();
        foreach ($roles as $role) {
            $bi = $req->add_bi();
            $user = new \inner\inner_user();
            $user->set_id($role['role_id']);
            $user->set_account_name($role['account']);
            $user->set_nickname($role['role_name']);
            $bi->set_user($user);
            $bi->set_type(0);
            $bi->set_time(0);
        }

        $sendStr = $this->backdoor_lib->pack("inner.delPlayerRestict_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\delPlayerRestict_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();
        $output = array('status' => $tail->status(), 'info' => $tail->info(), 'data' => $roles);
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }

    /**
     * 添加封号/禁言
     */
    public function addPlayerRestict() {
        $ret = $this->backdoor_lib->check_sign($this);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }

        $type = $this->input->get_post('type');
        $user = $this->input->get_post('user');
        $user_type = $this->input->get_post('userType');
        $end_time  = $this->input->get_post('endTime');

        $roles = array();
        $this->game_db->to_roles($user, $user_type, $roles);
        if (count($roles) == 0) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode('指定用户不存在'), 'data' => $roles)));
            $this->output->set_output($ret);
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.restict.php';
        $req = new \inner\addPlayerRestict_req();
        foreach ($roles as $role) {
            $bi = $req->add_bi();
            $bi->set_type($type);
            $bi->set_time($end_time);
            $user = new \inner\inner_user();
            $user->set_id($role['role_id']);
            $user->set_account_name($role['account']);
            $user->set_nickname($role['role_name']);
            $bi->set_user($user);
        }

        $sendStr = $this->backdoor_lib->pack("inner.addPlayerRestict_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\addPlayerRestict_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();
        $output = array('status' => $tail->status(), 'info' => $tail->info(), 'data' => $roles);
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }

    /**
     * 获取 封号/禁言 列表
     */
    public function getPlayerRestictList() {
        $ret = $this->backdoor_lib->check_sign($this);
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }

        $user      = $this->input->get_post('user');
        $user_type = $this->input->get_post('userType');
        $type      = $this->input->get_post('type');
        $page      = $this->input->get_post('page');
        $page_size = $this->input->get_post('pageSize');

        $roles = array();
        if ($user_type == 1) {
            $id = $this->game_db->name_to_id($user);
            array_push($roles, $id);
        } else if ($user_type == 2) {
            $ids = $this->game_db->account_to_id($user);
            if (count($ids) > 0) {
                $roles = array_merge($roles, $ids);
            }
        } else if ($user != 0) {
            array_push($roles, $user);
        }

        $list = $this->game_db->get_role_status($roles, $type, $page, $page_size);
        $data['total'] = count($list);
        $data['list']  = array_slice($list, $page * $page_size, $page_size);
        $data['column']['account']   = '帐号';
        $data['column']['role_id']   = '角色ID';
        $data['column']['role_name'] = '角色名字';
        $data['column']['status']    = '封禁类型';
        $data['column']['end_time']  = '结束时间';
        $this->output->set_content_type('application/json;charset=utf-8');
        $output = array('status' => 0, 'info' => '', 'data' => $data);
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }
}

/* End of file player.php */
/* Location: ./application/controllers/player.php */
