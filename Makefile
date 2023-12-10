CC = gcc
CFLAGS = -Wall -Werror 

SRC = $(wildcard *.c)
OUT = $(SRC:%.c=bin/%)
SRIPTS = $(wildcard *.sh)
CSV = $(wildcard *.csv)

SRC = philosophes.c philosophes-my-functions.c thread_test.c readerWriter.c readerWriter-my-functions.c
all: $(OUT) $(SRIPTS)

bin/%: %.c
	$(CC) $(CFLAGS) -g -o $@ $< -lpthread

clean:
	rm -f $(OUT) $(CSV)

.PHONY: clean
