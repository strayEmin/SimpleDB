#pragma once

#include <unordered_map>
#include <string>

class Record {
    private:
    std::unordered_map<std::string, std::string> field;

    public: 
    void setField(std::string field_name, std::string value);
    std::string getField(std::string field_name);
    bool matchesConditions(std::string field_name, std::string);
};