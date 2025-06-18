#include "storage/Record.hpp"

void Record::setField(const std::string &field_name, const std::string &value) {
    fields_[field_name] = value;
}

void Record::deleteField(const std::string &field_name) {
    if (not fields_.erase(field_name)) {
        throw std::invalid_argument("Field '" + field_name +
                                    "' does not exist in the record");
    }
}

void Record::setDefault(const std::string &field_name,
                        const std::string &default_value) {
    fields_[field_name] = default_value;
    // a stub that assumes further processing of the default value
}

std::string Record::getValue(const std::string &field_name) const {
    try {
        return fields_.at(field_name);
    } catch (std::out_of_range) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in the record");
    }
}

std::unordered_set<std::string> Record::getFieldNames() const {
    std::unordered_set<std::string> result{};
    for (auto &kv : fields_) {
        result.insert(kv.first);
    }

    return result;
}

std::unordered_map<std::string, std::string> Record::getFields() const {
    return fields_;
}

bool Record::matchesConditions(const std::string &field_name,
                               const std::string &value) const {
    if (not fields_.count(field_name)) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in record");
    }

    return fields_.at(field_name) == value;
}
