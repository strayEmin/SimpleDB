#pragma once

#include "Query.hpp"

/*
The class that is planned to be used for processing text queries to the database
has not yet been implemented.
*/
class QueryParser {
   public:
    static Query parse(std::string query_str);
};