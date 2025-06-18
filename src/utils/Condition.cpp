#include "utils/Condition.hpp"

bool Condition::evaluate(const Record &record) const {
    std::string record_value = record.getValue(field_name_);
    // if there is no value for the key, an out_of_range exception is thrown
    return operation_(record_value, value_);
}