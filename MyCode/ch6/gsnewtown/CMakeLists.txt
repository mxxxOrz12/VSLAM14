cmake_minimum_required(VERSION 3.0)

project(gaussNewton)

find_package(OpenCV)

add_executable(gaussNewton gaussNewton.cpp)
target_link_libraries(gaussNewton ${OpenCV_LIBS})