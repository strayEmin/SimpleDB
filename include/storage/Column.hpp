#pragma once

#include <string>

class Column {
   private:
    std::string name;
    std::string type;
    bool is_primary_key;

   public:
    explicit Column(std::string name, std::string type, bool is_primary_key)
        : name(name), type(type), is_primary_key(is_primary_key) {}

    std::string getName();

    std::string getType();

    bool isPK();
};