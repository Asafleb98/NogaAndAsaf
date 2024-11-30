# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: clean compile run

compile:
	g++ -Iinclude -o main src/main.cpp

run:
	./main

clean:
	rm -f main
