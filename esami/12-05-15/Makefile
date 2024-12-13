all: main.o procedure.o
	gcc -lpthread -o start main.o procedure.o

main.o: main.c header.h
	gcc -c -o main.o main.c

procedure.o: procedure.c header.h
	gcc -c -o procedure.o procedure.c

clean:
	rm -f *.o
	rm -f start
