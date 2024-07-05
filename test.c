#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "logger.h"

int main() {

    extern Logger logger;

    //initialize the library
    initLogger();

    logger.logInfo("Info message!");

    logger.log(LOG_WARNING, COLOUR_RED, "Issa %s of da month!", "first");

    usleep(3 * SECOND);

    logger.logInfo("Info message!");

    cleanupLogger();

    return 0;
}