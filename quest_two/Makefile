CC = gcc
CFLAGS = -Wall

SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.c)
EXECUTABLE = coder

.PHONY:	all	clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS)	$^ -o $@

clean:
	rm -f $(EXECUTABLE) 