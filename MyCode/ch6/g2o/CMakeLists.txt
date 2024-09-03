#G2O
list(APPEND CMAKE_MODULE_PATH /usr/local/include/g2o/cmake_modules) 
#set(G2O_ROOT "/usr/local/g2o2020/")
set(CMAKE_PREFIX_PATH "/usr/local/include/g2o/")

find_package(OpenCV REQUIRED)

find_package(G2O REQUIRED)
include_directories(${G2O_INCLUDE_DIRS})
# #这样写导致从默认/usr/local/include/g2o/找到其他版本g2o，导致冲突
# include_directories(/usr/local/g2o2020/include/)
 
SET(G2O_LIBRARIES ${G2O_STUFF_LIBRARY} ${G2O_CORE_LIBRARY} ${G2O_CLI_LIBRARY} ${G2O_SOLVER_CHOLMOD} ${G2O_SOLVER_CSPARSE} ${G2O_SOLVER_CSPARSE_EXTENSION} 
${G2O_SOLVER_DENSE} ${G2O_SOLVER_PCG} ${G2O_SOLVER_SLAM2D_LINEAR} ${G2O_SOLVER_STRUCTURE_ONLY} ${G2O_SOLVER_EIGEN} ${G2O_TYPES_DATA} ${G2O_TYPES_ICP} ${G2O_TYPES_SBA}
${G2O_TYPES_SCLAM2D} ${G2O_TYPES_SIM3} ${G2O_TYPES_SLAM2D} ${G2O_TYPES_SLAM3D})
 
add_executable(g2oCurveFitting g2oCurveFitting.cpp)
target_link_libraries(g2oCurveFitting ${OpenCV_LIBS}  ${G2O_LIBRARIES} )