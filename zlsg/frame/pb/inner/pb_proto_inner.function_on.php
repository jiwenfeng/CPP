<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class activity_control_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["activity_control_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["activity_control_req"]["1"] = "open_type";
    self::$fields["activity_control_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["activity_control_req"]["2"] = "activity_id";
    self::$fields["activity_control_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["activity_control_req"]["1000"] = "head";
  }
  function open_type()
  {
    return $this->_get_value("1");
  }
  function set_open_type($value)
  {
    return $this->_set_value("1", $value);
  }
  function activity_id()
  {
    return $this->_get_value("2");
  }
  function set_activity_id($value)
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
class activity_control_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["activity_control_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["activity_control_rsp"]["1"] = "result_type";
    self::$fields["activity_control_rsp"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["activity_control_rsp"]["2"] = "open_type";
  }
  function result_type()
  {
    return $this->_get_value("1");
  }
  function set_result_type($value)
  {
    return $this->_set_value("1", $value);
  }
  function open_type()
  {
    return $this->_get_value("2");
  }
  function set_open_type($value)
  {
    return $this->_set_value("2", $value);
  }
}
?>