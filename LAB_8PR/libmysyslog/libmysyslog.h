#ifndef LIBMYSYSLOG_H
#define LIBMYSYSLOG_H

// Уровни логирования
#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3
#define CRITICAL 4

// Форматы вывода
#define PLAIN_FORMAT 0
#define JSON_FORMAT 1

int mysyslog(const char* msg, int level, int driver, int format, const char* path);

#endif // LIBMYSYSLOG_H

