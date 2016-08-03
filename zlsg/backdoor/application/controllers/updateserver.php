<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Updateserver extends CI_Controller {

    /**
     * Index Page for this controller.
     *
     * Maps to the following URL
     * 		http://example.com/index.php/welcome
     *	- or -  
     * 		http://example.com/index.php/welcome/index
     *	- or -
     * Since this controller is set as the default controller in 
     * config/routes.php, it's displayed at http://example.com/
     *
     * So any other public methods not prefixed with an underscore will
     * map to /index.php/welcome/<method_name>
     * @see http://codeigniter.com/user_guide/general/urls.html
     */
    function __construct() {
        parent::__construct();
    }

    public function index()
    {
        //$this->load->view('welcome_message');
    }

    public function updateapk()
    {
        $version = $this->input->get('version');
        $pf      = $this->input->get('pf');
        if (!$pf) {
            $pf = 'qq';
        }
        /*
         * version:客户端当前APK版本号
         * imei:客户端手机imei号
         * model:客户端手机型号
         * sdk:客户端系统版本号
         * pf:客户端系统
         * tc:客户端纹理掩码(服务端暂时不用处理)
         * unionid:渠道号
         * img:客户端图片格式
         */

        $this->output->set_content_type('text/xml');
        $xml  = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

        if (!$version) {
            $xml .= "<error> <code>1</code> <info>param error not found version</info>  </error>";
            $this->output->set_output($xml);
            return ;
        }

        $this->load->database();
        $query = $this->db->query("select * from t_files where pf = \"$pf\" and sys=\"android\" and type=\"apk\"");
        $result['version'] = $version;
        foreach ($query->result() as $row) {
            //$result['id'] = $row->id;
            $result['type']    = $row->type;
            $result['url']     = $row->url;
            $result['version'] = $row->version;
        }

        $client_version = $this->db->get_where('t_client_version', array('type' => 'apk'))->result_array();
        $apk_version = -1;
        foreach ($client_version as $row) {
            $apk_version = $row['version'];
        }

        $needUpdate = $version < $result['version'] ? "true" : "false";
        if ($apk_version != -1) {
            $xml .= "<update clientVersion=\"" . $version . "\" newestVersion=\"" . $result['version'] . "\" needUpdate=\"" . $needUpdate . "\" apk=\"$apk_version\">";
        } else {
            $xml .= "<update clientVersion=\"" . $version . "\" newestVersion=\"" . $result['version'] . "\" needUpdate=\"" . $needUpdate . "\">";
        }
        if ($version < $result['version']) {
            $xml .= "<file type=\"" . $result['type'] . "\" url=\"" . $result['url'] . "\" ver=\"" . $result['version'] . "\" />";
        }
        $xml .= "</update>";
        $this->output->set_header('Content-length: ' . strlen($xml));
        $this->output->set_output($xml); 
    }

    private function _get_ret($pf, $sys, $type)
    {
        $sql = "select version, url from t_files where pf=\"$pf\" and sys=\"$sys\" and type=\"$type\"";
        $query = $this->db->query($sql);
        foreach ($query->result_array() as $row) {
            return $row;
        }
    }

    private function _get_rets($pf, $sys, $type, &$rets, $version)
    {
        $sql = "select `version`, `url` from `t_files` where `pf`='$pf' and `sys`='$sys' and `type`='$type' and `version` > '$version' order by `version`";
        $query = $this->db->query($sql);
        $rets = $query->result_array();
    }

    private function _get_android_result($pf, &$libresult, &$resresult, $resVersion)
    {
        $libresult = $this->_get_ret($pf, 'android', 'lib');
        $this->_get_rets($pf, 'android', 'res', $resresult, $resVersion);
        if (!isset($libresult['version'])) {
            $libresult = array('version'=>1);
        }

        /*if (!isset($resresult['version'])) {
            $resresult = array('version'=>1);
        }*/
    }

    private function _get_ios_result(&$libresult, &$resresult)
    {
        $libresult = $this->_get_ret($pf, 'ios', 'lib');
        $this->_get_rets($pf, 'ios', 'res', $resresult, $resVersion);
        if (!isset($libresult['version'])) {
            $libresult = array('version'=>1);
        }
    }

    private function _get_win32_result($pf, &$libresult, &$resresult, $resVersion)
    {
        $libresult = $this->_get_ret($pf, 'windows', 'lib');
        $this->_get_rets($pf, 'windows', 'res', $resresult, $resVersion);
        if (!isset($libresult['version'])) {
            $libresult = array('version'=>1);
        }
    }

    private function _get_result($pf, &$type, &$libresult, &$resresult, $resVersion)
    {
        $this->load->database();
        switch ((int)$type) {
        case 0: $this->_get_android_result($pf, $libresult, $resresult, $resVersion); break;
        case 1: $this->_get_ios_result($pf, $libresult, $resresult, $resVersion); break;
        case 2: $this->_get_win32_result($pf, $libresult, $resresult, $resVersion); break;
        default: $this->_get_android_result($pf, $libresult, $resresult, $resVersion); break;
        }
    }

    public function update()
    {
        $libversion = $this->input->get_post('libversion');
        $resversion = $this->input->get_post('resversion');
        $type       = $this->input->get_post('type');
        $pf         = $this->input->get_post('pf');
        if (!$pf || $pf == '') {
            $pf = 'qq';
        }

        $this->output->set_content_type('text/xml');
        $xml  = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

        if (!$libversion || !$resversion || !$pf) {
            $xml .= "<error> <code>1</code> <info>param error not found version</info>  </error>";
            $this->output->set_output($xml);
            return ;
        }

        $this->_get_result($pf, $type, $libresult, $resresult, $resversion);
        $libneedUpdate = $libversion < $libresult['version'] && $libversion > 0;
        $resneedUpdate = false;
        $maxResVersion = 1;
        foreach ($resresult as $row) {
            if ($maxResVersion < $row['version']) {
                $maxResVersion = $row['version'];
            }

            if ($resversion < $row['version']) {
                $resneedUpdate = true;
            }
        }

        if ($libversion < 0) {
            $libneedUpdate = FALSE;
        }

        if ($resversion < 0) {
            $resneedUpdate = FALSE;
        }

        $cdns = $this->db->get_where('t_cdn', array('pf' => $pf));
        $cdn_path = '';
        foreach ($cdns->result_array() as $row) {
            $cdn_path = $row['cdn'];
        }

        $needUpdate = $libneedUpdate || $resneedUpdate ? "true" : "false";
        $xml .= "<update newestVersion=\"" . $libresult['version'] . "," . $maxResVersion;
        $xml .= "\" clientVersion=\"$libversion,$resversion\" needUpdate=\"$needUpdate\"";
        if ($resversion == -1) {
            $xml .= '>';
        } else {
            $xml .= " cdn=\"$cdn_path\">";
        }

        if ($libneedUpdate) {
            $xml .= "<file type=\"lib\" url=\"" . $libresult['url'] . "\" ver=\"" . $libresult['version'] . "\" dmp=\"1\" />";
        }

        if ($resneedUpdate) {
            foreach ($resresult as $row) {
                $xml .= "<file type=\"res\" url=\"" . $row['url'] . "\" ver=\"" . $row['version'] . "\" dmp=\"1\" />";
            }
        }

        $xml .= "</update>";

        $this->output->set_header('Content-length: ' . strlen($xml));
        $this->output->set_output($xml);
    }
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
