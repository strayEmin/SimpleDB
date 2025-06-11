#include "storage/Record.hpp"

void Record::setField(std::string field_name, std::string value) {
    fields_[field_name] = value;
}

void Record::deleteField(std::string field_name) {
    if (not fields_.erase(field_name)) {
        throw std::invalid_argument("Field '" + field_name +
                                    "' does not exist in the record");
    }
}

void Record::setDefault(std::string field_name, std::string default_value) {
    fields_[field_name] = default_value;
    // a stub that assumes further processing of the default value
}

std::string Record::getValue(std::string field_name) const {
    try {
        return fields_.at(field_name);
    } catch (std::out_of_range) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in the record");
    }
}

std::unordered_set<std::string> Record::getFields() const {
    std::unordered_set<std::string> result;
    for (auto &kv : fields_) {
        result.insert(kv.first);
    }

    return result;
}

bool Record::matchesConditions(std::string field_name,
                               std::string value) const {
    if (not fields_.count(field_name)) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in record");
    }
    return fields_.at(field_name) == value;
}
