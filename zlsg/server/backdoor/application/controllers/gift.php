<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class gift extends CI_Controller {
    public function __construct() 
    {
        parent::__construct();
        $this->config->load('backdoor', TRUE);
        $this->load->database("game");
        $this->load->helper('url');
    }

    public function index()
    {
        $data = array();
        $this->load->view('gift.php', $data);
    }

    public function code()
    {
        $data = array();
        $data['gifts'] = $this->_load_gift();
        $this->load->view('gift_code.php', $data);
    }

    public function get_gift()
    {
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
        $query = $this->db->query("select * from t_gift");
        return $query->result_array();
    }

    private function _load_gift_by_id($id)
    {
        $query = $this->db->query("select * from t_gift where `id`='$id';");
        return $query->result_array();
    }

    private function _load_codes($id)
    {
        $query = $this->db->query("select * from t_gift_code where gift_id = $id");
        return $query->result_array();
    }

    public function itemsjson()
    {
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
        $query = $this->db->query($sql);
        if ($query) {
            return $this->db->insert_id();
        }

        return 0;
    }

    private function _save_code($id, $start_index, $num, $s, $role_id)
    {
        $key  = "@r0ckpi1e";
        $time = time();
        $table_index = $role_id % 10;
        for ($i = $start_index; $i < $num; ++$i) {
            $code = md5($id + $num + $i + $key + $role_id + $s) . "-" . $table_index;
            $sql = "insert into t_gift_code set gift_id='$id', code='$code', state=0, server='$s', role_id='$role_id', use_time='$time';";
            $this->db->query($sql);
        }
    }

    public function del()
    {
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
        $query = $this->db->query($sql);
        if ($query) {
            $sql = "delete from `t_gift_code` where (`gift_id`='$id');";
            $query = $this->db->query($sql);
        }

        return $query;
    }

    public function use_code()
    {
        $s       = $this->input->get_post('s');
        $role_id = $this->input->get_post('role_id');
        $code    = $this->input->get_post('code');

        $rsp = array(
            'ret' => 1,
            'info' => '使用失败');
        $this->output->set_content_type('application/json;charset=utf-8');
        if (!$role_id || !$code) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $gift = $this->_to_gift($code);
        if (!$gift) {
            $this->output->set_output(json_encode($rsp));
            return ;
        }

        $objs = json_decode($gift->items);
        $items = array();
        foreach($objs as $key => $val) {
            array_push($items, array('id' => $key, 'num' => $val));
        }

        $this->_send_mail($role_id, $gift, $items);


        $rsp['ret'] = 0;
        $rsp['info'] = '使用成功 请在邮件中收取';
        $this->output->set_output(json_encode($rsp));
    }

    private function _to_gift($code)
    {
        $sql = "select `gift_id` from `t_gift_code` where `code`='$code'";
        $query = $this->db->query($sql);
        foreach ($query->result() as $obj) {
            $sql = "select * from `t_gift` where `id`=$obj->gift_id";
            $query = $this->db->query($sql);
            foreach ($query->result() as $gift) {
                return $gift;
            }

        }

        return false;
    }

    public function _send_mail($role_id, $gift, $items)
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
        $result  = $this->backdoor_lib->write_and_send($sendStr);
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

        $rsp = array(
            'ret' => 1,
            'info' => '未开放此功能');
        $this->output->set_content_type('application/json;charset=utf-8');
        if (!$role_id) {
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

            $this->_save_code($id, 0, 1, $s, $role_id);
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
        $query = $this->db->query($sql);
        foreach ($query->result() as $obj) {
            return $obj->id;
        }

        return 0;
    }

    private function _load_role_code($s, $role_id)
    {
        $ret = array();
        $sql = "select * from `t_gift_code` where `role_id`='$role_id';";
        $query = $this->db->query($sql);
        foreach ($query->result() as $obj) {
            $ret['code'] = $obj; 
            $sql = "select * from `t_gift` where `id`=$obj->gift_id;";
            $query = $this->db->query($sql);
            foreach ($query->result() as $obj) {
                $ret['gift'] = $obj;
                return $ret;
            }
        }

        return false;
    }

    public function modify()
    {
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
        $query = $this->db->query($sql);

        return 0;
    }

    public function test()
    {
        $code = $this->input->get_post('code');

        $rsp = array();
        $rsp['index'] = -1;
        $this->output->set_content_type('application/json;charset=utf-8');

        $ret = $this->get_table_index($code);
        if ($ret != -1) {
            $rsp['index'] = $ret;
        }
       
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
}

?>
