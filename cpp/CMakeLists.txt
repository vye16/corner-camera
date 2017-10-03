cmake_minimum_required(VERSION 3.1)
project( corner-cam )

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV 3 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Find the flycapture2 library from Point Grey
#include_directories("/usr/include/flycapture")
#find_library(FLYCAPTURE flycapture)

subdirs( src demo tests scratch )
