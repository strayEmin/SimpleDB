#include "utils/Logger.hpp"

Logger& Logger::GetInstance(const std::string& filepath) {
    static Logger instance(filepath);
    return instance;
}

std::string Logger::typeToString(LogType type) const {
    return type_string_.at(type);
}

void Logger::setFilePath(const std::string& new_filepath) {
    logfile_.close();
    logfile_.open(new_filepath);
    if (not logfile_.is_open()) {
        throw std::runtime_error("failed to open file ('" + filepath_ + "')");
    }

    filepath_ = new_filepath;
}

Logger::Logger(const std::string& filepath) : filepath_(filepath) {
    logfile_.open(filepath, std::ios::app);
    if (not logfile_.is_open()) {
        throw std::runtime_error("failed to open file ('" + filepath + "')");
    }
}

void Logger::makeEntry(const std::string& message, LogType type) {
    if (not logfile_.is_open()) {
        throw std::runtime_error("file ('" + filepath_ + "' is not opened");
    }

    time_t seconds = time(NULL);
    tm* cur_time = localtime(&seconds);
    logfile_ << typeToString(type) + " "
             << std::put_time(cur_time, "%Y-%m-%d %H:%M:%S ") << message
             << std::endl;
}
