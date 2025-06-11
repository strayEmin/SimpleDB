#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "Column.hpp"
#include "Record.hpp"

class Table {
   private:
    std::string name_;
    std::vector<Column> columns_;
    std::vector<Record> records_;

    std::unordered_set<std::string> column_names_;
    bool hasPK_;

   public:
    Table(std::string name, std::vector<Column> columns);

    std::string getPrimaryKeyColumnName() const;

    void insertRecord(const Record record);

    void deleteRecord(std::string key);

    Record findRecord(std::string key) const;

    void addColumn(Column column);

    void dropColumn(std::string column_name);
};