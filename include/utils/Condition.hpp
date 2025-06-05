#include <string>

#include "../storage/Record.hpp"

class Condition {
   private:
    std::string field_name;
    std::string op;
    std::string value;

   public:
    void evaluate(Record record);
};