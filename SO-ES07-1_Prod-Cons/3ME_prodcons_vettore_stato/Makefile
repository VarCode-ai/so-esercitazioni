start: procedure.o semafori.o main.o
	gcc -o start main.o semafori.o procedure.o

main.o:main.c
	gcc -c main.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

semafori.o:semafori.c semafori.h
	gcc -c semafori.c

clean: 
	rm -f *.o
	rm -f start