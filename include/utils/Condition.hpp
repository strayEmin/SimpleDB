#pragma once

#include <stdexcept>
#include <string>

#include "../storage/Record.hpp"

class Condition {
   private:
    std::string field_name_;
    bool (*operation_)(std::string, std::string);
    std::string value_;

   public:
    Condition(std::string field_name,
              bool (*operation)(std::string, std::string), std::string value)
        : field_name_(field_name), operation_(operation), value_(value) {}

    bool evaluate(const Record& record) const;
};