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

    void setField(const std::string &field_name, const std::string &value);

    void deleteField(const std::string &field_name);

    void setDefault(const std::string &field_name,
                    const std::string &default_value = "NULL");

    std::string getValue(const std::string &field_name) const;

    std::unordered_map<std::string, std::string> getFields() const;

    std::unordered_set<std::string> getFieldNames() const;

    bool matchesConditions(const std::string &field_name,
                           const std::string &value) const;
};