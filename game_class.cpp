#include "src/board.h"
#include "src/pieces.h"
#include "src/game_class.h"

namespace Chess {
Game::Game() {

    log("Game was constructed\n");
    std::cout << "Type 0 to exit\n";
}  

Game::~Game() {
    log("Game was deconstructed");
}

void Game::Render() {
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
}

ChessPiece* Game::InputPiece() {
    std::cout << "Choose a piece: (chess notation):\n";
    std::string notation;
    std::cin >> notation;
    if (notation.size() == 1) {
        if (notation == "0") {
            running = false;
            throw "";
        }
        throw "No such square exist!";
    }
    Square input_square = {notation[0] - 'a' + 1, notation[1] - '0'};
    if (!board.Isinside(input_square)) {
        throw "No such square exist!";
    }
    if (board.IsEmpty(input_square)) {   
        throw "No piece there!";
    }
    return board.GetPiecePtr(input_square);
}

ChessMove Game::InputMove(ChessPiece* this_piece) {
    char color = this_piece->GetColor();
    if (color != colors[move % 2]) {
        throw "it's not your turn!";
    }
    const std::vector<ChessMove>& can_move = board.PossibleMovement(this_piece);
    if (can_move.empty()) {
        throw "This piece can't move!";
    }
    std::cout << "Possible moves:\n";
    for (ChessMove move : can_move) {
        std::cout << char('a' + move.square.v - 1) << move.square.h << ' ';
    }
    std::cout << "\nChoose move (chess notation):\n";
    std::string notation;
    std::cin >> notation;
    if (notation.size() == 1) {
        if (notation == "0") {
            running = false;
            throw "";
        }
        throw "No such move!";
    }
    ChessMove input_move = {notation[0] - 'a' + 1, notation[1] - '0'};
    input_move.moving_piece = this_piece;
    auto it = std::find(can_move.begin(), can_move.end(), input_move);
    if (it == can_move.end()) {
        throw "No such move!";
    }
    input_move = *it;
    if (input_move.special == PROMOTION) {
        // making a new piece based on input in case of promotion
        std::cout << "Select a piece for promotion:\n";
        std::string chosen_piece;
        std::cin >> chosen_piece;
        char piece_type = std::toupper(chosen_piece[0]);
        // you can't promote to pawn or king
        if (piece_type == 'P' || piece_type == 'K') {
            throw "Invalid piece!";
        }
        char piece_color = input_move.moving_piece->GetColor();
        input_move.moving_piece = board.CreatePiecePtr({this_piece->GetPosition(), piece_color, piece_type});
    }
    return input_move;
}

void Game::Update() {
    Render();
    if (!board.FindAllMoves(colors[move % 2])) {
        // checkmate

        // if black should move now
        if (colors[move % 2] == 'B')
            std::cout << "White wins\n";
        else
            std::cout << "Black wins\n";
        running = false;
        return;
    }
    ChessPiece* this_piece;
    try {
        this_piece = InputPiece();
    }   
    catch (const char* error_message) {
        std::cout << error_message << '\n';
        return;
    }
    ChessMove input_move;
    try {
        input_move = InputMove(this_piece);
    }
    catch (const char* error_message) {
        std::cout << error_message << '\n';
        return;
    }
    board.Move(input_move);
    move++;
}

void Game::Start() {
    running = true;
    while (running) {
        Update();
    }
}

bool Game::IsRunning() {
    return running;
}

void Game::End() {
    running = false;
}
}