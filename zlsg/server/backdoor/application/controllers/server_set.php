<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class server_set extends CI_Controller {
	public function __construct() {
		parent::__construct();
        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
	}

    private function _check_login() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return true;
        }

        return false;
    }

	public function index()
	{
        if ($this->_check_login()) {
            return ;
        }

		$this->load->view('server_set');
	}

	private function _rsp($ret, $info, $data) {
        $rsp = array('ret' => $ret, 'info' => $info, 'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

	public function gm() {
        if ($this->_check_login()) {
            return ;
        }

        $set = $this->input->get_post('set');
        $servers = $this->input->get_post('servers');

		$this->load->library('backdoor_lib');
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.server.php';
        $req  = new \inner\open_gm_req();
        $req->set_unixtime($set);

        $sendStr = $this->backdoor_lib->pack("inner.open_gm_req", $req->SerializeToString());
        if (!$servers) {
        	$result  = $this->backdoor_lib->write_and_send($sendStr);
        } else {
            foreach ($servers as $server) {
                $result  = $this->backdoor_lib->send($server['ip'], $server['inner_port'], $sendStr);
            }
        }
        
        if (!$result) {
            return $this->_rsp(1);
        }
        
        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\open_gm_rsp();
        $rsp->ParseFromString($result);

        $this->_rsp($rsp->tail()->status(), '', $rsp->unixtime());
	}

    public function set() {
        
    }
}

/* End of file template.php */
/* Location: ./application/controllers/template.php */
