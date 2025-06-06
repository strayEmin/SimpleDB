#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Record.hpp"
#include "Index.hpp"


class IndexManager {
   private:
    std::unordered_map<std::string, std::unique_ptr<Index>> indexes_;

   public:
    void addIndex(const std::string& columnName);
    void removeIndex(const std::string& columnName);
    std::vector<Record> searchByIndex(const std::string& columnName,
                                      const std::string& value);
};