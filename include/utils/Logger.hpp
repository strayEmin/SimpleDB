#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>

class Logger {
   public:
    enum class LogType { INFO, WARNING, ERROR };

    Logger(const Logger&) = delete;

    Logger& operator=(const Logger&) = delete;

    static Logger& GetInstance(const std::string& filepath = "app.log");

    void makeEntry(const std::string& message, LogType type);

   private:
    std::string filepath_;
    const std::unordered_map<Logger::LogType, std::string> type_string_ = {
        {Logger::LogType::INFO, "[INFO]"},
        {Logger::LogType::WARNING, "[WARNING]"},
        {Logger::LogType::ERROR, "[ERROR]"}};
    std::ofstream logfile_;

    std::string typeToString(LogType type) const;

    explicit Logger(const std::string& filepath);

    void setFilePath(const std::string& new_filepath);

    ~Logger() {
        if (logfile_.is_open()) {
            logfile_.close();
        }
    }
};