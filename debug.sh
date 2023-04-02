#!/bin/bash

# e.g.: ./test.sh 2
name=1
if [ $# -eq 1 ]
then
    name=$1
fi

# 以管理员身份运行执行killmain.bat文件，杀死main进程
# run killmain.bat as administrator to kill main process
# runas /user:Administrator "cmd /c killmain.bat"


# 编译项目
# build project
rm -r build
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Debug  -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_FLAGS="-g -O0" -DCMAKE_C_FLAGS="-g -O0"
make -C build -j4

# 判断系统类型
uNames=`uname -s`
osName=${uNames: 0: 4}
if [ "$osName" == "Linu" ]
then
    executor="./Robot"
else
    executor="./Robot_gui.exe"
fi


# 与答题器交互
CURRENT_DIR=$(cd $(dirname $0); pwd)
cd ../
$executor  "$CURRENT_DIR/build/main.exe" -d -m ./maps/$name.txt
cd $CURRENT_DIR