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


# 与答题器交互
# interact with discriminator
$current=`pwd`
cd ..
./Robot_gui "./CodeCraft2023/build/main" -f -d -m ./maps/$name.txt
# 搜索main程序的PID号
# search main process id
# pid=`ps -ef | grep "./CodeCraft2023/build/main" | grep -v "grep" | awk '{print $2}'`
# 输出main程序的PID号
# output main process id
# echo $pid
cd $current