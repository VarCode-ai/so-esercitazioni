start: teatro visualizzatore

teatro: semafori.o teatro.o cliente.o
	gcc -o start teatro.o semafori.o cliente.o

teatro.o: teatro.c
	gcc -c teatro.c

visualizzatore: visualizzatore.o 
	gcc -o visualizzatore visualizzatore.o

visualizzatore.o: visualizzatore.c cliente.h
	gcc -c visualizzatore.c

cliente.o: cliente.c cliente.h
	gcc -c cliente.c

semafori.o:semafori.c semafori.h
	gcc -c semafori.c

clean: 
	rm -f *.o
	rm -f start