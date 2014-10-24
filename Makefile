run: main
	./main

main: main.o ensemble.o
	gcc -Wall -o main main.o decalage.o

main.o: main.c decalage.h
	gcc -c -Wall main.c

decalage.o: decalage.c decalage.h
	gcc -c -Wall decalage.c

clean:
	rm -f main
	rm -f main.o
	rm -f decalage.o
