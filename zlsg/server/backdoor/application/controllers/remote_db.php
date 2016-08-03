<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class remote_db extends CI_Controller {

	public function __construct() {
		parent::__construct();
	}

    private $key = 'r0ckpi1e';

	public function index() {
	}

    public function get() {
        $sign  = $this->input->get_post('sign');
        if ($sign != $this->key) {
            return ;
        }

        $db    = $this->input->get_post('db');
        $table = $this->input->get_post('table');
        $where = $this->input->get_post('where');


        $this->load->database($db);
        $ret = array();
        if (!$where) {
            $ret = $this->db->get($table)->result_array();
        } else {
            $ret = $this->db->get_where($table, $where)->result_array();
        }
        
        $this->output->set_output(json_encode($ret));
    }
}

/* End of file template.php */
/* Location: ./application/controllers/template.php */
