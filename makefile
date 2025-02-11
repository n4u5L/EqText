all: main.exe
	./main

.PHONY: search
search: search.exe
	./search

main.exe: main.c
	gcc main.c -o main -Wall

search.exe: algorithm-search.c
	gcc algorithm-search.c -o search -Wall