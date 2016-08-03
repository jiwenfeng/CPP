<?php 
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/message/pb_message.php';
        require_once dirname(dirname(__FILE__)) . '/include/pb4php/pb/inner/pb_proto_inner.comm.php';

        $req = new inner\close_server();
        $req->set_unixtime(0);
        echo $req->toJson();
        //var_dump($req);
        //$this->output->set_output($req->);


/* End of file server.php */
/* Location: ./application/controllers/server.php */
