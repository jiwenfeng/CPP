<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class money extends CI_Controller {
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
        
		$this->load->view('money');
	}

	public function t_recharge() {
		$this->load->database('game');
		$ret = $this->db->get('t_recharge')->result_array();
		$this->output->set_output(json_encode($ret));
	}

	public function get() {
		return $this->t_recharge();

		$web = $this->input->get('web');

		$path = "$web/backdoor/index.php?/money/t_recharge";
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $path);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		$ret = curl_exec($ch);
		curl_close($ch);
		if (!$ret) {
			$this->_paojiao_rsp(-1, 'server error', $token);
			return ;
		}
	}
}

/* End of file template.php */
/* Location: ./application/controllers/template.php */
