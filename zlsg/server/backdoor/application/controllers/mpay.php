<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class mpay extends CI_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->library('curl'); 
        $this->load->library('snsSigCheck');
        //$this->load->helper('cookie');
        $this->config->load('backdoor', TRUE);

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.pay.php';

        $this->load->database('game');
        $this->load->model('backdoor_db', 'game_db');
        $this->load->library('backdoor_lib');
    }

    public function index() {
        //		$this->load->view('welcome_message');
    }

    private function _cookie(&$org_loc, $platform) {
        if (!$platform) {
            $platform = 2;
        }

        $session_id   = '';
        $session_type = '';
        if ($platform == 2) {
            $session_id   = 'openid'; 
            $session_type = 'kp_actoken';
        } else if ($platform == 1) {
            $session_id   = 'hy_gameid';
            $session_type = 'wc_actoken';
        }

        $cookie = array(
            'session_id'   => $session_id,
            'session_type' => $session_type,
            'org_loc'      => $org_loc,
            //'appip'        => '',
        );

        return $cookie;
    }

    private function _sig($method, &$url_path, &$post, $appkey) {
        //ksort($post);
        //$secret = $this->config->item('appkey', 'backdoor') . '&';
        $secret = $appkey . '&';
        $sig = $this->snssigcheck->makeSig($method, $url_path, $post, $secret);
        $post['sig'] = $sig;
    }

    private function _post($appid) {
        $openid    = $this->input->get_post('openid');
        $openkey   = $this->input->get_post('openkey');
        $pay_token = $this->input->get_post('pay_token');
        $pf        = $this->input->get_post('pf');
        $pfkey     = $this->input->get_post('pfkey');
        $zoneid    = $this->input->get_post('zoneid');
        $format    = 'json';
        return array(
            'openid'    => $openid,
            'openkey'   => $openkey,
            'pay_token' => $pay_token,
            'appid'     => $appid,
            'ts'        => time(),
            'pf'        => $pf,
            'pfkey'     => $pfkey,
            'zoneid'    => $zoneid,
            'format'    => $format,
        );
    }

    public function get_balance_m() {
        $base_url = $this->config->item('mpay_base_url', 'backdoor');
        $path_url = $this->config->item('mpay_get_balance_m', 'backdoor');

        $platform    = $this->input->get_post('platform');
        $appid  = '';
        $appkey = '';
        if ($platform == 2) {
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
        } else {
            //$appid  = $this->config->item('wxappid', 'backdoor');
            //$appkey = $this->config->item('wxappkey', 'backdoor');
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
        }

        $post = $this->_post($appid);
        $post['accounttype'] = 'common';

        if ($platform == 1) {
            $post['pay_token'] = '';
        }

        $ip = $this->input->ip_address(); if ($this->input->valid_ip($ip)) {
            $post['userip'] = $ip;
        }

        $this->_sig('GET', $path_url, $post, $appkey);

        $params = '?';
        $params .= http_build_query($post, NULL, '&');
        $path = $base_url . $path_url . $params;

        //初始化
        $ch = curl_init();
        //设置选项，包括URL
        curl_setopt($ch, CURLOPT_URL, $path);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_COOKIE, http_build_query($this->_cookie($path_url, $platform), NULL, ';'));   
        /*$header[]= 'Cookie: ' . 'session_id=openid;session_type=kp_actoken;org_loc=%2Fmpay%2Fget_balance_m';  
        curl_setopt($ch, CURLOPT_HTTPHEADER, $header);*/
        //执行并获取HTML文档内容
        $ret = curl_exec($ch);
        //释放curl句柄
        curl_close($ch);

        /*$this->curl->create($path);
        $cookie = $this->_cookie($path_url);
        $this->curl->set_cookies($cookie);
        //$this->curl->http_method('GET');
        $ret = $this->curl->execute();*/

        if (!$ret) {
            $rsp = json_decode("{\"ret\":1,\"msg\":\"连接支付服务器失败\"}");
        } else {
            $rsp = json_decode($ret);
        }

        if ($rsp->ret > 0) {
        }

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output($ret);
    }

    private function _get_balance() {
        $base_url = $this->config->item('mpay_base_url', 'backdoor');
        $path_url = $this->config->item('mpay_get_balance_m', 'backdoor');

        $platform    = $this->input->get_post('platform');
        $appid  = '';
        $appkey = '';
        if ($platform == 2) {
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
        } else {
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
            //$appid  = $this->config->item('wxappid', 'backdoor');
            //$appkey = $this->config->item('wxappkey', 'backdoor');
        }

        $post = $this->_post($appid);

        if ($platform == 1) {
            $post['pay_token'] = '';
        }

        $post['accounttype'] = 'common';
        $ip = $this->input->ip_address(); if ($this->input->valid_ip($ip)) {
            $post['userip'] = $ip;
        }

        $this->_sig('GET', $path_url, $post, $appkey);

        $params = '?';
        $params .= http_build_query($post, NULL, '&');
        $path = $base_url . $path_url . $params;

        //var_dump($path);

        //初始化
        $ch = curl_init();
        //设置选项，包括URL
        curl_setopt($ch, CURLOPT_URL, $path);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_COOKIE, http_build_query($this->_cookie($path_url, $platform), NULL, ';'));   
        //执行并获取HTML文档内容
        $ret = curl_exec($ch);
        //释放curl句柄
        curl_close($ch);

        if (!$ret) {
            $rsp = json_decode("{\"ret\":1,\"msg\":\"连接支付服务器失败\"}");
        } else {
            $rsp = json_decode($ret);
        }

        return $rsp;

        if ($rsp->ret == 0) {
            return $rsp->balance;
        } else {
            return FALSE;
        }
    }

    public function pay_m() {
        /*$output = array('status' => 1, 'info' => '删档测试中暂停充值', 'data' => $data);
        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output(json_encode($output));
        return ;*/

        $rsp = $this->_get_balance();
        $balance = 0;
        if ($rsp->ret != 0) {
            $data = $rsp->ret != 0 ? $rsp->msg : 'balance error';
            $output = array('status' => 0, 'info' => '充值失败请重新登陆重试', 'data' => $data);
            $this->output->set_content_type('application/json;charset=utf-8');
            if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
                $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
            } else {
                $this->output->set_output(json_encode($output));
            }

            return ;
        } else {
            $balance = $rsp->balance;
        }

        if ($balance <= 0) {
            return ;
        }

        $base_url = $this->config->item('mpay_base_url', 'backdoor');
        $path_url = $this->config->item('mpay_pay_m', 'backdoor');

        $platform    = $this->input->get_post('platform');
        $appid  = '';
        $appkey = '';
        if ($platform == 2) {
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
        } else {
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
            //$appid  = $this->config->item('wxappid', 'backdoor');
            //$appkey = $this->config->item('wxappkey', 'backdoor');
        }

        $post = $this->_post($appid);

        if ($platform == 1) {
            $post['pay_token'] = '';
        }

        $post['amt'] = $this->input->get_post('amt');
        $post['amt'] = $balance;
        $role_id     = $this->input->get_post('role_id');
        $post['accounttype'] = 'common';
        $post['appremark']   = 'test';
        $post['payitem']     = $this->input->get_post('payitem');
        if (!$post['payitem']) {
            $post['payitem'] = 'gold' . $post['amt'];
        }

        $ip = $this->input->ip_address(); 
        if ($this->input->valid_ip($ip)) {
            $post['userip'] = $ip;
        }

        $this->_sig('GET', $path_url, $post, $appkey);
        $params = http_build_query($post, NULL, '&');
        $path = $base_url . $path_url . '?' . $params;

        //var_dump($path);

        // 初始化
        $ch = curl_init();
        // 设置选项，包括URL
        curl_setopt($ch, CURLOPT_URL, $path);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_COOKIE, http_build_query($this->_cookie($path_url, $platform), NULL, ';'));   
        // 执行并获取HTML文档内容
        $output = curl_exec($ch);
        // 释放curl句柄
        curl_close($ch);

        if (!$output) {
            $rsp = json_decode("{\"ret\":1,\"msg\":\"连接支付服务器失败\"}");
        } else {
            $rsp = json_decode($output);
        }

        $status = 0;
        $info   = '扣款成功 ';
        if ($rsp->ret == 0) { // 发货
            $post['currency']    = 'rmb';
            $post['inpour_type'] = 'tencent';
            $post['addcoin'    ] = 0;
            $post['ex_type'    ] = $this->input->get_post('ex_type');;
            $post['depay']       = 0.0; 
            $post['g_depay']     = 0.0;
            if ($this->_do_pay($post, $role_id, $rsp->billno)) {
                $status = 1;
                $info .= $rsp->balance;
                $post['amt'] = $rsp->balance;
            } else { // 退款
                $rsp = $this->_cancel_pay_m($post, $rsp->billno, $platform, $appkey);
                $info .= '发货失败 ';
                if ($rsp->ret == 0) {
                    $info .= '退款成功';
                } else {
                    $info .= '退款失败 ' . $rsp->msg;
                }
            }
        } else {
            //var_dump($rsp);
            $info = '扣款失败 ' . $rsp->ret . ' ' . $rsp->msg;
        }

        $output = array('status' => $status, 'info' => $info, 'data' => $post['amt'], 'ret' => $rsp->ret);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }

    private function _do_pay(&$post, $id, $billno) {
        if (!$id) {
            return false;
        }

        $account = $post['openid'];
        $player_id = $this->game_db->account_to_player_id($post['openid']);
        if (!$player_id) {
            return false;
        }

        $req = new \inner\pay_req();
        $user = new \inner\inner_user();
        $user->set_id($id);
        $user->set_account_name($account);
        $user->set_nickname($player_id);
        $req->set_user       ($user);
        $req->set_amt        ($post['amt']);
        $req->set_pay_id     ($billno);
        $req->set_currency   ($post['currency'])   ; 
        $req->set_inpour_type($post['inpour_type']);
        $req->set_addcoin    ($post['addcoin'    ]);
        $req->set_ex_type    ($post['ex_type'    ]);
        $req->set_depay      ($post['depay']      );
        $req->set_g_depay    ($post['g_depay']    );

        $sendStr = $this->backdoor_lib->pack("inner.pay_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->write_and_send($sendStr);
        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\pay_rsp();
        $rsp->ParseFromString($result);
        return $rsp->tail()->status() == 1;
        /*$tail = $rsp->tail();
        $output = array('status' => $tail->status(), 'info' => $tail->info(), 'data' => NULL);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }*/
    }

    private function _cancel_pay_m(&$post, $billno, $platform, $appkey) {
        unset($post['sig']);
        unset($post['payitem']);
        unset($post['appremark']);
        $base_url = $this->config->item('mpay_base_url', 'backdoor');
        $path_url = $this->config->item('mpay_cancel_pay_m', 'backdoor');
        $post['billno'] = $billno;
        $post['ts']     = time();
        $this->_sig('GET', $path_url, $post, $appkey);
        $params = http_build_query($post, NULL, '&');
        $path = $base_url . $path_url . '?' . $params;

        /*$this->curl->create($base_url . $path_url . '?' . $params);
        $cookie = $this->_cookie($path_url);
        $this->curl->set_cookies($cookie);
        $ret = $this->curl->execute();*/

        //var_dump($path);

        //初始化
        $ch = curl_init();
        //设置选项，包括URL
        curl_setopt($ch, CURLOPT_URL, $path);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_COOKIE, http_build_query($this->_cookie($path_url, $platform), NULL, ';'));   
        //执行并获取HTML文档内容
        $ret = curl_exec($ch);
        //释放curl句柄
        curl_close($ch);

        if (!$ret) {
            $rsp = json_decode("{\"ret\":1,\"msg\":\"连接退款服务器失败\"}");
        } else {
            $rsp = json_decode($ret);
        }

        if ($rsp->ret == 0) {
            // 取消成功
        } else {
            // 取消失败
        }

        return $rsp;
    }

    public function present_m() {
        $platform = $this->input->get_post('platform');
        $appid  = '';
        $appkey = '';
        if ($platform == 2) {
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
        } else {
            $appid  = $this->config->item('appid', 'backdoor');
            $appkey = $this->config->item('appkey', 'backdoor');
            //$appid  = $this->config->item('wxappid', 'backdoor');
            //$appkey = $this->config->item('wxappkey', 'backdoor');
        }

        $base_url = $this->config->item('mpay_base_url', 'backdoor');
        $path_url = $this->config->item('mpay_present_m', 'backdoor');
        $post = $this->_post($appid);
        if ($platform == 1) {
            $post['pay_token'] = '';
        }
        $post['discountid'  ] = $this->input->get_post('discountid');
        $post['giftid'      ] = $this->input->get_post('giftid');
        $post['presenttimes'] = $this->input->get_post('presenttimes');
        $platform             = $this->input->get_post('platform');
        $this->_sig('GET', $path_url, $post, $appkey);
        $params = http_build_query($post, NULL, '&');
        $path = $base_url . $path_url . '?' . $params;

        //初始化
        $ch = curl_init();
        //设置选项，包括URL
        curl_setopt($ch, CURLOPT_URL, $path);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_COOKIE, http_build_query($this->_cookie($path_url, $platform), NULL, ';'));   
        //执行并获取HTML文档内容
        $ret = curl_exec($ch);
        //释放curl句柄
        curl_close($ch);

        if (!$ret) {
            $rsp = json_decode("{\"ret\":1,\"msg\":\"连接赠送服务器失败\"}");
        } else {
            $rsp = json_decode($ret);
        }

        $output = array('status' => $status, 'info' => $info, 'data' => $post['amt']);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }

    public function pay_m_t() {
        $post                = array();
        $post['openid']      = $this->input->get_post('openid');
        $role_id             = $this->input->get_post('role_id');
        $post['amt']         = $this->input->get_post('amt');
        $post['currency']    = 'rmb';
        $post['inpour_type'] = 'tencent';
        $post['addcoin'    ] = 0;
        $post['ex_type'    ] = 1;
        $post['depay']       = 0.1; 
        $post['g_depay']     = 0.2;
        $ret                 = FALSE;
        $billno              = '73272';

        $ret = $this->_do_pay($post, $role_id, $billno);

        $output = array('status' => 1, 'info' => $ret ? '成功' : '失败', 'data' => $post['amt']);
        $this->output->set_content_type('application/json;charset=utf-8');
        if (version_compare(PHP_VERSION, '5.4.0') >= 0) {
            $this->output->set_output(json_encode($output, JSON_UNESCAPED_UNICODE));
        } else {
            $this->output->set_output(json_encode($output));
        }
    }
}

/* End of file mpay.php */
/* Location: ./application/controllers/mpay.php */
