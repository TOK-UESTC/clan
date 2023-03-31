#!/bin/bash

# e.g.: ./test.sh 2
name=1
if [ $# -eq 1 ]
then
    name=$1
fi

# 编译项目
# build project
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Debug
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


# 与答题器交互
$executor "./build/main" -f -d -m ./maps/$name.txt
# 搜索main程序的PID号
# search main process id
# pid=`ps -ef | grep "./CodeCraft2023/build/main" | grep -v "grep" | awk '{print $2}'`
# 输出main程序的PID号
# output main process id
# echo $pid