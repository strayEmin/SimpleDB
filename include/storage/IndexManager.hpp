#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Index.hpp"
#include "Record.hpp"

/*
The class that is planned as part of the indexing implementation has not yet
been implemented.
*/
class IndexManager {
   private:
    std::unordered_map<std::string, std::unique_ptr<Index>> indexes_;

   public:
    void addIndex(const std::string& columnName);
    void removeIndex(const std::string& columnName);
    std::vector<Record> searchByIndex(const std::string& columnName,
                                      const std::string& value);
};