#include "utils/Logger.hpp"

void Logger::makeEntry(std::string message) {
    std::ofstream log_file;
    log_file.open(filename_, std::ios::app);

    time_t seconds = time(NULL);
    tm* cur_time = localtime(&seconds);
    log_file << std::put_time(cur_time, "%Y-%m-%d %H:%M:%S ") << message
             << std::endl;

    log_file.close();
}