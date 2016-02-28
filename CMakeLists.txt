cmake_minimum_required(VERSION 2.8)

project(road-recg)

set(CMAKE_CXX_FLAGS_RELEASE)
find_package(OpenCV REQUIRED)
add_executable(road-recg main.cpp road-recg.cpp)
target_link_libraries(road-recg ${OpenCV_LIBS})
