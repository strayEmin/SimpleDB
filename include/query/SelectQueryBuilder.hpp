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
#include "storage/Database.hpp"
#include "utils/Condition.hpp"

class SelectQueryBuilder {
   private:
    std::list<Record> records_;
    const Table& table_;
    const std::vector<std::string>& column_names_;

   public:
    explicit SelectQueryBuilder(const Table& table,
                                const std::vector<std::string>& column_names);

    SelectQueryBuilder& where(const std::vector<Condition>& conditions);

    SelectQueryBuilder& distinct();

    SelectQueryBuilder& limit(int limit_hight, int limit_low = 0);

    std::list<Record> getRecords();
};