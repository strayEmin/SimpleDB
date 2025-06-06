#pragma once

#include <stdexcept>
#include <string>

#include "../storage/Record.hpp"

class Condition {
   private:
    std::string field_name;
    bool (*operation)(std::string, std::string);
    std::string value;

   public:
    Condition(std::string field_name,
              bool (*operation)(std::string, std::string), std::string value)
        : field_name(field_name), operation(operation), value(value) {}

    bool evaluate(Record& record);
};