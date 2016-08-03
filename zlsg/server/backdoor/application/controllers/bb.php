<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class bb extends CI_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->database('server_list');

        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
    }

    private function _load_bb($s, $t) {
        //$sql = "SELECT `bulletin_board`,`revolving_door`,`qq`,`mail`,`share_info` FROM `server_list` WHERE `id`='$s' `t`='$t'";
        $this->db->select('bulletin_board,revolving_door,qq,mail,share_info');
        $this->db->where('id', $s);
        $this->db->where('t', $t);
        $ret = $this->db->get('server_list');
        foreach ($ret->result() as $v) {
            return $v;
        }

        return false;
    }

    public function index() {
        $s = $this->input->get_post('s');
        $t = $this->input->get_post('t');
        if (!$s) {
            $s = 0;
        }

        $rsp = array('ret' => 0);
        $info = $this->_load_bb($s, $t);
        if (!$info) {
            $rsp['data']           = '';
            $rsp['qq']             = '';            
            $rsp['revolving_door'] = '';
            $rsp['mail']           = '';
            $rsp['share_info']     = '';
        } else {
            $rsp['data']           = $info->bulletin_board;
            $rsp['qq']             = $info->qq;
            $rsp['revolving_door'] = $info->revolving_door;
            $rsp['mail']           = $info->mail;
            $rsp['share_info']     = $info->share_info;
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }
    
    private function _update_bb($s, $bb) {
        $sql = "UPDATE `server_list` SET `bulletin_board`='$bb' WHERE `id`='$s'";
        $ret = $this->db->query($sql);
    }

    public function modify() {
        $s  = $this->input->get_post('s');
        $bb = $this->input->get_post('bb');

        $rsp = array('ret' => 1);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (!$bb) {
            $rsp['info'] = "no can no bb!!!";
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        if (!$s) {
            $s = 0;
        }

        $this->_update_bb($s, $bb);

        $rsp['ret'] = 0;
        $rsp['data'] = 'success';
        $this->output->set_output(json_encode($rsp));
    }

    public function show() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->load->view('bb.php');
    }

    public function server_list() {
        $rsp = $this->_load_server();
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function white() {
        $open_id = $this->input->get_post('open_id');
        $rsp = array();
        if ($open_id) {
            $rsp = $this->_load_group($open_id);
        }
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function server() {
        $open_id = $this->input->get_post('open_id');
        $t       = $this->input->get_post('t');
        if (!$t) {
            $t = 0;
        }

        $rsp = array();
        $rsp['server_list'] = $this->_load_server_s($t);
        $this->output->set_content_type('application/json;charset=utf-8');
        if ($open_id) {
            $rsp['group']   = $this->_load_group($open_id);
            $rsp['version'] = $this->_load_client_version();
            $this->output->set_output(json_encode($rsp));
        } else {
            $this->output->set_output(json_encode($rsp['server_list']));
        }
    }

    public function server_info() {
        $id = $this->input->get_post('id');
    }

    public function add_server() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->load->view('add_server.php');
    }

    public function add_svr() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id             = $this->input->get_post('id');
        $name           = $this->input->get_post('name');
        $type           = $this->input->get_post('type');
        $ip             = $this->input->get_post('ip');
        $port           = $this->input->get_post('port');
        $inner_port     = $this->input->get_post('inner_port');
        $status         = $this->input->get_post('status');
        $bulletin_board = $this->input->get_post('bulletin_board');

        $sql = "INSERT INTO `server_list`(`id`, `name`, `ip`, `port`, `inner_port`, `status`, `bulletin_board`, `t`) VALUES ('$id', '$name', '$ip', '$port', '$inner_port', '$status', '$bulletin_board', '$type')";
        $ret = $this->db->query($sql);
        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp = array();
        if ($ret) {
            $rsp['info'] = 'success';
            $rsp['ret'] = 0;
        } else {
            $rsp['ret'] = 1;
            $rsp['info'] = 'error';
        }

        $this->output->set_output(json_encode($rsp));
    }

    public function del_svr() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id             = $this->input->get_post('id');
        $t              = $this->input->get_post('t');

        $sql = "DELETE FROM `server_list` WHERE (`id`='$id' and `t`='$t')";
        $ret = $this->db->query($sql);
        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp = array();
        if ($ret) {
            $rsp['info'] = 'success';
            $rsp['ret'] = 0;
        } else {
            $rsp['ret'] = 1;
            $rsp['info'] = 'error';
        }

        $this->output->set_output(json_encode($rsp));
    }

    private function _load_server_s($t) {
        $sql = "SELECT `id`, `name`, `ip`, `port`, `status`, `web`  FROM `server_list` WHERE `t`='$t' ORDER BY `id`";
        $ret = $this->db->query($sql);
        return $ret->result_array();
    }

    private function _load_group($open_id) {
        $ret = $this->db->get_where('t_white_list', array('open_id' => $open_id));
        return $ret->result_array();
    }

    private function _load_client_version() {
        $db = $this->load->database('filesmgr', TRUE);
        $ret = $db->get_where('t_client_version');
        return $ret->result_array();
    }

    private function _load_server() {
        $sql = "SELECT * FROM `server_list` ORDER BY `id`";
        $ret = $this->db->query($sql);
        return $ret->result_array();
    }

    public function set() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $set  = $this->input->get_post('set');
        foreach ($set as $arr) {
            $this->_update_bb($arr['id'], $arr['bb']);
        }

        $rsp = array('ret' => 0, );
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function set_name() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $set  = $this->input->get_post('set');
        foreach ($set as $arr) {
            $this->_update_name($arr['id'], $arr['name']);
        }

        $rsp = array('ret' => 0, );
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    private function _update_name($s, $name) {
        $sql = "UPDATE `server_list` SET `name`='$name' WHERE `id`='$s'";
        $ret = $this->db->query($sql);
    }

    public function set_status() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $set  = $this->input->get_post('set');
        foreach ($set as $arr) {
            $this->db->update('server_list', array('status'=>$arr['status']), array('id'=>$arr['id']));
        }

        $rsp = array('ret' => 0, );
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    private function _rsp($ret) {
        $rsp = array('ret' => $ret, );
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function set_id() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $info = $this->input->get_post('info');

        $ret = $this->db->update(
            'server_list', 
            array('id' => $info['id']), 
            array('name' => $info['name'], 't' => $info['t']));

        $this->_rsp(0);
    }

    public function set_t() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $info = $this->input->get_post('info');

        $ret = $this->db->update(
            'server_list', 
            array('t' => $info['t']), 
            array('name' => $info['name'], 'id' => $info['id']));

        $this->_rsp(0);
    }

    public function set_info() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $info = $this->input->get_post('info');
        if (count($info) < 4) {
            $this->_rsp(1);
            return ;
        }

        if ($info[2] == 'start_time') {
            return $this->_set_start_time($info);
        }

        //var_dump($info);
        $ret = $this->db->update(
            'server_list', 
            array($info[2] => $info[3]), 
            array('id' => $info[0], 't' => $info[1]));
        //var_dump($ret);

        $this->_rsp(0);
    }

    public function set_start_time() {
        $start_time = $this->input->get_post('start_time');
        $key        = $this->input->get_post('key');
        $path       = $this->input->get_post('path');
        if (md5($start_time) != $key) {
            return ;
        }

        $path .= '/config';
        $sh  = "sed -i \"s/start_svr_time.*/start_svr_time=$start_time/g\" $path";
        //var_dump($sh);
        $ret = exec($sh);
    }

    private function _set_start_time(&$info) {
        $id   = $info[0];
        $t    = $info[1];
        $date = $info[3];
        date_default_timezone_set("Asia/Shanghai");
        $start_time = strtotime($date);
        $this->db->update('server_list', array($info[2] => $date), array('id' => $id, 't' => $t));
        
        $server_list = $this->_load_server();
        $addr = "";
        $port = 0;
        $web  = '';
        $path = '';
        foreach ($server_list as $server) {
            if ($server['id'] == $id && $server['t'] == $t) {
                $addr = $server['ip'];
                $port = $server['inner_port'];
                $web  = $server['web'];
                $path = $server['path'];
                break;
            }
        }

        if (!$web || !$path) {
            $this->_rsp(1);
            return ;
        }

        $key = md5($start_time);
        $web .= '/backdoor/index.php?/bb/set_start_time/?start_time=' . $start_time;
        $web .= '&key=' . $key . '&path=' . $path;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $web);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        $ret = curl_exec($ch);
        //var_dump($ret);
        curl_close($ch);

        if (!$port) {
            return $this->_rsp(1);
        }

        $this->load->library('backdoor_lib');
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.server.php';
        $req  = new \inner\start_time_req();
        $req->set_unixtime($start_time);

        $sendStr = $this->backdoor_lib->pack("inner.start_time_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->send($addr, $port, $sendStr);
        if (!$result) {
            return $this->_rsp(1);
        }
        
        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\start_time_rsp();
        $rsp->ParseFromString($result);
        if ($rsp->tail()->status() == 1) {
            $this->_rsp(0);
        } else {
            $this->_rsp(1);
        }
    }
}

?>
