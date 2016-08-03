<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class db extends CI_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->database('game');

        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
    }

    public function tables() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $rsp = array();
        $sql = "show tables;";
        $ret = $this->db->query($sql);
        foreach ($ret->result_array() as $key => $value) {
            foreach ($value as $k => $v) {
                array_push($rsp, array('name' => $v));
            }
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function index() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->load->view('db');
    }

    public function fields() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $name = $this->input->get_post('name');
        $sql = "desc $name;";
        $ret = $this->db->query($sql);

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret->result_array()));
    }

    public function data() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        //$this->db->query("set character set 'latin1'");

        $name = $this->input->get_post('name');
        $sql = "SELECT * FROM `$name`;";
        $ret = $this->db->query($sql);

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret->result_array()));
    }
    
    private function _update_bb($s, $bb) {
        $sql = "UPDATE `server_list` SET `bulletin_board`='$bb' WHERE `id`='$s'";
        $ret = $this->db->query($sql);
    }

    public function add_svr() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id             = $this->input->get_post('id');
        $name           = $this->input->get_post('name');
        $ip             = $this->input->get_post('ip');
        $port           = $this->input->get_post('port');
        $inner_port     = $this->input->get_post('inner_port');
        $status         = $this->input->get_post('status');
        $bulletin_board = $this->input->get_post('bulletin_board');

        $sql = "INSERT INTO `server_list`(`id`, `name`, `ip`, `port`, `inner_port`, `status`, `bulletin_board`) VALUES ('$id', '$name', '$ip', '$port', '$inner_port', '$status', '$bulletin_board')";
        $ret = $this->db->query($sql);
        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp = array();
        if ($ret) {
            $rsp['info'] = 'success';
            $rsp['ret'] = 0;
        } else {
            $rsp['ret'] = 1;
            $rsp['info'] = 'error';
        }

        $this->output->set_output(json_encode($rsp));
    }

    public function del_svr() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id             = $this->input->get_post('id');
        
        $sql = "DELETE FROM `server_list` WHERE (`id`='$id')";
        $ret = $this->db->query($sql);
        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp = array();
        if ($ret) {
            $rsp['info'] = 'success';
            $rsp['ret'] = 0;
        } else {
            $rsp['ret'] = 1;
            $rsp['info'] = 'error';
        }

        $this->output->set_output(json_encode($rsp));
    }

    private function _update_name($s, $name) {
        $sql = "UPDATE `server_list` SET `name`='$name' WHERE `id`='$s'";
        $ret = $this->db->query($sql);
    }
}

?>
