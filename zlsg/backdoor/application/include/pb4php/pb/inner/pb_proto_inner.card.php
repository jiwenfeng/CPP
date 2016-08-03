<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class getCardList_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getCardList_req"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["getCardList_req"]["1"] = "cardId";
    self::$fields["getCardList_req"]["2"] = "\\PBString";
    $this->values["2"] = "";
    self::$fieldNames["getCardList_req"]["2"] = "cardName";
    self::$fields["getCardList_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["getCardList_req"]["3"] = "page";
    self::$fields["getCardList_req"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["getCardList_req"]["4"] = "pageSize";
    self::$fields["getCardList_req"]["1000"] = "inner\\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["getCardList_req"]["1000"] = "head";
  }
  function cardId()
  {
    return $this->_get_value("1");
  }
  function set_cardId($value)
  {
    return $this->_set_value("1", $value);
  }
  function cardName()
  {
    return $this->_get_value("2");
  }
  function set_cardName($value)
  {
    return $this->_set_value("2", $value);
  }
  function page()
  {
    return $this->_get_value("3");
  }
  function set_page($value)
  {
    return $this->_set_value("3", $value);
  }
  function pageSize()
  {
    return $this->_get_value("4");
  }
  function set_pageSize($value)
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
class getCardList_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getCardList_rsp"]["1000"] = "inner\\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["getCardList_rsp"]["1000"] = "tail";
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
class getGoodsList_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getGoodsList_rsp"]["1000"] = "inner\\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["getGoodsList_rsp"]["1000"] = "tail";
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
class goods_info extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["goods_info"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["goods_info"]["1"] = "goods_type";
    self::$fields["goods_info"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["goods_info"]["2"] = "goods_id";
    self::$fields["goods_info"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["goods_info"]["3"] = "goods_num";
    self::$fields["goods_info"]["4"] = "\\PBBool";
    $this->values["4"] = "";
    self::$fieldNames["goods_info"]["4"] = "bind";
  }
  function goods_type()
  {
    return $this->_get_value("1");
  }
  function set_goods_type($value)
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
  function goods_num()
  {
    return $this->_get_value("3");
  }
  function set_goods_num($value)
  {
    return $this->_set_value("3", $value);
  }
  function bind()
  {
    return $this->_get_value("4");
  }
  function set_bind($value)
  {
    return $this->_set_value("4", $value);
  }
}
class sendGoods_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["sendGoods_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["sendGoods_req"]["1"] = "type";
    self::$fields["sendGoods_req"]["2"] = "inner\\inner_user";
    $this->values["2"] = array();
    self::$fieldNames["sendGoods_req"]["2"] = "user";
    self::$fields["sendGoods_req"]["4"] = "\\PBString";
    $this->values["4"] = "";
    self::$fieldNames["sendGoods_req"]["4"] = "title";
    self::$fields["sendGoods_req"]["5"] = "\\PBString";
    $this->values["5"] = "";
    self::$fieldNames["sendGoods_req"]["5"] = "content";
    self::$fields["sendGoods_req"]["6"] = "inner\\goods_info";
    $this->values["6"] = array();
    self::$fieldNames["sendGoods_req"]["6"] = "gi";
    self::$fields["sendGoods_req"]["7"] = "\\PBInt";
    $this->values["7"] = "";
    self::$fieldNames["sendGoods_req"]["7"] = "invalidTime";
    self::$fields["sendGoods_req"]["8"] = "\\PBInt";
    $this->values["8"] = "";
    self::$fieldNames["sendGoods_req"]["8"] = "count";
    self::$fields["sendGoods_req"]["1000"] = "inner\\inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["sendGoods_req"]["1000"] = "head";
  }
  function type()
  {
    return $this->_get_value("1");
  }
  function set_type($value)
  {
    return $this->_set_value("1", $value);
  }
  function user($offset)
  {
    return $this->_get_arr_value("2", $offset);
  }
  function add_user()
  {
    return $this->_add_arr_value("2");
  }
  function set_user($index, $value)
  {
    $this->_set_arr_value("2", $index, $value);
  }
  function set_all_users($values)
  {
    return $this->_set_arr_values("2", $values);
  }
  function remove_last_user()
  {
    $this->_remove_last_arr_value("2");
  }
  function users_size()
  {
    return $this->_get_arr_size("2");
  }
  function get_users()
  {
    return $this->_get_value("2");
  }
  function title()
  {
    return $this->_get_value("4");
  }
  function set_title($value)
  {
    return $this->_set_value("4", $value);
  }
  function content()
  {
    return $this->_get_value("5");
  }
  function set_content($value)
  {
    return $this->_set_value("5", $value);
  }
  function gi($offset)
  {
    return $this->_get_arr_value("6", $offset);
  }
  function add_gi()
  {
    return $this->_add_arr_value("6");
  }
  function set_gi($index, $value)
  {
    $this->_set_arr_value("6", $index, $value);
  }
  function set_all_gis($values)
  {
    return $this->_set_arr_values("6", $values);
  }
  function remove_last_gi()
  {
    $this->_remove_last_arr_value("6");
  }
  function gis_size()
  {
    return $this->_get_arr_size("6");
  }
  function get_gis()
  {
    return $this->_get_value("6");
  }
  function invalidTime()
  {
    return $this->_get_value("7");
  }
  function set_invalidTime($value)
  {
    return $this->_set_value("7", $value);
  }
  function count()
  {
    return $this->_get_value("8");
  }
  function set_count($value)
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
class sendGoods_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["sendGoods_rsp"]["1000"] = "inner\\inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["sendGoods_rsp"]["1000"] = "tail";
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
