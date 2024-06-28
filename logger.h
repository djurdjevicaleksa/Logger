#pragma once

#include <pthread.h>
#include <time.h> //for timestamping
#include <stdio.h>

#define LOG_INFO        0
#define LOG_WARNING     1
#define LOG_ERROR       2
#define LOG_CRITICAL    3

#define true 1
#define false 0

#define SECOND 1000000

#define ANSI_COLOUR_RED              "\x1b[31m"
#define ANSI_COLOUR_GREEN            "\x1b[32m"
#define ANSI_COLOUR_YELLOW           "\x1b[33m"
#define ANSI_COLOUR_BLUE             "\x1b[34m"
#define ANSI_COLOUR_MAGENTA          "\x1b[35m"
#define ANSI_COLOUR_CYAN             "\x1b[36m"
#define ANSI_COLOUR_FAINT_RED        "\x1b[2;31m"
#define ANSI_COLOUR_BOLD_RED         "\x1b[1;31m"
#define ANSI_COLOUR_RESET            "\x1b[0m"

//////// CONFIGURABLE ////////
#ifndef SHOULD_DUMP
    #define SHOULD_DUMP false
#endif

#ifndef SHOULD_TIMESTAMP
    #define SHOULD_TIMESTAMP false
#endif

#ifndef DUMP_FILENAME
    #define DUMP_FILENAME "logger.txt"
#endif

#ifndef FLUSHING_INTERVAL
    #define FLUSHING_INTERVAL 5
#endif

#ifndef TIMESTAMP_LENGTH
    #define TIMESTAMP_LENGTH 25
#endif

#ifndef OUTPUT_BUFFER_LEN
    #define OUTPUT_BUFFER_LEN 128
#endif

#if SHOULD_TIMESTAMP

    #define FORMAT_INFO                 "%s\t[INFO] %s\n" 
    #define FORMAT_WARNING              "%s\t[WARNING] %s\n" 
    #define FORMAT_ERROR                "%s\t[ERROR] %s\n" 
    #define FORMAT_CRITICAL             "%s\t[CRITICAL] %s\n"

    #define FORMAT_INFO_COLOUR          ANSI_COLOUR_GREEN "%s\t[INFO] %s\n" 
    #define FORMAT_WARNING_COLOUR       ANSI_COLOUR_YELLOW "%s\t[WARNING] %s\n" 
    #define FORMAT_ERROR_COLOUR         ANSI_COLOUR_FAINT_RED "%s\t[ERROR] %s\n" 
    #define FORMAT_CRITICAL_COLOUR      ANSI_COLOUR_BOLD_RED "%s\t[CRITICAL] %s\n"  

#else

    #define FORMAT_INFO                 "\t[INFO] %s\n" 
    #define FORMAT_WARNING              "\t[WARNING] %s\n" 
    #define FORMAT_ERROR                "\t[ERROR] %s\n" 
    #define FORMAT_CRITICAL             "\t[CRITICAL] %s\n" 

    #define FORMAT_INFO_COLOUR          ANSI_COLOUR_GREEN "\t[INFO] %s\n" 
    #define FORMAT_WARNING_COLOUR       ANSI_COLOUR_YELLOW "\t[WARNING] %s\n" 
    #define FORMAT_ERROR_COLOUR         ANSI_COLOUR_FAINT_RED "\t[ERROR] %s\n" 
    #define FORMAT_CRITICAL_COLOUR      ANSI_COLOUR_BOLD_RED "\t[CRITICAL] %s\n" 

#endif

//////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*loggerFunction)(const char* message);

typedef struct {

    loggerFunction  logInfo;
    loggerFunction  logWarning;
    loggerFunction  logError;
    loggerFunction  logCriticalError;

} Logger;

void loggerFunctionTemplate(const char*, const char*, const char*, FILE*);

void loggerInfoFunction(const char*);
void loggerWarningFunction(const char*);
void loggerErrorFunction(const char*);
void loggerCriticalFunction(const char*);

void initLogger();
void cleanupLogger();

void* flushThread(void*);

void setCustomLoggerFunctions(loggerFunction, loggerFunction,
                loggerFunction, loggerFunction);


#ifdef __cplusplus
}
#endif

