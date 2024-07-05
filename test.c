#include "logger.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    extern Logger logger;

    //initialize the library
    initLogger();

    logger.logInfo("Info message!");

    logger.log(LOG_WARNING, COLOUR_RED, "Issa %s of da month!", "first");

    cleanupLogger();

    return 0;
}