#include "utils/Condition.hpp"

bool Condition::evaluate(Record &record) {
    std::string record_value = record.getValue(field_name);
    // if there is no value for the key, an out_of_range exception is thrown
    return operation(record_value, value);
}