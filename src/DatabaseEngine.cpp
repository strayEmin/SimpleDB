#include "DatabaseEngine.hpp"

DatabaseEngine::DatabaseEngine(const std::string& path_to_db_dir) {
    current_database_sptr_ = nullptr;
    setDBDir(path_to_db_dir);
    try {
        for (auto& entry : std::filesystem::directory_iterator(db_dir_)) {
            if (entry.is_regular_file() and
                entry.path().extension() == DB_FILE_EXTENSION) {
                std::string db_name = entry.path().stem().string();
                database_paths_[db_name] = entry.path().string();
            }
        }
    } catch (std::filesystem::filesystem_error) {
        throw std::runtime_error("error working with files at dir '" + db_dir_ +
                                 "'");
    }
}

void DatabaseEngine::saveChangesCurDB() const {
    current_database_sptr_->saveToFile(
        database_paths_.at(current_database_sptr_->getName()));
}

DatabaseEngine::~DatabaseEngine() { saveChangesCurDB(); }

void DatabaseEngine::setDBDir(const std::string& new_db_dir) {
    db_dir_ = new_db_dir;
    if (*new_db_dir.rbegin() != '/') {
        db_dir_ += '/';
    }
    std::filesystem::create_directory(db_dir_);
}

void DatabaseEngine::createDatabase(const std::string& db_name,
                                    bool if_not_exist) {
    if (database_paths_.contains(db_name)) {
        if (not if_not_exist) {
            throw std::invalid_argument("this database name already exists('" +
                                        db_name + "')");
        } else {
            return;
        }
    }

    Database db(db_name);
    std::string db_path = db_dir_ + db_name + DB_FILE_EXTENSION;
    db.saveToFile(db_path);
    database_paths_[db_name] = db_path;
}

void DatabaseEngine::useDatabase(const std::string& db_name) {
    try {
        if (current_database_sptr_ != nullptr) {
            saveChangesCurDB();
        }
        std::shared_ptr<Database> db_sptr(new Database(db_name));
        db_sptr->loadFromFile(database_paths_.at(db_name));
        current_database_sptr_ = db_sptr;
    } catch (std::out_of_range) {
        throw std::out_of_range("database named '" + db_name +
                                "' does not exist, cannot be used");
    }
}

void DatabaseEngine::dropDatabase(const std::string& db_name, bool if_exist) {
    if (not database_paths_.contains(db_name)) {
        if (not if_exist) {
            throw std::out_of_range("database named '" + db_name +
                                    "' does not exist, cannot be drop");
        } else {
            return;
        }
    }

    if (current_database_sptr_ != nullptr and
        current_database_sptr_->getName() == db_name) {
        current_database_sptr_ = nullptr;
    }

    try {
        std::filesystem::remove(database_paths_.at(db_name));

    } catch (std::filesystem::filesystem_error) {
        std::runtime_error("file by path '" + database_paths_.at(db_name) +
                           "' can not be deleted");
    }
    database_paths_.erase(db_name);
}

SelectQueryBuilder DatabaseEngine::selectFrom(
    const std::string& table_name,
    const std::vector<std::string>& column_names) const {
    return SelectQueryBuilder(*current_database_sptr_->getTable(table_name),
                              column_names);
}

void DatabaseEngine::insertInto(const std::string& table_name,
                                const std::vector<std::string>& column_names,
                                const std::vector<std::string>& values) {
    if (column_names.size() != values.size()) {
        throw std::invalid_argument(
            "InsertInto(): there should be as many values as there are fields");
    }

    Record record;
    for (int i = 0; i < column_names.size(); ++i) {
        record.setField(column_names[i], values[i]);
    }
    current_database_sptr_->getTable(table_name)->insertRecord(record);
    saveChangesCurDB();
}

void DatabaseEngine::createTable(const Table& table, bool if_not_exist) {
    try {
        current_database_sptr_->createTable(table);
        saveChangesCurDB();
    } catch (const std::invalid_argument& e) {
        if (not if_not_exist) {
            throw std::invalid_argument(e.what());
        }
    }
}

void DatabaseEngine::createTable(std::string table_name,
                                 std::vector<Column> columns,
                                 bool if_not_exist) {
    try {
        current_database_sptr_->createTable(table_name, columns);
        saveChangesCurDB();
    } catch (const std::invalid_argument& e) {
        if (not if_not_exist) {
            throw std::invalid_argument(e.what());
        }
    }
}

std::string DatabaseEngine::max(const std::string& table_name,
                                const std::string& column_name,
                                std::string (*max_s)(std::string,
                                                     std::string)) {
    const auto& table = current_database_sptr_->getTable(table_name);
    std::string max_value;
    std::list<std::string> values = table->getValuesInCol(column_name);
    for (auto& v : values) {
        max_value = max_s(max_value, v);
    }

    return max_value;
}
