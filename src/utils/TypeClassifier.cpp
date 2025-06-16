#include "utils/TypeClassifier.hpp"

std::string TypeClassifier::normalizeTypeName(const std::string& type) {
    size_t bracket_pos = type.find('(');
    if (bracket_pos != std::string::npos) {
        return type.substr(0, bracket_pos);
    }
    return type;
}

std::string TypeClassifier::toUpper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

bool TypeClassifier::isNumeric(const std::string& s) {
    static const std::regex num_regex(
        R"(^[-+]?(\d+\.?\d*|\.\d+)([eE][-+]?\d+)?$)");
    return std::regex_match(s, num_regex);
}

bool TypeClassifier::isDate(const std::string& s) {
    static const std::regex date_regex(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!std::regex_match(s, date_regex)) return false;

    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return !ss.fail();
}

bool TypeClassifier::isTime(const std::string& s) {
    static const std::regex time_regex(R"(^\d{2}:\d{2}:\d{2}$)");
    if (!std::regex_match(s, time_regex)) return false;

    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%H:%M:%S");
    return !ss.fail();
}

TypeClassifier::GeneralType TypeClassifier::specificToGeneralType(
    const std::string& type) {
    std::string normalized = toUpper(normalizeTypeName(type));

    if (normalized == "INT" || normalized == "INTEGER" ||
        normalized == "BIGINT" || normalized == "SMALLINT" ||
        normalized == "DECIMAL" || normalized == "NUMERIC" ||
        normalized == "REAL" || normalized == "DOUBLE" ||
        normalized == "FLOAT") {
        return GeneralType::NUMERIC;
    }

    else if (normalized == "DATE" || normalized == "TIME" ||
             normalized == "DATETIME" || normalized == "TIMESTAMP") {
        return GeneralType::DATE_OR_TIME;
    }

    else if (normalized == "CHAR" || normalized == "VARCHAR" ||
             normalized == "TEXT" || normalized == "BLOB" ||
             normalized == "CLOB") {
        return GeneralType::TEXT;
    }

    throw std::invalid_argument("unknown SQL type: " + type);
}

TypeClassifier::GeneralType TypeClassifier::parseEnteredValueType(
    const std::string& value) {
    if (value.empty()) {
        return GeneralType::TEXT;
    }

    bool is_quoted = (value.front() == '\'' && value.back() == '\'') ||
                     (value.front() == '"' && value.back() == '"');

    if (is_quoted) {
        std::string content = value.substr(1, value.length() - 2);

        if (isDate(content) || isTime(content)) {
            return GeneralType::DATE_OR_TIME;
        }
        return GeneralType::TEXT;
    } else {
        if (isNumeric(value)) {
            return GeneralType::NUMERIC;
        }
        throw std::invalid_argument("Non-quoted value must be numeric: " +
                                    value);
    }
}
