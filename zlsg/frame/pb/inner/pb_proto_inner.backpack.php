<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class getBackpackGoodsList_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getBackpackGoodsList_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["getBackpackGoodsList_req"]["1"] = "type";
    self::$fields["getBackpackGoodsList_req"]["2"] = "inner_user";
    $this->values["2"] = array();
    self::$fieldNames["getBackpackGoodsList_req"]["2"] = "user";
    self::$fields["getBackpackGoodsList_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["getBackpackGoodsList_req"]["1000"] = "head";
  }
  function type()
  {
    return $this->_get_value("1");
  }
  function set_type($value)
  {
    return $this->_set_value("1", $value);
  }
  function user($offset)
  {
    return $this->_get_arr_value("2", $offset);
  }
  function add_user()
  {
    return $this->_add_arr_value("2");
  }
  function set_user($index, $value)
  {
    $this->_set_arr_value("2", $index, $value);
  }
  function set_all_users($values)
  {
    return $this->_set_arr_values("2", $values);
  }
  function remove_last_user()
  {
    $this->_remove_last_arr_value("2");
  }
  function users_size()
  {
    return $this->_get_arr_size("2");
  }
  function get_users()
  {
    return $this->_get_value("2");
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
class backpackGoods extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["backpackGoods"]["1"] = "inner_goods";
    $this->values["1"] = array();
    self::$fieldNames["backpackGoods"]["1"] = "goods";
    self::$fields["backpackGoods"]["2"] = "inner_user";
    $this->values["2"] = "";
    self::$fieldNames["backpackGoods"]["2"] = "user";
  }
  function goods($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_goods()
  {
    return $this->_add_arr_value("1");
  }
  function set_goods($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_goodss($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_goods()
  {
    $this->_remove_last_arr_value("1");
  }
  function goodss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_goodss()
  {
    return $this->_get_value("1");
  }
  function user()
  {
    return $this->_get_value("2");
  }
  function set_user($value)
  {
    return $this->_set_value("2", $value);
  }
}
class getBackpackGoodsList_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getBackpackGoodsList_rsp"]["1"] = "\\inner\\backpackGoods";
    $this->values["1"] = array();
    self::$fieldNames["getBackpackGoodsList_rsp"]["1"] = "data";
    self::$fields["getBackpackGoodsList_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["getBackpackGoodsList_rsp"]["1000"] = "tail";
  }
  function data($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_data()
  {
    return $this->_add_arr_value("1");
  }
  function set_data($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_datas($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_data()
  {
    $this->_remove_last_arr_value("1");
  }
  function datas_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_datas()
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
class updateBackpackGoods_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["updateBackpackGoods_req"]["1"] = "\\inner\\backpackGoods";
    $this->values["1"] = array();
    self::$fieldNames["updateBackpackGoods_req"]["1"] = "update";
    self::$fields["updateBackpackGoods_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["updateBackpackGoods_req"]["1000"] = "head";
  }
  function update($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_update()
  {
    return $this->_add_arr_value("1");
  }
  function set_update($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_updates($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_update()
  {
    $this->_remove_last_arr_value("1");
  }
  function updates_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_updates()
  {
    return $this->_get_value("1");
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
class updateBackpackGoods_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["updateBackpackGoods_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["updateBackpackGoods_rsp"]["1000"] = "tail";
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
?>