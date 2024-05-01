cmake_minimum_required(VERSION 3.8)
project(Watermeter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} watermeter.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})