SOURCES = actual_game.cpp game_class.cpp board.cpp pieces.cpp
all:
	g++ -Wall -o game $(SOURCES)