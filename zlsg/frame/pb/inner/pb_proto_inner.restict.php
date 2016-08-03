<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class getPlayerRestictList_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getPlayerRestictList_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["getPlayerRestictList_req"]["1"] = "type";
    self::$fields["getPlayerRestictList_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["getPlayerRestictList_req"]["2"] = "page";
    self::$fields["getPlayerRestictList_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["getPlayerRestictList_req"]["3"] = "pageSize";
    self::$fields["getPlayerRestictList_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["getPlayerRestictList_req"]["1000"] = "head";
  }
  function type()
  {
    return $this->_get_value("1");
  }
  function set_type($value)
  {
    return $this->_set_value("1", $value);
  }
  function page()
  {
    return $this->_get_value("2");
  }
  function set_page($value)
  {
    return $this->_set_value("2", $value);
  }
  function pageSize()
  {
    return $this->_get_value("3");
  }
  function set_pageSize($value)
  {
    return $this->_set_value("3", $value);
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
class ban_info extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["ban_info"]["1"] = "inner_user";
    $this->values["1"] = "";
    self::$fieldNames["ban_info"]["1"] = "user";
    self::$fields["ban_info"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["ban_info"]["2"] = "type";
    self::$fields["ban_info"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["ban_info"]["3"] = "time";
  }
  function user()
  {
    return $this->_get_value("1");
  }
  function set_user($value)
  {
    return $this->_set_value("1", $value);
  }
  function type()
  {
    return $this->_get_value("2");
  }
  function set_type($value)
  {
    return $this->_set_value("2", $value);
  }
  function time()
  {
    return $this->_get_value("3");
  }
  function set_time($value)
  {
    return $this->_set_value("3", $value);
  }
}
class getPlayerRestictList_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getPlayerRestictList_rsp"]["1"] = "ban_info";
    $this->values["1"] = array();
    self::$fieldNames["getPlayerRestictList_rsp"]["1"] = "bi";
    self::$fields["getPlayerRestictList_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["getPlayerRestictList_rsp"]["1000"] = "tail";
  }
  function bi($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_bi()
  {
    return $this->_add_arr_value("1");
  }
  function set_bi($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_bis($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_bi()
  {
    $this->_remove_last_arr_value("1");
  }
  function bis_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_bis()
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
class addPlayerRestict_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["addPlayerRestict_req"]["1"] = "ban_info";
    $this->values["1"] = array();
    self::$fieldNames["addPlayerRestict_req"]["1"] = "bi";
    self::$fields["addPlayerRestict_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["addPlayerRestict_req"]["1000"] = "head";
  }
  function bi($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_bi()
  {
    return $this->_add_arr_value("1");
  }
  function set_bi($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_bis($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_bi()
  {
    $this->_remove_last_arr_value("1");
  }
  function bis_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_bis()
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
class addPlayerRestict_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["addPlayerRestict_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["addPlayerRestict_rsp"]["1000"] = "tail";
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
class delPlayerRestict_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["delPlayerRestict_req"]["1"] = "ban_info";
    $this->values["1"] = array();
    self::$fieldNames["delPlayerRestict_req"]["1"] = "bi";
    self::$fields["delPlayerRestict_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["delPlayerRestict_req"]["1000"] = "head";
  }
  function bi($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_bi()
  {
    return $this->_add_arr_value("1");
  }
  function set_bi($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_bis($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_bi()
  {
    $this->_remove_last_arr_value("1");
  }
  function bis_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_bis()
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
class delPlayerRestict_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["delPlayerRestict_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["delPlayerRestict_rsp"]["1000"] = "tail";
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