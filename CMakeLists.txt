cmake_minimum_required(VERSION 3.20)
project(SimpleDB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src/)


set(SOURCES
    src/query/Query.cpp
    src/query/QueryParser.cpp
    src/query/QueryResult.cpp
    src/storage/Column.cpp
    src/storage/Database.cpp
    src/storage/Index.cpp
    src/storage/IndexManager.cpp
    src/storage/Record.cpp
    src/storage/Table.cpp
    src/transaction/Transaction.cpp
    src/utils/Condition.cpp
    src/utils/Logger.cpp
    src/DatabaseEngine.cpp
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE DBLib)
