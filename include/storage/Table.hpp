#pragma once

#include <string>
#include <vector>

#include "Column.hpp"
#include "Record.hpp"

class Table {
   private:
    std::string name;
    std::vector<Column> columns;
    std::vector<Record> records;

   public:
    void insertRecord(Record record);
    void deleteRecord(std::string key);
    Record findRecord(std::string key);
    void addColumn(Column column);
    void dropColumn(std::string column_name);
};