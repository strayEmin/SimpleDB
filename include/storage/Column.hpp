#pragma once

#include <string>

#include "utils/TypeClassifier.hpp"

class Column {
   private:
    TypeClassifier::GeneralType general_type_;
    std::string name_;
    std::string specific_type_;
    bool is_primary_key_;

   public:
    explicit Column(std::string name, std::string type, bool is_primary_key)
        : name_(name),
          specific_type_(type),
          is_primary_key_(is_primary_key),
          general_type_(TypeClassifier::specificToGeneralType(type)) {}

    std::string getName() const;

    std::string getSpecificType() const;

    bool isPK() const;

    TypeClassifier::GeneralType getGeneralType() const;
    
};