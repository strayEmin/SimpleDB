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

class AlterTableQueryBuilder {
   private:
    std::shared_ptr<Table> table_ptr_;

   public:
    explicit AlterTableQueryBuilder(std::shared_ptr<Table> table_ptr)
        : table_ptr_(table_ptr) {}

    void drop(const std::string& column_name);

    void add(const Column& column);
};