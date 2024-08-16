#声明cmake最低版本
cmake_minimum_required(VERSION 3.0)


set(CMAKE_BUILD_TYPE "Debug")

#工程名称
project(HelloSLAM)

#添加一个可执行程序
#程序名 源代码文件
add_executable(helloSLAM helloSLAM.cpp)


#添加静态库
add_library(hello libHelloSLAM.cpp)
#添加动态库
add_library(hello_shared SHARED libHelloSLAM.cpp)

add_executable(useHello useHello.cpp)

target_link_libraries(useHello hello_shared)

