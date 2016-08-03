#!/bin/bash

#mysql -h192.168.1.242 -uroot -p'!Q@W#E$R%T^Y' -DBLCX -e "show create table tb_wing;"
rm ../data/mysql.tb_*.proto -f
rm ../data/*.a -f
rm ../data/*.o -f
rm ../data/*.h -f
rm ../data/*.cc -f
rm -f ./wj

mysqldump -uabel -p'0402' -d jwf > ./wj
#cat stn |awk -F'|' '{print $3}' >./sql_cfg

echo "parser.lua"
lua parser.lua
mv ./*.proto ../data/ -f
cd ../data/
protoc --cpp_out=./ ./*.proto

echo "gen_cc.lua"
cd ../generate
lua gen_cc.lua
mv ./pb_mgr_base.h ../../base/ -f
mv ./pb_mgr_base.cc ../../base/ -f

