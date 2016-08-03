<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class rechargeQuery_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["rechargeQuery_req"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["rechargeQuery_req"]["1"] = "user";
    self::$fields["rechargeQuery_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["rechargeQuery_req"]["2"] = "userType";
    self::$fields["rechargeQuery_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["rechargeQuery_req"]["3"] = "state";
    self::$fields["rechargeQuery_req"]["4"] = "\\PBString";
    $this->values["4"] = "";
    self::$fieldNames["rechargeQuery_req"]["4"] = "orderID";
    self::$fields["rechargeQuery_req"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["rechargeQuery_req"]["5"] = "beginTime";
    self::$fields["rechargeQuery_req"]["6"] = "\\PBInt";
    $this->values["6"] = "";
    self::$fieldNames["rechargeQuery_req"]["6"] = "endTime";
  }
  function user()
  {
    return $this->_get_value("1");
  }
  function set_user($value)
  {
    return $this->_set_value("1", $value);
  }
  function userType()
  {
    return $this->_get_value("2");
  }
  function set_userType($value)
  {
    return $this->_set_value("2", $value);
  }
  function state()
  {
    return $this->_get_value("3");
  }
  function set_state($value)
  {
    return $this->_set_value("3", $value);
  }
  function orderID()
  {
    return $this->_get_value("4");
  }
  function set_orderID($value)
  {
    return $this->_set_value("4", $value);
  }
  function beginTime()
  {
    return $this->_get_value("5");
  }
  function set_beginTime($value)
  {
    return $this->_set_value("5", $value);
  }
  function endTime()
  {
    return $this->_get_value("6");
  }
  function set_endTime($value)
  {
    return $this->_set_value("6", $value);
  }
}
class rechargeQuery_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["rechargeQuery_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["rechargeQuery_rsp"]["1000"] = "tail";
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