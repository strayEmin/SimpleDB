#include "storage/Record.hpp"

void Record::setField(std::string field_name, std::string value) {
    field[field_name] = value;
}

std::string Record::getField(std::string field_name) {
    if (not field.count(field_name)) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in record");
    }
    return field[field_name];
}

bool Record::matchesConditions(std::string field_name, std::string value) {
    if (not field.count(field_name)) {
        throw std::out_of_range("Field '" + field_name +
                                "' does not exist in record");
    }
    return field[field_name] == value;
}
