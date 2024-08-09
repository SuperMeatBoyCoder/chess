#pragma once
#include "structs.h"

namespace Chess {
class ChessPiece {
    // v = verical, h = horizontal
    // verticals is used first due to traditional notation in chess
protected:
    Square m_square;
    const char m_color, m_figure_type;
public:
    int times_moved = 0;

    ChessPiece(PieceInfo info) : m_square(info.square), m_color(info.color), m_figure_type(info.figure_type) {}

    Square GetPosition() const {
        return m_square;
    }

    char GetType() const {
        return m_figure_type;
    }

    char GetColor() const {
        return m_color;
    }
    
    void UpdatePosition(Square new_square) {
        m_square = new_square;
    }

    // returns vector of possible moves without taking into account checks
    virtual std::vector<ChessMove> PossibleMovement(const Board* board) const {
        return {};
    }

    // is this piece checking other side's king
    virtual bool IsChecking(const Board* board, Square king_square) const {
        return false;
    }
};
}