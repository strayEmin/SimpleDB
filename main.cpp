#include <gtest/gtest.h>

#include <chrono>
#include <cmath>
#include <filesystem>
#include <ios>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "DatabaseEngine.hpp"
#include "storage/Column.hpp"
#include "storage/Database.hpp"
#include "tests/test.hpp"
#include "utils/Logger.hpp"
#include "utils/TypeClassifier.hpp"

const std::filesystem::path project_root = PROJECT_ROOT_PATH;

int main() {
    DatabaseEngine dbe(project_root.string() + "tmp/",
                       project_root.string() + "app.log");

    dbe.dropDatabase("mydb2");
    dbe.createDatabase("mydb2", true);

    dbe.useDatabase("mydb2");
    Table t("users",
            std::vector<Column>{Column{"id", "INT", false},
                                Column{"user", "VARCHAR(255)", false}});
    dbe.createTable("users",
                    std::vector<Column>{Column{"id", "INT", false},
                                        Column{"user", "VARCHAR(255)", false}},
                    true);

    dbe.insertInto("users", {"id", "user"}, {"3", "\"20-30-00\""});
    dbe.insertInto("users", {"id", "user"}, {"3", "\"semen\""});
    dbe.insertInto("users", {"id", "user"}, {"3", "\"semen\""});
    dbe.insertInto("users", {"id", "user"}, {"3", "\"semen\""});
    dbe.insertInto("users", {"id", "user"}, {"3", "\"semen\""});

    dbe.update("users", {{"id", "1"}});
    dbe.insertInto("users", {"id", "user"}, {"3", "\"semen\""});

    auto max_i{[](std::string a, std::string b) {
        return std::to_string(std::max(std::stoi(a), std::stoi(b)));
    }};

    std::cout << dbe.max("users", "id", max_i) << std::endl;

    dbe.alterTable("users").add(Column{"", "VARCHAR(255)", false});

    dbe.insertInto("users", {"nasral"}, {"\"erevan\""});
}
