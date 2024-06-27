#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdatomic.h>

#include "logger.h"

Logger logger;
static atomic_bool terminateFlushThread = false;

void loggerFunctionTemplate(const char* format, const char* formatColour, const char* message, FILE* consoleOutputStream) {

    pthread_mutex_lock(&logger._mutex);

    char stdBuffer[OUTPUT_BUFFER_LEN];
    char dumpBuffer[OUTPUT_BUFFER_LEN];

    #if SHOULD_TIMESTAMP

        time(&logger._timestamp);
        snprintf(stdBuffer, OUTPUT_BUFFER_LEN - 1, formatColour, ctime(&logger._timestamp), message);
        snprintf(dumpBuffer, OUTPUT_BUFFER_LEN - 1, format, ctime(&logger._timestamp), message);

    #else

        snprintf(stdBuffer, OUTPUT_BUFFER_LEN - 1, formatColour, message);
        snprintf(dumpBuffer, OUTPUT_BUFFER_LEN - 1, format, message);

    #endif

    fprintf(consoleOutputStream, "%s", stdBuffer);

    #if SHOULD_DUMP //dump to file

        if(logger.dump != NULL) {

            fprintf(logger.dump, "%s", dumpBuffer);
        }

    #endif


    pthread_mutex_unlock(&logger._mutex);
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

    pthread_mutex_init(&logger._mutex, NULL);

    #if SHOULD_DUMP //safely open dump file

        logger.dump = fopen(DUMP_FILENAME, "a");

        if(logger.dump == NULL) {

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
            pthread_create(&logger.flushing, NULL, flushThread, NULL);

    #endif

}

void cleanupLogger() {

    #if SHOULD_DUMP //safely close dump file

        //terminate thread
        atomic_store(&terminateFlushThread, true);
        pthread_join(logger.flushing, NULL);

        if(logger.dump != NULL) {

            if(fclose(logger.dump) != 0) {

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

    pthread_mutex_destroy(&logger._mutex);
}

void* flushThread() {

    while(atomic_load(&terminateFlushThread) == false) {

        if(fflush(logger.dump) != 0) {

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
    }

    return NULL;
}

void setCustomLoggerFunctions(loggerFunction logInfo, loggerFunction logWarning,
                loggerFunction logError, loggerFunction logCriticalError) {

    logger.logInfo = logInfo;
    logger.logWarning = logWarning;
    logger.logError = logError;
    logger.logCriticalError = logCriticalError;

}