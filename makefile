all: main.o functions.o
	gcc -o shell main.o functions.o

main.o: main.c functions.h
	gcc -c -g main.c

functions.o: functions.c functions.h
	gcc -c -g functions.c

run:
	./shell < tests.txt

run2:
	./shell

clean:
	rm *.o
	rm shell
