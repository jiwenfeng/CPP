<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class start_time_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["start_time_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["start_time_req"]["1"] = "unixtime";
    self::$fields["start_time_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["start_time_req"]["1000"] = "head";
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
class start_time_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["start_time_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["start_time_rsp"]["1"] = "unixtime";
    self::$fields["start_time_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["start_time_rsp"]["1000"] = "tail";
  }
  function unixtime()
  {
    return $this->_get_value("1");
  }
  function set_unixtime($value)
  {
    return $this->_set_value("1", $value);
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
class open_gm_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["open_gm_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["open_gm_req"]["1"] = "unixtime";
    self::$fields["open_gm_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["open_gm_req"]["1000"] = "head";
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
class open_gm_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["open_gm_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["open_gm_rsp"]["1"] = "unixtime";
    self::$fields["open_gm_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["open_gm_rsp"]["1000"] = "tail";
  }
  function unixtime()
  {
    return $this->_get_value("1");
  }
  function set_unixtime($value)
  {
    return $this->_set_value("1", $value);
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