<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Backdoor_lib {

    public function __construct()
    {
    }

    public function check_sign(&$obj) {
        $timestamp = $obj->input->get_post('timestamp');
        $sign      = $obj->input->get_post('sign');
        $domain    = $obj->input->get_post('domain');
        if (!$timestamp || !$sign) {
            return '缺失timestamp或sign';
        }

        $cur_time = time();
        if ($timestamp + 60 < $cur_time || $timestamp - 60 > $cur_time) {
//            return '超时timestamp与服务器当前时间相差超过60秒';
        }

        if ($sign == md5($timestamp . $obj->config->item('key', 'backdoor'))) {
            return 0;
        } else {
            echo md5($timestamp . $obj->config->item('key', 'backdoor'));
            return '验证sign失败';
        }
    }

    public function parse($protofile) {
        $pb = new PBParser();
        $pb->parse(dirname(dirname(__FILE__)) . '/proto/proto/' . $protofile);
    }
    
    public function pack($name, $data) {
        $name      = $name."0";
        $name_len  = strlen($name);
        $body      = pack("N", $name_len).$name.$data;
        $pack_len  = strlen($body);
        $send_data = pack("N", $pack_len).$body;
        return $send_data;
    }

    public function unpack($data) {
        $arrdata = unpack("N2len/C*body", $data);
        $name = "";
        $bin  = "";
        for ($i = 1; $i <= $arrdata["len1"] - 4; ++$i) {
            if ($i <= $arrdata["len2"]) {
                $name .= pack("c",$arrdata["body".$i]);
            } else {
                $bin .= pack("c", $arrdata["body".$i]);
            }
        }

        return $bin;
    }

    public function write_and_send($sendStr) {
        if (($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === FALSE) {
            echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
            return false;
        }

        $CI =& get_instance();
        $CI->config->load('backdoor', TRUE);
        $addr     = $CI->config->item('addr',     'backdoor');
        $port     = $CI->config->item('port',     'backdoor');
        $recv_len = $CI->config->item('recv_len', 'backdoor');

        if (socket_connect($socket, $addr, $port)) {
            $length = strlen($sendStr);
            while (true) {
                $sent = socket_write($socket, $sendStr, $length);
                if ($sent === false) {
                    echo "socket_write() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
                    return false;
                }

                if ($sent < $length) {
                    $sendStr = substr($sendStr, $sent);
                    $length -= $sent;
                } else {
                    break;
                }
            }

            $receiveStr = socket_read($socket, $recv_len);
        } else {
            return false;
        }

        socket_close($socket);
        return $receiveStr;
    }

    public function send($addr, $port, $sendStr) {
        if (($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === FALSE) {
            echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
            return false;
        }

        $recv_len = 10240;

        if (@socket_connect($socket, $addr, $port)) {
            $length = strlen($sendStr);
            while (true) {
                $sent = socket_write($socket, $sendStr, $length);
                if ($sent === false) {
                    echo "socket_write() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
                    return false;
                }

                if ($sent < $length) {
                    $sendStr = substr($sendStr, $sent);
                    $length -= $sent;
                } else {
                    break;
                }
            }

            $receiveStr = socket_read($socket, $recv_len);
        } else {
            return false;
        }

        socket_close($socket);
        return $receiveStr;
    }

    public function load_session() {
        $CI =& get_instance();
        $CI->load->library('session');
        $CI->load->helper('url');
    }

    public function logged_in() {
        $CI =& get_instance();
        return $CI->session->userdata('logged_in');
    }
}

/* End of file Backdoor.php */
/* Location: ./application/libraries/backdoor.php */
