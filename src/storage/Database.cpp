#include "storage/Database.hpp"

#include "nlohmann/json.hpp"  //this is a stub, and it is planned to store it in binary format in the future

using nlohmann::json;

void Database::createTable(const std::string& table_name,
                           const std::vector<Column>& columns) {
    if (tables_.contains(table_name)) {
        throw std::invalid_argument(
            "Database cannot contain two tables with the same name ('" +
            table_name + "')");
    }

    tables_[table_name] =
        std::shared_ptr<Table>(new Table(table_name, columns));
}

void Database::createTable(const Table& table) {
    tables_[table.getName()] = std::shared_ptr<Table>(new Table(table));
}

void Database::dropTable(const std::string& table_name) {
    if (not tables_.contains(table_name)) {
        throw std::out_of_range("Database does not contain table '" +
                                table_name + "'");
    }

    tables_.erase(table_name);
}

std::shared_ptr<const Table> Database::getTable(
    const std::string& table_name) const {
    try {
        return tables_.at(table_name);
    } catch (std::out_of_range) {
        throw std::out_of_range("Database does not contain table '" +
                                table_name + "'");
    }
}

// implemented in such a way as to avoid unnecessary copying
std::vector<std::shared_ptr<const Table>> Database::getTables() const {
    std::vector<std::shared_ptr<const Table>> result;
    for (auto& kv : tables_) {
        result.push_back(kv.second);
    }

    return result;
}

void Database::saveToFile(const std::string& filename) {
    json j;
    std::ofstream f;
    f.open(filename);

    j["name"] = name_;
    auto tables = getTables();
    j["tables"] = {};
    for (auto& table : tables) {
        json jtable;

        jtable["name"] = table->getName();
        std::vector<Column> columns = table->getColumns();
        jtable["columns"] = {};
        for (auto& column : columns) {
            jtable["columns"].push_back({{"name", column.getName()},
                                         {"type", column.getType()},
                                         {"is_primary_key", column.isPK()}});
        }

        jtable["records"] = {};
        std::vector<Record> records = table->getRecords();
        for (auto& record : records) {
            json fields(record.getFields());
            jtable["records"].push_back(fields);
        }

        j["tables"].push_back(jtable);
    }

    f << j.dump(4);
}
