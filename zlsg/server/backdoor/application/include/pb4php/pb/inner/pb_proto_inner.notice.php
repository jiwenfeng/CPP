<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class addNotice_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["addNotice_req"]["1"] = "\\PBString";
    $this->values["1"] = "";
    self::$fieldNames["addNotice_req"]["1"] = "message";
    self::$fields["addNotice_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["addNotice_req"]["2"] = "intervalTime";
    self::$fields["addNotice_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["addNotice_req"]["3"] = "startTime";
    self::$fields["addNotice_req"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["addNotice_req"]["4"] = "endTime";
    self::$fields["addNotice_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["addNotice_req"]["1000"] = "head";
  }
  function message()
  {
    return $this->_get_value("1");
  }
  function set_message($value)
  {
    return $this->_set_value("1", $value);
  }
  function intervalTime()
  {
    return $this->_get_value("2");
  }
  function set_intervalTime($value)
  {
    return $this->_set_value("2", $value);
  }
  function startTime()
  {
    return $this->_get_value("3");
  }
  function set_startTime($value)
  {
    return $this->_set_value("3", $value);
  }
  function endTime()
  {
    return $this->_get_value("4");
  }
  function set_endTime($value)
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
class addNotice_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["addNotice_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["addNotice_rsp"]["1000"] = "tail";
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
class getNoticeList_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getNoticeList_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["getNoticeList_req"]["1"] = "id";
    self::$fields["getNoticeList_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["getNoticeList_req"]["2"] = "page";
    self::$fields["getNoticeList_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["getNoticeList_req"]["3"] = "pageSize";
    self::$fields["getNoticeList_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["getNoticeList_req"]["1000"] = "head";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
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
class notice_info extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["notice_info"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["notice_info"]["1"] = "id";
    self::$fields["notice_info"]["2"] = "\\PBString";
    $this->values["2"] = "";
    self::$fieldNames["notice_info"]["2"] = "message";
    self::$fields["notice_info"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["notice_info"]["3"] = "intervalTime";
    self::$fields["notice_info"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["notice_info"]["4"] = "startTime";
    self::$fields["notice_info"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["notice_info"]["5"] = "endTime";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function message()
  {
    return $this->_get_value("2");
  }
  function set_message($value)
  {
    return $this->_set_value("2", $value);
  }
  function intervalTime()
  {
    return $this->_get_value("3");
  }
  function set_intervalTime($value)
  {
    return $this->_set_value("3", $value);
  }
  function startTime()
  {
    return $this->_get_value("4");
  }
  function set_startTime($value)
  {
    return $this->_set_value("4", $value);
  }
  function endTime()
  {
    return $this->_get_value("5");
  }
  function set_endTime($value)
  {
    return $this->_set_value("5", $value);
  }
}
class getNoticeList_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["getNoticeList_rsp"]["1"] = "notice_info";
    $this->values["1"] = array();
    self::$fieldNames["getNoticeList_rsp"]["1"] = "ni";
    self::$fields["getNoticeList_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["getNoticeList_rsp"]["1000"] = "tail";
  }
  function ni($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_ni()
  {
    return $this->_add_arr_value("1");
  }
  function set_ni($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function set_all_nis($values)
  {
    return $this->_set_arr_values("1", $values);
  }
  function remove_last_ni()
  {
    $this->_remove_last_arr_value("1");
  }
  function nis_size()
  {
    return $this->_get_arr_size("1");
  }
  function get_nis()
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
class updateNotice_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["updateNotice_req"]["1"] = "notice_info";
    $this->values["1"] = "";
    self::$fieldNames["updateNotice_req"]["1"] = "ni";
    self::$fields["updateNotice_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["updateNotice_req"]["1000"] = "head";
  }
  function ni()
  {
    return $this->_get_value("1");
  }
  function set_ni($value)
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
class updateNotice_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["updateNotice_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["updateNotice_rsp"]["1000"] = "tail";
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
class delNotice_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["delNotice_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["delNotice_req"]["1"] = "id";
    self::$fields["delNotice_req"]["1000"] = "inner_head";
    $this->values["1000"] = "";
    self::$fieldNames["delNotice_req"]["1000"] = "head";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
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
class delNotice_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["delNotice_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["delNotice_rsp"]["1000"] = "tail";
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