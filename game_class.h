#include "global_config.h"
#include "board.h"
#include "pieces.h"

class Game {
private:
    const std::vector<piece_info> default_board_config = {
        {1, 2, 'W', 'P'}, {2, 2, 'W', 'P'}, {3, 2, 'W', 'P'}, {4, 2, 'W', 'P'},
        {5, 2, 'W', 'P'}, {6, 2, 'W', 'P'}, {7, 2, 'W', 'P'}, {8, 2, 'W', 'P'},
        {1, 7, 'B', 'P'}, {2, 7, 'B', 'P'}, {3, 7, 'B', 'P'}, {4, 7, 'B', 'P'},
        {5, 7, 'B', 'P'}, {6, 7, 'B', 'P'}, {7, 7, 'B', 'P'}, {8, 7, 'B', 'P'},
        {2, 1, 'W', 'N'}, {7, 1, 'W', 'N'}, {2, 8, 'B', 'N'}, {7, 8, 'B', 'N'},
        {3, 1, 'W', 'B'}, {6, 1, 'W', 'B'}, {3, 8, 'B', 'B'}, {6, 8, 'B', 'B'},
        {1, 1, 'W', 'R'}, {8, 1, 'W', 'R'}, {1, 8, 'B', 'R'}, {8, 8, 'B', 'R'},
        {4, 1, 'W', 'Q'}, {4, 8, 'B', 'Q'},
        {5, 1, 'W', 'K'}, {5, 8, 'B', 'K'},
        };

    bool running = false;
    int move = 1;
    Board* board = new Board();

    void CreateBoard() {
        for (const piece_info raw_piece : default_board_config) {
            std::shared_ptr<ChessPiece> piece;
            switch (raw_piece.figure_type) {
                case 'K':
                    piece = std::make_shared<King>(raw_piece);
                    break;
                case 'P':
                    piece = std::make_shared<Pawn>(raw_piece);
                    break;
                case 'N':
                    piece = std::make_shared<Night>(raw_piece);
                    break;
                case 'B':
                    piece = std::make_shared<Bishop>(raw_piece);
                    break;
                case 'R':
                    piece = std::make_shared<Rook>(raw_piece);
                    break;
                case 'Q':
                    piece = std::make_shared<Queen>(raw_piece);
                    break;
                default:
                    throw "InvalidPiece";
            }
            board->AddFigure(piece);
        }
    }

public:
    Game() {
        CreateBoard();
        file_log << "Game was constructed" << std::endl;
        std::cout << "Type 0 to exit\n";
    }

    ~Game() {
        delete board;
        file_log << "Game was deconstructed" << std::endl;
    }

    void Update() {
        for (int h = 8; h >= 1; h--) {
            for (int v = 1; v <= 8; v++) {
                if (!board->IsEmpty(v, h))
                    std::cout << board->GetColor(v, h) << board->GetType(v, h) << ' ';
                else
                    std::cout << "-- ";
            }
            std::cout << '\n';
        }
        std::cout << "Choose a piece: (chess notation):\n";
        std::string notation;
        std::cin >> notation;
        if (notation.size() == 1) {
            running = false;
            return;
        }
        int input_v = notation[0] - 'a' + 1, input_h = notation[1] - '0';
        if (!board->Isinside(input_v, input_h)){
            std::cout << "No such place exist!\n";
            return;
        }
        if (board->IsEmpty(input_v, input_h)){
            std::cout << "No piece there!\n";
            return;
        }
        char color = board->GetColor(input_v, input_h);
        if ((color == 'W' && move % 2 == 0) ||
            (color == 'B' && move % 2 == 1)){
            std::cout << "it's not your turn!\n";
            return;
        }
        const std::shared_ptr<ChessPiece> this_piece = board->GetFigurePtr(input_v, input_h);
        std::vector<std::pair<int, int>> can_move;
        this_piece->PossibleMovementChecked(board, can_move);
        if (can_move.empty()) {
            std::cout << "This piece can't move!\n";
            return;
        }
        std::cout << "Possible moves:\n";
        for (std::pair<int, int> move : can_move) {
            std::cout << char('a'+ move.first - 1) << move.second << ' ';
        }
        std::cout << "\nChoose move (chess notation):\n";
        std::cin >> notation;
        if (notation.size() == 1) {
            running = false;
            return;
        }
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