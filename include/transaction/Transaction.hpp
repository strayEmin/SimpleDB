#pragma once

#include <memory>
#include <vector>

#include "../query/Query.hpp"

class Transaction {
   public:
    void addQuery(std::unique_ptr<Query> query);
    bool commit();
    void rollback();

   private:
    std::vector<std::unique_ptr<Query>> queries_;
};