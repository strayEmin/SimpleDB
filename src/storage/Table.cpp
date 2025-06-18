#include "storage/Table.hpp"

bool isAcceptableType(TypeClassifier::GeneralType col_type,
                      TypeClassifier::GeneralType val_type) {
    using Type = TypeClassifier::GeneralType;
    return col_type == Type::DATE_OR_TIME and val_type == Type::DATE_OR_TIME or
           col_type == Type::TEXT and
               (val_type == Type::TEXT or val_type == Type::DATE_OR_TIME) or
           col_type == Type::NUMERIC and val_type == Type::NUMERIC;
}

std::string Table::getName() const { return name_; }

void Table::changeName(const std::string &new_name) { name_ = new_name; }

std::vector<Column> Table::getColumns() const {
    std::vector<Column> result{};
    for (auto &column_name : column_names_) {
        result.push_back(columns_.at(column_name));
    }

    return result;
}

std::list<Record> Table::getRecords() const { return records_; }

std::string Table::getPrimaryKeyColumnName() const {
    if (not hasPK_) {
        throw std::runtime_error("Primary key column not found");
    }

    for (const auto &column : columns_) {
        if (column.second.isPK()) {
            return column.second.getName();
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

void Table::insertRecord(const Record &record) {
    std::unordered_set<std::string> field_names = record.getFieldNames();

    for (auto &fn : field_names) {
        if (not columns_.contains(fn)) {
            throw std::out_of_range("Column '" + fn + "' does not exist");
        }
        if (columns_.at(fn).isPK()) {
            for (auto &r : records_) {
                if (r.getValue(fn) == record.getValue(fn)) {
                    throw std::invalid_argument(
                        "such an entry already exists in the " + fn +
                        " field, which is the primary key ('" +
                        record.getValue(fn) + "')");
                }
            }
        }
    }

    Record final_record{};

    for (auto &cn : column_names_) {
        if (field_names.contains(cn)) {
            using Type = TypeClassifier::GeneralType;

            Type col_type = columns_.at(cn).getGeneralType();
            Type val_type =
                TypeClassifier::parseEnteredValueType(record.getValue(cn));

            if (not isAcceptableType(col_type, val_type)) {
                throw std::invalid_argument(
                    "incorrect type of the entered value (" +
                    record.getValue(cn) + ")");
            }
            final_record.setField(cn, record.getValue(cn));
        } else {
            final_record.setDefault(cn);
        }
    }

    records_.push_back(final_record);
}

void Table::deleteRecord(const std::string &key) {
    std::string pk_column = getPrimaryKeyColumnName();
    for (auto it = records_.begin(); it != records_.end(); ++it) {
        if (it->getValue(pk_column) == key) {
            it = records_.erase(it);
            return;
        }
    }

    throw std::runtime_error("Record with key '" + key + "' not found");
}

Record Table::findRecord(const std::string &key) const {
    std::string pk_column = getPrimaryKeyColumnName();
    for (auto &record : records_) {
        if (record.getValue(pk_column) == key) {
            return record;
        }
    }

    throw std::runtime_error("Record with key '" + key + "' not found");
}

void Table::addColumn(const Column &column) {
    if (columns_.contains(column.getName())) {
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

    columns_.emplace(column.getName(), column);
    column_names_.push_back(column.getName());

    for (auto &record : records_) {
        record.setDefault(column.getName());
    }
}

void Table::dropColumn(const std::string &column_name) {
    if (not columns_.contains(column_name)) {
        throw std::invalid_argument("Column '" + column_name +
                                    "does not exist");
    }

    for (auto it = column_names_.begin(); it != column_names_.end();) {
        if (*it == column_name) {
            it = column_names_.erase(it);
        } else {
            ++it;
        }
    }

    columns_.erase(column_name);

    for (auto &record : records_) {
        record.deleteField(column_name);
    }
}

std::list<std::string> Table::getValuesInCol(const std::string column_name) {
    std::list<std::string> result{};
    for (auto &record : records_) {
        result.push_back(record.getValue(column_name));
    }

    return result;
}

std::vector<std::string> Table::getColumnNames() const {
    std::vector<std::string> result{};
    for (auto &cn : column_names_) {
        result.push_back(cn);
    }

    return result;
}

void Table::deleteRecords(const std::vector<Condition> &conditions) {
    if (conditions.empty()) {
        records_.clear();
    } else {
        for (auto it = records_.begin(); it != records_.end();) {
            bool is_deleted{false};
            for (auto &condition : conditions) {
                if (condition.evaluate(*it)) {
                    it = records_.erase(it);
                    is_deleted = true;
                    break;
                }
            }

            if (not is_deleted) {
                ++it;
            }
        }
    }
}

void Table::updateRecords(
    const std::unordered_map<std::string, std::string> &fieldvalues,
    std::vector<Condition> conditions) {
        if (fieldvalues.empty()) {
        throw std::invalid_argument("'fieldvalue' is empty, nothing to update");
    }

    if (conditions.empty()) {
        auto first_pair = *fieldvalues.begin();
        conditions.push_back(Condition(
            first_pair.first, [](std::string, std::string) { return true; },
            first_pair.second));
    }

    for (auto it = records_.begin(); it != records_.end(); ++it) {
        for (auto &condition : conditions) {
            if (condition.evaluate(*it)) {
                for (auto &fv : fieldvalues) {
                    using Type = TypeClassifier::GeneralType;

                    Type col_type = columns_.at(fv.first).getGeneralType();
                    Type val_type =
                        TypeClassifier::parseEnteredValueType(fv.second);

                    if (isAcceptableType(col_type, val_type)) {
                        it->setField(fv.first, fv.second);
                    }
                }
            }
        }
    }
}
