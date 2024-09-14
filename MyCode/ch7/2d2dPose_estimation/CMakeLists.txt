cmake_minimum_required(VERSION 3.0)

project(pose2d2d)


find_package(OpenCV REQUIRED)


add_executable(pose2d2d pose_estimation_2d2d.cpp)
target_link_libraries(pose2d2d ${OpenCV_LIBS})