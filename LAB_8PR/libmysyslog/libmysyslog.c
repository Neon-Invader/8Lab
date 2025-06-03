#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libmysyslog.h"

static FILE *log_file = NULL;
static char *current_path = NULL;

int mysyslog(const char* msg, int level, int driver, int format, const char* path) {
    // Проверяем, изменился ли путь
    if (log_file != NULL && current_path != NULL && strcmp(current_path, path) != 0) {
        fclose(log_file);
        free(current_path);
        log_file = NULL;
        current_path = NULL;
    }

    // Открываем файл, если он ещё не открыт
    if (log_file == NULL) {
        log_file = fopen(path, "a");
        if (log_file == NULL) {
            return -1;
        }
        current_path = strdup(path);
        if (current_path == NULL) {
            fclose(log_file);
            log_file = NULL;
            return -1;
        }
    }

    // Формируем временную метку
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0';

    // Преобразуем уровень логирования в строку
    const char *level_str;
    switch (level) {
        case DEBUG: level_str = "DEBUG"; break;
        case INFO: level_str = "INFO"; break;
        case WARN: level_str = "WARN"; break;
        case ERROR: level_str = "ERROR"; break;
        case CRITICAL: level_str = "CRITICAL"; break;
        default: level_str = "UNKNOWN"; break;
    }

    // Запись в лог в выбранном формате
    if (format == 0) {
        fprintf(log_file, "%s %s %d %s\n", timestamp, level_str, driver, msg);
    } else {
        fprintf(log_file, "{\"timestamp\":\"%s\",\"log_level\":\"%s\",\"driver\":%d,\"message\":\"%s\"}\n",
                timestamp, level_str, driver, msg);
    }

    // Принудительный сброс буфера
    fflush(log_file);

    return 0;
}
