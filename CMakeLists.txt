# cmake 最低版本
cmake_minimum_required(VERSION 3.15)

# 最少需要满足C++17标准
set(CMAKE_CXX_STANDARD 17)

# 指定编译器为g++
add_definitions(-DCMAKE_CXX_COMPILER="g++")

# 项目名称
project(kuinkerm)

# 包含头文件
include_directories(${PROJECT_SOURCE_DIR})

# 搜索所有cpp文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

# 设置静态库输出地址
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 生成静态库
add_library(kuinkerm STATIC ${SRC_LIST})

