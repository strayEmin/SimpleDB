#pragma once

#include <vector>

#include "../storage/Record.hpp"
#include "string"

class QueryResult {
   private:
    bool success;
    std::vector<Record> records;
    std::string message;

   public:
    bool isSuccess();
    std::string getMessage();
    std::vector<Record> getRecords();
};
