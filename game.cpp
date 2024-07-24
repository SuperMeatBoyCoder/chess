#include "global_config.h"
#include "figures.h"

int move = 0;

struct pieces {
    std::vector<Pawn*> pawns;

    ~pieces() {
        for (Pawn* i : pawns) delete i;
    }
};

class Game {
private:
    bool running = false;
    int move = 0;
    Board board;

    pieces white_pieces, black_pieces;

    void CreateBoard() {
        for (int i = 1; i <= 8; i++) {
            white_pieces.pawns.push_back(new Pawn(&board, i, 2, "white"));
            black_pieces.pawns.push_back(new Pawn(&board, i, 7, "black"));
        }
    }

public:
    Game() {
        log("Game was constructed");
    }

    ~Game() {
        log("Game was deconstructed");
    }

    void Config() {
        running = true;
        CreateBoard();
    }

    void Update() {
        running = false;
    }

    void End() {
        running = false;
    }

    bool IsRunning() {
        return running;
    }
};

int main() {
    Game chess_game;
    chess_game.Config();
    while (chess_game.IsRunning()) {
        chess_game.Update();
    }
    return 0;
}
