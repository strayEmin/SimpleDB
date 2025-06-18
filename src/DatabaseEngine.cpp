#include "DatabaseEngine.hpp"

DatabaseEngine::DatabaseEngine(const std::string& path_to_db_dir,
                               const std::string& path_to_logfile)
    : path_to_logfile_(path_to_logfile) {
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
        Logger::GetInstance(path_to_logfile_)
            .makeEntry("DatabaseEngine init failed: filesystem error",
                       Logger::LogType::ERROR);

        throw std::runtime_error("error working with files at dir '" + db_dir_ +
                                 "'");
    }

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("DatabaseEngine inited", Logger::LogType::INFO);
}

void DatabaseEngine::saveChangesCurDB() const {
    current_database_sptr_->saveToFile(
        database_paths_.at(current_database_sptr_->getName()));
}

DatabaseEngine::~DatabaseEngine() {
    saveChangesCurDB();
    Logger::GetInstance(path_to_logfile_)
        .makeEntry("DatabaseEngine destroyed", Logger::LogType::INFO);
}

void DatabaseEngine::setDBDir(const std::string& new_db_dir) {
    db_dir_ = new_db_dir;
    if (*new_db_dir.rbegin() != '/') {
        db_dir_ += '/';
    }

    try {
        std::filesystem::create_directory(db_dir_);
    } catch (const std::filesystem::filesystem_error& e) {
        Logger::GetInstance(path_to_logfile_)
            .makeEntry(e.what(), Logger::LogType::ERROR);

        throw std::runtime_error(e.what());
    }

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("changed database directory to '" + new_db_dir + "'",
                   Logger::LogType::INFO);
}

void DatabaseEngine::createDatabase(const std::string& db_name,
                                    bool if_not_exist) {
    if (database_paths_.contains(db_name)) {
        if (not if_not_exist) {
            Logger::GetInstance(path_to_logfile_)
                .makeEntry(
                    "failed to create database: this database name already "
                    "exists('" +
                        db_name + "')",
                    Logger::LogType::ERROR);

            throw std::invalid_argument("this database name already exists('" +
                                        db_name + "')");
        } else {
            Logger::GetInstance(path_to_logfile_)
                .makeEntry(
                    "failed to create database: this database name already "
                    "exists('" +
                        db_name + "'), but IF_NOT_EXIST = TRUE",
                    Logger::LogType::WARNING);

            return;
        }
    }

    Database db(db_name);
    std::string db_path = db_dir_ + db_name + DB_FILE_EXTENSION;
    db.saveToFile(db_path);
    database_paths_[db_name] = db_path;

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful database creation ('" + db_name + ")'",
                   Logger::LogType::INFO);
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
        Logger::GetInstance(path_to_logfile_)
            .makeEntry("failed to use database , ('" + db_name +
                           "') does not exist, cannot be used",
                       Logger::LogType::ERROR);

        throw std::out_of_range("database named '" + db_name +
                                "' does not exist, cannot be used");
    }

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful using database ('" + db_name + "')",
                   Logger::LogType::INFO);
}

void DatabaseEngine::dropDatabase(const std::string& db_name, bool if_exist) {
    if (not database_paths_.contains(db_name)) {
        if (not if_exist) {
            Logger::GetInstance(path_to_logfile_)
                .makeEntry("failed to drop database, ('" + db_name +
                               "') does not exist, cannot be drop",
                           Logger::LogType::ERROR);

            throw std::out_of_range("database named '" + db_name +
                                    "' does not exist, cannot be drop");
        } else {
            Logger::GetInstance(path_to_logfile_)
                .makeEntry("failed to drop database, ('" + db_name +
                               "') does not exist, cannot be drop, but "
                               "IF_EXIST = TRUE",
                           Logger::LogType::WARNING);

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
        std::string error_message = "file by path '" +
                                    database_paths_.at(db_name) +
                                    "' can not be deleted";
        Logger::GetInstance(path_to_logfile_)
            .makeEntry(
                "failed to drop database ('" + db_name + "'): " + error_message,
                Logger::LogType::ERROR);

        throw std::runtime_error(error_message);
    }

    database_paths_.erase(db_name);

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful database drop", Logger::LogType::INFO);
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
        std::string error_message =
            "failed to insert into: there should be as many values as there "
            "are fields";
        Logger::GetInstance(path_to_logfile_)
            .makeEntry(error_message, Logger::LogType::ERROR);

        throw std::invalid_argument(error_message);
    }

    Record record{};
    for (int i = 0; i < column_names.size(); ++i) {
        record.setField(column_names[i], values[i]);
    }

    current_database_sptr_->getTable(table_name)->insertRecord(record);
    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful insert into in database '" +
                       current_database_sptr_->getName() + "'",
                   Logger::LogType::INFO);
}

