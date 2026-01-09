cmake_minimum_required(VERSION 3.10)
SET( CMAKE_CXX_STANDARD 17 )

project( learnopencv )
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS}  include/)
add_library(shapeDetectionLib src/shape_detection.cpp)


add_executable( program main.cpp )
target_link_libraries( program ${OpenCV_LIBS} shapeDetectionLib)