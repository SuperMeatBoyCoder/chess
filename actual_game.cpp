#include "src/game_class.h"

const int WIDTH = 800, HEIGHT = 600;

int main(int argc, char* argv[]) {
    Chess::Game chess_game(WIDTH, HEIGHT);
    chess_game.Start();
    std::cin.get();
    return 0;
}