#pragma once

#include <pthread.h>
#include <time.h> //for timestamping
#include <stdio.h>

#include "config.h"

#define true 1
#define false 0

#define SECOND 1000000

///  LOGGING LEVELS  ////
#define LOG_INFO        0
#define LOG_WARNING     1
#define LOG_ERROR       2
#define LOG_CRITICAL    3
/////////////////////////

//                COLOURS               //
#define COLOUR_NONE             ""     
#define COLOUR_RED              "\x1b[31m"
#define COLOUR_GREEN            "\x1b[32m"
#define COLOUR_YELLOW           "\x1b[33m"
#define COLOUR_BLUE             "\x1b[34m"
#define COLOUR_MAGENTA          "\x1b[35m"
#define COLOUR_CYAN             "\x1b[36m"
#define COLOUR_FAINT_RED        "\x1b[2;31m"
#define COLOUR_BOLD_RED         "\x1b[1;31m"

#define COLOUR_RESET            "\x1b[0m"
//////////////////////////////////////////

//      DEFAULT PARAMETERS      //
#ifndef DUMP_FILENAME
    #define DUMP_FILENAME "logger.txt"
#endif

#ifndef FLUSHING_INTERVAL
    #define FLUSHING_INTERVAL 5
#endif

#ifndef OUTPUT_BUFFER_LEN
    #define OUTPUT_BUFFER_LEN 128
#endif
//////////////////////////////////


//      COLOUR FORMATS FOR DEFAULT PRINTING     //
#ifdef TIMESTAMP_BUILTIN

    #define FORMAT_INFO                 "%s\t[INFO] %s\n" 
    #define FORMAT_WARNING              "%s\t[WARNING] %s\n" 
    #define FORMAT_ERROR                "%s\t[ERROR] %s\n" 
    #define FORMAT_CRITICAL             "%s\t[CRITICAL] %s\n"

    #define FORMAT_INFO_COLOUR          COLOUR_GREEN "%s\t[INFO] %s\n" COLOUR_RESET
    #define FORMAT_WARNING_COLOUR       COLOUR_YELLOW "%s\t[WARNING] %s\n" COLOUR_RESET
    #define FORMAT_ERROR_COLOUR         COLOUR_FAINT_RED "%s\t[ERROR] %s\n" COLOUR_RESET
    #define FORMAT_CRITICAL_COLOUR      COLOUR_BOLD_RED "%s\t[CRITICAL] %s\n" COLOUR_RESET

#else

    #define FORMAT_INFO                 "[INFO] %s\n" 
    #define FORMAT_WARNING              "[WARNING] %s\n" 
    #define FORMAT_ERROR                "[ERROR] %s\n" 
    #define FORMAT_CRITICAL             "[CRITICAL] %s\n" 

    #define FORMAT_INFO_COLOUR          COLOUR_GREEN "[INFO] %s\n" COLOUR_RESET
    #define FORMAT_WARNING_COLOUR       COLOUR_YELLOW "[WARNING] %s\n" COLOUR_RESET
    #define FORMAT_ERROR_COLOUR         COLOUR_FAINT_RED "[ERROR] %s\n" COLOUR_RESET
    #define FORMAT_CRITICAL_COLOUR      COLOUR_BOLD_RED "[CRITICAL] %s\n" COLOUR_RESET

#endif
//////////////////////////////////////////////////


#ifdef __cplusplus
extern "C" {
#endif

typedef void     (*loggerFunction)  (const char* message);
typedef void (*loggerLineFunction)  (int level, char* colour, const char* _format, ...);

typedef struct {

    loggerFunction  logInfo;
    loggerFunction  logWarning;
    loggerFunction  logError;
    loggerFunction  logCriticalError;
    loggerLineFunction log;

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

void _log(int level, char* colour, const char* _format, ...);


#ifdef __cplusplus
}
#endif

