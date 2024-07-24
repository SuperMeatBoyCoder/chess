#pragma once
#include "board.h"

class Pawn : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        // Justin Case
        if (captured) return {};
        std::vector<std::pair<int, int>> can_move;
        int h_delta = 1;
        if (color == "Black")
            h_delta = -1;
        int to_h = horizontal + h_delta, to_v = vertical;
        
        // Pawns can't be limited by the border when going straight
        if (board->IsEmpty(to_v, to_h)) {
            can_move.push_back(std::pair(to_v, to_h));
            if (times_moved == 0 && board->IsEmpty(to_v, to_h + h_delta)) {
                can_move.push_back(std::pair(to_v, to_h + h_delta));
            }
        }
        for (int capture_delta : {-1, 1}) {
            if (!board->IsEmpty(to_v + capture_delta, to_h) &&
                board->GetColor(to_v + capture_delta, to_h) != color) {
                can_move.push_back(std::pair(to_v + capture_delta, to_h));
            }
        }

        return can_move;
    }

    bool IsChecking(Board* board, int end_v, int end_h) override {
        if (captured) return false;
        int h_delta = 1;
        if (color == "Black")
            h_delta = -1;
        return horizontal + h_delta == end_h && abs(vertical - end_v) == 1;
    }
};

class King : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
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
        return can_move;
    }

    virtual bool IsChecking(Board* board, int end_v, int end_h) override {
        return (abs(vertical - end_v) + abs(horizontal - end_h)) <= 1;
    }
};

class Knight : public ChessPiece {
public:
    // Knight is Night, it's a feature
    using ChessPiece::ChessPiece;
    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        if (captured) return {};
        std::vector<std::pair<int, int>> can_move;
        int to_v, to_h;
        for (int v_delta : {-1, 1}) {
            for (int h_delta : {-1, 1}) {
                to_v = vertical + v_delta;
                to_h = horizontal + h_delta * 2;
                if (board->Isinside(to_v, to_h) && board->GetColor(to_v, to_h) != color) {
                    can_move.push_back(std::pair(to_v, to_h));
                }
                to_h -= h_delta;
                to_v += v_delta;
                if (board->Isinside(to_v, to_h) && board->GetColor(to_v, to_h) != color) {
                    can_move.push_back(std::pair(to_v, to_h));
                }
            }
        }
        return can_move;
    }

    virtual bool IsChecking(Board* board, int end_v, int end_h) override {
        std::pair<int, int> check(abs(vertical - end_v), abs(horizontal - end_h));
        if (check.first > check.second) std::swap(check.first, check.second);
        return check == std::pair(1, 2);
    }
};