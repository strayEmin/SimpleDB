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
    Record() : fields_() {}

    Record(std::unordered_map<std::string, std::string> &fields)
        : fields_(fields) {}

    void setField(std::string field_name, std::string value);

    void deleteField(std::string field_name);

    void setDefault(std::string field_name, std::string default_value = "NULL");

    std::string getValue(std::string field_name) const;

    std::unordered_set<std::string> getFields() const;

    bool matchesConditions(std::string field_name, std::string value) const;
};