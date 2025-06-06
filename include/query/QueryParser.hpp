#pragma once

#include "Query.hpp"

class QueryParser {
   public:
    static Query parse(std::string query_str);
};