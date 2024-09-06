#include "src/game_class.h"

int main(int argc, char* argv[]) {
    Chess::Game chess_game;
    chess_game.Start();
    std::cin.get();
    return 0;
}