#include "global_config.h"
#include "figures.h"

int move = 0;

class Game {
private:
    bool running = false;
    int move = 0;

    std::vector<std::shared_ptr<Figure>> white_pieces, black_pieces;
    Board* board;

    void CreateBoard() {
        board = new Board(&white_pieces, &black_pieces);
        for (int i = 1; i <= 8; i++) {
            std::shared_ptr<Figure> white_pawn = std::make_shared<Pawn>(i, 2, "White");
            white_pieces.emplace_back(white_pawn);
            board->AddFigure(white_pawn, i, 2);
            std::shared_ptr<Figure> black_pawn =  std::make_shared<Pawn>(i, 7, "Black");
            black_pieces.push_back(black_pawn);
            board->AddFigure(black_pawn, i, 7);
        }
    }

public:
    Game() {
        CreateBoard();
        log("Game was constructed");
        std::cout << "Type 0 0 to exit\n";
    }

    ~Game() {
        delete board;
        log("Game was deconstructed");
    }

    void Update() {
        for (int h = 8; h >= 1; h--) {
            std::string to_log; 
            for (int v = 1; v <= 8; v++) {
                to_log += board->GetColor(v, h)[0];
                to_log += board->GetType(v, h)[0];
                to_log += ' ';
            }
            log(to_log);
        }
        int v, h;
        std::cout << "Choose a space: (two numbers):\n";
        std::cin >> v >> h;
        if (v == h && v == 0) {
            running = false;
            return;
        }
        if (board->IsEmpty(v, h)) std::cout << "No piece there!\n";
        else {
            std::shared_ptr<Figure> this_figure = board->GetFigure(v, h);
            std::cout << "Possible moves:\n";
            std::cout << this_figure->PossibleMovement(board) << '\n';
        }
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
