

all: start

start:chat.o procedure.o
	gcc -o start chat.o procedure.o

chat.o:chat.c header.h
	gcc -c chat.c

procedure.o:procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f *~
	rm -f ./start
