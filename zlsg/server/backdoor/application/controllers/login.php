<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class login extends CI_Controller {

	public function __construct() {
        parent::__construct();
        $this->load->database('server_list');
        $this->load->library('session');
    }

    public function captcha() {
        $this->load->helper('captcha');
        var_dump(gd_info());
    	$vals = array(
    		'word'          => 'Random word',
    		'img_path'      => '/www/captcha/',
    		'img_url'       => 'http://192.168.23.217:9800/captcha/',
    		'font_path'     => './path/to/fonts/texb.ttf',
    		'img_width'     => '150',
    		'img_height'    => 30,
    		'expiration'    => 7200,
    		'word_length'   => 8,
    		'font_size'     => 16,
    		'img_id'        => 'Imageid',
    		'pool'          => '234579acdefghkmnpqrstuvwxyz',
        // White background and border, black text and red grid
    		'colors'        => array(
    		'background'    => array(255, 255, 255),
    		'border'        => array(255, 255, 255),
    		'text'          => array(0, 0, 0),
    		'grid'          => array(255, 40, 40)
    			)
    		);

    	$cap = create_captcha($vals);
    	var_dump($cap);
    	$this->output->set_output($cap['image']);
    }

	public function index() {
		$data = array('is_login' => $this->_is_login(),
			'name' => $this->_name()
			);

		$this->load->view('login', $data);
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

	private function _load_user($name) {
		$ret = $this->db->get_where('t_user', array('name' => $name));
		foreach ($ret->result() as $user) {
			return $user;
		}

		return false;
	}

	private function _check_user($name, $psw) {
		$ret = $this->db->get_where('t_user', array('name' => $name));
		foreach ($ret->result() as $user) {
			if ($user->psw == md5($psw)) {
				return true;
			}
		}

		return false;
	}

	public function signin() {
		if ($this->_is_login()) {
			$this->_rsp(0, 'ready', '');
			return ;
		}

		$name = $this->input->get_post('name');
		$psw  = $this->input->get_post('psw');
		if (!$name || !$psw || !$this->_check_user($name, $psw)) {
			$this->_rsp(2, 'name or psw error', '');
			return ;
		}

		$arr = array(
			'logged_in' => TRUE,
			'name' => $name
			);
		$this->session->set_userdata($arr);

		$this->_rsp(0, 'success', $name);
	}

	private function _is_login() {
		return $this->session->userdata('logged_in');
	}

	private function _name() {
		return $this->session->userdata('name');
	}

	public function signout() {
		$name = $this->session->userdata('name');//$this->input->get_post('name');

		if ($this->_is_login()) {
			$this->session->sess_destroy();
			$this->_rsp(0, 'success', $name);
			return ;
		}

		$this->_rsp(1, 'not sign in', $name);
	}

	private function _save_user($name, $psw, $group) {
		$user = array(
			'name' => $name,
			'psw' => md5($psw),
			'group' => $group
			);

		return $this->db->insert('t_user', $user);
	}

	public function init() {
		if ($this->_load_user('admin')) {
			$this->_rsp(1, 'have admin', 'admin');
			return ;
		}

		$this->_save_user('admin', 'admin', 0);
		$this->_rsp(0, 'init admin', 'admin');
	}

	public function add() {
		if (!$this->_is_login()) {
			$this->_rsp(1, 'not login', '');
			return ;
		}

		$user = $this->_load_user($this->_name());
		if (!$user) {
			$this->_rsp(1, 'user error', '');
			return ;
		}

		$data = array('group' => $user->group);
		$this->load->view('add_user', $data);
	}

	public function add_user() {
		if (!$this->_is_login()) {
			$this->_rsp(1, 'not login', '');
			return ;
		}

		$user = $this->_load_user($this->_name());
		if ($user->group != 0) {
			$this->_rsp(1, 'group error', $name);
			return ;
		}

		$name  = $this->input->get_post('name');
		$psw   = $this->input->get_post('psw');
		$group = $this->input->get_post('group');

		$add = $this->_load_user($name);
		if ($add) {
			$this->_rsp(1, 'have name', $name);
			return ;
		}

		if ($this->_save_user($name, $psw, $group)) {
			$this->_rsp(0, 'success', $name);
		} else {
			$this->_rsp(1, 'error', $name);
		}
	}

	public function psw() {
		if (!$this->_is_login()) {
			$this->_rsp(1, 'not login', '');
			return ;
		}

		$this->load->view('psw');
	}

	public function set_psw() {
		if (!$this->_is_login()) {
			$this->_rsp(1, 'not login', '');
			return ;
		}

		$name  = $this->_name();
		$oldpsw = $this->input->get_post('oldpsw');
		if (!$this->_check_user($name, $oldpsw)) {
			$this->_rsp(1, 'old psw error', $name);
			return ;
		}

		$psw   = $this->input->get_post('psw');

		if ($this->_update_psw($name, $psw)) {
			$this->_rsp(0, 'success', $name);
		} else {
			$this->_rsp(1, 'error', $name);
		}
	}

	private function _update_psw($name, $psw) {
		$where = array('name' => $name);
		$data = array('psw' => md5($psw));
		//$this->db->where('name', $name);
		return $this->db->update('t_user', $data, $where);
	}
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */