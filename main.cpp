#include <chrono>
#include <filesystem>
#include <ios>
#include <iostream>
#include <mutex>
#include <thread>

#include "DatabaseEngine.hpp"
#include "storage/Column.hpp"
#include "storage/Database.hpp"
#include "utils/Logger.hpp"

const std::filesystem::path project_root = PROJECT_ROOT_PATH;

int main() {
    DatabaseEngine dbe(project_root.string() + "tmp/");

    dbe.createDatabase("mydb2", true);

    dbe.useDatabase("mydb2");
    Table t("users",
            std::vector<Column>{Column{"id", "INT", true},
                                Column{"user", "VARCHAR(255)", false}});
    dbe.createTable("users",
                    std::vector<Column>{Column{"id", "INT", true},
                                        Column{"user", "VARCHAR(255)", false}},
                    true);

    dbe.insertInto("users", {"id", "user"}, {"3", "semen"});
}