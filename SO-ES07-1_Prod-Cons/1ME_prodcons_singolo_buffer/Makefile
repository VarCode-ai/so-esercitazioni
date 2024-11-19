
start: main.o procedure.o semafori.o
	gcc -o main_ex main.o procedure.o semafori.o

main.o: main.c
	gcc -c main.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

clean:
	rm -f *.o
	rm -f main_ex