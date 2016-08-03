<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class pay extends CI_Controller {
	public function __construct() {
        parent::__construct();

        $this->config->load('backdoor', TRUE);
        //$this->load->database('game');
        //$this->load->model('backdoor_db', 'game_db');
        $this->load->library('backdoor_lib');
    }

    private function _get_server_info()
    {
        $db = $this->load->database('server_list', TRUE);
        $ret = $db->get('server_list');
        return $ret->result_array();
    }

    private function _do_pay(&$post, $id, $server_id) {
        if (!$id) {
            return false;
        }

        $account = $post['openid'];
        $player_id = 0;
        //$player_id = $this->game_db->account_to_player_id($post['openid']);
        //if (!$player_id) {
            //return false;
        //}

        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.pay.php';

        $req  = new \inner\pay_req();
        $user = new \inner\inner_user();
        $user->set_id($id);
        $user->set_account_name($account);
        $user->set_nickname($player_id);
        $req->set_user       ($user);
        $req->set_amt        ($post['amt'        ]);
        $req->set_pay_id     ($post['pay_id'     ]);
        $req->set_currency   ($post['currency'   ]); 
        $req->set_inpour_type($post['inpour_type']);
        $req->set_addcoin    ($post['addcoin'    ]);
        $req->set_ex_type    ($post['ex_type'    ]);
        $req->set_depay      ($post['depay'      ]);
        $req->set_g_depay    ($post['g_depay'    ]);

        $server_info = $this->_get_server_info();
        $addr = "";
        $port = 0;
        foreach ($server_info as $server) {
            if ($server['id'] == $server_id) {
                $addr = $server['ip'];
                $port = $server['inner_port'];
                break;
            }
        }

        if (!$port) {
        	return false;
        }

        $sendStr = $this->backdoor_lib->pack("inner.pay_req", $req->SerializeToString());
        $result  = $this->backdoor_lib->send($addr, $port, $sendStr);
        if (!$result) {
        	return false;
        }
        
        $result  = $this->backdoor_lib->unpack($result);

        $rsp = new \inner\pay_rsp();
        $rsp->ParseFromString($result);
        return $rsp->tail()->status() == 1;
    }

    private function _check_sign($uid, $price, $orderNo, $remark, $status, $subject, $gameId, $payTime, $ext, $sign) {
    	$str = 'uid=$uid' . 'price=$price' . 'orderNo=$orderNo' . 'remark=$remark' . 'status=$status' . 'subject=$subject' . 'gameId=$gameId' . 'payTime=$payTime' . 'ext=$ext';
    	$appkey = 'P2GmeFW0szKCLP40CK2tOMRKQ9tHGd8J';
    	//var_dump($sign);
    	//var_dump(md5($str . $appkey));
    	return md5($str . $appkey) == $sign;
    }

    private function _check_order_paojiao(&$orderid) {
        $this->load->database('game');
        $result = $this->db->get_where('t_pay_paojiao', array('orderNo' => $orderid))->result();
        foreach ($result as $obj) {
            if ($obj->orderNo == $orderid) {
                return true;
            }
        }

        return false;
    }

    public function paojiao() {
    	$uid     = $this->input->get_post('uid');
    	$orderNo = $this->input->get_post('orderNo');
    	$price   = $this->input->get_post('price');
    	$status  = $this->input->get_post('status');
    	$remark  = $this->input->get_post('remark');
    	$subject = $this->input->get_post('subject');
    	$gameId  = $this->input->get_post('gameId');
    	$payTime = $this->input->get_post('payTime');
    	$ext     = $this->input->get_post('ext');
    	$sign    = $this->input->get_post('sign');
    	if (!$this->_check_sign($uid, $orderNo, $price, $status, $remark, $subject, $gameId, $payTime, $ext, $sign)) {
    		$this->output->set_output('success');
    		//return ;
    	}

        if ($this->_check_order_paojiao($orderNo)) {
    		$this->output->set_output('success');
            return ;
        } else {
            $row = array(
                'uid'     =>  $uid     , 
                'orderNo' =>  $orderNo ,
                'price'   =>  $price   ,
                'status'  =>  $status  ,
                'remark'  =>  $remark  ,
                'subject' =>  $subject ,
                'gameId'  =>  $gameId  ,
                'payTime' =>  $payTime ,
                'ext'     =>  $ext     ,
                'sign'    =>  $sign    ,);

            $this->db->insert('t_pay_paojiao', $row);
        }

        $post = array(
            'openid'      => $uid,
            'amt'         => $price * 10,
            'pay_id'      => $orderNo,
            'currency'    => 'rmb',
            'inpour_type' => 'paojiao',
            'addcoin'     => 1,
            'ex_type'     => 1,
            'depay'       => 0,
            'g_depay'     => 0,
        );

        if ($this->_do_pay($post, $ext, $remark)) {
            $this->output->set_output('success');
            return ;
        }

        $this->output->set_output('success');
    }

        private function _check_jiayou_sign($orderid, $username, $gameid, $roleid, $serverid, $paytype, $amount, $paytime, $attach, $sign) {
        /*gameid：41
            appid：19
            appkey ：72de577e7acae60e6385400cb154bc67
        agent：defalut*/
            $appkey = '72de577e7acae60e6385400cb154bc67';
            $attach = rawurlencode($attach);
            return $sign == md5("orderid=$orderid&username=$username&gameid=$gameid&roleid=$roleid&serverid=$serverid&paytype=$paytype&amount=$amount&paytime=$paytime&attach=$attach&appkey=$appkey");
        }

        public function _check_order_jiayou(&$orderid) {
            $this->load->database('game');
            $result = $this->db->get_where('t_pay_jiayou', array('orderid' => $orderid))->result();
            foreach ($result as $obj) {
                if ($obj->orderid == $orderid) {
                    return true;
                }
            }

            return false;
        }

        public function jiayou() {
            $orderNo = $this->input->get_post('orderid');
            $uid     = $this->input->get_post('username');
            $gameId  = $this->input->get_post('gameid');
            $ext     = $this->input->get_post('roleid');
            $remark  = $this->input->get_post('serverid');
            $payType = $this->input->get_post('paytype');
            $price   = $this->input->get_post('amount');
            $payTime = $this->input->get_post('paytime');
            $attach  = $this->input->get_post('attach');
            $sign    = $this->input->get_post('sign');

            if (!$this->_check_jiayou_sign($orderNo, $uid, $gameId, $ext, $remark, $payType, $price, $payTime, $attach, $sign)) {
                $this->output->set_output('errorSign');
                return ;
            }

            var_dump($orderNo);

            if ($this->_check_order_jiayou($orderNo)) {
                $this->output->set_output('success');
                return ;
            } else {
                $row = array(
                    'orderid'  => $orderNo,
                    'username' => $uid    , 
                    'gameid'   => $gameId ,
                    'roleid'   => $ext    ,
                    'serverid' => $remark ,
                    'paytype'  => $payType,
                    'amount'   => $price  ,
                    'paytime'  => $payTime,
                    'attach'   => $attach ,
                    'sign'     => $sign   ,);

                $this->db->insert('t_pay_jiayou', $row);
            }

            $post = array(
                'openid'      => $uid,
                'amt'         => $price * 10,
                'pay_id'      => $orderNo,
                'currency'    => 'rmb',
                'inpour_type' => 'jiayou',
                'addcoin'     => 1,
                'ex_type'     => 1,
                'depay'       => 0,
                'g_depay'     => 0,
            );

            if ($this->_do_pay($post, $ext, $remark)) {
                $this->output->set_output('success');
                //$this->output->set_output('error');
                return ;
            }

            $this->output->set_output('error');
        }

        public function callback() {

        }
    }
