all: main.o procedure.o
	gcc -lpthread -o start  main.o procedure.o

main.o: main.c procedure.h
	gcc -lpthread -c -o main.o  main.c

procedure.o: procedure.c procedure.h
	gcc -lpthread -c -o procedure.o  procedure.c

clean:
	rm -f *.o
	rm -f start
