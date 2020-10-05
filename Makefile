CC = gcc
CFLAGS = -Iinclude -Wall
LIBS = -lm

BIN = ./bin/
SRC = ./source/main.c ./source/project.c ./source/file.c ./source/function.c ./source/parameter.c

doc_gen: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(BIN)$@

.PHONY: clean

clean:
	rm bin/*
	rm docs/*.html
	rm example/*.html