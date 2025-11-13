CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude # -Iinclude tells the compiler to search
                                    # for headers in the include directory
LIBS = -lncurses
SRC = src/main.c src/velo.c src/interface.c src/client.c src/file_io.c \
src/io.c src/bd.c
OBJ = $(SRC:.c=.o)
TARGET = location_velo

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all
