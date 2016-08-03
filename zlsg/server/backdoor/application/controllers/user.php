<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class user extends CI_Controller {

	public function __construct() {
		parent::__construct();

		$this->config->load('backdoor', TRUE);
	}

	public function check() {
		$pf  = $this->input->get_post('pf');
        $sid = $this->input->get_post('sid');

        switch ($pf) {
        case "paojiao": $this->paojiao(); return;
        case "jiayou" : $this->jiayou(); return;
        case "qq"     : $this->_auth_verify_login(); return;
        case "weixin" : $this->_auth_check_token(); return;
        }
	}

	public function sign_in() {

	}

	// 腾讯平台 
	public function test() {
		$platform = $this->input->get_post('platform');
		if ($platform == 2) {
			$this->_auth_verify_login();
		} else {
			$this->_auth_check_token();
		}
	}

	private function _get_app($pf) {
		$appid    = '';
		$appkey   = '';
		if ($pf == 'qq') { // qq
			$appid  = $this->config->item('appid', 'backdoor');
			$appkey = $this->config->item('appkey', 'backdoor');
		} else {
			$appid  = $this->config->item('wxappid', 'backdoor');
			$appkey = $this->config->item('wxappkey', 'backdoor');
		}

		return array('appid' => $appid, 'appkey' => $appkey);
	}

	private function _get_url_params($base, $appid, $appkey, $openid) {
		$ts  = time();
		$sig = md5($appkey.$ts);

		return "/?timestamp=$ts&appid=$appid&sig=$sig&openid=$openid&encode=1";
	}

	// qq
	private function _auth_verify_login() {
		$openid  = $this->input->get_post('openid');
		$openkey = $this->input->get_post('openkey');
		//$userip  = $this->input->get_post('userip');
		$userip  = $this->input->ip_address();

		require_once dirname(dirname(__FILE__)) . '/include/class/SnsNetwork.php';

		$base_url = $this->config->item('mpay_base_url', 'backdoor').'/auth/verify_login';
		//$base_url = 'http://msdktest.qq.com/auth/verify_login';
		$app      = $this->_get_app('qq');

		$url = $base_url.$this->_get_url_params($base_url, $app['appid'], $app['appkey'], $openid);

		$param = array(
			'appid'   => $app['appid'],
			'openid'  => $openid,
			'openkey' => $openkey,
			'userip'  => $userip,
			);

        //var_dump($url);
        //var_dump($param);
		$result = SnsNetwork::makeRequest($url, json_encode($param), '');
		//var_dump($result);
		$this->output->set_output(json_encode($result));
	}

	private function _relation_qqprofile() {

	}

	// 微信
	private function _auth_check_token() {
		$openid      = $this->input->get_post('openid');
		$accessToken = $this->input->get_post('accessToken');

		require_once dirname(dirname(__FILE__)) . '/include/class/SnsNetwork.php';

		$base_url = $this->config->item('mpay_base_url', 'backdoor').'/auth/check_token';
		//$base_url = 'http://msdktest.qq.com/auth/verify_login';
		$app      = $this->_get_app('weixin');

		$url = $base_url.$this->_get_url_params($base_url, $app['appid'], $app['appkey'], $openid);

		$param = array(
			'openid'      => $openid,
			'accessToken' => $accessToken,
			);

		$result = SnsNetwork::makeRequest($url, json_encode($param), '');
		//var_dump($result);
		$this->output->set_output(json_encode($result));
	}

	private function _relation_wxuserinfo() {

	}

	// 泡椒平台
	private function _paojiao_rsp($msg, $code, $data) {
		$rsp = array(
			'msg' => $msg,
			'code' => $code,
			'data' => $data
			);

		$this->output->set_content_type('application/json;charset=utf-8');
		$this->output->set_output(json_encode($rsp));
	}

	public function paojiao() {
		$appid = '1362';
		$appkey = 'P2GmeFW0szKCLP40CK2tOMRKQ9tHGd8J';
		$token = $this->input->get_post('token');
		if (!$token) {
			$this->_paojiao_rsp(-1, 'token null', $token);
			return ;
		}

		$path = "http://ng.sdk.paojiao.cn/api/user/token.do?token=$token&appid=$appid";
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $path);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		$ret = curl_exec($ch);
		curl_close($ch);
		if (!$ret) {
			$this->_paojiao_rsp(-1, 'server error', $token);
			return ;
		}

		$this->output->set_content_type('application/json;charset=utf-8');
		$this->output->set_output($ret);
	}

	public function jiayou() {
		$username  = $this->input->get_post('username');
		$logintime = $this->input->get_post('logintime');
		$sign      = $this->input->get_post('sign');
		$appkey    = '72de577e7acae60e6385400cb154bc67';
		$check = $sign == md5("username=$username&appkey=$appkey&logintime=$logintime");

        $msg = '';
        $ret = -1;
        if ($check) {
            $msg = '成功';
            $ret = 1;
        } else {
            $msg = '验证失败';
        }

		$ret = array('msg' => $msg, 'code' => $ret);

		$this->output->set_content_type('application/json;charset=utf-8');
		$this->output->set_output(json_encode($ret));
	}
}
