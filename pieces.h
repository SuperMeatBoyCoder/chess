#include "board.h"

class Pawn : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        std::vector<std::pair<int, int>> can_move;
        int h_delta = 1;
        if (color == 'B')
            h_delta = -1;
        int to_h = horizontal + h_delta, to_v = vertical;

        if (board->IsMovable(to_v, to_h)) {
            can_move.push_back(std::pair(to_v, to_h));
            if (times_moved == 0 && board->IsMovable(to_v, to_h + h_delta)) {
                can_move.push_back(std::pair(to_v, to_h + h_delta));
            }
        }
        for (int v_delta : {-1, 1}) {
            if (board->IsCapturable(to_v + v_delta, to_h, color)) {
                can_move.push_back(std::pair(to_v + v_delta, to_h));
            }
        }

        return can_move;
    }

    bool IsChecking(Board* board, int end_v, int end_h) override {
        int h_delta = 1;
        if (color == 'B')
            h_delta = -1;
        return horizontal + h_delta == end_h && abs(vertical - end_v) == 1;
    }
};

class King : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        std::vector<std::pair<int, int>> can_move;
        for (int to_v = vertical - 1; to_v <= vertical + 1; to_v++) {
            for (int to_h = horizontal - 1; to_h <= horizontal + 1; to_h++) {
                if (board->isMovableOrCapturable(to_v, to_h, color)) {
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
        std::vector<std::pair<int, int>> can_move;
        int to_v, to_h;
        for (int v_delta : {-1, 1}) {
            for (int h_delta : {-1, 1}) {
                to_v = vertical + v_delta;
                to_h = horizontal + h_delta * 2;
                if (board->isMovableOrCapturable(to_v, to_h, color)) {
                    can_move.push_back(std::pair(to_v, to_h));
                }
                to_h -= h_delta;
                to_v += v_delta;
                if (board->isMovableOrCapturable(to_v, to_h, color)) {
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

class Bishop : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        std::vector<std::pair<int, int>> can_move;
        int to_v, to_h;
        for (int v_delta : {-1, 1}) {
            for (int h_delta : {-1, 1}) {
                to_v = vertical + v_delta;
                to_h = horizontal + h_delta;
                while (board->IsMovable(to_v, to_h)) {
                    can_move.push_back(std::pair(to_v, to_h));
                    to_v += v_delta;
                    to_h += h_delta;
                }
                if (board->IsCapturable(to_v, to_h, color))
                    can_move.push_back(std::pair(to_v, to_h));
            }
        }
        return can_move;
    }

    virtual bool IsChecking(Board* board, int end_v, int end_h) override {
        int to_v = vertical, to_h = horizontal;
        int v_delta = 1, h_delta = 1;
        if (to_v > end_v) v_delta = -1;
        if (to_h > end_h) h_delta = -1;
        to_v += v_delta;
        to_h += h_delta;
        while(to_v != end_v && to_h != end_h && board->IsMovable(to_v, to_h)) {
            to_v += v_delta;
            to_h += h_delta;
        }
        return to_v == end_v && to_h == end_h;
    }
};