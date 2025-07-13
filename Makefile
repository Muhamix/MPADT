CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = PADL
all: $(TARGET)
$(TARGET): $(OBJ)
    $(CC) $(CFLAGS) -o $@ $^
clean:
    rm -f $(OBJ) $(TARGET)
