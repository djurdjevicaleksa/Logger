CC = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP
LIBS = -pthread

BIN = example
SRC = logger.c test.c
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -DSHOULD_DUMP=1 -DSHOULD_TIMESTAMP=0 -DDUMP_FILENAME="\"logger.txt\""

clean:
	rm -f *.o $(BIN) *.d

-include $(SRC:.c=.d)
