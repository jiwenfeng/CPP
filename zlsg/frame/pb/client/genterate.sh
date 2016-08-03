#!/bin/sh

if [ $# != 1 ] 
then
    rm ./*.h -f
    rm ./*.cc -f
    rm ./*.o -f
    protoc --cpp_out=./ ./*.proto
else
    rm ./$1.h -f
	rm ./$1.cc -f

    protoc --cpp_out=./ ./$1.proto
fi




