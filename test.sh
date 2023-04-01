#!/bin/bash

# e.g.: ./test.sh 2

# cmake编译项目并将可执行文件与过程文件放在build文件夹中
# cmake build project and put executable file and process file in build folder
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Release 
make -C build -j4

# 判断系统类型
uNames=`uname -s`
osName=${uNames: 0: 4}
if [ "$osName" == "Linu" ]
then
    executor="../Robot"
else
    executor="../Robot.exe"
fi

if [ -e "/proc/sys/fs/binfmt_misc/WSLInterop" ]
then
    executor="../Robot.exe"
fi

if [ $# -eq 1 ]
then
    name=$1
    # 与答题器交互
    # interact with discriminator
    $executor "./build/main" -f  -m ../maps/$name.txt
else
    # 与答题器交互
    # interact with discriminator
    # 清空error.txt文件的内容
    # clear error.txt
    echo "" > error.txt
    $executor "./build/main" -f -d -m  ../maps/1.txt >> error.txt
    $executor "./build/main" -f -d -m  ../maps/2.txt >> error.txt
    $executor "./build/main" -f -d -m  ../maps/3.txt >> error.txt
    $executor "./build/main" -f -d -m  ../maps/4.txt >> error.txt
    # 读取error.txt文件的内容并输出其中的score行
    # read error.txt and output score line
    awk '{if(match($0, /"score":[0-9]+/)){sum+=substr($0,RSTART+8,RLENGTH-8)}} END {print sum}' error.txt
fi
