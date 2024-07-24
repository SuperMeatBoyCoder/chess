#include "global_config.h"
#include "board.h"
#include "pieces.h"

class Game {
private:
    const std::vector<piece_config> default_board_config = {
        {1, 2, "White", "Pawn"}, {2, 2, "White", "Pawn"}, {3, 2, "White", "Pawn"}, {4, 2, "White", "Pawn"},
        {5, 2, "White", "Pawn"}, {6, 2, "White", "Pawn"}, {7, 2, "White", "Pawn"}, {8, 2, "White", "Pawn"},
        {1, 7, "Black", "Pawn"}, {2, 7, "Black", "Pawn"}, {3, 7, "Black", "Pawn"}, {4, 7, "Black", "Pawn"},
        {5, 7, "Black", "Pawn"}, {6, 7, "Black", "Pawn"}, {1, 7, "Black", "Pawn"}, {8, 7, "Black", "Pawn"},
        {5, 1, "White", "King"}, {5, 8, "Black", "King"},
        {2, 1, "White", "Night"}, {7, 1, "White", "Night"}, {2, 8, "Black", "Night"}, {7, 8, "Black", "Night"},
        };

    bool running = false;
    int move = 1;
    Board* board;

    void CreateBoard() {
        // verticals is used first due to traditional notation in chess
        board = new Board();

        for (piece_config raw_piece : default_board_config) {
            std::shared_ptr<ChessPiece> piece;
            switch (raw_piece.figure_type[0]) {
                case 'K':
                    piece = std::make_shared<King>(raw_piece);
                    break;
                case 'P':
                    piece = std::make_shared<Pawn>(raw_piece);
                    break;
                case 'N':
                    piece = std::make_shared<Knight>(raw_piece);
                    break;
                default:
                    throw "InvalidPiece";
            }
            board->AddFigure(piece);
        }
    }

    std::vector<std::pair<int, int>> PossibleMovementChecked(std::shared_ptr<ChessPiece> moving_piece) {
        int piece_v, piece_h;
        std::tie(piece_v, piece_h) = moving_piece->GetPosition();
        std::vector<std::pair<int, int>> can_move_checked;
        for (std::pair<int, int> move : moving_piece->PossibleMovement(board)) {
            board->Move(piece_v, piece_h, move.first, move.second);
            if (!board->CheckForCheck(moving_piece->color))
                can_move_checked.push_back(move);
            board->Revert();
        }
        return can_move_checked;
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
        std::cout << "Choose a piece: (chess notation):\n";
        std::string notation;
        std::cin >> notation;
        int input_v = notation[0] - 'a' + 1, input_h = notation[1] - '0';
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
        std::shared_ptr<ChessPiece> this_piece = board->GetFigure(input_v, input_h);
        std::cout << "Possible moves:\n";
        std::vector<std::pair<int, int>> can_move = PossibleMovementChecked(this_piece);
        for (std::pair<int, int> move : can_move) {
            std::cout << char('a'+ move.first - 1) << move.second << ' ';
        }
        std::cout << "\nChoose move (chess notation):\n";
        std::cin >> notation;
        input_v = notation[0] - 'a' + 1, input_h = notation[1] - '0';
        if (std::find(can_move.begin(), can_move.end(), std::pair(input_v, input_h)) == can_move.end()) {
            std::cout << "No such move!\n";
            return;
        }
        board->Move(this_piece, input_v, input_h);
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
