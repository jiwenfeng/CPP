<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Another_database extends CI_Controller {
	public function index()
	{
		$this->load->view('welcome_message');
	}

    public function show()
    {
        $this->load->database();
    }
}

