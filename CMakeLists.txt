cmake_minimum_required(VERSION 2.8)

project(road_line)

set(CMAKE_CXX_FLAGS_RELEASE)
find_package(OpenCV REQUIRED)
add_executable(
 road_line projective_tf.cpp 
 DetectObject.cpp
 main.cpp
)
target_link_libraries(road_line ${OpenCV_LIBS})
