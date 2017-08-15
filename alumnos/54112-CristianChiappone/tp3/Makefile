CC=gcc
CFLAGS=-g -Wall
TARGETS=tp3

all: $(TARGETS)

format:
	./tp3 -i input_file

clean:
	rm -f *~ .o $(TARGETS) 


tp2:  main.c contar.c
	$(CC) $(CFLAGS) -o $@ $^
