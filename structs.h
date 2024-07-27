#include "global_config.h"

class Board;

class ChessPiece;

struct chess_move {
    std::shared_ptr<ChessPiece> moved = nullptr;
    int start_v, start_h;
    std::shared_ptr<ChessPiece> captured = nullptr;
    int special = 0;
};

struct piece_info {
    int v, h;
    const char color, figure_type;
};

struct possible_move {
    int end_v, end_h;
    int special = 0;

    bool operator==(possible_move other) {
        return end_v == other.end_v && end_h == other.end_h;
    }
};