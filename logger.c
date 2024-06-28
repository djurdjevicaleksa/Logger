#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "logger.h"

Logger logger;

static int terminateFlushThread = false;

static pthread_mutex_t _logger_mutex;
static pthread_mutex_t _flush_mutex;

static time_t _timestamp;
static FILE* _dump;
static pthread_t _flushing;

void loggerFunctionTemplate(const char* format, const char* formatColour, const char* message, FILE* consoleOutputStream) {

    pthread_mutex_lock(&_logger_mutex);

    char stdBuffer[OUTPUT_BUFFER_LEN];
    char dumpBuffer[OUTPUT_BUFFER_LEN];

    #if SHOULD_TIMESTAMP

        time(&_timestamp);
        snprintf(stdBuffer, OUTPUT_BUFFER_LEN - 1, formatColour, ctime(&_timestamp), message);
        snprintf(dumpBuffer, OUTPUT_BUFFER_LEN - 1, format, ctime(&_timestamp), message);

    #else

        snprintf(stdBuffer, OUTPUT_BUFFER_LEN - 1, formatColour, message);
        snprintf(dumpBuffer, OUTPUT_BUFFER_LEN - 1, format, message);

    #endif

    fprintf(consoleOutputStream, "%s", stdBuffer);

    #if SHOULD_DUMP //dump to file

        if(_dump != NULL) {

            fprintf(_dump, "%s", dumpBuffer);
        }

    #endif


    pthread_mutex_unlock(&_logger_mutex);
}

void loggerInfoFunction(const char* info) {
   
    loggerFunctionTemplate(FORMAT_INFO, FORMAT_INFO_COLOUR, info, stdout);
}

void loggerWarningFunction(const char* warning) {

    loggerFunctionTemplate(FORMAT_WARNING, FORMAT_WARNING_COLOUR, warning, stdout);
}

void loggerErrorFunction(const char* error) {

    loggerFunctionTemplate(FORMAT_ERROR, FORMAT_ERROR_COLOUR, error, stdout);
}

void loggerCriticalFunction(const char* critical) {

    loggerFunctionTemplate(FORMAT_CRITICAL, FORMAT_CRITICAL_COLOUR, critical, stdout);
}

void initLogger() {

    logger.logInfo = loggerInfoFunction;
    logger.logWarning = loggerWarningFunction;
    logger.logError = loggerErrorFunction;
    logger.logCriticalError = loggerCriticalFunction;

    pthread_mutex_init(&_logger_mutex, NULL);
    pthread_mutex_init(&_flush_mutex, NULL);

    #if SHOULD_DUMP //safely open dump file

        _dump = fopen(DUMP_FILENAME, "a");

        if(_dump == NULL) {

            switch(errno) {

                case ENOENT: {

                    fprintf(stderr, "Couldn't find dump file.\n");
                    break;
                }

                case EACCES: {

                    fprintf(stderr, "Couldn't open dump file. Permission denied.\n");
                    break;
                }

                default: {

                    fprintf(stderr, "Couldn't open dump file due to an unknown error.\n");
                    break;
                }
            }
        }
        else
            pthread_create(&_flushing, NULL, flushThread, NULL);

    #endif

}

void cleanupLogger() {

    #if SHOULD_DUMP //safely close dump file

       if(_dump != NULL) {
            
            //terminate thread
            pthread_mutex_lock(&_flush_mutex);
            terminateFlushThread = true;
            pthread_mutex_unlock(&_flush_mutex);

            pthread_join(_flushing, NULL);

            if(fclose(_dump) != 0) {

                switch(errno) {

                    case EBADF: {

                        fprintf(stderr, "Couldn't close dump file. Invalid stream.\n");
                        break;
                    }

                    default: {

                        fprintf(stderr, "Couldn't close dump file due to an unknown error.\n");
                        break;
                    }
                }   
            }
        }

    #endif

    pthread_mutex_destroy(&_logger_mutex);
    pthread_mutex_destroy(&_flush_mutex);
}

void* flushThread(void* arg) {

    pthread_mutex_lock(&_flush_mutex);

    while(terminateFlushThread == false) {

        pthread_mutex_unlock(&_flush_mutex);

        if(fflush(_dump) != 0) {

            switch(errno) {

                case EBADF: {

                    fprintf(stderr, "Couldn't flush the specified stream. The stream isn't opened or it can't be written to.\n");
                    break;
                }

                default: {

                    fprintf(stderr, "Couln't flush the specified stream due to an unknown error.\n");
                    break;
                }
            }
        }

        usleep(FLUSHING_INTERVAL * SECOND);

        pthread_mutex_lock(&_flush_mutex);
    }

    pthread_mutex_unlock(&_flush_mutex);

    return NULL;
}

void setCustomLoggerFunctions(loggerFunction logInfo, loggerFunction logWarning,
                loggerFunction logError, loggerFunction logCriticalError) {

    logger.logInfo = logInfo;
    logger.logWarning = logWarning;
    logger.logError = logError;
    logger.logCriticalError = logCriticalError;

}