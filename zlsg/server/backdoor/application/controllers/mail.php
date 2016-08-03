<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Mail extends CI_Controller {
    function __construct() {
        parent::__construct();
        $this->config->load('backdoor', TRUE);
        $game_db_name = $this->config->item('game_db_name', 'backdoor');
        $this->load->database($game_db_name);
        $this->load->model('backdoor_db', 'game_db');
        $this->load->library('backdoor_lib');
        $this->backdoor_lib->load_session();
    }

    private function _rsp($ret, $info, $data) {
        $rsp = array(
            'ret' => $ret,
            'info' => $info,
            'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($rsp));
    }

    public function index() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $this->load->view('mail.php');
    }

    public function get_items() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $ret = $this->_getGoodsList();
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($ret));
    }

    public function sendAll() {
        if (!$this->backdoor_lib->logged_in()) {
            redirect("/login", 'refresh');
            return ;
        }

        $title     = $this->input->get_post('title');
        $content   = $this->input->get_post('contents');
        $items     = $this->input->get_post('items');
        $server    = $this->input->get_post('server');
        $role_id   = $this->input->get_post('role_id');
        $all       = $this->input->get_post('all');

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.card.php';

        $req = new \inner\sendGoods_req();
        $req->set_type(7);
        $req->set_title($title);
        $req->set_content($content);
        $req->set_invalidTime(0);
        $req->set_count(0);

        if (!$items) {
            $items = array();
        }

        foreach ($items as $item) {
            $gi = $req->add_gi();
            $gi->set_goods_type($item['type']);
            $gi->set_goods_id  ($item['id']);
            if ($item['num'] == 0) {
                continue;
            }

            $gi->set_goods_num ($item['num']);

            if (isset($item['bind'])) {
                $gi->set_bind($item['bind']);
            } else {
                $gi->set_bind(1);
            }  
        }

        //var_dump($all);

        if ($all === 'false') {
            if (!$role_id) {
                //var_dump($role_id);
                return ; 
            }

            $req->set_type(1);
            $role_ids = explode(',', $role_id);
            foreach ($role_ids as $item) {
                if ($item == '' || $item == 0)  {
                    continue;
                }

                $user = $req->add_user();
                $user->set_id($item);
            }
        }

        /*$this->db->select('role_id');
        $ret = $this->db->get('tb_player_role');
        $index = 1;
        foreach ($ret->result_array() as $row) {
            $user = $req->add_user();
            $user->set_id($row['role_id']);
        }*/

        $sendStr = $this->backdoor_lib->pack("inner.sendGoods_req", $req->SerializeToString());

        foreach ($server as $svr) {
            $result  = $this->backdoor_lib->Send($svr['ip'], $svr['inner_port'], $sendStr);
            //$result  = $this->backdoor_lib->write_and_send($sendStr);
            if (!$result) {
                return ;
            }

            $result  = $this->backdoor_lib->unpack($result);
            $rsp = new \inner\sendGoods_rsp();
            $rsp->ParseFromString($result);
            $tail = $rsp->tail();
            $output = array('status' => $tail->status(), 'info' => $tail->info(), 'data' => NULL);
        }

        $this->_rsp(0, "success", $output);
    }

    public function sendEmail() {
        $ret = $this->backdoor_lib->check_sign($this);
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }

        $type      = $this->input->get_post('type');
        $user      = $this->input->get_post('user');
        $usre_type = $this->input->get_post('userType');
        $title     = $this->input->get_post('title');
        $content   = $this->input->get_post('content');
        /*if ($type == 1) {
            $this->load->model('backdoor', 'db_role');
            $roles = array();
            $this->db_role->to_roles($this->input->get_post('user'), $this->input->get_post('user_type'), $roles);
        } else {

        }*/
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.email.php';

        $req = new \inner\sendEmail_req();

        $roles = array();
        $this->game_db->to_roles($user, $user_type, $roles);
        foreach ($roles as $role) {
            $user = $req->add_user($user);
            $user->set_id($role['role_id']);
            $user->set_account_name($role['account']);
            $user->set_nickname($role['role_name']);
        }

        $req->set_type($type);
        $req->set_title($title);
        $req->set_content($content);

        $sendStr = $this->backdoor_lib->pack("inner.sendEmail_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\sendEmail_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();
        $output = array('status' => $tail->status(), 'info' => $tail->info(), 'data' => NULL);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }

    private $goods_types = array('0' => '金钱',
        '1' => '普通物品',
        '2' => '主角装备',
        '3' => '美人装备',
        '4' => '传送卷轴',
        '5' => '使用道具',
        '7' => '碎片',
        '6' => '药水',
        );

    public function getCardList() {
    }

    public function getGoodsList() {
        $ret = $this->backdoor_lib->check_sign($this);
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }

        $data = $this->_getGoodsList();

        $output = array('status' => 1, 'info' => '', 'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }

    private function _getGoodsList() {
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
            if (!array_key_exists($type, $goods)) {
                $goods[$type] = array();
            }

            $goods[$type][$id] = $name;
            //echo 'id:' . $id . ' ' . 'name:' . $name . ' ' . 'type:' . $type . '<br/>';
        }

        $goods['type'] = $this->goods_types;

        return $goods;
    }

    public function sendGoods() {
        $ret = $this->backdoor_lib->check_sign($this);
        if (is_string($ret)) {
            $ret = urldecode(json_encode(array('status' => 0, 'info' => urlencode($ret), 'data' => NULL)));
            $this->output->set_output($ret);
            return ;
        }

        $type         = $this->input->get_post('type');
        $user         = $this->input->get_post('user');
        $user_type    = $this->input->get_post('userType');
        $name         = $this->input->get_post('name');
        $title        = $this->input->get_post('title');
        $content      = $this->input->get_post('content');
        $goods        = $this->input->get_post('goods');
        $invalid_time = $this->input->get_post('invalidTime');
        $count        = $this->input->get_post('count');

        $items = array();
        $goods_str = explode('&', $goods);
        foreach ($goods_str as $str) {
            $desc = explode('_', $str);
            if (count($desc) == 3) {
                $itme = array();
                $item['type'] = $desc[0];
                $item['id']   = $desc[1];
                $item['num']  = $desc[2];
                $item['bind'] = 0;
                array_push($items, $item);
            } else if (count($desc) == 4) {
                $itme = array();
                $item['type'] = $desc[0];
                $item['id']   = $desc[1];
                $item['num']  = $desc[2];
                $item['bind'] = $desc[3];
                array_push($items, $item);
            }
        }

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.card.php';

        $req = new \inner\sendGoods_req();

        $roles = array();
        $this->game_db->to_roles($user, $user_type, $roles);
        if (count($roles) == 0) {
            $this->output->set_content_type('application/json;charset=utf-8')
                ->set_output(json_encode(array('status' => 0, 'info' => '参数用户列表为空', 'data' => null), JSON_UNESCAPED_UNICODE));
            return ;
        }

        foreach ($roles as $role) {
            $user = $req->add_user();
            $user->set_id($role['role_id']);
            $user->set_account_name($role['account']);
            $user->set_nickname($role['role_name']);
        }

        foreach ($items as $item) {
            $gi = $req->add_gi();
            $gi->set_goods_type($item['type']);
            $gi->set_goods_id  ($item['id']);
            $gi->set_goods_num ($item['num']);
            $gi->set_bind      ($item['bind']);
        }

        $req->set_type($type);
        $req->set_title($title);
        $req->set_content($content);
        $req->set_invalidTime(0);
        $req->set_count(0);

        $sendStr = $this->backdoor_lib->pack("inner.sendGoods_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\sendGoods_rsp();
        $rsp->ParseFromString($result);
        $tail = $rsp->tail();
        $output = array('status' => $tail->status(), 'info' => $tail->info(), 'data' => null);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }
}

/* End of file mail.php */
/* Location: ./application/controllers/mail.php */
