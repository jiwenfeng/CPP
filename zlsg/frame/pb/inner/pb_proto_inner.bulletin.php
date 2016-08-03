<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class bulletin_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["bulletin_req"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["bulletin_req"]["1"] = "contents";
    self::$fields["bulletin_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["bulletin_req"]["1000"] = "head";
  }
  function contents()
  {
    return $this->_get_value("1");
  }
  function set_contents($value)
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
class bulletin_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["bulletin_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["bulletin_rsp"]["1000"] = "tail";
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
class revolving_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["revolving_req"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["revolving_req"]["1"] = "contents";
    self::$fields["revolving_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["revolving_req"]["2"] = "start_time";
    self::$fields["revolving_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["revolving_req"]["3"] = "end_time";
    self::$fields["revolving_req"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["revolving_req"]["4"] = "repeat_time";
    self::$fields["revolving_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["revolving_req"]["1000"] = "head";
  }
  function contents()
  {
    return $this->_get_value("1");
  }
  function set_contents($value)
  {
    return $this->_set_value("1", $value);
  }
  function start_time()
  {
    return $this->_get_value("2");
  }
  function set_start_time($value)
  {
    return $this->_set_value("2", $value);
  }
  function end_time()
  {
    return $this->_get_value("3");
  }
  function set_end_time($value)
  {
    return $this->_set_value("3", $value);
  }
  function repeat_time()
  {
    return $this->_get_value("4");
  }
  function set_repeat_time($value)
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
class revolving_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["revolving_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["revolving_rsp"]["1000"] = "tail";
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
class revolving extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["revolving"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["revolving"]["1"] = "msg_id";
    self::$fields["revolving"]["2"] = "\\PBString";
    $this->values["2"] = "";
    self::$fieldNames["revolving"]["2"] = "contents";
    self::$fields["revolving"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["revolving"]["3"] = "start_time";
    self::$fields["revolving"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["revolving"]["4"] = "end_time";
    self::$fields["revolving"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["revolving"]["5"] = "repeat_time";
    self::$fields["revolving"]["6"] = "\\PBInt";
    $this->values["6"] = "";
    self::$fieldNames["revolving"]["6"] = "is_over";
  }
  function msg_id()
  {
    return $this->_get_value("1");
  }
  function set_msg_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function contents()
  {
    return $this->_get_value("2");
  }
  function set_contents($value)
  {
    return $this->_set_value("2", $value);
  }
  function start_time()
  {
    return $this->_get_value("3");
  }
  function set_start_time($value)
  {
    return $this->_set_value("3", $value);
  }
  function end_time()
  {
    return $this->_get_value("4");
  }
  function set_end_time($value)
  {
    return $this->_set_value("4", $value);
  }
  function repeat_time()
  {
    return $this->_get_value("5");
  }
  function set_repeat_time($value)
  {
    return $this->_set_value("5", $value);
  }
  function is_over()
  {
    return $this->_get_value("6");
  }
  function set_is_over($value)
  {
    return $this->_set_value("6", $value);
  }
}
class get_revolving_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["get_revolving_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["get_revolving_req"]["1"] = "unixtime";
    self::$fields["get_revolving_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["get_revolving_req"]["1000"] = "head";
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
class get_revolving_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["get_revolving_rsp"]["1"] = "\\inner\\revolving";
    $this->values["1"] = array();
    self::$fieldNames["get_revolving_rsp"]["1"] = "rs";
    self::$fields["get_revolving_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["get_revolving_rsp"]["1000"] = "tail";
  }
  function rs($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_rs()
  {
    return $this->_add_arr_value("1");
  }
  function set_rs($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_rss($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_rs()
  {
    $this->_remove_last_arr_value("1");
  }
  function rss_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_rss()
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
class del_revolving_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["del_revolving_req"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["del_revolving_req"]["1"] = "ids";
    self::$fields["del_revolving_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["del_revolving_req"]["1000"] = "head";
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
    $v = new self::$fields["del_revolving_req"]["1"]();
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
class del_revolving_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["del_revolving_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["del_revolving_rsp"]["1"] = "ids";
    self::$fields["del_revolving_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["del_revolving_rsp"]["1000"] = "tail";
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
    $v = new self::$fields["del_revolving_rsp"]["1"]();
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
class stop_revolving_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["stop_revolving_req"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["stop_revolving_req"]["1"] = "ids";
    self::$fields["stop_revolving_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["stop_revolving_req"]["1000"] = "head";
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
    $v = new self::$fields["stop_revolving_req"]["1"]();
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
class stop_revolving_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["stop_revolving_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["stop_revolving_rsp"]["1"] = "ids";
    self::$fields["stop_revolving_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["stop_revolving_rsp"]["1000"] = "tail";
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
    $v = new self::$fields["stop_revolving_rsp"]["1"]();
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
class chat extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["chat"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["chat"]["1"] = "contents";
  }
  function contents()
  {
    return $this->_get_value("1");
  }
  function set_contents($value)
  {
    return $this->_set_value("1", $value);
  }
}
?>