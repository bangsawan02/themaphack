
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

inline void log_to_file(const std::string& message) {
    std::ofstream log_file("/sdcard/Android/data/com.mobile.legends/files/log.txt", std::ios_base::app | std::ios_base::out);
    if (log_file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        log_file << "[" << ss.str() << "] " << message << std::endl;
    }
}

#endif // LOGGER_H
