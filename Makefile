run: main
	./main

main: main.c
	clang -std=c23 -Wall -Wextra -o main main.c

.PHONY: run
