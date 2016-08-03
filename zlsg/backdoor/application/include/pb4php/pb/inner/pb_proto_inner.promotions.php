<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class promotions extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["promotions"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["promotions"]["1"] = "id";
    self::$fields["promotions"]["2"] = "\\PBString";
    $this->values["2"] = "";
    self::$fieldNames["promotions"]["2"] = "name";
    self::$fields["promotions"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["promotions"]["3"] = "type";
    self::$fields["promotions"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["promotions"]["4"] = "start_time";
    self::$fields["promotions"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["promotions"]["5"] = "duration_time";
    self::$fields["promotions"]["6"] = "\\PBInt";
    $this->values["6"] = "";
    self::$fieldNames["promotions"]["6"] = "end_time";
    self::$fields["promotions"]["7"] = "\\PBInt";
    $this->values["7"] = "";
    self::$fieldNames["promotions"]["7"] = "loop_time";
    self::$fields["promotions"]["8"] = "\\PBInt";
    $this->values["8"] = "";
    self::$fieldNames["promotions"]["8"] = "state";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function name()
  {
    return $this->_get_value("2");
  }
  function set_name($value)
  {
    return $this->_set_value("2", $value);
  }
  function type()
  {
    return $this->_get_value("3");
  }
  function set_type($value)
  {
    return $this->_set_value("3", $value);
  }
  function start_time()
  {
    return $this->_get_value("4");
  }
  function set_start_time($value)
  {
    return $this->_set_value("4", $value);
  }
  function duration_time()
  {
    return $this->_get_value("5");
  }
  function set_duration_time($value)
  {
    return $this->_set_value("5", $value);
  }
  function end_time()
  {
    return $this->_get_value("6");
  }
  function set_end_time($value)
  {
    return $this->_set_value("6", $value);
  }
  function loop_time()
  {
    return $this->_get_value("7");
  }
  function set_loop_time($value)
  {
    return $this->_set_value("7", $value);
  }
  function state()
  {
    return $this->_get_value("8");
  }
  function set_state($value)
  {
    return $this->_set_value("8", $value);
  }
}
class get_promotions_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["get_promotions_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["get_promotions_req"]["1"] = "unixtime";
    self::$fields["get_promotions_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["get_promotions_req"]["1000"] = "head";
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
class get_promotions_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["get_promotions_rsp"]["1"] = "\\inner\\promotions";
    $this->values["1"] = array();
    self::$fieldNames["get_promotions_rsp"]["1"] = "ps";
    self::$fields["get_promotions_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["get_promotions_rsp"]["1000"] = "tail";
  }
  function ps($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_ps()
  {
    return $this->_add_arr_value("1");
  }
  function set_ps($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_pss($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_ps()
  {
    $this->_remove_last_arr_value("1");
  }
  function pss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_pss()
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
class set_promotions_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["set_promotions_req"]["1"] = "\\inner\\promotions";
    $this->values["1"] = array();
    self::$fieldNames["set_promotions_req"]["1"] = "ps";
    self::$fields["set_promotions_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["set_promotions_req"]["1000"] = "head";
  }
  function ps($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_ps()
  {
    return $this->_add_arr_value("1");
  }
  function set_ps($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_pss($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_ps()
  {
    $this->_remove_last_arr_value("1");
  }
  function pss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_pss()
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
class set_promotions_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["set_promotions_rsp"]["1"] = "\\inner\\promotions";
    $this->values["1"] = array();
    self::$fieldNames["set_promotions_rsp"]["1"] = "ps";
    self::$fields["set_promotions_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["set_promotions_rsp"]["1000"] = "tail";
  }
  function ps($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_ps()
  {
    return $this->_add_arr_value("1");
  }
  function set_ps($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_pss($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_ps()
  {
    $this->_remove_last_arr_value("1");
  }
  function pss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_pss()
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
class del_promotions_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["del_promotions_req"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["del_promotions_req"]["1"] = "ids";
    self::$fields["del_promotions_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["del_promotions_req"]["1000"] = "head";
  }
  function ids($offset)
  {
    $v = $this->_get_arr_value("1", $offset);
    return $v->get_value();
  }
  function append_ids($value)
  {
    $v = $this->_add_arr_value("1");
    $v->set_value($value);
  }
  function set_ids($index, $value)
  {
    $v = new self::$fields["del_promotions_req"]["1"]();
    $v->set_value($value);
    $this->_set_arr_value("1", $index, $v);
  }
  function remove_last_ids()
  {
    $this->_remove_last_arr_value("1");
  }
  function idss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_idss()
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
class del_promotions_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["del_promotions_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["del_promotions_rsp"]["1"] = "ids";
    self::$fields["del_promotions_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["del_promotions_rsp"]["1000"] = "tail";
  }
  function ids($offset)
  {
    $v = $this->_get_arr_value("1", $offset);
    return $v->get_value();
  }
  function append_ids($value)
  {
    $v = $this->_add_arr_value("1");
    $v->set_value($value);
  }
  function set_ids($index, $value)
  {
    $v = new self::$fields["del_promotions_rsp"]["1"]();
    $v->set_value($value);
    $this->_set_arr_value("1", $index, $v);
  }
  function remove_last_ids()
  {
    $this->_remove_last_arr_value("1");
  }
  function idss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_idss()
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
?>