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
            board->AddFigure(white_pawn);
            std::shared_ptr<Figure> black_pawn =  std::make_shared<Pawn>(i, 7, "Black");
            black_pieces.push_back(black_pawn);
            board->AddFigure(black_pawn);
        }
    }

public:
    Game() {
        CreateBoard();
        file_log << "Game was constructed\n";
        std::cout << "Type 0 0 to exit\n";
    }

    ~Game() {
        delete board;
        file_log << "Game was deconstructed\n";
    }

    void Update() {
        for (int h = 8; h >= 1; h--) {
            for (int v = 1; v <= 8; v++) {
                std::cout << board->GetColor(v, h)[0] << board->GetType(v, h)[0] << ' ';
            }
            std::cout << '\n';
        }
        int v, h;
        std::cout << "Choose a space: (two numbers):\n";
        std::cin >> v >> h;
        if (v == h && v == 0) {
            running = false;
            return;
        }
        if (!board->Isinside(v, h)) {
            std::cout << "That's some big expectatitons!\n";
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
