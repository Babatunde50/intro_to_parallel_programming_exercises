CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread

SRC = runtime.c initial.c solution.c
OBJ = $(SRC:.c=.o)
EXEC = runtime

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)