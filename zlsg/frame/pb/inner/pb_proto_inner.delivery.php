<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class delivery_ingot_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["delivery_ingot_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["delivery_ingot_req"]["1"] = "role_id";
    self::$fields["delivery_ingot_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["delivery_ingot_req"]["2"] = "ingot";
    self::$fields["delivery_ingot_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["delivery_ingot_req"]["1000"] = "head";
  }
  function role_id()
  {
    return $this->_get_value("1");
  }
  function set_role_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function ingot()
  {
    return $this->_get_value("2");
  }
  function set_ingot($value)
  {
    return $this->_set_value("2", $value);
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
class delivery_ingot_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["delivery_ingot_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["delivery_ingot_rsp"]["1"] = "role_id";
    self::$fields["delivery_ingot_rsp"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["delivery_ingot_rsp"]["2"] = "ingot";
    self::$fields["delivery_ingot_rsp"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["delivery_ingot_rsp"]["3"] = "ret";
    self::$fields["delivery_ingot_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["delivery_ingot_rsp"]["1000"] = "tail";
  }
  function role_id()
  {
    return $this->_get_value("1");
  }
  function set_role_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function ingot()
  {
    return $this->_get_value("2");
  }
  function set_ingot($value)
  {
    return $this->_set_value("2", $value);
  }
  function ret()
  {
    return $this->_get_value("3");
  }
  function set_ret($value)
  {
    return $this->_set_value("3", $value);
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