#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <android/log.h>
#include <time.h>

// Define the log file path
#define LOG_FILE_PATH "/sdcard/Android/data/com.mobile.legends/files/log.txt"

// Function to log messages to a file
static void log_to_file(const std::string& message) {
    // Use an output file stream in append mode
    std::ofstream log_file(LOG_FILE_PATH, std::ios_base::app | std::ios_base::out);
    if (log_file.is_open()) {
        // Get current time for the log entry
        time_t now = time(0);
        char* dt = ctime(&now);
        // Write the timestamp and message to the file
        log_file << dt << ": " << message << std::endl;
        log_file.close();
    } else {
        // If file can't be opened, log to logcat as a fallback
        __android_log_print(ANDROID_LOG_ERROR, "MLL_MOD_LOG", "Failed to open log file for writing.");
    }
}

#endif // LOGGER_H
