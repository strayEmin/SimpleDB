#include "storage/Column.hpp"

std::string Column::getName() const { return name_; }

std::string Column::getType() const { return type_; }

bool Column::isPK() const { return is_primary_key_; }