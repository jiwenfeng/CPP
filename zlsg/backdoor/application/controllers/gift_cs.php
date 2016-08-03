<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class gift_cs extends CI_Controller {

    public function __construct() 
    {
        parent::__construct();
        $this->config->load('backdoor', TRUE);
        $this->gift_db = $this->load->database("gift", TRUE);
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
        $this->load->view('gift_cs.php', $data);
    }

    public function code()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $data = array();
        $data['gifts'] = $this->_load_gift();
        $this->load->view('gift_code_cs.php', $data);
    }

    public function get_gift()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id = $this->input->get_post('id');
        $rsp = array();
        $rsp['ret'] = 1;
        if (!$id) {
            return ;
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp['ret']   = 0;
        $rsp['gifts'] = $this->_load_gift_by_id($id);
        date_default_timezone_set("Asia/Shanghai");
        foreach ($rsp['gifts'] as &$gift) {
            $gift['start_time'] = date("m/d/Y", $gift['start_time']);
            $gift['end_time']   = date("m/d/Y", $gift['end_time']);
        }

        $this->output->set_output(json_encode($rsp));
    }

    public function giftsjson()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp = array();
        //if (!$id || $id == "0") {
        //    $this->output->set_output(json_encode($rsp));
        //    return ;
        //}

        $rsp = $this->_load_gift();
        $this->output->set_output(json_encode($rsp));
    }

    public function codesjson()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id = $this->input->get_post('id');
        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp = array();
        if (!$id || $id == "0") {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $rsp = $this->_load_codes($id);
        $this->output->set_output(json_encode($rsp));
    }

    private function _load_gift()
    {
        $query = $this->gift_db->query("select * from t_gift");
        return $query->result_array();
    }

    private function _load_gift_by_id($id)
    {
        $query = $this->gift_db->query("select * from t_gift where `id`='$id';");
        return $query->result_array();
    }

    private function _load_codes($id)
    {
        $ret = array();
        for ($i=0; $i < 10; $i++) { 
            $query = $this->gift_db->query("select * from t_gift_code_$i where gift_id = $id");
            if (count($query->result_array()) > 0) {
                $ret = array_merge($ret, $query->result_array());
            }
        }
        
        return $ret;
    }

    public function itemsjson()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id = $this->input->get_post('id');
        $this->output->set_content_type('application/json;charset=utf-8');
        $rsp = array();
        /*if (!$id || $id == "0") {
            $this->output->set_output(json_encode($rsp));
            return ;
        }*/

        $this->output->set_output(json_encode($this->_items()));
    }

    private function _items()
    {
        $this->config->load('backdoor', TRUE);
        $file_bin = $this->config->item('config_item_bin_path', 'backdoor');
        $file_str = $this->config->item('config_item_str_path', 'backdoor');

        $handle = fopen($file_bin, 'rb');
        $bin  = fread($handle, filesize($file_bin));
        fclose($handle);

        $str    = file_get_contents($file_str);
        $fields = explode('^', $str);

        $name_index = unpack("V3int/C*content", $bin);
        $fields[$name_index['int1'] - 1];
        $field_count  = $name_index['int2'];
        $field_record = $name_index['int3'];
        $field_keys   = array();
        $i = 1;
        for ($index = 0; $index < $field_count; ++$index, $i += 4) {
            $content = pack('cccc', $name_index['content' . $i] , $name_index["content" . ($i+1)] , $name_index["content" . ($i+2)] , $name_index["content" . ($i+3)]);
            $var = unpack('V', $content);
            array_push($field_keys, $var[1]);
            //echo $var[1] . ' ' . $fields[$var[1] - 1] . '<br/>';
        }

        $data_length = $field_record * $field_count;
        $data_key = array();
        for ($index = 0; $index < $data_length; ++$index, $i += 4) {
            $content = pack('cccc', $name_index['content' . $i] , $name_index['content' . ($i + 1)] , $name_index['content' . ($i + 2)] , $name_index['content' . ($i + 3)]);
            $var = unpack('V', $content);
            array_push($data_key, $var[1]);
            //echo $var[1] . '<br/>';
        }

        $goods = array();
        for ($index = 0; $index < $field_record; ++$index) {
            $id   = $data_key[$index * $field_count + 0];
            $key  = $data_key[$index * $field_count + 1];
            $type = "{$data_key[$index * $field_count + 2]}";
            $name = $key < 1 ? '' : $fields[$key - 1];
            /*if (!array_key_exists($type, $goods)) {
                $goods[$type] = array();
            }*/

            array_push($goods, array("id" => $id,
                "type" => $type,
                "name" => $name,
                "num" => 0,
                ));
            //$goods[$type][$id] = $name;
            //echo 'id:' . $id . ' ' . 'name:' . $name . ' ' . 'type:' . $type . '<br/>';
        }

        return $goods;
    }

    public function add()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $gift_type    = $this->input->get_post('gift_type');
        $gift_name    = $this->input->get_post('gift_name');
        $gift_num     = $this->input->get_post('gift_num');
        $gift_context = $this->input->get_post('gift_context');
        $begin_date   = $this->input->get_post('begin_date'  );  
        $end_date     = $this->input->get_post('end_date'    );  
        $day          = $this->input->get_post('day'         );  
        $limit_dev    = $this->input->get_post('limit_dev'   );  
        $limit_acc    = $this->input->get_post('limit_acc'   );  
        $limit_num    = $this->input->get_post('limit_num'   );  
        $items        = $this->input->get_post('items'       );  

        $rsp = array('ret' => 1);
        $this->output->set_content_type('application/json;charset=utf-8');
        if ($gift_name == '') {
            $rsp['info'] = '名字不能为空';
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        /*$gift_type    = 0;
        $gift_name    = "null";
        $gift_num     = 0;
        $gift_context = "null";
        $begin_date   = 0;
        $end_date     = 0;
        $day          = 0;
        $limit_dev    = 0;
        $limit_acc    = 0;
        $limit_num    = 0;
        $items        = "null";*/
        date_default_timezone_set("Asia/Shanghai");
        if (!$begin_date) {
            $begin_date = 0;
        } else {
            $begin_date = strtotime($begin_date);
        }

        if (!$end_date) {
            $end_date = 0;
        } else {
            $end_date   = strtotime($end_date);
        }

        $id = $this->_save_gift($gift_type, $gift_name, $gift_num, $gift_context, $begin_date, $end_date, $day, $limit_dev, $limit_acc, $limit_num, $items);
        if ($id != 0) {
            $this->_save_code($id, 0, $gift_num, 0, 0);
        }

        $rsp['ret'] = 0;
        $rsp['gift_name'] = $gift_name;
        $rsp['gift_num']  = $gift_num;
        $rsp['items'] = $items;
        $this->output->set_output(json_encode($rsp));
    }

    private function _save_gift($gift_type, $gift_name, $gift_num, $gift_context, $begin_date, $end_date, $day, $limit_dev, $limit_acc, $limit_num, $items)
    {
        $sql = "insert into t_gift set type='$gift_type', name='$gift_name', num='$gift_num', context='$gift_context', items='$items', start_time='$begin_date', end_time='$end_date', day='$day', limit_device='$limit_dev', limit_account='$limit_acc', limit_use='$limit_num';";
        $query = $this->gift_db->query($sql);
        if ($query) {
            return $this->gift_db->insert_id();
        }

        return 0;
    }

    private function _save_user_code($id, $start_index, $num, $s, $role_id)
    {
        $key  = "@r0ckpi1e";
        $time = time();
        $table_index = $role_id % 10;
        for ($i = $start_index; $i < $num; ++$i) {
            //$code = md5($id + $num + $i + $key + $role_id + $s) . "-" . $table_index;
            $code = $role_id . $s . "-" . $table_index;
            $sql = "insert into t_gift_code_$table_index set gift_id='$id', code='$code', state=0, server='$s', role_id='$role_id', use_time='$time';";
            $this->gift_db->query($sql);
        }
    }

    private function _save_code($id, $start_index, $num, $s, $role_id)
    {
        $key  = "@r0ckpi1e";
        $time = time();
        $table_index = $role_id % 10;
        for ($i = $start_index; $i < $num; ++$i) {
            $code = 'c'.crc32($id + $num + $i + $key + $role_id + $s) . "-" . $table_index;
            //$code = "c$time$i-$table_index";
            //$code = $role_id . "-" . $table_index;
            $sql = "insert into t_gift_code_$table_index set gift_id='$id', code='$code', state=0, server='$s', role_id='$role_id', use_time='$time';";
            $this->gift_db->query($sql);
        }
    }

    public function del()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id        = $this->input->get_post('id'       );  

        $rsp = array(
            'ret' => 1,
            'info' => '删除失败');
        $this->output->set_content_type('application/json;charset=utf-8');
        if ($id == 0) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        if ($this->_del_gift($id)) {
            $rsp['ret']  = 0;
            $rsp['info'] = '删除成功';
        }

        $this->output->set_output(json_encode($rsp));
    }

    private function _del_gift($id)
    {
        $sql = "delete from `t_gift` where (`id`='$id');";
        $query = $this->gift_db->query($sql);
        if ($query) {
            for ($i=0; $i < 10; ++$i) { 
                $sql = "delete from `t_gift_code_$i` where (`gift_id`='$id');";
                $query = $this->gift_db->query($sql);
            }
        }

        return $query;
    }

    private function _check_key($role_id, $s, $dev, $time, $key) {
        $k = md5($role_id . $s . $dev . "lhsg$%#*bi03g34ns0j3r0ckpi1e!@*)" . $time);
        return $k == $key;
    }

    public function use_code()
    {
        $s       = $this->input->get_post('s');
        $role_id = $this->input->get_post('role_id');
        $code    = $this->input->get_post('code');
        $dev     = $this->input->get_post('dev');
        $time    = $this->input->get_post('time');
        $key     = $this->input->get_post('key');

        $rsp = array(
            'ret' => 1,
            'info' => '使用失败');
        $this->output->set_content_type('application/json;charset=utf-8');

        if (time() < ($time - 120) || time() > ($time + 120)) {
            $rsp['info'] = '超时重试';
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $check_key = $this->_check_key($role_id, $s, $dev, $time, $key);
        if (!$role_id || !$code || !$check_key) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $gift = $this->_to_gift($code);
        if (!$gift) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $index   = $this->get_table_index($code);
        $code_id = $this->_get_code_id($index, $code);
        if ($code_id == -1) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $use = $this->_get_code_use($index, $code_id);
        if (count($use) >= $gift->limit_use) {
            $rsp['info'] = '超过使用次数';
            //var_dump($rsp);
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        foreach ($use as $info) {
            if ($info['role_id'] == $role_id && $info['s'] == $s) {
                $rsp['info'] = '已经使用过此礼包';
                $this->output->set_output(json_encode($rsp));
                return ;
            }

        }

        $role_use = $this->_get_role_use($index, $role_id);
        foreach ($role_use as $info) {
            $type = $this->_to_type($index, $info['code_id']);
            if ($type == 0 && $gift->type == 0) {
                $rsp['info'] = '已经获得过邀请礼包';
                $this->output->set_output(json_encode($rsp));
                return ;
            }
        }

        $ret = $this->_get_server_info();
        $addr = "";
        $port = 0;
        foreach ($ret as $server) {
            if ($server['id'] == $s) {
                $addr = $server['ip'];
                $port = $server['inner_port'];
                break;
            }
        }

        if ($port == 0) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $objs = json_decode($gift->items);
        $items = array();
        foreach($objs as $key => $val) {
            array_push($items, array('id' => $key, 'num' => $val));
        }

        $this->_send_mail($addr, $port, $role_id, $gift, $items);

        $this->_insert_use($index, $code_id, $s, $dev, $role_id);

        $rsp['ret'] = 0;
        $rsp['info'] = '使用成功 请在邮件中收取';
        $this->output->set_output(json_encode($rsp));
    }

    private function _to_type($index, $code_id) {
        $sql = "select `gift_id` from `t_gift_code_$index` where `id`='$code_id'";
        $query = $this->gift_db->query($sql);
        foreach ($query->result() as $obj) {
            $sql = "select `type` from `t_gift` where `id`='$obj->gift_id'";
            $query = $this->gift_db->query($sql);
            foreach ($query->result() as $gift) {
                return $gift->type;
            }

        }

        return -1;
    }

    private function _to_gift($code)
    {
        $index = $this->get_table_index($code);
        if ($index == -1) {
            return false;
        }

        $sql = "select `gift_id` from `t_gift_code_$index` where `code`='$code'";
        $query = $this->gift_db->query($sql);
        foreach ($query->result() as $obj) {
            $sql = "select * from `t_gift` where `id`=$obj->gift_id";
            $query = $this->gift_db->query($sql);
            foreach ($query->result() as $gift) {
                return $gift;
            }

        }

        return false;
    }

    private function _send_mail($addr, $port, $role_id, $gift, $items)
    {
        $this->load->library('backdoor_lib');
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.card.php';

        $req = new \inner\sendGoods_req();

        $user = $req->add_user();
        $user->set_id($role_id);

        foreach ($items as $item) {
            $gi = $req->add_gi();
            $gi->set_goods_type(0);
            $gi->set_goods_id  ($item['id']);
            $gi->set_goods_num ($item['num']);
            $gi->set_bind      (1);
        }

        $type    = 0;
        $title   = $gift->name;
        $content = $gift->context;

        $req->set_type($type);
        $req->set_title($title);
        $req->set_content($content);
        $req->set_invalidTime(0);
        $req->set_count(0);

        $sendStr = $this->backdoor_lib->pack("inner.sendGoods_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->send($addr, $port, $sendStr);
        if (!$result) {
            return false;
        }

        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\sendGoods_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();
    }

    public function get()
    {
        $s       = $this->input->get_post('s');
        $role_id = $this->input->get_post('role_id');
        $dev     = $this->input->get_post('dev');
        $time    = $this->input->get_post('time');
        $key     = $this->input->get_post('key');

        $rsp = array(
            'ret' => 1,
            'info' => '未开放此功能');
        $this->output->set_content_type('application/json;charset=utf-8');

        if (time() < ($time - 120) || time() > ($time + 120)) {
            $rsp['info'] = '超时重试';
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $check_key = $this->_check_key($role_id, $s, $dev, $time, $key);

        if (!$role_id || !$check_key) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        if (!$s) {
            $s = 0; 
        }

        $ret = $this->_load_role_code($s, $role_id);
        if (!$ret) {
            $id = $this->_load_role_gift_id($s);
            if (!$id) {
                $this->output->set_output(json_encode($rsp));
                return ;
            }

            $this->_save_user_code($id, 0, 1, $s, $role_id);
            $ret = $this->_load_role_code($s, $role_id);
        }

        $data = array();
        $data['code']  = $ret['code']->code;
        $data['items'] = $ret['gift']->items;
        $rsp['ret']  = 0;
        $rsp['info'] = '成功';
        $rsp['data'] = $data;
        $this->output->set_output(json_encode($rsp));
    }

    private function _load_role_gift_id($s)
    {
        $sql = "select id from `t_gift` where `type`=0;";
        $query = $this->gift_db->query($sql);
        foreach ($query->result() as $obj) {
            return $obj->id;
        }

        return 0;
    }

    private function _load_role_code($s, $role_id)
    {
        $ret = array();
        $index = $role_id % 10;
        $sql = "select * from `t_gift_code_$index` where `role_id`='$role_id';";
        $query = $this->gift_db->query($sql);
        foreach ($query->result() as $obj) {
            $ret['code'] = $obj; 
            $sql = "select * from `t_gift` where `id`=$obj->gift_id;";
            $query = $this->gift_db->query($sql);
            foreach ($query->result() as $obj) {
                $ret['gift'] = $obj;
                return $ret;
            }
        }

        return false;
    }

    public function modify()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $id           = $this->input->get_post('id');
        $gift_type    = $this->input->get_post('gift_type');
        $gift_name    = $this->input->get_post('gift_name');
        $gift_num     = $this->input->get_post('gift_num');
        $gift_context = $this->input->get_post('gift_context');
        $begin_date   = $this->input->get_post('begin_date'  );  
        $end_date     = $this->input->get_post('end_date'    );  
        $day          = $this->input->get_post('day'         );  
        $limit_dev    = $this->input->get_post('limit_dev'   );  
        $limit_acc    = $this->input->get_post('limit_acc'   );  
        $limit_num    = $this->input->get_post('limit_num'   );  
        $items        = $this->input->get_post('items'       );  

        $rsp = array('ret' => 1);
        $rsp['info'] = '修改失败';
        $this->output->set_content_type('application/json;charset=utf-8');
        if ($gift_name == '') {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        date_default_timezone_set("Asia/Shanghai");
        if (!$begin_date) {
            $begin_date = 0;
        } else {
            $begin_date = strtotime($begin_date);
        }

        if (!$end_date) {
            $end_date = 0;
        } else {
            $end_date   = strtotime($end_date);
        }

        $gift = $this->_load_gift_by_id($id);
        if (count($gift) < 1) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $this->_update_gift($id, $gift_type, $gift_name, $gift_num, $gift_context, $begin_date, $end_date, $day, $limit_dev, $limit_acc, $limit_num, $items);
        if ($gift_num - $gift[0]['num'] > 0) {
            $this->_save_code($id, $gift[0]['num'], $gift_num, 0, 0);
        }

        $rsp['ret'] = 0;
        $rsp['info'] = '成功';
        $rsp['gift_name'] = $gift_name;
        $rsp['gift_num' ] = $gift_num;
        $rsp['items'    ] = $items;
        $this->output->set_output(json_encode($rsp));
    }

    private function _update_gift($id, $gift_type, $gift_name, $gift_num, $gift_context, $begin_date, $end_date, $day, $limit_dev, $limit_acc, $limit_num, $items)
    {
        $sql = "UPDATE `t_gift` SET `type`='$gift_type', name='$gift_name', num='$gift_num', context='$gift_context', items='$items', start_time='$begin_date', end_time='$end_date', day='$day', limit_device='$limit_dev', limit_account='$limit_acc', limit_use='$limit_num' WHERE `id`='$id';";
        $query = $this->gift_db->query($sql);

        return 0;
    }

    public function test()
    {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $code = $this->input->get_post('code');

        $rsp = array();
        $rsp['index'] = -1;
        $this->output->set_content_type('application/json;charset=utf-8');

        $ret = $this->get_table_index($code);
        if ($ret != -1) {
            $rsp['index'] = $ret;
        }

        $ret = $this->_get_server_info();
        $this->_insert_use(0, 0, 0, 0, 0);

        $this->output->set_output(json_encode($rsp));
    }

    private function get_table_index($code)
    {
        $str = explode('-', $code);
        if (count($str) != 2 || strlen($str[1]) != 1) {
            return -1;
        }

        return $str[1];
    }

    private function _get_server_info()
    {
        $db = $this->load->database('server_list', TRUE);
        $ret = $db->query('select * from server_list;');
        return $ret->result_array();
    }

    private function _get_code_id($index, $code)
    {
        $sql = "select * from `t_gift_code_$index` where `code`='$code'";
        $ret = $this->gift_db->query($sql);
        foreach ($ret->result_array() as $info) {
            return $info['id'];
        }

        return -1;
    }

    private function _get_code_use($index, $id)
    {
        $sql = "select * from `t_gift_use_$index` where `code_id`='$id'";
        $ret = $this->gift_db->query($sql);
        return $ret->result_array();
    }

    private function _get_role_use($index, $id)
    {
        $sql = "select * from `t_gift_use_$index` where `role_id`='$id'";
        $ret = $this->gift_db->query($sql);
        return $ret->result_array();
    }

    private function _insert_use($index, $code_id, $s, $dev, $role_id)
    {
        $time = time();
        $sql = "insert into `t_gift_use_$index`(`code_id`,`s`,`dev`,`role_id`,`time`) values ('$code_id','$s','$dev','$role_id', '$time')";
        $ret = $this->gift_db->query($sql);
    }
}

?>
