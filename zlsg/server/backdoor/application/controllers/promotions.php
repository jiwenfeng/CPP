<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class promotions extends CI_Controller {
    
	public function __construct() {
		parent::__construct();

        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.promotions.php';
	}

    private $key = 'r0ckpi1e';

    private function _check_login() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return true;
        }

        return false;
    }

    private function _rsp($ret, $info, $data) {
        $rsp = array('ret' => $ret, 'info' => $info, 'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

	public function index() {
        if ($this->_check_login()) {
            return ;
        }

        $this->load->view('promotions');
	}

    private function objs_to_array(&$objs, &$data) {
        foreach ($rsp->get_pss() as $obj) {
            array_push($data, array(
                'id'            => $obj->id(),
                'name'          => $obj->name(),
                'type'          => $obj->type(),
                'start_time'    => $obj->start_time(),
                'duration_time' => $obj->duration_time(),
                'end_time'      => $obj->end_time(),
                'loop_time'     => $obj->loop_time(),
                'state'         => $obj->state(),
                ));
        }
    }

    public function get_by_game() {
        if ($this->_check_login()) {
            return ;
        }

        $addr = $this->input->get_post('addr');
        $port = $this->input->get_post('port');

        $data = array();
        if (!$port) {
            $this->output->set_content_type('application/json;charset=utf-8');
            $this->output->set_output(json_encode($data));
        }

        $req  = new \inner\get_promotions_req();
        $req->set_unixtime(1);
        $sendStr = $this->backdoor_lib->pack("inner.get_promotions_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->send($addr, $port, $sendStr);
        if (!$result) {
            return $this->_rsp(1, 'error', '');
        }
        
        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\get_promotions_rsp();
        $rsp->ParseFromString($result);

        if (!$rsp->get_pss()) {
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($data));
        return ;
        }

        foreach ($rsp->get_pss() as $obj) {
            array_push($data, array(
                'id'            => $obj->id(),
                'name'          => $obj->name(),
                'type'          => $obj->type(),
                'start_time'    => $obj->start_time(),
                'duration_time' => $obj->duration_time(),
                'end_time'      => $obj->end_time(),
                'loop_time'     => $obj->loop_time(),
                'state'         => $obj->state(),
                ));
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($data));
    }

    public function get_by_db() {
        if ($this->_check_login()) {
            return ;
        }

        $web = $this->input->get_post('web');

        require_once dirname(dirname(__FILE__)) . '/include/class/SnsNetwork.php';

        $web .= '/backdoor/index.php?/remote_db/get';
        $param = array(
            'sign' => $this->key,
            'db' => 'game',
            'table' => 't_promotions',
        );

        $ret = SnsNetwork::makeRequest($web, $param, '');
        $this->output->set_output(json_encode($ret['msg']));
    }

    public function set() {
        if ($this->_check_login()) {
            return ;
        }

        $servers = $this->input->get_post('servers');
        $row    = $this->input->get_post('row');
        if (!$servers || !$row) {
            $this->_rsp(1, 'param error', '');
            return ;
        }

        //var_dump($servers);
        //var_dump($row);

        $req  = new \inner\set_promotions_req();
        $ps = $req->add_ps();
        $ps->set_id($row['id']);
        $ps->set_name($row['name']);
        $ps->set_type($row['type']);
        $ps->set_start_time($row['start_time']);
        $ps->set_duration_time($row['duration_time']);
        $ps->set_end_time(0);
        $ps->set_loop_time($row['loop_time']);
        $ps->set_state(0);

        $sendStr = $this->backdoor_lib->pack("inner.set_promotions_req", $req->SerializeToString());
        $result = false;
        foreach ($servers as $server) {
            $addr = $server['ip'];
            $port = $server['inner_port'];
            if (!$addr || !$port) {
                continue;
            }

            $ret = $this->backdoor_lib->send($addr, $port, $sendStr);
            if (!$ret) {
                continue;
            } else {
                $result = $ret;
            }
        }

        $data = array();
        if ($result) {
            $result = $this->backdoor_lib->unpack($result);

            $rsp = new \inner\set_promotions_rsp();
            $rsp->ParseFromString($result);

            $this->objs_to_array($rsp->get_pss(), $data);
        }
        
        $this->_rsp(0, 'success', $data);
    }

    public function del() {
        if ($this->_check_login()) {
            return ;
        }
        
        $servers = $this->input->get_post('servers');
        $rows    = $this->input->get_post('rows');
        if (!$servers || !$rows) {
            $this->_rsp(1, 'param error', '');
            return ;
        }

        //var_dump($servers);
        //var_dump($rows);

        $req  = new \inner\del_promotions_req();
        foreach ($rows as $row) {
            $req->append_ids($row['id']);
        }
        
        $sendStr = $this->backdoor_lib->pack("inner.del_promotions_req", $req->SerializeToString());
        $result = false;
        foreach ($servers as $server) {
            $addr = $server['ip'];
            $port = $server['inner_port'];
            if (!$addr || !$port) {
                continue;
            }

            $ret = $this->backdoor_lib->send($addr, $port, $sendStr);
            if (!$ret) {
                continue;
            } else {
                $result = $ret;
            }
        }

        $data = array();
        if ($result) {
            $result = $this->backdoor_lib->unpack($result);

            $rsp = new \inner\del_promotions_rsp();
            $rsp->ParseFromString($result);

            $data = $rsp->get_idss();
        }
        
        $this->_rsp(0, 'success', $data);
    }
}

/* End of file promotions.php */
/* Location: ./application/controllers/promotions.php */
