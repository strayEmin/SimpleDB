#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Record {
   private:
    std::unordered_map<std::string, std::string> fields_;

   public:
    Record(std::unordered_map<std::string, std::string> &fields)
        : fields_(fields) {}

    void setField(std::string field_name, std::string value);

    std::string getValue(std::string field_name);

    std::unordered_set<std::string> getFields();

    bool matchesConditions(std::string field_name, std::string value);
};