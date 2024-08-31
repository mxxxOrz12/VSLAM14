cmake_minimum_required(VERSION 3.0)

project(ceresCurve)

find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(ceresCurve ceresCurveFitting.cpp)
target_link_libraries(ceresCurve ${CERES_LIBRARIES} ${OpenCV_LIBS})

