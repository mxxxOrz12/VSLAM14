cmake_minimum_required(VERSION 3.0)

project(3dVision)

find_package(OpenCV REQUIRED)
find_package(Pangolin REQUIRED)

add_executable(3dVision 3dVision.cpp)
target_link_libraries(3dVision ${OpenCV_LIBS} ${Pangolin_LIBRARIES})