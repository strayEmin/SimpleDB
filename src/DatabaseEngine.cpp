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

void DatabaseEngine::setDBDir(const std::string& new_db_dir) {
    db_dir_ = new_db_dir;
    if (*new_db_dir.rbegin() != '/') {
        db_dir_ += '/';
    }
    std::filesystem::create_directory(db_dir_);
}

void DatabaseEngine::createDatabase(const std::string& db_name) {
    if (database_paths_.contains(db_name)) {
        throw std::invalid_argument("this database name already exists('" +
                                    db_name + "')");
    }

    Database db(db_name);
    std::string db_path = db_dir_ + db_name + DB_FILE_EXTENSION;
    db.saveToFile(db_path);
    database_paths_[db_name] = db_path;
}

void DatabaseEngine::useDatabase(const std::string& db_name) {
    try {
        std::shared_ptr<Database> db_sptr(new Database(db_name));
        db_sptr->loadFromFile(database_paths_.at(db_name));
        current_database_sptr_ = db_sptr;
    } catch (std::out_of_range) {
        throw std::out_of_range("database named '" + db_name +
                                "' does not exist, cannot be used");
    }
}

void DatabaseEngine::dropDatabase(const std::string& db_name) {
    try {
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

    } catch (std::out_of_range) {
        throw std::out_of_range("database named '" + db_name +
                                "' does not exist, cannot be drop");
    }
}