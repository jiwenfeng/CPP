<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class bulletin extends CI_Controller {

	public function __construct() {
        parent::__construct();
        $this->load->database('server_list');
        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
    }

	public function index() {
        if ($this->_check_login()) {
            return ;
        }

		$this->load->view('bulletin');
	}

    public function revolving_req() {
        if ($this->_check_login()) {
            return ;
        }

        $server      = $this->input->get_post('server'     );
        $contents    = $this->input->get_post('contents'   );
        $start_time  = $this->input->get_post('start_time' );
        $end_time    = $this->input->get_post('end_time'   );
        $repeat_time = $this->input->get_post('repeat'     );
        if (!$server || count($server) < 1 || !isset($server[0]['ip']) || !isset($server[0]['inner_port'])) {
            var_dump($server);
            return ;
        }

        date_default_timezone_set("Asia/Shanghai");
        $start_time = strtotime($start_time);
        $end_time   = strtotime($end_time);

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.bulletin.php';

        $req = new \inner\revolving_req();
        $req->set_contents($contents);
        $req->set_start_time($start_time);
        $req->set_end_time($end_time);
        $req->set_repeat_time($repeat_time);
        $sendStr = $this->backdoor_lib->pack("inner.revolving_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->send($server[0]['ip'], $server[0]['inner_port'], $sendStr);
        if (!$result) {
            var_dump($result);
            return ;
        }

        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\revolving_rsp();
        $rsp->ParseFromString($result);
        var_dump($rsp->tail());
    }

    private function _check_login() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return true;
        }

        return false;
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

    public function get() {
        if ($this->_check_login()) {
            return ;
        }

        $addr = $this->input->get_post('addr');
        $port = $this->input->get_post('port');
        $data = array();
        if (!$addr || !$port) {
            $this->output->set_content_type('application/json;charset=utf-8');
            $this->output->set_output(json_encode($data));
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.bulletin.php';

        $req = new \inner\get_revolving_req();
        $req->set_unixtime(time());
        $sendStr = $this->backdoor_lib->pack("inner.get_revolving_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->send($addr, $port, $sendStr);
        if (!$result) {
            $this->output->set_content_type('application/json;charset=utf-8');
            $this->output->set_output(json_encode($data));
            return ;
        }

        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\get_revolving_rsp();
        $rsp->ParseFromString($result);
        if (!$rsp->get_rss()) {
            $this->output->set_content_type('application/json;charset=utf-8');
            $this->output->set_output(json_encode($data));
            return ;
        }

        foreach ($rsp->get_rss() as $obj) {
            array_push($data, array(
                'msg_id'      => $obj->msg_id(),
                'contents'    => $obj->contents(),
                'start_time'  => $obj->start_time(),
                'end_time'    => $obj->end_time(),
                'repeat_time' => $obj->repeat_time(),
                'is_over'     => $obj->is_over(),
                ));
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($data));
    }

    public function pause() {
        if ($this->_check_login()) {
            return ;
        }

        $id = $this->input->get_post('id');
        $server = $this->input->get_post('server');

        if (!$id || !$server) {
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.bulletin.php';

        $req = new \inner\stop_revolving_req();
        foreach ($id as $item) {
            $req->append_ids($item['msg_id']);
        }
        
        $sendStr = $this->backdoor_lib->pack("inner.stop_revolving_req", $req->SerializeToString());
        foreach ($server as $item) {
            $ret  = $this->backdoor_lib->send($item['ip'], $item['inner_port'], $sendStr);
            if (!$ret) {
                //$this->output->set_content_type('application/json;charset=utf-8');
                //$this->output->set_output(json_encode($data));
                continue ;
            } else {
                $result = $ret;
            }
        }
        
        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\stop_revolving_rsp();
        $rsp->ParseFromString($result);
        $data = array();
        if (!$rsp->get_idss()) {
            $this->output->set_content_type('application/json;charset=utf-8');
            $this->output->set_output(json_encode($data));
            return ;
        }

        foreach ($rsp->get_idss() as $obj) {
            array_push($data, array(
                'msg_id'      => $obj,
                ));
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($data));
    }

    public function minus() {
        if ($this->_check_login()) {
            return ;
        }

        $id     = $this->input->get_post('id');
        $server = $this->input->get_post('server');

        if (!$id || !$server) {
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.bulletin.php';

        $req = new \inner\del_revolving_req();
        foreach ($id as $item) {
            $req->append_ids($item['msg_id']);
        }
        
        $sendStr = $this->backdoor_lib->pack("inner.del_revolving_req", $req->SerializeToString());
        foreach ($server as $item) {
            $ret  = $this->backdoor_lib->send($item['ip'], $item['inner_port'], $sendStr);
            if (!$ret) {
                //$this->output->set_content_type('application/json;charset=utf-8');
                //$this->output->set_output(json_encode($data));
                continue ;
            } else {
                $result = $ret;
            }
        }
        
        $result  = $this->backdoor_lib->unpack($result);
        $rsp = new \inner\del_revolving_rsp();
        $rsp->ParseFromString($result);
        $data = array();
        if (!$rsp->get_idss()) {
            $this->output->set_content_type('application/json;charset=utf-8');
            $this->output->set_output(json_encode($data));
            return ;
        }

        foreach ($rsp->get_idss() as $obj) {
            array_push($data, array(
                'msg_id'      => $obj,
                ));
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($data));
    }
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
