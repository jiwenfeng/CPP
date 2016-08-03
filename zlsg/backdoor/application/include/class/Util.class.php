<?php

require_once dirname(dirname(__FILE__)) . '/pb4php/parser/pb_parser.php';

class Util {
    /*
     * 解析.proto文件为.php文件
     */

    public static function parse($protofile) {
        $test = new PBParser();
        $test->parse(dirname(dirname(__FILE__)) . '/proto/proto/' . $protofile);
    }

    /*
     * 验证
     */
    static private $key = '3y789fyf8hf34fu0suivjcxv89la98s2';
    static private $data_key = 'aBEsf998fuds908DA8fdkllsajdsafsF';

    public static function customer_check_sign($time, $sign) {
        return true;
        //return (md5(intval($time).self::$key) == $sign);
    }

    public static function data_center_check_sign($time, $key) {
        return true;
        //return md5('time=' . $time . "&key=" . self::$data_key) == $sign;
    }

    /*
     * 数据库
     */
    public static function open_id_to_role_id($db, $open_id) {
        $query = $db->query('SELECT * FROM tb_player WHERE openid = "' . $open_id . '"');
        $sql = "";
        foreach ($query->result() as $row) {
            $sql .= 'WHERE player_id = "' . $row->id . '"';
            break;
        }

        $role_id = 0;
        if ($sql != "") {
            $query = $db->query('SELECT * FROM tb_player_role ' . $sql);
            foreach ($query->result() as $row) {
                $role_id = $row->role_id;
                break;
            }
        }

        return $role_id;
    }

    public static function role_id_to_open_id($db, $role_id) {
        $query = $db->query('SELECT * FROM tb_player_role WHERE role_id = "' . $role_id . '"');
        $sql = "";
        foreach ($query->result() as $row) {
            $sql .= 'WHERE id = "' . $row->player_id . '"';
            break;
        }

        $open_id = "";
        if ($sql != "") {
            $query = $db->query('SELECT * FROM tb_player '. $sql);
            foreach ($query->result() as $row) {
                $open_id = $row->openid;
                break;
            }
        }

        return $open_id;
    }

    public static function role_name_to_role_id($db, $role_name) {
        $query = $db->query('SELECT * FROM tb_player_role WHERE role_name = "' . $role_name . '"');
        $role_id = 0;
        foreach ($query->result() as $row) {
            $role_id = $row->role_id;
            break;
        }

        return $role_id;
    }

    public static function role_id_to_role_name($db, $role_id) {
        $query = $db->query('SELECT * FROM tb_player_role WHERE role_id = "' . $role_id . '"');
        $role_name = 0;
        foreach ($query->result() as $row) {
            $role_name = $row->role_name;
            break;
        }

        return $role_name;
    }

    /*
     * Socket的读写
     */
    public static function Pack($name, $data) {
        $name      = $name."0";
        $name_len  = strlen($name);
        $body      = pack("N", $name_len).$name.$data;
        $pack_len  = strlen($body);
        $send_data = pack("N", $pack_len).$body;
        return $send_data;
    }

    public static function Unpack($data) {
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

    public static function writeAndSend($sendStr) {
        if (($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
            echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
            return false;
        }

        if (socket_connect($socket, "127.0.0.1", 1389)) {
            $length = strlen($sendStr);
            while (true) {
                $sent = socket_write($socket, $sendStr, $length);
                if ($sent === false) {
                    echo "socket_write() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
                    return false;
                }
                // Check if the entire message has been sented
                if ($sent < $length) {
                    // If not sent the entire message.
                    // Get the part of the message that has not yet been sented as message
                    $sendStr = substr($sendStr, $sent);
                    // Get the length of the not sented part
                    $length -= $sent;
                } else {
                    break;
                }
            }

            $receiveStr = socket_read($socket, 1024000);
        } else {
            return false;
        }

        socket_close($socket);
        return $receiveStr;
    }
}

?>
