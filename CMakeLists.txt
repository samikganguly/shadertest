cmake_minimum_required(VERSION 3.10)

project(shadertest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(INC include)
set(SRC src)
set(BUILD build)

add_executable(shadertest WIN32
	${SRC}/main.cpp)

target_include_directories(shadertest PUBLIC 
	${INC})