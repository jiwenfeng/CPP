<?php
require_once('./pb4php-read-only/parser/pb_parser.php');
$handle = opendir('./');   
while (false !== ($file = readdir($handle)))   
{   
    $basename = pathinfo($file, PATHINFO_EXTENSION);   
    if ($basename == 'proto')   
    {   
        echo "$file ";
        $test = new PBParser();
        $test->parse($file);
        echo "success\n";
    }   
}   
closedir($handle);
?>
