#include <string>
#include <unordered_map>

#include "query/Query.hpp"
#include "query/QueryResult.hpp"
#include "storage/Database.hpp"

class DatabaseEngine {
   private:
    std::unordered_map<std::string, Database> databases;
    Database current_database;

   public:
    void createDatabase(std::string db_name);
    void useDatabase(std::string);
    void dropDatabase(std::string db_name);
    QueryResult executeQuery(Query query);
};