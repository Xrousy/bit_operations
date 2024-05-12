TARGER = varint


CC = gcc
CFLAGS = -Wall -Werror
SRCDER = ./src/
SOURCES = $(wildcard *.dat)

$(TARGER): $(SRCDER)$(TARGER).c
	$(CC) $(SRCDER)$(TARGER).c -o $(TARGER)

clean:
	rm $(TARGER) *.dat