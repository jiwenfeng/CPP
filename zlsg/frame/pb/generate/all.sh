#!/bin/bash

#mysql -h192.168.1.243 -uroot -p'!Q@W#E$R%T^Y' -DBLCX -e "show create table tb_wing;"
cd ../client/
protoc --cpp_out=./ ./*.proto
cd ../generate/
cd ../server/
protoc --cpp_out=./ ./*.proto
cd ../generate/
cd ../inner/
protoc --cpp_out=./ ./*.proto
cd ../generate/
./data.sh
./config.sh


