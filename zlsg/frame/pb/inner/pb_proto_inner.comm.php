<?php
namespace inner;
class inner_head extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["inner_head"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    $this->values["1"] = new \PBInt();
    $this->values["1"]->value = 0;
    self::$fieldNames["inner_head"]["1"] = "svr_id";
    self::$fields["inner_head"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    $this->values["2"] = new \PBInt();
    $this->values["2"]->value = 0;
    self::$fieldNames["inner_head"]["2"] = "agent_id";
    self::$fields["inner_head"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    $this->values["3"] = new \PBInt();
    $this->values["3"]->value = 0;
    self::$fieldNames["inner_head"]["3"] = "client_id";
    self::$fields["inner_head"]["4"] = "\\PBString";
    $this->values["4"] = "";
    self::$fieldNames["inner_head"]["4"] = "client_ip";
  }
  function svr_id()
  {
    return $this->_get_value("1");
  }
  function set_svr_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function agent_id()
  {
    return $this->_get_value("2");
  }
  function set_agent_id($value)
  {
    return $this->_set_value("2", $value);
  }
  function client_id()
  {
    return $this->_get_value("3");
  }
  function set_client_id($value)
  {
    return $this->_set_value("3", $value);
  }
  function client_ip()
  {
    return $this->_get_value("4");
  }
  function set_client_ip($value)
  {
    return $this->_set_value("4", $value);
  }
}
class inner_tail extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["inner_tail"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["inner_tail"]["1"] = "status";
    self::$fields["inner_tail"]["2"] = "\\PBString";
    $this->values["2"] = "";
    self::$fieldNames["inner_tail"]["2"] = "info";
    self::$fields["inner_tail"]["3"] = "\\PBString";
    $this->values["3"] = "";
    self::$fieldNames["inner_tail"]["3"] = "data";
  }
  function status()
  {
    return $this->_get_value("1");
  }
  function set_status($value)
  {
    return $this->_set_value("1", $value);
  }
  function info()
  {
    return $this->_get_value("2");
  }
  function set_info($value)
  {
    return $this->_set_value("2", $value);
  }
  function data()
  {
    return $this->_get_value("3");
  }
  function set_data($value)
  {
    return $this->_set_value("3", $value);
  }
}
class inner_user extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["inner_user"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["inner_user"]["1"] = "id";
    self::$fields["inner_user"]["2"] = "\\PBString";
    $this->values["2"] = "";
    self::$fieldNames["inner_user"]["2"] = "account_name";
    self::$fields["inner_user"]["3"] = "\\PBString";
    $this->values["3"] = "";
    self::$fieldNames["inner_user"]["3"] = "nickname";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function account_name()
  {
    return $this->_get_value("2");
  }
  function set_account_name($value)
  {
    return $this->_set_value("2", $value);
  }
  function nickname()
  {
    return $this->_get_value("3");
  }
  function set_nickname($value)
  {
    return $this->_set_value("3", $value);
  }
}
class inner_goods extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["inner_goods"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["inner_goods"]["1"] = "index";
    self::$fields["inner_goods"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["inner_goods"]["2"] = "goods_id";
    self::$fields["inner_goods"]["3"] = "\\PBString";
    $this->values["3"] = "";
    self::$fieldNames["inner_goods"]["3"] = "goods_name";
    self::$fields["inner_goods"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["inner_goods"]["4"] = "goods_num";
    self::$fields["inner_goods"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["inner_goods"]["5"] = "goods_type";
  }
  function index()
  {
    return $this->_get_value("1");
  }
  function set_index($value)
  {
    return $this->_set_value("1", $value);
  }
  function goods_id()
  {
    return $this->_get_value("2");
  }
  function set_goods_id($value)
  {
    return $this->_set_value("2", $value);
  }
  function goods_name()
  {
    return $this->_get_value("3");
  }
  function set_goods_name($value)
  {
    return $this->_set_value("3", $value);
  }
  function goods_num()
  {
    return $this->_get_value("4");
  }
  function set_goods_num($value)
  {
    return $this->_set_value("4", $value);
  }
  function goods_type()
  {
    return $this->_get_value("5");
  }
  function set_goods_type($value)
  {
    return $this->_set_value("5", $value);
  }
}
class close_server extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["close_server"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["close_server"]["1"] = "unixtime";
    self::$fields["close_server"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["close_server"]["1000"] = "head";
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
?>