#include "query/QueryResult.hpp"

bool QueryResult::isSuccess() { return success_; }

std::string QueryResult::getMessage() { return message_; }

std::list<Record> QueryResult::getRecords() { return records_; }