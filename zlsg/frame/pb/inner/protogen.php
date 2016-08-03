<?php
require_once('./pb4php-read-only/parser/pb_parser.php');
/*$file = "inner.server.proto";
$test = new PBParser();
$test->parse($file);
$file = "inner.online.proto";
$test = new PBParser();
$test->parse($file);*/
//$file = "inner.promotions.proto";
//$test = new PBParser();
//$test->parse($file);
$file = "inner.bulletin.proto";
$test = new PBParser();
$test->parse($file);
//$file = "inner.player.proto";
//$test = new PBParser();
//$test->parse($file);
echo "success\n";
?>
