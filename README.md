# Logger

This is a simple, thread-safe, lightweight library that implements logging functionalities in C. Lots of things can be customized to better suit your environment or needs.

# Features
This logger includes **multiple logging levels** for different types of messages:
* **LOG_INFO** : For general purpose logging, written to stdout
* **LOG_WARNING** : For potential problems, written to stdout
* **LOG_ERROR** : For errors that cause a subsystem to shut down or lose functionality, written to stderr
* **LOG_CRITICAL** : For errors that cause system-wide shutdown to prevent hardware failure, written to stderr

It supports **colorful text output** to the stdout and stderr streams. Colours **can be customized** inside `logger.h` file.

You can **override its functions** to change the behaviour to better suit your needs. To override, use `setCustomLoggerFunctions`.

You can **enable file output**, as well as **time stamping**. To enable or disable this feature, you need to use one or more of the following flags during compilation:
* -DSHOULD_DUMP=N, where N is true or false
* -DSHOULD_TIMESTAMP=M, where M is true or false
* -DDUMP_FILENAME="\\"FILENAME.EXT\\"", where FILENAME is the name of the dump file and the path to it and EXT is the appropriate file extension

To change **dump file flushing cooldown period or output buffer size**, provide one or more of the following flags during compilation:
* -DFLUSHING_INTERVAL=N, where N is the number of seconds
* -DOUTPUT_BUFFER_LEN=M, where M is the number of bytes
 
File output and time stamping are **off by default** and the default dump file is called `logger.txt` which will be inside the current directory. If you want to put it elsewhere, **enter a relative path** to where you want it to be.





## Data dumping

If enabled, dumps console output into a file specified by the -DDUMP_FILENAME flag. Logs are appended at the end of previous ones.

To reduce overhead, output file is opened only once, during `initLogger`and a separate thread is responsible for making sure logs are physically written to the file in a timely fashion to prevent data loss due to a critical program failure. The thread is terminated during `cleanupLogger`.

## Time stamping

If enabled, prefixes a log message with a ctime string representing current time, day and date.

# Functions
Initialization and cleanup:
* `initLogger` - Initializes the logger by setting logging function pointers to their default values, initializing mutexes and, if data dumping is enabled, it opens the output file and starts the flushing thread. *Note that if a file couldn't be opened for any reason, the library will not cause undefined behaviour.*
* `cleanupLogger` - Terminates the logger by destroying mutexes and, if data dumping is enabled, it stops the flushing thread and closes the dump file. *Note that using logger functions after `cleanupLogger`is called may cause undefined behaviour.*

Logging functions:
* `logger.logInfo` - Writes a log to the output stream and, if enabled, dumps it into the dump file.
* `logger.logWarning` - Writes a warning to the output stream and, if enabled, dumps it into the dump file.
* `logger.logError` - Writes an error to the output stream and, if enabled, dumps it into the dump file.
* `logger.logCriticalError` - Writes a critical error to the output stream and, if enabled, dumps it into the dump file.

To use custom functions, call `setCustomLoggerFunctions`.

# Building and running
## Building
This library uses a thread for data dump flushing and for mutexes and **therefore the binary needs to be compiled with the -lpthread flag**. The intended to build this library is **static**. When compiling the `logger.c` into a `.o` file, specify flags previously mentioned for enabling or disabling features such as time stamping or data dumping.

## Running
Since the logger is meant for multi-threaded environments the logger must be unique and it is defined in its `.c` file. You just need to link your reference to it, meaning you need the following line in your program's source code:

**extern Logger logger;**

You can then call its functions using the dot operator.