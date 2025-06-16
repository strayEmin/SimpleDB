#pragma once

#include <filesystem>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "query/Query.hpp"
#include "query/QueryResult.hpp"
#include "query/SelectQueryBuilder.hpp"
#include "storage/Database.hpp"

#define DB_FILE_EXTENSION ".json"

class DatabaseEngine {
   private:
    // key - database name, value - absolute path to database
    std::unordered_map<std::string, std::string> database_paths_;
    std::shared_ptr<Database> current_database_sptr_;
    std::string db_dir_;

   public:
    DatabaseEngine(const std::string& path_to_db_dir);

    ~DatabaseEngine();

    void saveChangesCurDB() const;

    void setDBDir(const std::string& new_db_dir);

    void createDatabase(const std::string& db_name, bool if_not_exist = false);

    void useDatabase(const std::string& db_name);

    void dropDatabase(const std::string& db_name, bool if_exist = false);

    SelectQueryBuilder selectFrom(
        const std::string& table_name,
        const std::vector<std::string>& column_names) const;
    // to get the result, you need to use
    // the SelectQueryBuilder class methods

    void insertInto(const std::string& table_name,
                    const std::vector<std::string>& column_names,
                    const std::vector<std::string>& values);

    std::string max(const std::string& table_name,
                    const std::string& column_name,
                    std::string (*max_s)(std::string, std::string)) const;

    void update(std::string table_name,
                std::unordered_map<std::string, std::string> fieldvalues,
                std::vector<Condition> conditions = {});

    void createTable(const Table& table, bool if_not_exist = false);

    void createTable(std::string table_name, std::vector<Column> columns,
                     bool if_not_exist = false);

    std::vector<Column> getColumns(const std::string& table_name) const;
};