
game : main.o game.o othello.o
	g++ -o game main.o game.o othello.o

main.o : main.cc game.h othello.h
	g++ -c main.cc

game.o : game.cc game.h
	g++ -c game.cc

othello.o : othello.cc othello.h
	g++ -c othello.cc othello.h

clean :
	rm game *.o