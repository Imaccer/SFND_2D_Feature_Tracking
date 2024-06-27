cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(camera_fusion)

# Set C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV headers and link libraries
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/usr/include/opencv4")

link_directories(${OpenCV_LIBRARY_DIRS})

# Executable for create matrix exercise
add_executable(2D_feature_tracking src/matching2D_Student.cpp src/MidTermProject_Camera_Student.cpp)
target_link_libraries(2D_feature_tracking ${OpenCV_LIBS})

# cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# add_definitions(-std=c++11)

# set(CXX_FLAGS "-Wall")
# set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

# project(camera_fusion)

# find_package(OpenCV 4.1 REQUIRED)

# include_directories(${OpenCV_INCLUDE_DIRS})
# link_directories(${OpenCV_LIBRARY_DIRS})
# add_definitions(${OpenCV_DEFINITIONS})

# # Executable for create matrix exercise
# add_executable (2D_feature_tracking src/matching2D_Student.cpp src/MidTermProject_Camera_Student.cpp)
# target_link_libraries (2D_feature_tracking ${OpenCV_LIBRARIES})