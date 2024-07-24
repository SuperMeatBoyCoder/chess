#pragma once
#include "board.h"

class Pawn : public Figure {
private:
    int base_row;
public:
    Pawn(int v, int h, std::string c) : Figure(v, h, c, "Pawn"), base_row(h) {}

    std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        // Justin Case
        if (captured) return {};
        std::vector<std::pair<int, int>> can_move;
        int move_delta = 1;
        if (color == "Black")
            move_delta = -1;
        int to_horizontal = horizontal + move_delta;
        
        // Pawns can't be limited by the border when going straight
        if (board->IsEmpty(vertical, to_horizontal)) {
            can_move.push_back(std::pair(vertical, to_horizontal));
            if (horizontal == base_row && board->IsEmpty(vertical, to_horizontal + move_delta)) {
                can_move.push_back(std::pair(vertical, to_horizontal + move_delta));
            }
        }
        for (int capture_delta : {-1, 1}) {
            if (!board->IsEmpty(vertical + capture_delta, to_horizontal) &&
                board->GetColor(vertical + capture_delta, to_horizontal) != color) {
                can_move.push_back(std::pair(vertical + capture_delta, to_horizontal));
            }
        }

        std::vector<std::pair<int, int>> can_move_checked;
        for (std::pair<int, int> move : can_move) {
            board->Move(vertical, horizontal, move.first, move.second);
            if (!board->CheckForCheck(color))
                can_move_checked.push_back(move);
            board->Revert();
        }

        return can_move_checked;
    }

    bool IsChecking(Board* board, int end_v, int end_h) override {
        if (captured) return false;
        int move_delta = 1;
        if (color == "Black")
            move_delta = -1;
        return horizontal + move_delta == end_h && abs(vertical - end_v) == 1;
    }
};

class King : public Figure {
private:
public:
    King(int v, int h, std::string c) : Figure(v, h, c, "King") {}

    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        return {};
    }

    virtual bool IsChecking(Board* board, int end_v, int end_h) override {
        return false;
    }
};