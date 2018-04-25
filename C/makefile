CC=gcc
DEPS = ppm.h
OBJS = ppm.o test.o
CFLAGS = 

default: run

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
