<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class online_player_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["online_player_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["online_player_req"]["1"] = "unixtime";
    self::$fields["online_player_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["online_player_req"]["1000"] = "head";
  }
  function unixtime()
  {
    return $this->_get_value("1");
  }
  function set_unixtime($value)
  {
    return $this->_set_value("1", $value);
  }
  function head()
  {
    return $this->_get_value("1000");
  }
  function set_head($value)
  {
    return $this->_set_value("1000", $value);
  }
}
class online_player_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["online_player_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["online_player_rsp"]["1"] = "role_id";
    self::$fields["online_player_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["online_player_rsp"]["1000"] = "tail";
  }
  function role_id($offset)
  {
    $v = $this->_get_arr_value("1", $offset);
    return $v->get_value();
  }
  function append_role_id($value)
  {
    $v = $this->_add_arr_value("1");
    $v->set_value($value);
  }
  function set_role_id($index, $value)
  {
    $v = new self::$fields["online_player_rsp"]["1"]();
    $v->set_value($value);
    $this->_set_arr_value("1", $index, $v);
  }
  function remove_last_role_id()
  {
    $this->_remove_last_arr_value("1");
  }
  function role_ids_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_role_ids()
  {
    return $this->_get_value("1");
  }
  function tail()
  {
    return $this->_get_value("1000");
  }
  function set_tail($value)
  {
    return $this->_set_value("1000", $value);
  }
}
class kick_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["kick_req"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["kick_req"]["1"] = "role_id";
  }
  function role_id($offset)
  {
    $v = $this->_get_arr_value("1", $offset);
    return $v->get_value();
  }
  function append_role_id($value)
  {
    $v = $this->_add_arr_value("1");
    $v->set_value($value);
  }
  function set_role_id($index, $value)
  {
    $v = new self::$fields["kick_req"]["1"]();
    $v->set_value($value);
    $this->_set_arr_value("1", $index, $v);
  }
  function remove_last_role_id()
  {
    $this->_remove_last_arr_value("1");
  }
  function role_ids_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_role_ids()
  {
    return $this->_get_value("1");
  }
}
?>