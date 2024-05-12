TARGER = varint


CC = gcc
CFLAGS = -Wall -Werror
SOURCES = $(wildcard *.dat)

$(TARGER): $(TARGER).c
	$(CC) $(TARGER).c -o $(TARGER)

clean:
	rm $(TARGER) *.dat