#include "logger.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    extern Logger logger;

    //initialize the library
    initLogger();

    logger.logInfo("info");

    usleep(SECOND);

    logger.logWarning("warning");

    usleep(SECOND);

    logger.logError("error");

    usleep(SECOND);

    logger.logCriticalError("critical");

    //terminate the library
    cleanupLogger();

    return 0;
}