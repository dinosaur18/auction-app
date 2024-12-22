#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "message_type.h"

// Hàm ghi log hệ thống
void logSystem(const char *agent, const char *logType, const char *format, ...) {
    FILE *file = fopen("data/system.log", "a");
    if (file == NULL) {
        perror("Error opening log file");
        return;
    }

    // Lấy thời gian hiện tại
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    if (localTime == NULL) {
        perror("Error getting local time");
        fclose(file);
        return;
    }

    // Ghi thời gian vào log
    fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] [%s]",
            localTime->tm_year + 1900,
            localTime->tm_mon + 1,
            localTime->tm_mday,
            localTime->tm_hour,
            localTime->tm_min,
            localTime->tm_sec,
            agent,
            logType);

    // Ghi nội dung log (sử dụng variadic arguments)
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fprintf(file, "\n"); 
    fclose(file);
}
