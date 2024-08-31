cmake_minimum_required(VERSION 3.0)

project(joinMap)

find_package(OpenCV REQUIRED)
find_package(Pangolin REQUIRED)

add_executable(joinMap joinMap.cpp)
target_link_libraries(joinMap ${OpenCV_LIBS} ${Pangolin_LIBRARIES})