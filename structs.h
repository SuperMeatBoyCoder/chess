#include "config.h"

namespace Chess {
class Board;

class ChessPiece;

struct Square {
    int v = -1, h = -1;
    bool operator==(const Square other) {
        return v == other.v && h == other.h;
    }
    operator int() const {return v * 8 + h;}
};

struct ChessMove {
    Square square;
    std::shared_ptr<ChessPiece> captured_piece = nullptr;
    std::shared_ptr<ChessPiece> moving_piece = nullptr;
    int special = NORMAL_MOVE;
    bool done = false;

    bool operator==(const ChessMove other) {
        return moving_piece == other.moving_piece && done == other.done && square == other.square;
    }
};

struct PieceInfo {
    Square square;
    const char color, figure_type;
};
}