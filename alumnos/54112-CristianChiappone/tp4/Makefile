CC = gcc
CFLAGS = -g -Wall

TARGETS = ser_sock

all: $(TARGETS)

ser_sock: server.c arg.c service.c 
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGETS) *.o *~

