set(SOURCES
    query/Query.cpp
    query/QueryParser.cpp
    query/QueryResult.cpp
    storage/Column.cpp
    storage/Database.cpp
    storage/Index.cpp
    storage/IndexManager.cpp
    storage/Record.cpp
    storage/Table.cpp
    transaction/Transaction.cpp
    utils/Condition.cpp
    utils/Logger.cpp
    utils/TypeClassifier.cpp
    query/SelectQueryBuilder.cpp
    query/AlterTableQueryBuilder.cpp
    DatabaseEngine.cpp
)

add_library(DBLib STATIC ${SOURCES})
target_include_directories(DBLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)