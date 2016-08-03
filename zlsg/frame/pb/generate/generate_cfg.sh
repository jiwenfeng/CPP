#!/bin/bash


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
