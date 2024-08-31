cmake_minimum_required(VERSION 3.10)
project(imageBasics)

find_package(OpenCV REQUIRED)

add_executable(undistoredImage undistoredImage.cpp)
add_executable(imageBasics imageBasics.cpp)
target_link_libraries(imageBasics ${OpenCV_LIBS})
target_link_libraries(undistoredImage ${OpenCV_LIBS})