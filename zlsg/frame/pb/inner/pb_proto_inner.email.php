<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class sendEmail_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["sendEmail_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["sendEmail_req"]["1"] = "type";
    self::$fields["sendEmail_req"]["2"] = "inner_user";
    $this->values["2"] = array();
    self::$fieldNames["sendEmail_req"]["2"] = "user";
    self::$fields["sendEmail_req"]["3"] = "\\PBString";
    $this->values["3"] = "";
    self::$fieldNames["sendEmail_req"]["3"] = "title";
    self::$fields["sendEmail_req"]["4"] = "\\PBString";
    $this->values["4"] = "";
    self::$fieldNames["sendEmail_req"]["4"] = "content";
    self::$fields["sendEmail_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["sendEmail_req"]["1000"] = "head";
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
  function title()
  {
    return $this->_get_value("3");
  }
  function set_title($value)
  {
    return $this->_set_value("3", $value);
  }
  function content()
  {
    return $this->_get_value("4");
  }
  function set_content($value)
  {
    return $this->_set_value("4", $value);
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
class sendEmail_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["sendEmail_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["sendEmail_rsp"]["1000"] = "tail";
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