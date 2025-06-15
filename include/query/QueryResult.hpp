#pragma once

#include <list>
#include <string>
#include <vector>

#include "storage/Record.hpp"

class QueryResult {
   private:
    bool success_;
    std::list<Record> records_;
    std::string message_;

   public:
    bool isSuccess();

    std::string getMessage();

    std::list<Record> getRecords();
};
