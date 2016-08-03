<?php
echo date("Y-m-d H:i:s")."\n";
require_once('../parser/pb_parser.php');
echo date("Y-m-d H:i:s")."\n";
$test = new PBParser();
$test->parse('./inner.comm.proto');
$test = new PBParser();
$test->parse('./inner.notice.proto');
echo date("Y-m-d H:i:s")."\n";
var_dump('File parsing done!');
echo date("Y-m-d H:i:s")."\n";
?>
