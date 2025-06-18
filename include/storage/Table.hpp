#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "Column.hpp"
#include "Record.hpp"
#include "utils/Condition.hpp"

class Table {
   private:
    std::string name_;
    std::vector<std::string> column_names_;
    std::list<Record> records_;
    std::unordered_map<std::string, Column> columns_;
    bool hasPK_ = false;

   public:
    Table(std::string name, std::vector<Column> columns);

    std::string getName() const;

    void changeName(const std::string& new_name);

    std::vector<Column> getColumns() const;

    std::vector<std::string> getColumnNames() const;

    std::list<Record> getRecords() const;

    std::string getPrimaryKeyColumnName() const;

    void insertRecord(const Record& record);

    void deleteRecord(const std::string& key);

    Record findRecord(const std::string& key) const;

    void addColumn(const Column& column);

    void dropColumn(const std::string& column_name);

    std::list<std::string> getValuesInCol(const std::string column_name);

    void deleteRecords(const std::vector<Condition>& conditions = {});

    void updateRecords(
        const std::unordered_map<std::string, std::string>& fieldvalues,
        std::vector<Condition> conditions = {});
};