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

    Square GetPosition() {
        return m_square;
    }

    char GetType() {
        return m_figure_type;
    }

    char GetColor() {
        return m_color;
    }
    
    void UpdatePosition(Square new_square) {
        m_square = new_square;
    }

    virtual std::vector<ChessMove> PossibleMovement(Board* board) {
        return {};
    }

    virtual bool IsChecking(Board* board, Square king_square) {
        return false;
    }
};
}