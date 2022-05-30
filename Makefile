CC=gcc
CFLAGS=-Wall -Werror
LDFLAG=-l bcm2835
SOURCES=$(wildcard *.c)

brewd: $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) $? $(LDFLAG) -o $@

clean:
	rm -rf *.o brewd

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
