CC=gcc
ARCH=$(shell uname -p)
CFLAGS=-Wall -Werror
LDFLAG=
SOURCES=$(wildcard *.c)

ifneq ($(ARCH),x86_64)
LDFLAG+=-l bcm2835
endif

brewd: $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) $? $(LDFLAG) -o $@

clean:
	rm -rf *.o brewd

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
