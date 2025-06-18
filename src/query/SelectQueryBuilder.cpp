#include "query/SelectQueryBuilder.hpp"

SelectQueryBuilder::SelectQueryBuilder(
    const Table& table, const std::vector<std::string>& column_names) {
    const auto& records = table.getRecords();

    if (column_names.size() == 1 and column_names[0] == "*") {
        column_names_ = table.getColumnNames();

        records_ = records;
    } else {
        column_names_ = column_names;
        for (auto& record : records) {
            Record subrecord{};
            for (auto& cn : column_names) {
                subrecord.setField(cn, record.getValue(cn));
            }

            records_.push_back(subrecord);
        }
    }
}

SelectQueryBuilder& SelectQueryBuilder::where(
    const std::vector<Condition>& conditions) {
    for (auto it = records_.begin(); it != records_.end();) {
        bool is_deleted{false};
        for (auto& condition : conditions) {
            if (not condition.evaluate(*it)) {
                it = records_.erase(it);
                is_deleted = true;
                break;
            }
        }
        if (not is_deleted) {
            ++it;
        }
    }

    return *this;
}

SelectQueryBuilder& SelectQueryBuilder::distinct() {
    std::unordered_set<std::string> unique_elems{};
    for (auto it = records_.begin(); it != records_.end();) {
        char delimeter = '\x1E';
        std::string record_in_str{};
        for (auto& cn : column_names_) {
            record_in_str += it->getValue(cn) + delimeter;
        }

        if (unique_elems.contains(record_in_str)) {
            it = records_.erase(it);
        } else {
            unique_elems.insert(record_in_str);
            ++it;
        }
    }

    return *this;
}

SelectQueryBuilder& SelectQueryBuilder::limit(int limit, int offset) {
    int cur_i{0};
    for (auto it = records_.begin(); it != records_.end();) {
        if (cur_i >= limit + offset or cur_i < offset) {
            it = records_.erase(it);
        } else {
            ++it;
        }

        ++cur_i;
    }

    return *this;
}

std::list<Record> SelectQueryBuilder::getRecords() { return records_; }