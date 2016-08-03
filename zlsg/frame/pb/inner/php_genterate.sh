#!/bin/sh

if [ $# != 1 ] 
then
    rm ./pb_proto_*.php -f
    php protocall.php 
else
    echo "not support"
fi

