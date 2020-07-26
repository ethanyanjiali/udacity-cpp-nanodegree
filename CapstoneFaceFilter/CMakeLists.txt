cmake_minimum_required(VERSION 3.11.3)

set(CMAKE_CXX_STANDARD 17)

project(FaceFilter)

file(GLOB project_SRCS src/*.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("include")

add_executable(FaceFilter ${project_SRCS})

target_link_libraries(FaceFilter ${OpenCV_LIBS})