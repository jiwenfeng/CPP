<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class getLogType_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getLogType_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["getLogType_rsp"]["1000"] = "tail";
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