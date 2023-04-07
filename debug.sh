#!/bin/bash

# build project
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-DW_DEBUG -g -O0"
make -C build -j

# 判断系统类型
uNames=`uname -s`
osName=${uNames: 0: 4}
if [ "$osName" == "Linu" ]
then
    executor="./Robot"
else
    executor="./Robot_gui.exe"
fi

if [ $# -eq 1 ]
then
    name=$1
    # 与答题器交互
    cd ..
    $executor -f -d -m ./maps/$name.txt -s 0 "./clan/build/main.exe"
    cd clan
else
    echo "no map specified"
fi
