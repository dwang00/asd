all: main.o shell.o
	gcc -o program main.o shell.o

main.o: main.c shell.h
	gcc -c -g main.c

shell.o: shell.c shell.h
	gcc -c -g shell.c

run:
	./program < TEST_COMMANDS

run2:
	./program

clean:
	rm *.o
	rm program
