#include "global_config.h"
#include "figures.h"

int move = 0;

class Game {
private:
    bool running = false;
    int move = 0;
    Board board;

    std::vector<Figure*> white_pieces, black_pieces;

    void CreateBoard() {
        for (int i = 1; i <= 8; i++) {
            white_pieces.push_back(new Pawn(i, 2, "White"));
            board.AddFigure(white_pieces.back(), i, 2);
            black_pieces.push_back(new Pawn(i, 7, "Black"));
            board.AddFigure(black_pieces.back(), i, 7);
        }
    }

public:
    Game() {
        CreateBoard();
        log("Game was constructed");
    }

    ~Game() {
        log("Game was deconstructed");
    }

    void Update() {
        for (int h = 8; h >= 1; h--) {
            std::string to_log; 
            for (int v = 1; v <= 8; v++) {
                to_log += board.GetColor(v, h)[0];
                to_log += board.GetType(v, h)[0];
                to_log += ' ';
            }
            log(to_log);
        }
        running = false;
    }

    void Start() {
        running = true;
    }

    void End() {
        running = false;
    }

    bool IsRunning() {
        return running;
    }
};

Game chess_game;

int main() {
    chess_game.Start();
    while (chess_game.IsRunning()) {
        chess_game.Update();
    }
    return 0;
}
