#ifndef STRUCTS_H
#define STRUCTS_H
#include "config.h"

namespace Chess {

class Board;
class ChessPiece;

enum move_types {
    NORMAL_MOVE = 0, SHORT_CASTLE, LONG_CASTLE, EN_PASSANT, PROMOTION
};

struct Square {
    int v = -1, h = -1;
    bool operator==(const Square other) {
        return v == other.v && h == other.h;
    }
    operator int() const {return v * 8 + h;}
};

struct ChessMove {
    Square square;
    ChessPiece* captured_piece = nullptr;
    ChessPiece* moving_piece = nullptr;
    int special = NORMAL_MOVE;
    bool done = false;

    bool operator==(const ChessMove other) const {
        return moving_piece == other.moving_piece && done == other.done && square == other.square;
    }
};

struct PieceInfo {
    Square square;
    const char color, figure_type;
};
}
#endif // STRUCTS_H