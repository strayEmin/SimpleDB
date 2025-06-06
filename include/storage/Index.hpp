#pragma once

#include <string>
#include <vector>

#include "Record.hpp"

class Index {
   public:
    virtual ~Index() = default;
    virtual void insert(const std::string& key, const Record& record) = 0;
    virtual void remove(const std::string& key) = 0;
    virtual std::vector<Record> search(const std::string& key) const = 0;
};

