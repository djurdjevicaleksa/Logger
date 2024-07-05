CC = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP 
LIBS = -pthread

BIN = log
SRC = logger.c test.c
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(BIN) *.d

-include $(SRC:.c=.d)
