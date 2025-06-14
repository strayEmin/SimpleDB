#include "storage/Database.hpp"

#include "nlohmann/json.hpp"  //this is a stub, and it is planned to store it in binary format in the future

using nlohmann::json;

std::string Database::getName() { return name_; }

void Database::createTable(const std::string& table_name,
                           const std::vector<Column>& columns) {
    if (tables_.contains(table_name)) {
        throw std::invalid_argument(
            "database cannot contain two tables with the same name ('" +
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

void Database::saveToFile(const std::string& filename) const {
    std::ofstream f;
    f.open(filename);
    if (not f.is_open()) {
        throw std::runtime_error("failed to create database '" + name_ +
                                 "' as file by path '" + filename + "'");
    }

    json j;
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
    f.close();
}

/*
structure of .json:
{
    "name": <name_of_database: string>,
    "tables": [
        {
            "name": <name_of_table_1: string>,
            "columns": [
                {
                    "is_primary_key": <column_1_is_PK: bool>,
                    "name": <column_1_name: string>,
                    "type": <column_1_type: string>
                },
                {
                    "is_primary_key": <column_2_is_PK: bool>,
                    "name": <column_2_name: string>,
                    "type": <column_2_type: string>
                }
            ],
            "records": [
                {
                    <record_1_field_1_name: string> :
<record_1_value_at_field_1>, <record_1_field_2_name: string> :
<record_1_value_at_field_2>
                },
                {
                    <record_2_field_1_name: string> :
<record_2_value_at_field_1>, <record_2_field_2_name: string> :
<record_2_value_at_field_2>
                }
            ]
        },
        {
            "name": <name_of_table_2: string>,
            "columns": [
                {
                    "is_primary_key": <column_1_is_PK: bool>,
                    "name": <column_1_name: string>,
                    "type": <column_1_type: string>
                },
                {
                    "is_primary_key": <column_2_is_PK: bool>,
                    "name": <column_2_name: string>,
                    "type": <column_2_type: string>
                }
            ],
            "records": [
                {
                    <record_1_field_1_name: string> :
<record_1_value_at_field_1>, <record_1_field_2_name: string> :
<record_1_value_at_field_2>
                },
                {
                    <record_2_field_1_name: string> :
<record_2_value_at_field_1>, <record_2_field_2_name: string> :
<record_2_value_at_field_2>
                }
            ]
        }
    ]
}
*/

void Database::loadFromFile(const std::string& filename) {
    std::ifstream f(filename);
    if (not f.is_open()) {
        throw std::runtime_error("error loading the database from a file '" +
                                 filename + "'");
    }

    json j = json::parse(f);
    f.close();

    try {
        name_ = j.at("name");
    } catch (std::out_of_range) {
        throw std::out_of_range("file '" + filename +
                                "' does not contain field 'name'");
    }

    for (auto& jtable : j.at("tables")) {
        std::vector<Column> columns;
        for (auto& jcolumn : jtable.at("columns")) {
            columns.push_back(Column(jcolumn.at("name"), jcolumn.at("type"),
                                     jcolumn.at("is_primary_key")));
        }

        Table table(jtable.at("name"), columns);
        for (auto& jrecord : jtable.at("records")) {
            Record record;
            for (auto& column : columns) {
                record.setField(column.getName(), jrecord.at(column.getName()));
            }
            table.insertRecord(record);
        }

        createTable(table);
    }
}