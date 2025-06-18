#pragma once

#include <list>
#include <string>
#include <vector>

#include "storage/Record.hpp"
#include "utils/Logger.hpp"

/*
The class that is planned to be used for processing text queries to the database
has not yet been implemented.
*/
class QueryResult {
   private:
    bool success_;
    std::list<Record> records_;
    std::string message_;

   public:
    QueryResult(bool success, std::string message,
                std::list<Record> records = {})
        : success_(success), message_(message), records_(records) {}

    bool isSuccess();

    std::string getMessage();

    std::list<Record> getRecords();
};
