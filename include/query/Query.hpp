#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../utils/Condition.hpp"

/*
The class that is planned to be used for processing text queries to the database
has not yet been implemented.
*/
class Query {
   private:
    enum Type { SELECT, INSERT, UPDATE, DELETE, CREATE_TABLE, DROP_TABLE };

    Type type;
    std::string table_name;
    std::vector<std::string> columns;
    std::vector<Condition> conditions;

   public:
    Type getType();
    std::string getTableName();
    std::vector<Condition> getConditions();
};

void PrintOK();