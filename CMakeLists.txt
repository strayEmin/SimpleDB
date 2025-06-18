cmake_minimum_required(VERSION 3.20)
project(SimpleDB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src/)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE DBLib)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

add_compile_definitions(PROJECT_ROOT_PATH="${PROJECT_ROOT}/")