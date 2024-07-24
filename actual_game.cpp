#include "game_class.h"

int main() {
    Game chess_game;
    chess_game.Start();
    while (chess_game.IsRunning()) {
        chess_game.Update();
    }
    return 0;
}
