#pragma once
#include "board.h"

class Pawn : public ChessPiece {
private:
public:
    Pawn(int v, int h, std::string c) : ChessPiece(v, h, c, "Pawn") {}

    std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        // Justin Case
        if (captured) return {};
        std::vector<std::pair<int, int>> can_move;
        int move_delta = 1;
        if (color == "Black")
            move_delta = -1;
        int to_h = horizontal + move_delta, to_v = vertical;
        
        // Pawns can't be limited by the border when going straight
        if (board->IsEmpty(to_v, to_h)) {
            can_move.push_back(std::pair(to_v, to_h));
            if (times_moved == 0 && board->IsEmpty(to_v, to_h + move_delta)) {
                can_move.push_back(std::pair(to_v, to_h + move_delta));
            }
        }
        for (int capture_delta : {-1, 1}) {
            if (!board->IsEmpty(to_v + capture_delta, to_h) &&
                board->GetColor(to_v + capture_delta, to_h) != color) {
                can_move.push_back(std::pair(to_v + capture_delta, to_h));
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

class King : public ChessPiece {
private:
public:
    King(int v, int h, std::string c) : ChessPiece(v, h, c, "King") {}

    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        if (captured) return {};
        std::vector<std::pair<int, int>> can_move;
        for (int to_v = vertical -1; to_v <= vertical + 1; to_v++) {
            for (int to_h = horizontal -1; to_h <= horizontal + 1; to_h++) {
                if (board->Isinside(to_v, to_h) && board->GetColor(to_v, to_h) != color) {
                    can_move.push_back(std::pair(to_v, to_h));
                }
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

    virtual bool IsChecking(Board* board, int end_v, int end_h) override {
        return (abs(vertical - end_v) + abs(horizontal - end_h)) <= 1;
    }
};