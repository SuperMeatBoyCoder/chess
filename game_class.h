#include "config.h"
#include "pieces.h"

namespace Chess {
class Game {
private:
    
    Board board;
    bool running = false;
    int move = 1;

public:
    Game() {
        log("Game was constructed\n");
        std::cout << "Type 0 to exit\n";
    }

    ~Game() {
        log("Game was deconstructed");
    }

    void Update() {
        for (int h = 8; h >= 1; h--) {
            for (int v = 1; v <= 8; v++) {
                Square square = {v, h};
                if (!board.IsEmpty(square))
                    std::cout << board.GetColor(square) << board.GetType(square) << ' ';
                else
                    std::cout << "-- ";
            }
            std::cout << '\n';
        }
        std::cout << "Choose a piece: (chess notation):\n";
        std::string notation;
        std::cin >> notation;
        if (notation.size() == 1) {
            if (notation == "0")
                running = false;
            return;
        }
        Square input_square = {notation[0] - 'a' + 1, notation[1] - '0'};
        if (!board.Isinside(input_square)) {
            std::cout << "No such place exist!\n";
            return;
        }
        if (board.IsEmpty(input_square)) {   
            std::cout << "No piece there!\n";
            return;
        }
        char color = board.GetColor(input_square);
        if ((color == 'W' && move % 2 == 0) ||
            (color == 'B' && move % 2 == 1)) {
            std::cout << "it's not your turn!\n";
            return;
        }
        const std::shared_ptr<ChessPiece> this_piece = board.GetPiecePtr(input_square);
        std::vector<ChessMove> can_move;
        board.PossibleMovementChecked(this_piece, can_move);
        if (can_move.empty()) {
            std::cout << "This piece can't move!\n";
            return;
        }
        std::cout << "Possible moves:\n";
        for (ChessMove move : can_move) {
            std::cout << char('a' + move.square.v - 1) << move.square.h << ' ';
        }
        std::cout << "\nChoose move (chess notation):\n";
        std::cin >> notation;
        if (notation.size() == 1) {
            if (notation == "0")
                running = false;
            return;
        }
        ChessMove input_move = {notation[0] - 'a' + 1, notation[1] - '0'};
        input_move.moving_piece = this_piece;
        auto it = std::find(can_move.begin(), can_move.end(), input_move);
        if (it == can_move.end()) {
            std::cout << "No such move!\n";
            return;
        }
        input_move = *it;
        if (input_move.special == PROMOTION) {
            // making a new piece based on input in case of promotion
            std::cout << "Select a piece for promotion:\n";
            std::string chosen_piece;
            std::cin >> chosen_piece;
            char piece = toupper(chosen_piece[0]);
            // you can't promote to pawn or king
            if (piece == 'P' || piece == 'K') {
                std::cout << "Invalid piece!\n";
                return;
            }
            char piece_color = input_move.moving_piece->GetColor();
            try {
                input_move.moving_piece = board.CreatePiecePtr({this_piece->GetPosition(), piece_color, chosen_piece[0]});
            }   
            catch (const char* error_message) {
                std::cout << error_message << '\n';
                return;
            }
        }
        board.Move(input_move);
        move++;
    }

    void Start() {
        running = true;
        while (IsRunning()) {
            Update();
        }
    }

    void End() {
        running = false;
    }

    bool IsRunning() {
        return running;
    }
};
}