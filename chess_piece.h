#pragma once
#include "global_config.h"

class Board;

struct piece_info {
    // v = verical, h = horizontal
    int v, h;
    char color, figure_type;
};

class ChessPiece {
    // verticals is used first due to traditional notation in chess
protected:
    piece_info m_info;
public:
    int times_moved = 0;

    std::pair<int, int> GetPosition() {
        return std::pair(m_info.v, m_info.h);
    }

    char GetType() {
        return m_info.figure_type;
    }

    char GetColor() {
        return m_info.color;
    }
    
    void UpdatePosition(int v, int h) {
        m_info.v = v;
        m_info.h = h;
    }

    virtual std::vector<std::pair<int, int>> PossibleMovement(Board& board) {
        return {};
    }

    virtual bool IsChecking(Board& board, int end_v, int end_h) {
        return false;
    }
};

struct chess_move {
    std::shared_ptr<ChessPiece> moved = nullptr;
    int start_v, start_h;
    std::shared_ptr<ChessPiece> captured = nullptr;
};