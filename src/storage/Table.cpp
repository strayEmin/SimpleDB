#include "storage/Table.hpp"

std::string Table::getPrimaryKeyColumnName() const {
    if (not hasPK_) {
        throw std::runtime_error("Primary key column not found");
    }

    for (const auto &column : columns_) {
        if (column.isPK()) {
            return column.getName();
        }
    }

    throw std::logic_error(
        "unexpected error -- the hasPK flag was not set to the correct "
        "position");
}

Table::Table(std::string name, std::vector<Column> columns) : name_(name) {
    try {
        for (auto &column : columns) {
            addColumn(column);
        }
    } catch (std::invalid_argument) {
        std::invalid_argument(
            "Table cannot contain two columns with the same name");
    }
}

void Table::insertRecord(const Record record) {
    std::unordered_set<std::string> field_names = record.getFields();

    for (auto &fn : field_names) {
        if (not column_names_.contains(fn)) {
            throw std::out_of_range("Column '" + fn + "' does not exist");
        }
    }

    Record final_record;
    for (auto &cn : column_names_) {
        if (field_names.contains(cn)) {
            final_record.setField(cn, record.getValue(cn));
        } else {
            final_record.setDefault(cn);
        }
    }

    records_.push_back(final_record);
}

void Table::deleteRecord(std::string key) {
    std::string pk_column = getPrimaryKeyColumnName();
    for (auto it = records_.begin(); it != records_.end(); ++it) {
        if (it->getValue(pk_column) == key) {
            records_.erase(it);
            return;
        }
    }
    throw std::runtime_error("Record with key '" + key + "' not found");
}

Record Table::findRecord(std::string key) const {
    std::string pk_column = getPrimaryKeyColumnName();
    for (auto &record : records_) {
        if (record.getValue(pk_column) == key) {
            return record;
        }
    }
    throw std::runtime_error("Record with key '" + key + "' not found");
}

void Table::addColumn(Column column) {
    if (column_names_.contains(column.getName())) {
        throw std::invalid_argument("Column '" + column.getName() +
                                    "' already exists in the table");
    }

    if (column.isPK()) {
        if (hasPK_) {
            throw std::invalid_argument(
                "Table cannot contain two primary key column");
        } else {
            hasPK_ = true;
        }
    }

    columns_.push_back(column);
    column_names_.insert(column.getName());

    for (auto &record : records_) {
        record.setDefault(column.getName());
    }
}

void Table::dropColumn(std::string column_name) {
    if (not column_names_.contains(column_name)) {
        throw std::invalid_argument("Column '" + column_name +
                                    "does not exist");
    }

    for (auto it = columns_.begin(); it != columns_.end(); ++it) {
        if (it->getName() == column_name) {
            columns_.erase(it);
        }
    }

    for (auto &record : records_) {
        record.deleteField(column_name);
    }
}