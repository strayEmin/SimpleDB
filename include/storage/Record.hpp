#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

class Record {
   private:
    std::unordered_map<std::string, std::string> field;

   public:
    Record(std::unordered_map<std::string, std::string> &field)
        : field(field) {}

    void setField(std::string field_name, std::string value);

    std::string getField(std::string field_name);

    bool matchesConditions(std::string field_name, std::string value);
};