#include "global_config.h"
#include "board.h"
#include "pieces.h"

int move = 0;

class Game {
private:
    bool running = false;
    int move = 1;

    std::vector<std::shared_ptr<ChessPiece>> white_pieces, black_pieces;
    Board* board;

    void CreateBoard() {
        // verticals is used first due to traditional notation in chess
        std::shared_ptr<ChessPiece> white_king = std::make_shared<King>(5, 1, "White");
        white_pieces.emplace_back(white_king);
        std::shared_ptr<ChessPiece> black_king = std::make_shared<King>(5, 8, "Black");
        black_pieces.emplace_back(black_king);
        board = new Board(&white_pieces, &black_pieces, white_king, black_king);
        board->AddFigure(white_king);
        board->AddFigure(black_king);
        for (int i = 1; i <= 8; i++) {
            std::shared_ptr<ChessPiece> white_pawn = std::make_shared<Pawn>(i, 2, "White");
            white_pieces.emplace_back(white_pawn);
            board->AddFigure(white_pawn);
            std::shared_ptr<ChessPiece> black_pawn =  std::make_shared<Pawn>(i, 7, "Black");
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
        int input_v, input_h;
        std::cout << "Choose a piece: (two numbers):\n";
        std::cin >> input_v >> input_h;
        if (input_v == input_h && input_v == 0) {
            running = false;
            return;
        }
        if (board->IsEmpty(input_v, input_h)){
            std::cout << "No piece there!\n";
            return;
        }
        std::string color = board->GetColor(input_v, input_h);
        if ((color == "White" && move % 2 == 0) ||
            (color == "Black" && move % 2 == 1)){
            std::cout << "it's not your turn!\n";
            return;
        }
        std::shared_ptr<ChessPiece> this_figure = board->GetFigure(input_v, input_h);
        std::cout << "Possible moves:\n";
        std::vector<std::pair<int, int>> can_move = this_figure->PossibleMovement(board);
        std::cout << can_move << '\n';
        std::cout << "Choose move (two numbers):\n";
        std::cin >> input_v >> input_h;
        if (std::find(can_move.begin(), can_move.end(), std::pair(input_v, input_h)) == can_move.end()) {
            std::cout << "No such move!\n";
            return;
        }
        board->Move(this_figure, input_v, input_h);
        move++;
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
