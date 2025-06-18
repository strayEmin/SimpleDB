#pragma once

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

class TypeClassifier {
   private:
    // "VARCHAR(255)" -> "VARCHAR"
    static std::string normalizeTypeName(const std::string& type);

    static std::string toUpper(const std::string& s);

    // YYYY-MM-DD
    static bool isDate(const std::string& s);

    // HH:MM:SS
    static bool isTime(const std::string& s);

    static bool isNumeric(const std::string& s);

   public:
    enum class GeneralType { TEXT, NUMERIC, DATE_OR_TIME };

    static GeneralType specificToGeneralType(const std::string& type);

    static GeneralType parseEnteredValueType(const std::string& value);
};