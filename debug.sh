#!/bin/bash

# build project
rm -r build
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-DW_DEBUG -g -O0"
# make -DP_DEBUG -C build -j
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
    $executor -f -d -m -s 0 ./maps/$name.txt "./clan/build/main.exe"
    cd clan
else
    echo "no map specified"
fi


