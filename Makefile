
game : main.o game.o othello.o
	g++ -o game main.o game.o othello.o

main.o : main.cc game.h othello.h
	g++ -c main.cc

game.o : game.cc game.h
	g++ -c game.cc

othello.o : othello.cc othello.h game.h colors.h piece.h
	g++ -c othello.cc othello.h game.h colors.h piece.h

archive: main.cc game.cc game.h othello.cc othello.h colors.h piece.h Makefile
	tar cvf main.cc game.cc game.h othello.cc othello.h colors.h piece.h Makefile

clean :
	rm game *.o *.gch
