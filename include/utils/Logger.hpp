#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>

class Logger {
   private:
    std::string filename_;

   public:
    Logger(std::string logfilename) : filename_(logfilename) {}

    void makeEntry(std::string message);
};