#pragma once
#include "global_config.h"

class Board;

struct piece_config {
    int vertical, horizontal;
    char color;
    std::string figure_type;
};

class ChessPiece {
protected:
    int vertical = -1, horizontal = -1;
public:
    const std::string figure_type;
    char color;
    int times_moved = 0;

    ChessPiece(piece_config raw_piece) :
           vertical(raw_piece.vertical), horizontal(raw_piece.horizontal), color(raw_piece.color), figure_type(raw_piece.figure_type) {
        file_log << "Piece of type " + figure_type + " was constructed\n";
    }

    virtual ~ChessPiece() {
        file_log << "Piece of type " + figure_type + " was deconstructed\n";
    }

    std::pair<int, int> GetPosition() {
        return std::pair(vertical, horizontal);
    }
    
    void UpdatePosition(int v, int h) {
        vertical = v;
        horizontal = h;
    }

    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) {
        return {};
    }

    virtual bool IsChecking(Board* board, int end_v, int end_h) {
        return false;
    }
};