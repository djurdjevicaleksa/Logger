#pragma once

/*
    Commented: Default logging functions will not dump data into the output file.
    Uncommented: Default logging functions will dump data into the output file specified by DUMP_FILENAME.
*/
//#define DUMP_BUILTIN


/*
    Commented: Default logging functions will not timestamp messages.
    Uncommented: Default logging functions will timestamp messages.
*/
//#define TIMESTAMP_BUILTIN


/*
    Commented: Messages will be dumped into the default output file called "logger.txt" in the working directory.
    Uncommented: Provide path and filename to customize output file name and location.
*/
//#define DUMP_FILENAME "logger.txt"


/*
    Commented: Messages to be dumped will be flushed into the output file by a thread every 5 seconds by default.
    Uncommented: Messages to be dumped will be flushed into the output file by a thread every specified amount of seconds.
*/
//#define FLUSHING_INTERVAL 5


/*
    Commented: Maximum log buffer will be 128 bytes by default.
    Uncommented: Maximum log buffer will be the amount of bytes specified.
*/
//#define OUTPUT_BUFFER_LEN 128


