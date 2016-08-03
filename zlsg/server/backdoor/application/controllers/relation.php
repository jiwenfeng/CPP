<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class relation extends CI_Controller {
    public function __construct() {
        parent::__construct();

        $this->load->library('curl'); 
        $this->load->library('snsSigCheck');

        $this->config->load('backdoor', TRUE);
    }

    public function index() {
    }

    private function _params($appid, $appkey) {
        $params = array();
        $params['appid'      ] = $appid;
        $params['timestamp'  ] = time();
        $params['sig'        ] = md5($appkey .$params['timestamp']);
        $params['encode'     ] = 1;
        $params['conn'       ] = 1;
        $params['msdkExtInfo'] = 1;
        $params['openid'     ] = $this->input->get_post('openid');

        return $params;
    }

    private function _parmas_url($params) {
        $url = "?";
        foreach ($params as $k => $v) {
            $url .= "$k=$v&";
        }

        return $url;
    }

    private function _profile($appid, $appkey, $path) {
        $base_url = $this->config->item('mpay_base_url', 'backdoor');
        $path_url = $path;//$this->config->item('relation_wxuserinfo', 'backdoor');

        $params = $this->_params($appid, $appkey);
        $params_url = $this->_parmas_url($params);
        $path = $base_url . $path_url . $params_url;

        $post_body = array();
        $post_body['openid'     ] = $this->input->get_post('openid');
        $post_body['accessToken'] = $this->input->get_post('accessToken');
        $post_body['appid'      ] = $appid;

        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $path);
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($post_body));
        $ret = curl_exec($ch);
        curl_close($ch);

        $this->output->set_content_type('application/json;charset=utf-8');
        $this->output->set_output($ret);
    }

    public function qqprofile() {
        $this->_profile($this->config->item('appid', 'backdoor'), 
        $this->config->item('appkey', 'backdoor'),
        $this->config->item('relation_qqprofile', 'backdoor'));
    }

    public function wxuserinfo() {
        $this->_profile($this->config->item('wxappid', 'backdoor'), 
        $this->config->item('wxappkey', 'backdoor'),
        $this->config->item('relation_wxuserinfo', 'backdoor'));
    }

}

/* End of file mpay.php */
/* Location: ./application/controllers/mpay.php */
