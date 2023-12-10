CC = gcc
CFLAGS = -Wall -Werror 

SRC = $(wildcard src/*.c)
OUT = $(SRC:src/%.c=bin/%)
SRIPTS = $(wildcard *.sh)
CSV = $(wildcard *.csv)

all: $(OUT) $(SRIPTS)

bin/%: src/%.c
	$(CC) $(CFLAGS) -g -o $@ $< -lpthread

clean:
	rm -r $(OUT) $(CSV)

.PHONY: clean
