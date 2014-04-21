CC      = gcc
CFLAGS  = -W -Wall -pedantic
LDFLAGS =
EXEC    = queens
SRC     = main.c
OBJ     = $(SRC:.c=.o)

all: $(EXEC)

queens: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o *.data $(EXEC)

mrproper: clean
	rm -rf $(EXEC)
