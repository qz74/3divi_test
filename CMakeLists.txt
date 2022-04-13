cmake_minimum_required(VERSION 3.5)

project(3divi_test1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(3divi_test1 main.cpp context.cpp lib/jsoncpp/jsoncpp.cpp)
target_include_directories(3divi_test1 PRIVATE lib/jsoncpp)
