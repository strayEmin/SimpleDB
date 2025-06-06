#pragma once

#include <string>

class Column {
   private:
    std::string name;
    std::string type;
    bool is_primary_key;

   public:
    std::string getName();
    std::string getType();
    bool isPK();
};