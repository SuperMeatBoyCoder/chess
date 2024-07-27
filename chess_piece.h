#include "structs.h"

class ChessPiece {
    // v = verical, h = horizontal
    // verticals is used first due to traditional notation in chess
protected:
    int m_v, m_h;
    const char m_color, m_figure_type;
public:
    int times_moved = 0;

    ChessPiece(piece_info info) : m_v(info.v), m_h(info.h), m_color(info.color), m_figure_type(info.figure_type) {}

    std::pair<int, int> GetPosition() {
        return std::pair(m_v, m_h);
    }

    char GetType() {
        return m_figure_type;
    }

    char GetColor() {
        return m_color;
    }
    
    void UpdatePosition(int v, int h) {
        m_v = v;
        m_h = h;
    }

    virtual std::vector<possible_move> PossibleMovement(Board* board) {
        return {};
    }

    virtual bool IsChecking(Board* board, int end_v, int end_h) {
        return false;
    }

};