#include "storage/Record.hpp"

void Record::setField(std::string field_name, std::string value) {
    fields_[field_name] = value;
}

std::string Record::getValue(std::string field_name) {
    if (not fields_.count(field_name)) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in record");
    }
    return fields_[field_name];
}

std::unordered_set<std::string> Record::getFields() {
    std::unordered_set<std::string> result;
    for (auto &kv : fields_) {
        result.insert(kv.first);
    }

    return result;
}

bool Record::matchesConditions(std::string field_name, std::string value) {
    if (not fields_.count(field_name)) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in record");
    }
    return fields_[field_name] == value;
}
