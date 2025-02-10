CC = gcc
EXEC = main
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
    RM = del /s /q *.o $(EXEC).exe
else
    RM = rm -f *.o $(EXEC)
endif

all: $(EXEC)

exe: all
	./$(EXEC)

%.o: %.c
	$(CC) -o $@ -c $<

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

clean:
	$(RM)