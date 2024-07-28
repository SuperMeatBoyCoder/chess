#include "board.h"

namespace Chess {
class King : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    King(PieceInfo info) : ChessPiece(info) {file_log << "King was constructed\n";}
    ~King() {file_log << "King was deconstructed\n";}

    std::vector<ChessMove> PossibleMovement(Chess::Board* board) override {
        std::vector<ChessMove> can_move;
        ChessMove this_move;
        for (int v_delta : {-1, 0, 1}) {
            for (int h_delta : {-1, 0, 1}) {
                this_move.square = {m_square.v + v_delta, m_square.h + h_delta};
                if (board->isMovableOrCapturable(this_move.square, m_color)) {
                    this_move.captured_piece = board->GetPiecePtr(this_move.square);
                    can_move.push_back(this_move);
                    this_move.captured_piece = nullptr;
                }
            }
        }
        return can_move;
    }

    bool IsChecking(Board* board, Square king_square) override {
        return (abs(m_square.v - king_square.v) + abs(m_square.h - king_square.h)) <= 1;
    }
};


class Pawn : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Pawn(PieceInfo info) : ChessPiece(info) {file_log << "Pawn was constructed\n";}
    ~Pawn() {file_log << "Pawn was deconstructed\n";}

    std::vector<ChessMove> PossibleMovement(Board* board) override {
        std::vector<ChessMove> can_move;
        int h_delta = 1;
        if (m_color == 'B')
            h_delta = -1;
        ChessMove this_move = {m_square.v, m_square.h + h_delta};

        if (board->IsMovable(this_move.square)) {
            can_move.push_back(this_move);
            this_move.square.h += h_delta;
            if (times_moved == 0 && board->IsMovable(this_move.square)) {
                can_move.push_back(this_move);
            }
            this_move.square.h -= h_delta;
        }
        for (int v_delta : {-1, 1}) {
            this_move.square.v += v_delta;
            if (board->IsCapturable(this_move.square, m_color)) {
                this_move.captured_piece = board->GetPiecePtr(this_move.square);
                can_move.push_back(this_move);
                this_move.captured_piece = nullptr;
            }

            // En Passant
            if (board->IsCapturable({this_move.square.v, m_square.h}, m_color) && board->IsMovable(this_move.square)) {
                std::shared_ptr<ChessPiece> other_piece = board->GetPiecePtr({this_move.square.v, m_square.h});
                ChessMove last_move = board->GetLastMove();
                if (other_piece->GetType() == 'P' && other_piece->times_moved == 1 && last_move.moving_piece == other_piece) {
                    this_move.captured_piece = other_piece;
                    this_move.special = EN_PASSANT;
                    can_move.push_back(this_move);
                    this_move.captured_piece = nullptr;
                    this_move.special = NORMAL_MOVE;
                }
            }
            this_move.square.v -= v_delta;
        }

        return can_move;
    }

    bool IsChecking(Board* board, Square king_square) override {
        int h_delta = 1;
        if (m_color == 'B')
            h_delta = -1;
        return m_square.h + h_delta == king_square.h && abs(m_square.v - king_square.v) == 1;
    }
};

// Knight is Night. It's a feature, not a bug
class Night : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Night(PieceInfo info) : ChessPiece(info) {file_log << "Night was constructed\n";}
    ~Night() {file_log << "Night was deconstructed\n";}

    std::vector<ChessMove> PossibleMovement(Board* board) override {
        std::vector<ChessMove> can_move;
        ChessMove this_move;
        for (int v_delta : {-1, 1}) {
            for (int h_delta : {-1, 1}) {
                this_move.square = {m_square.v + v_delta, m_square.h + h_delta * 2};
                if (board->isMovableOrCapturable(this_move.square, m_color)) {
                    this_move.captured_piece = board->GetPiecePtr(this_move.square);
                    can_move.push_back(this_move);
                    this_move.captured_piece = nullptr;
                }
                this_move.square.h -= h_delta;
                this_move.square.v += v_delta;
                if (board->isMovableOrCapturable(this_move.square, m_color)) {
                    this_move.captured_piece = board->GetPiecePtr(this_move.square);
                    can_move.push_back(this_move);
                    this_move.captured_piece = nullptr;
                }
            }
        }
        return can_move;
    }

    bool IsChecking(Board* board, Square king_square) override {
        std::pair<int, int> check(abs(m_square.v - king_square.v), abs(m_square.h - king_square.h));
        if (check.first > check.second) std::swap(check.first, check.second);
        return check == std::pair(1, 2);
    }
};

