#pragma once

#include <string>

class Column {
   private:
    std::string name_;
    std::string type_;
    bool is_primary_key_;

   public:
    explicit Column(std::string name, std::string type, bool is_primary_key)
        : name_(name), type_(type), is_primary_key_(is_primary_key) {}

    std::string getName() const;

    std::string getType() const;

    bool isPK() const;
};