#include <string>
#include <vector>

#include "Column.hpp"
#include "Table.hpp"

class Database {
   private:
    std::string name;
    std::vector<Table> tables;

   public:
    void createTable(std::string table_name, std::vector<Column> columns);
    void dropTable(std::string table_name);
    Table getTable(std::string table_name);
    void saveToFile(std::string filename);
    void loadFromFile(std::string filename);
};