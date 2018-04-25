CC=gcc
DEPS = ppm.h
OBJS = ppm.o test.o
CFLAGS = 

default: run clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

clear:
	rm -f *.o run
