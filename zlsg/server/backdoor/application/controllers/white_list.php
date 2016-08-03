<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class white_list extends CI_Controller {

    public $white_list = 't_white_list';

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

		$this->load->view('white_list');
	}

    public function table() {
        if ($this->_check_login()) {
            return ;
        }

        $ret = $this->db->query("show full fields from $this->white_list");

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret->result_array()));
    }

    public function get() {
        if ($this->_check_login()) {
            return ;
        }

        $ret = $this->db->get($this->white_list);

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret->result_array()));
    }

    public function add() {
        if ($this->_check_login()) {
            return ;
        }

        $white = array(
            'open_id' => $this->input->get_post('open_id'),
            'group'   => $this->input->get_post('group'),
            'remark'  => $this->input->get_post('remark'),
        );

        $this->db->insert($this->white_list, $white);
        $white['id'] = $this->db->insert_id();

        $this->_rsp(0, 'success', $white);
    }

    public function del() {
        if ($this->_check_login()) {
            return ;
        }

        $items = $this->input->get_post('items');
        foreach ($items as $item) {
            unset($item['check']);
            $this->db->delete($this->white_list, $item);
        }

        $this->_rsp(0, 'success', $items);
    }

    public function set() {
        if ($this->_check_login()) {
            return ;
        }

        $white = array(
            'row' => $this->input->get_post('row'),
        );

        $this->db->replace($this->white_list, $white['row']);

        $this->_rsp(0, 'success', $white);
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
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
