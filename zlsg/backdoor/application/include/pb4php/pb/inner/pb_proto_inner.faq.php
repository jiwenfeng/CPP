<?php
namespace inner;
require_once( "pb_proto_inner.comm.php" );
class faqType_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqType_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["faqType_req"]["1"] = "lang";
    self::$fields["faqType_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["faqType_req"]["2"] = "game_id";
  }
  function lang()
  {
    return $this->_get_value("1");
  }
  function set_lang($value)
  {
    return $this->_set_value("1", $value);
  }
  function game_id()
  {
    return $this->_get_value("2");
  }
  function set_game_id($value)
  {
    return $this->_set_value("2", $value);
  }
}
class faqType_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqType_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["faqType_rsp"]["1000"] = "tail";
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
class faqList_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqList_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["faqList_req"]["1"] = "kind_id";
    self::$fields["faqList_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["faqList_req"]["2"] = "page";
    self::$fields["faqList_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["faqList_req"]["3"] = "ps";
  }
  function kind_id()
  {
    return $this->_get_value("1");
  }
  function set_kind_id($value)
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
  function ps()
  {
    return $this->_get_value("3");
  }
  function set_ps($value)
  {
    return $this->_set_value("3", $value);
  }
}
class faqList_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqList_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["faqList_rsp"]["1000"] = "tail";
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
class faqDetail_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqDetail_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["faqDetail_req"]["1"] = "id";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
  {
    return $this->_set_value("1", $value);
  }
}
class faqDetail_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqDetail_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["faqDetail_rsp"]["1000"] = "tail";
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
class faqEvaluate_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqEvaluate_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["faqEvaluate_req"]["1"] = "id";
    self::$fields["faqEvaluate_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["faqEvaluate_req"]["2"] = "faq_whether";
    self::$fields["faqEvaluate_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["faqEvaluate_req"]["3"] = "faq_opinion";
    self::$fields["faqEvaluate_req"]["4"] = "\\PBString";
    $this->values["4"] = "";
    self::$fieldNames["faqEvaluate_req"]["4"] = "content";
  }
  function id()
  {
    return $this->_get_value("1");
  }
  function set_id($value)
  {
    return $this->_set_value("1", $value);
  }
  function faq_whether()
  {
    return $this->_get_value("2");
  }
  function set_faq_whether($value)
  {
    return $this->_set_value("2", $value);
  }
  function faq_opinion()
  {
    return $this->_get_value("3");
  }
  function set_faq_opinion($value)
  {
    return $this->_set_value("3", $value);
  }
  function content()
  {
    return $this->_get_value("4");
  }
  function set_content($value)
  {
    return $this->_set_value("4", $value);
  }
}
class faqEvaluate_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqEvaluate_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["faqEvaluate_rsp"]["1000"] = "tail";
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
class faqHot_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqHot_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["faqHot_req"]["1"] = "lang";
    self::$fields["faqHot_req"]["2"] = "\\PBInt";
    $this->values["2"] = "";
    self::$fieldNames["faqHot_req"]["2"] = "ps";
    self::$fields["faqHot_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["faqHot_req"]["3"] = "game_id";
  }
  function lang()
  {
    return $this->_get_value("1");
  }
  function set_lang($value)
  {
    return $this->_set_value("1", $value);
  }
  function ps()
  {
    return $this->_get_value("2");
  }
  function set_ps($value)
  {
    return $this->_set_value("2", $value);
  }
  function game_id()
  {
    return $this->_get_value("3");
  }
  function set_game_id($value)
  {
    return $this->_set_value("3", $value);
  }
}
class faqHot_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqHot_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["faqHot_rsp"]["1000"] = "tail";
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
class faqSearch_req extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqSearch_req"]["1"] = "\\PBInt";
    $this->values["1"] = "";
    self::$fieldNames["faqSearch_req"]["1"] = "lang";
    self::$fields["faqSearch_req"]["2"] = "\\PBString";
    $this->values["2"] = "";
    self::$fieldNames["faqSearch_req"]["2"] = "key_word";
    self::$fields["faqSearch_req"]["3"] = "\\PBInt";
    $this->values["3"] = "";
    self::$fieldNames["faqSearch_req"]["3"] = "game_id";
    self::$fields["faqSearch_req"]["4"] = "\\PBInt";
    $this->values["4"] = "";
    self::$fieldNames["faqSearch_req"]["4"] = "page";
    self::$fields["faqSearch_req"]["5"] = "\\PBInt";
    $this->values["5"] = "";
    self::$fieldNames["faqSearch_req"]["5"] = "ps";
  }
  function lang()
  {
    return $this->_get_value("1");
  }
  function set_lang($value)
  {
    return $this->_set_value("1", $value);
  }
  function key_word()
  {
    return $this->_get_value("2");
  }
  function set_key_word($value)
  {
    return $this->_set_value("2", $value);
  }
  function game_id()
  {
    return $this->_get_value("3");
  }
  function set_game_id($value)
  {
    return $this->_set_value("3", $value);
  }
  function page()
  {
    return $this->_get_value("4");
  }
  function set_page($value)
  {
    return $this->_set_value("4", $value);
  }
  function ps()
  {
    return $this->_get_value("5");
  }
  function set_ps($value)
  {
    return $this->_set_value("5", $value);
  }
}
class faqSearch_rsp extends \PBMessage
{
  var $wired_type = \PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    self::$fields["faqSearch_rsp"]["1000"] = "inner_tail";
    $this->values["1000"] = "";
    self::$fieldNames["faqSearch_rsp"]["1000"] = "tail";
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