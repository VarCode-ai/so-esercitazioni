
start: main.o procedure.o semafori.o
	gcc -o prod_cons_circ_sing main.o procedure.o semafori.o

main.o: main.c
	gcc -c main.c


semafori.o: semafori.c semafori.h
	gcc -c semafori.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f prod_cons_circ_sing