#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "../utils/Condition.hpp"

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