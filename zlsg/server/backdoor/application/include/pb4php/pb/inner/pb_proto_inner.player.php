<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class getPlayerList_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getPlayerList_req"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["getPlayerList_req"]["1"] = "user";
    self::$fields["getPlayerList_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["getPlayerList_req"]["2"] = "userType";
    self::$fields["getPlayerList_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["getPlayerList_req"]["3"] = "regBeginTime";
    self::$fields["getPlayerList_req"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["getPlayerList_req"]["4"] = "regEndTime";
    self::$fields["getPlayerList_req"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["getPlayerList_req"]["5"] = "loginBeginTime";
    self::$fields["getPlayerList_req"]["6"] = "\\PBInt";
    $this->values["6"] = "";
    self::$fieldNames["getPlayerList_req"]["6"] = "loginEndTime";
    self::$fields["getPlayerList_req"]["7"] = "\\PBInt";
    $this->values["7"] = "";
    self::$fieldNames["getPlayerList_req"]["7"] = "page";
    self::$fields["getPlayerList_req"]["8"] = "\\PBInt";
    $this->values["8"] = "";
    self::$fieldNames["getPlayerList_req"]["8"] = "pageSize";
    self::$fields["getPlayerList_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["getPlayerList_req"]["1000"] = "head";
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
  function regBeginTime()
  {
    return $this->_get_value("3");
  }
  function set_regBeginTime($value)
  {
    return $this->_set_value("3", $value);
  }
  function regEndTime()
  {
    return $this->_get_value("4");
  }
  function set_regEndTime($value)
  {
    return $this->_set_value("4", $value);
  }
  function loginBeginTime()
  {
    return $this->_get_value("5");
  }
  function set_loginBeginTime($value)
  {
    return $this->_set_value("5", $value);
  }
  function loginEndTime()
  {
    return $this->_get_value("6");
  }
  function set_loginEndTime($value)
  {
    return $this->_set_value("6", $value);
  }
  function page()
  {
    return $this->_get_value("7");
  }
  function set_page($value)
  {
    return $this->_set_value("7", $value);
  }
  function pageSize()
  {
    return $this->_get_value("8");
  }
  function set_pageSize($value)
  {
    return $this->_set_value("8", $value);
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
class getPlayerList_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getPlayerList_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["getPlayerList_rsp"]["1000"] = "tail";
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
class t_online_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["t_online_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["t_online_req"]["1"] = "unixtime";
    self::$fields["t_online_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["t_online_req"]["1000"] = "head";
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
class t_online_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["t_online_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["t_online_rsp"]["1"] = "id";
    self::$fields["t_online_rsp"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["t_online_rsp"]["2"] = "sid";
    self::$fields["t_online_rsp"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["t_online_rsp"]["3"] = "allcount";
    self::$fields["t_online_rsp"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["t_online_rsp"]["4"] = "cashcount";
    self::$fields["t_online_rsp"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["t_online_rsp"]["5"] = "createdate";
    self::$fields["t_online_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["t_online_rsp"]["1000"] = "tail";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function sid()
  {
    return $this->_get_value("2");
  }
  function set_sid($value)
  {
    return $this->_set_value("2", $value);
  }
  function allcount()
  {
    return $this->_get_value("3");
  }
  function set_allcount($value)
  {
    return $this->_set_value("3", $value);
  }
  function cashcount()
  {
    return $this->_get_value("4");
  }
  function set_cashcount($value)
  {
    return $this->_set_value("4", $value);
  }
  function createdate()
  {
    return $this->_get_value("5");
  }
  function set_createdate($value)
  {
    return $this->_set_value("5", $value);
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
class realtime_roles extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["realtime_roles"]["1"] = "\\PBInt";
    $this->values["1"] = array();
    self::$fieldNames["realtime_roles"]["1"] = "id";
  }
  function id($offset)
  {
    $v = $this->_get_arr_value("1", $offset);
    return $v->get_value();
  }
  function append_id($value)
  {
    $v = $this->_add_arr_value("1");
    $v->set_value($value);
  }
  function set_id($index, $value)
  {
    $v = new self::$fields["realtime_roles"]["1"]();
    $v->set_value($value);
    $this->_set_arr_value("1", $index, $v);
  }
  function remove_last_id()
  {
    $this->_remove_last_arr_value("1");
  }
  function ids_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_ids()
  {
    return $this->_get_value("1");
  }
}
class set_attr_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["set_attr_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["set_attr_req"]["1"] = "index";
    self::$fields["set_attr_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["set_attr_req"]["2"] = "value";
    self::$fields["set_attr_req"]["3"] = "inner\inner_user";
    $this->values["3"] = array();
    self::$fieldNames["set_attr_req"]["3"] = "user";
    self::$fields["set_attr_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["set_attr_req"]["1000"] = "head";
  }
  function index()
  {
    return $this->_get_value("1");
  }
  function set_index($value)
  {
    return $this->_set_value("1", $value);
  }
  function value()
  {
    return $this->_get_value("2");
  }
  function set_value($value)
  {
    return $this->_set_value("2", $value);
  }
  function user($offset)
  {
    return $this->_get_arr_value("3", $offset);
  }
  function add_user()
  {
    return $this->_add_arr_value("3");
  }
  function set_user($index, $value)
  {
    $this->_set_arr_value("3", $index, $value);
  }
  function set_all_users($values)
  {
    return $this->_set_arr_values("3", $values);
  }
  function remove_last_user()
  {
    $this->_remove_last_arr_value("3");
  }
  function users_size()
  {
    return $this->_get_arr_size("3");
  }
  function get_users()
  {
    return $this->_get_value("3");
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
class set_attr_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["set_attr_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["set_attr_rsp"]["1"] = "index";
    self::$fields["set_attr_rsp"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["set_attr_rsp"]["2"] = "value";
    self::$fields["set_attr_rsp"]["3"] = "inner\inner_user";
    $this->values["3"] = array();
    self::$fieldNames["set_attr_rsp"]["3"] = "user";
    self::$fields["set_attr_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["set_attr_rsp"]["1000"] = "tail";
  }
  function index()
  {
    return $this->_get_value("1");
  }
  function set_index($value)
  {
    return $this->_set_value("1", $value);
  }
  function value()
  {
    return $this->_get_value("2");
  }
  function set_value($value)
  {
    return $this->_set_value("2", $value);
  }
  function user($offset)
  {
    return $this->_get_arr_value("3", $offset);
  }
  function add_user()
  {
    return $this->_add_arr_value("3");
  }
  function set_user($index, $value)
  {
    $this->_set_arr_value("3", $index, $value);
  }
  function set_all_users($values)
  {
    return $this->_set_arr_values("3", $values);
  }
  function remove_last_user()
  {
    $this->_remove_last_arr_value("3");
  }
  function users_size()
  {
    return $this->_get_arr_size("3");
  }
  function get_users()
  {
    return $this->_get_value("3");
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
class set_vip_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["set_vip_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["set_vip_req"]["1"] = "lv";
    self::$fields["set_vip_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["set_vip_req"]["2"] = "month";
    self::$fields["set_vip_req"]["3"] = "inner\inner_user";
    $this->values["3"] = array();
    self::$fieldNames["set_vip_req"]["3"] = "user";
    self::$fields["set_vip_req"]["1000"] = "inner\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["set_vip_req"]["1000"] = "head";
  }
  function lv()
  {
    return $this->_get_value("1");
  }
  function set_lv($value)
  {
    return $this->_set_value("1", $value);
  }
  function month()
  {
    return $this->_get_value("2");
  }
  function set_month($value)
  {
    return $this->_set_value("2", $value);
  }
  function user($offset)
  {
    return $this->_get_arr_value("3", $offset);
  }
  function add_user()
  {
    return $this->_add_arr_value("3");
  }
  function set_user($index, $value)
  {
    $this->_set_arr_value("3", $index, $value);
  }
  function set_all_users($values)
  {
    return $this->_set_arr_values("3", $values);
  }
  function remove_last_user()
  {
    $this->_remove_last_arr_value("3");
  }
  function users_size()
  {
    return $this->_get_arr_size("3");
  }
  function get_users()
  {
    return $this->_get_value("3");
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
class set_vip_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["set_vip_rsp"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["set_vip_rsp"]["1"] = "lv";
    self::$fields["set_vip_rsp"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["set_vip_rsp"]["2"] = "month";
    self::$fields["set_vip_rsp"]["3"] = "inner\inner_user";
    $this->values["3"] = array();
    self::$fieldNames["set_vip_rsp"]["3"] = "user";
    self::$fields["set_vip_rsp"]["1000"] = "inner\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["set_vip_rsp"]["1000"] = "tail";
  }
  function lv()
  {
    return $this->_get_value("1");
  }
  function set_lv($value)
  {
    return $this->_set_value("1", $value);
  }
  function month()
  {
    return $this->_get_value("2");
  }
  function set_month($value)
  {
    return $this->_set_value("2", $value);
  }
  function user($offset)
  {
    return $this->_get_arr_value("3", $offset);
  }
  function add_user()
  {
    return $this->_add_arr_value("3");
  }
  function set_user($index, $value)
  {
    $this->_set_arr_value("3", $index, $value);
  }
  function set_all_users($values)
  {
    return $this->_set_arr_values("3", $values);
  }
  function remove_last_user()
  {
    $this->_remove_last_arr_value("3");
  }
  function users_size()
  {
    return $this->_get_arr_size("3");
  }
  function get_users()
  {
    return $this->_get_value("3");
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