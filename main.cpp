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
    Database d("main");
    d.loadFromFile("testbd/in.json");

    d.saveToFile("testbd/out.json");

    d.loadFromFile("testbd/out.json");

    // ошибок не обнаружено
}