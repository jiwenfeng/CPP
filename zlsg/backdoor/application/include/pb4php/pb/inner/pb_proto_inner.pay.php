<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class pay_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["pay_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["pay_req"]["1"] = "amt";
    self::$fields["pay_req"]["2"] = "inner\inner_user";
    $this->values["2"] = "";
    self::$fieldNames["pay_req"]["2"] = "user";
    self::$fields["pay_req"]["3"] = "\\PBString";
    $this->values["3"] = "";
    self::$fieldNames["pay_req"]["3"] = "pay_id";
    self::$fields["pay_req"]["4"] = "\\PBString";
    $this->values["4"] = "";
    self::$fieldNames["pay_req"]["4"] = "currency";
    self::$fields["pay_req"]["5"] = "\\PBString";
    $this->values["5"] = "";
    self::$fieldNames["pay_req"]["5"] = "inpour_type";
    self::$fields["pay_req"]["6"] = "\\PBInt";
    $this->values["6"] = "";
    self::$fieldNames["pay_req"]["6"] = "addcoin";
    self::$fields["pay_req"]["7"] = "\\PBInt";
    $this->values["7"] = "";
    self::$fieldNames["pay_req"]["7"] = "ex_type";
    self::$fields["pay_req"]["8"] = "\\PB64Bit";
    $this->values["8"] = "";
    self::$fieldNames["pay_req"]["8"] = "depay";
    self::$fields["pay_req"]["9"] = "\\PB64Bit";
    $this->values["9"] = "";
    self::$fieldNames["pay_req"]["9"] = "g_depay";
    self::$fields["pay_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["pay_req"]["1000"] = "head";
  }
  function amt()
  {
    return $this->_get_value("1");
  }
  function set_amt($value)
  {
    return $this->_set_value("1", $value);
  }
  function user()
  {
    return $this->_get_value("2");
  }
  function set_user($value)
  {
    return $this->_set_value("2", $value);
  }
  function pay_id()
  {
    return $this->_get_value("3");
  }
  function set_pay_id($value)
  {
    return $this->_set_value("3", $value);
  }
  function currency()
  {
    return $this->_get_value("4");
  }
  function set_currency($value)
  {
    return $this->_set_value("4", $value);
  }
  function inpour_type()
  {
    return $this->_get_value("5");
  }
  function set_inpour_type($value)
  {
    return $this->_set_value("5", $value);
  }
  function addcoin()
  {
    return $this->_get_value("6");
  }
  function set_addcoin($value)
  {
    return $this->_set_value("6", $value);
  }
  function ex_type()
  {
    return $this->_get_value("7");
  }
  function set_ex_type($value)
  {
    return $this->_set_value("7", $value);
  }
  function depay()
  {
    return $this->_get_value("8");
  }
  function set_depay($value)
  {
    return $this->_set_value("8", $value);
  }
  function g_depay()
  {
    return $this->_get_value("9");
  }
  function set_g_depay($value)
  {
    return $this->_set_value("9", $value);
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
class pay_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["pay_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["pay_rsp"]["1"] = "amt";
    self::$fields["pay_rsp"]["2"] = "inner\inner_user";
    $this->values["2"] = "";
    self::$fieldNames["pay_rsp"]["2"] = "user";
    self::$fields["pay_rsp"]["3"] = "\\PBString";
    $this->values["3"] = "";
    self::$fieldNames["pay_rsp"]["3"] = "pay_id";
    self::$fields["pay_rsp"]["4"] = "\\PBString";
    $this->values["4"] = "";
    self::$fieldNames["pay_rsp"]["4"] = "currency";
    self::$fields["pay_rsp"]["5"] = "\\PBString";
    $this->values["5"] = "";
    self::$fieldNames["pay_rsp"]["5"] = "inpour_type";
    self::$fields["pay_rsp"]["6"] = "\\PBInt";
    $this->values["6"] = "";
    self::$fieldNames["pay_rsp"]["6"] = "addcoin";
    self::$fields["pay_rsp"]["7"] = "\\PBInt";
    $this->values["7"] = "";
    self::$fieldNames["pay_rsp"]["7"] = "ex_type";
    self::$fields["pay_rsp"]["8"] = "\\PB64Bit";
    $this->values["8"] = "";
    self::$fieldNames["pay_rsp"]["8"] = "depay";
    self::$fields["pay_rsp"]["9"] = "\\PB64Bit";
    $this->values["9"] = "";
    self::$fieldNames["pay_rsp"]["9"] = "g_depay";
    self::$fields["pay_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["pay_rsp"]["1000"] = "tail";
  }
  function amt()
  {
    return $this->_get_value("1");
  }
  function set_amt($value)
  {
    return $this->_set_value("1", $value);
  }
  function user()
  {
    return $this->_get_value("2");
  }
  function set_user($value)
  {
    return $this->_set_value("2", $value);
  }
  function pay_id()
  {
    return $this->_get_value("3");
  }
  function set_pay_id($value)
  {
    return $this->_set_value("3", $value);
  }
  function currency()
  {
    return $this->_get_value("4");
  }
  function set_currency($value)
  {
    return $this->_set_value("4", $value);
  }
  function inpour_type()
  {
    return $this->_get_value("5");
  }
  function set_inpour_type($value)
  {
    return $this->_set_value("5", $value);
  }
  function addcoin()
  {
    return $this->_get_value("6");
  }
  function set_addcoin($value)
  {
    return $this->_set_value("6", $value);
  }
  function ex_type()
  {
    return $this->_get_value("7");
  }
  function set_ex_type($value)
  {
    return $this->_set_value("7", $value);
  }
  function depay()
  {
    return $this->_get_value("8");
  }
  function set_depay($value)
  {
    return $this->_set_value("8", $value);
  }
  function g_depay()
  {
    return $this->_get_value("9");
  }
  function set_g_depay($value)
  {
    return $this->_set_value("9", $value);
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