#pragma once

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Column.hpp"
#include "Record.hpp"
#include "Table.hpp"

class Database {
   private:
    std::string name_;
    std::unordered_map<std::string, std::shared_ptr<Table>> tables_;

   public:
    explicit Database(const std::string& name) : name_(name) {}

    // explicit Database(
    //     const std::string& name,
    //     const std::unordered_map<std::string, const std::shared_ptr<Table>>&
    //         tables);

    // explicit Database(const std::string& name,
    //                   const std::vector<std::shared_ptr<Table>>& tables);

    std::string getName();

    void createTable(const std::string& table_name,
                     const std::vector<Column>& columns);

    void createTable(const Table& table);

    void dropTable(const std::string& table_name);

    std::shared_ptr<Table> getTable(const std::string& table_name) const;

    std::vector<std::shared_ptr<const Table>> getTables() const;

    void saveToFile(const std::string& filename) const;

    void loadFromFile(const std::string& filename);
};