#! /bin/sh
if [ "$#" -lt 1 ] #判断一下参数的个数对不对  
then  
    echo "need file name"  
    exit  
fi  
echo $1

sed -i 's/inner_head/inner\\inner_head/g' $1
sed -i 's/inner_tail/inner\\inner_tail/g' $1
sed -i 's/inner_user/inner\\inner_user/g' $1
