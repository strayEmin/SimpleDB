#include <chrono>
#include <ios>
#include <iostream>
#include <mutex>
#include <thread>

#include "DatabaseEngine.hpp"
#include "storage/Column.hpp"
#include "storage/Database.hpp"
#include "utils/Logger.hpp"

int main() {
    DatabaseEngine dbe("tmp/");

    dbe.createDatabase("mydb");
    dbe.createDatabase("mydb2");

    dbe.useDatabase("mydb");
    dbe.dropDatabase("mydb");
}