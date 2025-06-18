#pragma once

#include <string>
#include <vector>

#include "Record.hpp"

/*
The class that is planned as part of the indexing implementation has not yet
been implemented.
*/
class Index {
   public:
    virtual ~Index() = default;
    virtual void insert(const std::string& key, const Record& record) = 0;
    virtual void remove(const std::string& key) = 0;
    virtual std::vector<Record> search(const std::string& key) const = 0;
};
