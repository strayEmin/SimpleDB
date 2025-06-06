#include "storage/Column.hpp"

std::string Column::getName() { return name; }

std::string Column::getType() { return type; }

bool Column::isPK() { return is_primary_key; }