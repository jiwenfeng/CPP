<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

//ini_set('max_input_time','0');
//ini_set('max_execution_time', '0');
//ini_set('memory_limit','2000M');

class filesmgr extends CI_Controller {
    public function __construct() 
    {
        parent::__construct();
        $this->config->load('backdoor', TRUE);
        $this->load->helper('url');
        $this->_init_db();

        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
    }

    public function index()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $data = array();
        $this->_load_version($data);
        $this->load->view('filesmgr.php', $data);
    }

    private function _init_db()
    {
        $this->load->database("filesmgr");
    }

    private function _load_db($table, $field, &$data)
    {
        $sql = "select $field from $table where id = 1";
        $query = $this->db->query($sql);
        if ($query->num_rows() > 0) {
            foreach ($query->result_array() as $row) {
                $data = $row[$field];
                break;
            }
        }
    }

    private function _load_android_version(&$data)
    {
        $data['android_lib_info'] = $this->_load_file_info('android', 'lib');
        $data['android_res_info'] = $this->_load_file_info('android', 'res');
        $data['android_app_info'] = $this->_load_file_info('android', 'apk');
    }

    private function init_upload($path)
    {
        // 把需要的配置放入config数组
        $upload_root = $this->config->item('upload_root', 'backdoor');
        $config['upload_path'] = $upload_root . $path;//'/www/uploads/filesmgr/win32/';
        if (!is_dir($config['upload_path'])) {
            mkdir($config['upload_path'], 0777, true);
        }

        $config['file_name'] = '';
        $config['overwrite'] = TRUE;
        $config['allowed_types'] = '*';
        $config['max_size'] = '0';
        $this->load->library('upload', $config); //调用CI的upload类
    }

    function android()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $file    = '';
        $version = 1;
        $lib = $this->input->get_post('exeversion');
        $res = $this->input->get_post('resversion');
        $apk = $this->input->get_post('apkversion');
        if ($lib) {
            $version = $lib;
            $file    = 'lib';
        } else if ($res) {
            $version = $res;
            $file    = 'res';
        } else if ($apk) {
            $version = $apk;
            $file    = 'apk';
        } else {
            echo "error version null";
            return ;
        }

        $pf   = $this->input->get_post('pf');
        if (!$pf || $pf == '') {
            $pf = 'default';
        }

        $path = 'uploads/filesmgr/android/' . $pf;
        $this->init_upload($path);

        if ($this->upload->do_upload($file)) {
            $data = $this->upload->data();

            $sys  = "android";
            $url  = $this->base_site_url() . $path;
            $url .= '/' . $data['file_name'];

            if ($file != 'res') {
                $id = $this->_load_id($pf, $sys, $file);
                $this->_save_file_info($id, $pf, $sys, $file, $url, $version);
            } else {
                $this->_save_file_info(0, $pf, $sys, $file, $url, $version);
            }

            redirect('/filesmgr/index', 'refresh');
        } else { //上传失败
            $error = array('error' => $this->upload->display_errors());//将错误信息存入数组
            var_dump($error); //打印错误信息
        }
    }

    private function _load_ios_version(&$data)
    {
        $data['ios_ipa'] = 1;
        $data['ios_res'] = 1;

        /*$sql = "select ipa_version, res_version from ios where id = 1";
        $query = $this->db->query($sql);
        if ($query->num_rows() > 0) {
            foreach ($query->result_array() as $row) {
                $data['ios_ipa'] = $row['ipa_version'];
                $data['ios_res'] = $row['res_version'];
                break;
            }
        }*/
    }

    public function ios()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $path = 'uploads/filesmgr/ios/';
        $this->init_upload($path);
        $url = 'http://192.168.23.217:9800/' . $path;
        $file    = '';
        $version = 1;
        $ipa = $this->input->get_post('ipaversion');
        $res = $this->input->get_post('resversion');
        if ($ipa) 
        {
            $version = $ipa;
            $file    = 'ipa';
        } 
        else if ($res)
        {
            $version = $res;
            $file    = 'res';
        }
        else 
        {
            echo "error version null";
            die;
        }

        if ($this->upload->do_upload($file)) 
        {
            $data = $this->upload->data();
            $this->init_db();
            if ($ipa) 
            {
                $sql  = "update ios set ipa_url = '{$url}{$data['file_name']}'";
                $sql .= ", ipa_version = {$version} ";
                $sql .= ' where id = 1';
                $this->db->query($sql);
            } 
            else if ($res) 
            {
                $sql  = "update ios set res_url = '{$url}{$data['file_name']}'";
                $sql .= ", res_version = {$version} ";
                $sql .= ' where id = 1';
                $this->db->query($sql);
            } 

            $this->load_version($data);
            $this->load->view('filesmgr.php', $data);
        }
        else 
        {
            $error = array('error' => $this->upload->display_errors());//将错误信息存入数组
            var_dump($error); //打印错误信息
        }
    }

    private function _load_version(&$data)
    {
        $this->_load_android_version($data);
        $this->_load_ios_version($data);
        $this->_load_win_version($data);
    }

    private function _load_win_version(&$data)
    {
        $data['libinfo'] = $this->_load_file_info('windows', 'lib');
        $data['resinfo'] = $this->_load_file_info('windows', 'res');
        $data['exeinfo'] = $this->_load_file_info('windows', 'exe');
    }

    public function base_site_url()
    {
        $matches = array();
        preg_match('/^((https?\:\/\/).*?\/)/', base_url(), $matches);
        //var_dump($matches);
        return $matches[1];

    }

    public function win32()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $type = $this->input->get_post('type');
        $pf   = $this->input->get_post('pf');
        $version  = $this->input->get_post('version');
        $lib  = $this->input->get_post('path');
        $path = 'uploads/filesmgr/windows/' . $pf;
        $this->init_upload($path);

        $url     = $this->base_site_url() . $path;

        if ($this->upload->do_upload('path')) 
        {
            $data = $this->upload->data();
            $url .= '/' . $data['file_name'];
            $sys = "windows";
            if ($type != 'res')  {
                $id = $this->_load_id($pf, $sys, $type);
                $this->_save_file_info($id, $pf, $sys, $type, $url, $version);
            } else {
                $this->_save_file_info(0, $pf, $sys, $type, $url, $version);
            }

            //$this->_load_version($data);
            //$this->load->view('filesmgr.php', $data);
            redirect('/filesmgr/index', 'refresh');
        }
        else 
        {
            $error = array('error' => $this->upload->display_errors());//将错误信息存入数组
            var_dump($error); //打印错误信息
        }

        return ;

        // 把需要的配置放入config数组
        $config['upload_path'] = '/www/uploads/filesmgr/win32/';
        $config['file_name'] = '';
        $config['overwrite'] = TRUE;
        $config['allowed_types'] = '*';
        $config['max_size'] = '0';
        $this->load->library('upload', $config); //调用CI的upload类
        //$this->upload->do_upload('resfilename'); //使用do_upload('上传框的name')方法进行上传
        //echo $this->input->post('resversion');

        // 以下代码为拓展的，非必要
        if ($this->upload->do_upload('resfilename')) { //上传成功
            $data = array('upload_data' => $this->upload->data()); //将文件信息存入数组
            var_dump($data); //打印文件信息
        } else { //上传失败
            $error = array('error' => $this->upload->display_errors());//将错误信息存入数组
            var_dump($error); //打印错误信息
        }
    }

    private function _load_id($pf, $sys, $type)
    {
        $sql = "select id from t_files where pf=\"$pf\" and sys=\"$sys\" and type=\"$type\"";
        $query = $this->db->query($sql);
        if ($query->num_rows() > 0) {
            foreach ($query->result_array() as $row) {
                return $row['id'];
            }
        }

        return 0;
    }

    private function _load_file_info($sys, $type)
    {
        $ret   = '<br/>';
        $sql   = "select pf, url, version from t_files where sys=\"$sys\" and type=\"$type\"";
        $query = $this->db->query($sql);
        if ($query->num_rows() > 0) {
            foreach ($query->result_array() as $row) {
                $pf      = $row['pf'];
                $url     = $row['url'];
                $version = $row['version'];
                $ret     .= "pf : $pf <br/>";
                $ret     .= "version : $version <br/> ";
                $ret     .= "url : $url <br/>";
            }
        }

        return $ret;
    }

    private function _save_file_info($id, $pf, $sys, $type, $url, $version)
    {
        $sql = "replace into t_files(id, pf, sys, type, url, version) value($id, \"$pf\", \"$sys\", \"$type\", \"$url\", \"$version\");";
        $this->db->query($sql);
    }

    public function client() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->load->view("client_version");
    }

    public function files_info() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $opt = $this->input->get_post('opt');
        $ret = '';
        $this->db->order_by("id", "desc");
        if ($opt) {
            $ret = $this->db->get('t_files_update'); 
        } else {
            $ret = $this->db->get('t_files'); 
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret->result_array()));
    }

    public function set_files_info() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $set = $this->input->get_post('set');
        //var_dump($set);
        $opt = $this->input->get_post('opt');
        $table_name = 't_files';
        if ($opt) {
            $table_name = 't_files_update';
        }

        //var_dump($set);
        foreach ($set as $arr) {
            foreach ($arr as $key => $value) {
                if ($key == 'id') {
                    continue;
                }

                $this->db->update($table_name, array($key => $value), array('id' => $arr['id']));
            }
        }

        $rsp = array('ret' => 0,
            'info' => 'success',
            'data' => $set);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function insert() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id      = $this->input->get_post('id');
        $pf      = $this->input->get_post('pf');
        $sys     = $this->input->get_post('sys');
        $type    = $this->input->get_post('type');
        $url     = $this->input->get_post('url');
        $version = $this->input->get_post('version');

        $opt = $this->input->get_post('opt');
        $table_name = 't_files';
        if ($opt) {
            $table_name = 't_files_update';
        }

        $data = array(
            'pf'      => $pf     ,
            'sys'     => $sys    ,
            'type'    => $type   ,
            'url'     => $url    ,
            'version' => $version,
        );

        if ($id) {
            $data['id'] = $id;
        } else {
            $data['id'] = 0;
        }

        $this->db->insert($table_name, $data);
        $id = $this->db->insert_id();
        $data['id'] = $id;
        $rsp = array('ret' => 0,
            'info' => 'success',
            'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function delete() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $opt = $this->input->get_post('opt');
        $table_name = 't_files';
        if ($opt) {
            $table_name = 't_files_update';
        }

        $data = array('id' => $this->input->get_post('id'));
        $this->db->delete($table_name, $data);

        $rsp = array('ret' => 0,
            'info' => 'success',
            'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function modify() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $opt = $this->input->get_post('opt');
        $table_name = 't_files';
        if ($opt) {
            $table_name = 't_files_update';
        }

        $id      = $this->input->get_post('id');
        $pf      = $this->input->get_post('pf');
        $sys     = $this->input->get_post('sys');
        $type    = $this->input->get_post('type');
        $url     = $this->input->get_post('url');
        $version = $this->input->get_post('version');

        $data = array(
            'pf'      => $pf     ,
            'sys'     => $sys    ,
            'type'    => $type   ,
            'url'     => $url    ,
            'version' => $version,
        );

        $this->db->update($table_name, $data, array('id' => $id));

        $rsp = array('ret' => 0,
            'info' => 'success',
            'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    private function _rsp($ret, $info, $data) {
        $rsp = array(
            'ret'  => $ret,
            'info' => $info,
            'data' => $data,
        );
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function files_check() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $ret = $this->db->query("call files_check");
        //$ret = $ret->result_array()[0]['ret'];
        //$info = 'files_check';
        //if ($ret) {
        //   $info = $this->db->_error_message();
        //}
        $this->_rsp($ret, $info, $ret ? '有重复ID' : 'success');
    }

    public function files_update() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $rsp = $this->db->query('call files_update');
        foreach ($rsp->result_array() as $row) {
            $ret = $row['ret'];
        }

        $this->_rsp($ret, 'files_check', $ret ? '有重复ID' : 'success');
    }

    public function get_pf_cdn() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $ret = $this->db->get('t_cdn');

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret->result_array()));
    }

    public function set_pf_cdn() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $row = $this->input->get_post('row');

        $ret = $this->db->replace('t_cdn', $row);

        $this->_rsp(0, 'set_pf_cdn', 'success');
    }

    public function del_pf_cdn() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $rows = $this->input->get_post('rows');
        
        foreach ($rows as $row) {
            $this->db->delete('t_cdn', $row);
        }

        $this->_rsp(0, 'set_pf_cdn', 'success');
    }

    public function get_version() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $ret = $this->db->get('t_client_version')->result_array();

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret));
    }

    public function set_version() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $row = $this->input->get_post('row');

        $ret = $this->db->replace('t_client_version', $row);

        $this->_rsp(0, 'set_version', 'success');
    }

    public function del_version() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $rows = $this->input->get_post('rows');
        
        foreach ($rows as $row) {
            $this->db->delete('t_client_version', $row);
        }

        $this->_rsp(0, 'del_version', 'success');
    }
}

?>
