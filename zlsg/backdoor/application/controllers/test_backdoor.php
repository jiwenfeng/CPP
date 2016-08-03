<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Test_backdoor extends CI_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->database('game');
        $this->db->query("set character set 'latin1'"); 
    }

    public function modules() 
    {
        $this->load->model('backdoor');
        $name = $this->backdoor->id_to_name(238);
        $id   = $this->backdoor->name_to_id($name);
        $acc  = $this->backdoor->id_to_account($id);
        $aid  = $this->backdoor->account_to_id($acc);

        echo $name . '<br/>';
        echo $id . '<br/>';
        echo $acc. '<br/>';
        echo var_dump($aid). '<br/>';
    }

    public function config()
    {
        $this->config->load('backdoor', TRUE);
        echo 'addr: ' . $this->config->item('addr', 'backdoor') . '<br/>';
        echo 'port: ' . $this->config->item('port', 'backdoor') . '<br/>';
        echo 'recv_len: ' . $this->config->item('recv_len', 'backdoor') . '<br/>';
        echo 'key: ' . $this->config->item('key', 'backdoor') . '<br/>';
        echo 'log_path: ' . $this->config->item('log_path', 'backdoor') . '<br/>';
    }
}

/* End of file test_backdoor.php */
/* Location: ./application/controllers/test_backdoor.php */