void DatabaseEngine::createTable(const Table& table, bool if_not_exist) {
    try {
        current_database_sptr_->createTable(table);
    } catch (const std::invalid_argument& e) {
        std::string error_message = "failed to create table in database '" +
                                    current_database_sptr_->getName() +
                                    "' :" + e.what();
        if (not if_not_exist) {
            Logger::GetInstance(path_to_logfile_)
                .makeEntry(error_message, Logger::LogType::ERROR);

            throw std::invalid_argument(e.what());
        }
        Logger::GetInstance(path_to_logfile_)
            .makeEntry(error_message + ", but IF_NOT_EXIST = TRUE",
                       Logger::LogType::WARNING);
    }

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful create table in database '" +
                       current_database_sptr_->getName() + "'",
                   Logger::LogType::INFO);
}

void DatabaseEngine::createTable(const std::string& table_name,
                                 const std::vector<Column>& columns,
                                 const bool if_not_exist) {
    try {
        current_database_sptr_->createTable(table_name, columns);
    } catch (const std::invalid_argument& e) {
        std::string error_message = "failed to create table in database '" +
                                    current_database_sptr_->getName() +
                                    "' :" + e.what();
        if (not if_not_exist) {
            Logger::GetInstance(path_to_logfile_)
                .makeEntry(error_message, Logger::LogType::ERROR);

            throw std::invalid_argument(e.what());
        }

        Logger::GetInstance(path_to_logfile_)
            .makeEntry(error_message + ", but IF_NOT_EXIST = TRUE",
                       Logger::LogType::WARNING);
    }

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful create table in database '" +
                       current_database_sptr_->getName() + "'",
                   Logger::LogType::INFO);
}

std::string DatabaseEngine::max(const std::string& table_name,
                                const std::string& column_name,
                                std::string (*max_s)(std::string,
                                                     std::string)) const {
    const auto& table = current_database_sptr_->getTable(table_name);
    std::list<std::string> values = table->getValuesInCol(column_name);
    std::string max_value{values.front()};
    for (auto& v : values) {
        max_value = max_s(max_value, v);
    }

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful get max in database '" +
                       current_database_sptr_->getName() + "', column -- '" +
                       column_name + "'",
                   Logger::LogType::INFO);

    return max_value;
}

std::vector<Column> DatabaseEngine::getColumns(
    const std::string& table_name) const {
    auto result = current_database_sptr_->getTable(table_name)->getColumns();

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful get columns in database '" +
                       current_database_sptr_->getName() + "'",
                   Logger::LogType::INFO);

    return result;
}

void DatabaseEngine::deleteFrom(const std::string& table_name,
                                std::vector<Condition> conditions) {
    current_database_sptr_->getTable(table_name)->deleteRecords(conditions);
    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful delete from '" + table_name + "' in database '" +
                       current_database_sptr_->getName() + "'",
                   Logger::LogType::INFO);
}

void DatabaseEngine::update(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& fieldvalues,
    const std::vector<Condition>& conditions) {
    current_database_sptr_->getTable(table_name)
        ->updateRecords(fieldvalues, conditions);

    Logger::GetInstance(path_to_logfile_)
        .makeEntry("successful update from '" + table_name + "' in database '" +
                       current_database_sptr_->getName() + "'",
                   Logger::LogType::INFO);
}

AlterTableQueryBuilder DatabaseEngine::alterTable(
    const std::string& table_name) {
    return AlterTableQueryBuilder{current_database_sptr_->getTable(table_name)};
}