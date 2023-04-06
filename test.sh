#!/bin/bash

# build project
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS=""
make -C build -j

# 判断系统类型
uNames=`uname -s`
osName=${uNames: 0: 4}
if [ "$osName" == "Linu" ]
then
    executor="../Robot"
else
    executor="../Robot.exe"
fi

if [ $# -eq 1 ]
then
    name=$1
    $executor "./build/main" -f  -m ../maps/$name.txt
else
    echo "" > error.txt
    for i in $(seq 1 1 4)
    do
        $executor "./build/main" -f -m  ../maps/$i.txt >> error.txt
    done
    # 读取error.txt文件的内容并输出其中的score行
    awk '{if(match($0, /"score":[0-9]+/)){sum+=substr($0,RSTART+8,RLENGTH-8)}} END {print sum}' error.txt
fi
