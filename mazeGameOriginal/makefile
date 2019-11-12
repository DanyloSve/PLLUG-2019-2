all: random-utils game-map-utils main final clean run

random-utils: random-utils.cpp
	clang++ -c -o random-utils.o random-utils.cpp

game-map-utils: game-map-utils.cpp
	clang++ -c -o game-map-utils.o game-map-utils.cpp

main:
	clang++ -c -o main.o main.cpp

final: random-utils.o game-map-utils.o main.o
	clang++ random-utils.o game-map-utils.o main.o -o maze-game

clean:
	rm -rf *o

run:
	./maze-game
