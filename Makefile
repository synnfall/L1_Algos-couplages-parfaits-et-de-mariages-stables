CC = gcc
EXEC = main
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
    RM = del /s /q *.o $(EXEC).exe $(EXEC)
else
    RM = rm -f *.o $(EXEC) $(EXEC).exe
endif

simple:
	$(CC) mariage_simple.c -o $(EXEC) -Wextra -Wall
	./$(EXEC)

c:
	$(CC) mariage_complexe.c -o $(EXEC) -Wextra -Wall
	./$(EXEC)

g:
	$(CC) couple_graph_biparti.c -o $(EXEC) -Wextra -Wall
	./$(EXEC)

clean:
	$(RM)