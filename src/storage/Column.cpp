#include "storage/Column.hpp"

std::string Column::getName() const { return name_; }

std::string Column::getSpecificType() const { return specific_type_; }

bool Column::isPK() const { return is_primary_key_; }

TypeClassifier::GeneralType Column::getGeneralType() const {
    return general_type_;
}