CC=gcc
DEPS = ppm.h filter.h
OBJS = ppm.o test.o filter.o
CFLAGS = -g

default: run

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

clear:
	rm -f *.o run
