#include <chrono>
#include <ios>
#include <iostream>
#include <mutex>
#include <thread>

#include "DatabaseEngine.hpp"
#include "storage/Column.hpp"
#include "utils/Logger.hpp"

int main() {
    Logger l("/home/bungo/reposes/C2/CourseWorks/OOP/database/db.log");
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        l.makeEntry("All right");
    }
}