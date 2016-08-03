#!/bin/bash

#mysql -h192.168.1.242 -uroot -p'!Q@W#E$R%T^Y' -DBLCX -e "show create table tb_wing;"
#mysqldump -h192.168.1.242 -uroot -p'!Q@W#E$R%T^Y' -d BLCX >./wj
rm ../config/*.o -f
rm ../config/*.h -f
rm ../config/*.cc -f
rm ../config/mysql.tb_*.proto -f
rm ../config/*.a -f

rm ../client_config/*.proto -f

#rm ./type_name -f
#cat config |awk -F'|' '{print $2}' >./type_name

cd ../client_config
svn del *.proto
svn commit --message="auto pb generate file commit"
cd ../generate

cat ./config|while read line; do
    is_client=`awk -F'|' 'BEGIN {split("'"$line"'",arr);print arr[1]}'`
    cfg_name=`awk -F'|' 'BEGIN {split("'"$line"'",arr);print arr[2]}'`
    sql_query=`awk -F'|' 'BEGIN {split("'"$line"'",arr);print arr[3]}'`
	
	if [[ $cfg_name == tb_* ]]; then
	    rm ./wj -f
	    mysqldump -h192.168.1.242 -uroot -p'!Q@W#E$R%T^Y' -d BLCX $cfg_name >./wj
	    lua parser.lua

		if [[ $is_client == y ]]; then
	        cp mysql.$cfg_name.proto ../client_config/ -f
		fi
	fi
done

mv ./*.proto ../config/ -f

echo "--------------step 1. generat config proto and move to config------------------------------"
	
cd ../config/
protoc --cpp_out=./ ./*.proto	

echo "--------------step 2. generat config proto h cpp file------------------------------"

cd ../generate
lua gen_cc.lua
mv ./pb_mgr_base.h ../../base/ -f
mv ./pb_mgr_base.cc ../../base/ -f

echo "--------------step 3. generat pb_mgr_base.h and cc file------------------------------"

cd ../../../
make clean
make -j3

echo "--------------step 4. make and make clean------------------------------"

cd ./frame/pb/generate/cfg_create
#make clean
#make
cd ..

echo "--------------step 5. make cfg_create file------------------------------"

rm ./*.cfg -rf
./cfgcreate ./config
mv *.cfg ../../../bin/game_cfg/ -f

echo "--------------step 6. copy server cfg to game_cfg dir------------------------------"

cat ./config|while read line; do
    is_client=`awk -F'|' 'BEGIN {split("'"$line"'",arr);print arr[1]}'`
	cfg_name=`awk -F'|' 'BEGIN {split("'"$line"'",arr);print arr[2]}'`
	if [[ $is_client == y ]]; then
	    zip static.pbz ./mysql.$cfg_name.pb
	fi
done
rm ./*.pb -rf
cp static.pbz static.pbz.copy
mv static.pbz /home/www/merge_env/res_api/tres/ -f
mv static.pbz.copy /home/www/res_api/tres/static.pbz -f

echo "--------------step 7. copy client cfg to tres dir------------------------------"

cd ../client_config
svn add *.proto
svn commit --message="auto pb generate file commit"
 
