SOURCES = actual_game.cpp game_class.cpp board.cpp pieces.cpp
all:
	g++ -Wall -I src/include -L src/lib -o game $(SOURCES) -lmingw32 -lSDL2main -lSDL2