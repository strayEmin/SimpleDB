#pragma once

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "query/Query.hpp"
#include "query/QueryResult.hpp"
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

    void setDBDir(const std::string& new_db_dir);

    void createDatabase(const std::string& db_name);

    void useDatabase(const std::string& db_name);

    void dropDatabase(const std::string& db_name);

    QueryResult executeQuery(const Query& query);
};