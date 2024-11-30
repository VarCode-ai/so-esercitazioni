all: padre lettore scrittore


padre: padre.o procedure.o semafori.o scrittore lettore
	gcc -o padre padre.o procedure.o semafori.o 

lettore: lettore.o procedure.o semafori.o
	gcc -o lettore lettore.o procedure.o semafori.o

scrittore: scrittore.o procedure.o semafori.o
	gcc -o scrittore scrittore.o procedure.o semafori.o



#compilare i singoli

padre.o: padre.c procedure.h semafori.h scrittore.c lettore.c
	gcc -c padre.c

lettore.o: lettore.c semafori.h procedure.h
	gcc -c lettore.c

scrittore.o: scrittore.c semafori.h procedure.h
	gcc -c scrittore.c

procedure.o: semafori.h procedure.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

clean:
	rm -f *.o
	rm -f start