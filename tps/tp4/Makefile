#
# J. Federico Heranandez <federico.hernandez@um.edu.ar>
#

CC = gcc
CFLAGS = -g -Wall

TARGETS = webito

all: $(TARGETS)

webito: srv-example.c service.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


clean:
	rm -f $(TARGETS) *.o *~

