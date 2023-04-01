project(CodeCraftSDK)
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

if (NOT WIN32)
    link_libraries(pthread m)
endif (NOT WIN32)

AUX_SOURCE_DIRECTORY(src SRC_LIST)
add_executable(main ${SRC_LIST} main.cpp)
target_include_directories(main PUBLIC include)

# ADD_EXECUTABLE(main ${src})