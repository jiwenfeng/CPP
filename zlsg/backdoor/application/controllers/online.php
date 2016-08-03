<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class online extends CI_Controller {

	public function __construct() {
        parent::__construct();
        $this->load->database('game');
        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
    }

	public function index() {
        if ($this->_check_login()) {
            return ;
        }

		$this->load->view('online');
	}

    public function get_online() {
        $start = $this->input->get_post('start');
        $end   = $this->input->get_post('end');

        $where = array('createDate >=' => $start,
            'createDate <=' => $end,
            );

        $ret = $this->db->get_where('t_online', $where)->result_array();

        $this->_rsp(0, "", $ret);
    }

    public function get_online_player_list() {
        $server = $this->input->get_post('server');
        if (!$server || count($server) < 1 || !isset($server[0]['ip']) || !isset($server[0]['inner_port'])) {
            var_dump($server);
            return ;
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.online.php';

        $req = new \inner\online_player_req();
        $req->set_unixtime(0);
        $sendStr = $this->backdoor_lib->pack("inner.online_player_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->send($server[0]['ip'], $server[0]['inner_port'], $sendStr);
        if (!$result) {
            var_dump($result);
            return ;
        }

        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\online_player_rsp();
        $rsp->ParseFromString($result);
        echo '在线人数: ' . $rsp->role_ids_size();
        //var_dump($rsp->role_ids_size());
    }

    private function _check_login() {
        //return false;
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
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
