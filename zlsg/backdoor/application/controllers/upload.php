<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Upload extends CI_Controller {
    function index()
    {
        $this->load->view('uploader.php');
    }

    function up()
    {
        // 把需要的配置放入config数组
        $config['upload_path'] = '/www/uploads/';
        $config['file_name'] = '';
        $config['overwrite'] = TRUE;
        $config['allowed_types'] = '*';
        $config['max_size'] = '0';
        $this->load->library('upload', $config); //调用CI的upload类
        $this->upload->do_upload('upfile'); //使用do_upload('上传框的name')方法进行上传

        // 以下代码为拓展的，非必要
        if ($this->upload->do_upload('upfile')) { //上传成功
            $data = array('upload_data' => $this->upload->data()); //将文件信息存入数组
            var_dump($data); //打印文件信息
        } else { //上传失败
            $error = array('error' => $this->upload->display_errors());//将错误信息存入数组
            var_dump($error); //打印错误信息
        }
    }
}
?>
