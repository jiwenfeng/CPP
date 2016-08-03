#!/bin/bash

#mysql -h192.168.1.243 -uroot -p'!Q@W#E$R%T^Y' -DBLCX -e "show create table tb_wing;"
lua gen_cc.lua
mv ./pb_mgr_base.h ../../base/ -f
mv ./pb_mgr_base.cc ../../base/ -f