class Bishop : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Bishop(PieceInfo info) : ChessPiece(info) {file_log << "Bishop was constructed\n";}
    ~Bishop() {file_log << "Bishop was deconstructed\n";}

    std::vector<ChessMove> PossibleMovement(Board* board) override {
        std::vector<ChessMove> can_move;
        ChessMove this_move;
        for (int v_delta : {-1, 1}) {
            for (int h_delta : {-1, 1}) {
                this_move.square = {m_square.v + v_delta, m_square.h + h_delta};
                while (board->IsMovable(this_move.square)) {
                    can_move.push_back(this_move);
                    this_move.square.v += v_delta;
                    this_move.square.h += h_delta;
                }
                if (board->IsCapturable(this_move.square, m_color)) {
                    this_move.captured_piece = board->GetPiecePtr(this_move.square);
                    can_move.push_back(this_move);
                    this_move.captured_piece = nullptr;
                }
            }
        }
        return can_move;
    }

    bool IsChecking(Board* board, Square king_square) override {
        Square to_square = m_square;
        int v_delta = 1, h_delta = 1;
        if (to_square.v > king_square.v) v_delta = -1;
        if (to_square.h > king_square.h) h_delta = -1;
        to_square.v += v_delta;
        to_square.h += h_delta;
        while(to_square.v != king_square.v && to_square.h != king_square.h && board->IsMovable(to_square)) {
            to_square.v += v_delta;
            to_square.h += h_delta;
        }
        return to_square == king_square;
    }
};

class Rook : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Rook(PieceInfo info) : ChessPiece(info) {file_log << "Rook was constructed\n";}
    ~Rook() {file_log << "Rook was deconstructed\n";}

    std::vector<ChessMove> PossibleMovement(Board* board) override {
        std::vector<ChessMove> can_move;
        ChessMove this_move;
        for (int v_delta : {-1, 1}) {
            this_move.square = {m_square.v + v_delta, m_square.h};
            while (board->IsMovable(this_move.square)) {
                can_move.push_back(this_move);
                this_move.square.v += v_delta;
            }
            if (board->IsCapturable(this_move.square, m_color)) {
                this_move.captured_piece = board->GetPiecePtr(this_move.square);
                can_move.push_back(this_move);
                this_move.captured_piece = nullptr;
            }
        }
        for (int h_delta : {-1, 1}) {
            this_move.square = {m_square.v, m_square.h + h_delta};
            while (board->IsMovable(this_move.square)) {
                can_move.push_back(this_move);
                this_move.square.h += h_delta;
            }
            if (board->IsCapturable(this_move.square, m_color)) {
                this_move.captured_piece = board->GetPiecePtr(this_move.square);
                can_move.push_back(this_move);
                this_move.captured_piece = nullptr;
            }
        }
        return can_move;
    }

    bool IsChecking(Board* board, Square king_square) override {
        Square to_square = m_square;
        int v_delta = 0, h_delta = 0;
        if (to_square.v < king_square.v) v_delta = 1;
        else if (to_square.v > king_square.v) v_delta = -1;
        if (to_square.h < king_square.h) h_delta = 1;
        else if (to_square.h > king_square.h) h_delta = -1;
        if (abs(v_delta) + abs(h_delta) != 1) return false;
        to_square.v += v_delta;
        to_square.h += h_delta;
        while(to_square.v != king_square.v && to_square.h != king_square.h && board->IsMovable(to_square)) {
            to_square.v += v_delta;
            to_square.h += h_delta;
        }
        return to_square == king_square;
    }
};

class Queen : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Queen(PieceInfo info) : ChessPiece(info) {file_log << "Queen was constructed\n";}
    ~Queen() {file_log << "Queen was deconstructed\n";}

    std::vector<ChessMove> PossibleMovement(Board* board) override {
        std::vector<ChessMove> can_move;
        ChessMove this_move;
        for (int v_delta : {-1, 0, 1}) {
            for (int h_delta : {-1, 0, 1}) {
                this_move.square = {m_square.v + v_delta, m_square.h + h_delta};
                while (board->IsMovable(this_move.square)) {
                    can_move.push_back(this_move);
                    this_move.square.v += v_delta;
                    this_move.square.h += h_delta;
                }
                if (board->IsCapturable(this_move.square, m_color)) {
                    this_move.captured_piece = board->GetPiecePtr(this_move.square);
                    can_move.push_back(this_move);
                    this_move.captured_piece = nullptr;
                }
            }
        }
        return can_move;
    }

    bool IsChecking(Board* board, Square king_square) override {
        Square to_square = m_square;
        int v_delta = 0, h_delta = 0;
        if (to_square.v < king_square.v) v_delta = 1;
        else if (to_square.v > king_square.v) v_delta = -1;
        if (to_square.h < king_square.h) h_delta = 1;
        else if (to_square.h > king_square.h) h_delta = -1;
        to_square.v += v_delta;
        to_square.h += h_delta;
        while(to_square.v != king_square.v && to_square.h != king_square.h && board->IsMovable(to_square)) {
            to_square.v += v_delta;
            to_square.h += h_delta;
        }
        return to_square == king_square;
    }
};
}