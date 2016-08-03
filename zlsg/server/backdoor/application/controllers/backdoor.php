<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Backdoor extends CI_Controller {
    function index()
    {
        $data = array();
        $this->load->view('backdoor/head.php');
        $this->load->view('backdoor/nav.php');
        $this->load->view('backdoor/modal.php');
        $this->load->view('backdoor/tail.php');
    }
}

/* End of file backdoor.php */ 
/* Location: ./application/controllers/backdoor.php */
