.PHONY = all clean
CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -pedantic -pthread -g
#add -Werror
TARGET = proj2

all: $(TARGET)

proj2: proj2.o
	$(CC) $(CFLAGS) $^ -o $@

proj2.o: proj2.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf *.o $(TARGET) 

zip:
	zip proj2.zip *.c *.h Makefile
