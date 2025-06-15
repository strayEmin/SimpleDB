#include "query/SelectQueryBuilder.hpp"

SelectQueryBuilder::SelectQueryBuilder(
    const Table& table, const std::vector<std::string>& column_names)
    : table_(table), column_names_(column_names) {
    const auto& records = table.getRecords();
    for (auto& record : records) {
        Record subrecord;
        for (auto& cn : column_names) {
            subrecord.setField(cn, record.getValue(cn));
        }

        records_.push_back(subrecord);
    }
}

SelectQueryBuilder& SelectQueryBuilder::where(
    const std::vector<Condition>& conditions) {
    for (auto it = records_.begin(); it != records_.end(); ++it) {
        for (auto& condition : conditions) {
            if (not condition.evaluate(*it)) {
                records_.erase(it);
                break;
            }
        }
    }

    return *this;
}