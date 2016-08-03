<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class loginGame_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["loginGame_req"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["loginGame_req"]["1"] = "user";
    self::$fields["loginGame_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["loginGame_req"]["2"] = "userType";
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
}
class loginGame_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["loginGame_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["loginGame_rsp"]["1000"] = "tail";
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