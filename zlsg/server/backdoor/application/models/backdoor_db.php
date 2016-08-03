<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Backdoor_db extends CI_Model {

    public function __construct()
    {
        parent::__construct();
        $this->db->query("set character set 'latin1'");
    }

    public function id_to_name($id)
    {
        $this->db->select('role_name');
        $query = $this->db->get_where('tb_player_role', "`role_id` = '{$id}'");
        foreach ($query->result() as $row)
        {
            return $row->role_name;
        }

        return 'null';
    }

    public function name_to_id($name)
    {
        $this->db->select('role_id');
        $query = $this->db->get_where('tb_player_role', "role_name = '{$name}'");
        foreach ($query->result() as $row)
        {
            return $row->role_id;
        }

        return FALSE;
    }

    public function id_to_account($id)
    {
        $this->db->select('player_id');
        $query = $this->db->get_where('tb_player_role', "`role_id` = '{$id}'");
        foreach ($query->result() as $row)
        {
            $row->player_id;
            $this->db->select('openid');
            $query = $this->db->get_where('tb_player', "id = {$row->player_id}");
            foreach ($query->result() as $row)
            {
                return $row->openid;
            }
        }

        return FALSE;
    }

    public function account_to_id($account)
    {
        $roles = array();
        $this->db->select('id');
        $query = $this->db->get_where('tb_player', "openid = '{$account}'");
        foreach ($query->result() as $row)
        {
            $this->db->select('role_id');
            $query_role = $this->db->get_where('tb_player_role', "player_id = {$row->id}");
            foreach ($query_role->result() as $row_role)
            {
                array_push($roles, $row_role->role_id);
            }
        }

        return $roles;
    }

    public function account_to_player_id($account)
    {
        $roles = array();
        $this->db->select('id');
        $query = $this->db->get_where('tb_player', "openid = '{$account}'");
        foreach ($query->result() as $row)
        {
            return $row->id;
        }

        return FALSE;
    }

    public function get_role_status(&$ids, $type, $page, $page_size)
    {
        date_default_timezone_set('Asia/Chongqing');
        $ret = array();
        foreach ($ids as $id)
        {
            $account = $this->id_to_account($id);
            $this->db->select('role_name, status, is_suspended_account');
            $query = $this->db->get_where('tb_player_role', "role_id = {$id}");
            foreach ($query->result() as $row)
            {
                if ($type != 0 && $type != $row->status) {
                    continue;
                }

                $role['account']              = $account;
                $role['role_id']              = $id;
                $role['role_name']            = $row->role_name;
                $role['status']               = $row->status;
                $role['end_time']             = date('Y-m-d H:i:s', $row->is_suspended_account);
                array_push($ret, $role);
            }
        }

        if ($page and $page_size)
        {
            /*if ($page > 0)
            {
                $page -= 1;
            }
            else
            {
                $page = 0;
            }*/

            $this->db->select('role_id, role_name, status, is_suspended_account');
            $query = $this->db->get('tb_player_role');//, $page_size, $page * $page_size);
            foreach ($query->result() as $row)
            {
                if ($type != 0 && $type != $row->status || $row->status == 0) {
                    continue;
                }

                $account                      = $this->id_to_account($row->role_id);
                $role['account']              = $account;
                $role['role_id']              = $row->role_id;
                $role['role_name']            = $row->role_name;
                $role['status']               = $row->status;
                $role['end_time']             = date('Y-m-d H:i:s', $row->is_suspended_account);
                array_push($ret, $role);
            }
        }

        return $ret;
    }

    private function _fill_roles(&$account, &$id, &$name, &$roles)
    {
        $role = array();
        $role['account']   = $account;
        $role['role_id']   = $id;
        $role['role_name'] = $name;
        array_push($roles, $role);
    }

    public function to_roles(&$user, &$user_type, &$roles)
    {
        if ($user === FALSE) {
            return 0;
        }

        $users = explode(',', $user);
        switch ($user_type)
        {
        case 0:
            foreach($users as $user) {
                $this->_fill_roles($this->id_to_account($user), $user, $this->id_to_name($user), $roles);
            }
            break;
        case 1:
            foreach($users as $user) {
                $ids = $this->account_to_id($user);
                foreach ($ids as $id) {
                    $this->_fill_roles($user, $id, $this->id_to_name($id), $roles);
                }
            }
            break;
        case 2:
            foreach($users as $user) {
                $id = $this->name_to_id($user);
                if ($id) {
                    $this->_fill_roles($this->id_to_account($id), $id, $user, $roles);
                }
            }
            break;
        }

        return count($roles);
    }

    public function user_source(&$source) 
    {
        $this->db->select('openid, platform, channel_id');
        $query = $this->db->get('tb_player');
        $source = $query->result();
        /*foreach ($query->result() as $row) {
           $source  
        }*/
    }
}

/* End of file backdoor.php */
/* Location: ./application/models/backdoor.php */
