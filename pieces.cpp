#include "src/board.h"
#include "src/pieces.h"

namespace Chess {

std::vector<ChessMove> King::PossibleMovement(const Board* board) const {
    std::vector<ChessMove> can_move;
    ChessMove this_move;
    // We check each square in 3x3 area, because the king's square is not movable.
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
    // Castle
    if (times_moved == 0) {
        for (int rook_v : {1, 8}) {
            this_move.special = rook_v == 1 ? LONG_CASTLE : SHORT_CASTLE;
            bool pieces_between = false;
            this_move.square = {rook_v, m_square.h};
            ChessPiece* rook = board->GetPiecePtr(this_move.square);
            if (rook->GetType() == 'R' && rook->times_moved == 0) {
                for (int v = std::min(m_square.v, rook_v) + 1; v < std::max(rook_v, m_square.v); v++) {
                    if (!board->IsEmpty({v, m_square.h})) pieces_between = true;
                }
                if (!pieces_between) {
                    can_move.push_back(this_move);
                    if (rook_v == 1) this_move.square.v += 2;
                    else this_move.square.v--;
                    can_move.push_back(this_move);
                }
            }
        }
    }
    return can_move;
}


bool King::IsChecking(const Board* board, Square king_square) const {
    return abs(m_square.v - king_square.v) <= 1 && abs(m_square.h - king_square.h) <= 1;
}

std::vector<ChessMove> Pawn::PossibleMovement(const Board* board) const {
    std::vector<ChessMove> can_move;
    int h_delta = 1;
    if (m_color == 'B')
        h_delta = -1;
    ChessMove this_move = {m_square.v, m_square.h + h_delta};
    
    if (this_move.square.h == 8 || this_move.square.h == 1) {
        this_move.special = PROMOTION;
    }

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
            ChessPiece* other_pawn = board->GetPiecePtr({this_move.square.v, m_square.h});
            ChessMove last_move = board->GetLastMove();
            if (other_pawn->GetType() == 'P' && other_pawn->times_moved == 1 && last_move.moving_piece == other_pawn) {
                this_move.captured_piece = other_pawn;
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

bool Pawn::IsChecking(const Board* board, Square king_square) const {
    int h_delta = 1;
    if (m_color == 'B')
        h_delta = -1;
    return m_square.h + h_delta == king_square.h && abs(m_square.v - king_square.v) == 1;
}

std::vector<ChessMove> Night::PossibleMovement(const Board* board) const {
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

bool Night::IsChecking(const Board* board, Square king_square) const {
    std::pair<int, int> check(abs(m_square.v - king_square.v), abs(m_square.h - king_square.h));
    if (check.first > check.second) std::swap(check.first, check.second);
    return check == std::pair(1, 2);
}

std::vector<ChessMove> Bishop::PossibleMovement(const Board* board) const {
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

bool Bishop::IsChecking(const Board* board, Square king_square) const {
    Square to_square = m_square;
    int v_delta = 1, h_delta = 1;
    if (to_square.v > king_square.v) v_delta = -1;
    if (to_square.h > king_square.h) h_delta = -1;
    to_square.v += v_delta;
    to_square.h += h_delta;
    while(to_square != king_square && board->IsMovable(to_square)) {
        to_square.v += v_delta;
        to_square.h += h_delta;
    }
    return to_square == king_square;
}

std::vector<ChessMove> Rook::PossibleMovement(const Board* board) const {
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

bool Rook::IsChecking(const Board* board, Square king_square) const {
    Square to_square = m_square;
    int v_delta = 0, h_delta = 0;
    if (to_square.v < king_square.v) v_delta = 1;
    else if (to_square.v > king_square.v) v_delta = -1;
    if (to_square.h < king_square.h) h_delta = 1;
    else if (to_square.h > king_square.h) h_delta = -1;
    if (abs(v_delta) + abs(h_delta) != 1) return false;
    to_square.v += v_delta;
    to_square.h += h_delta;
    while(to_square != king_square && board->IsMovable(to_square)) {
        to_square.v += v_delta;
        to_square.h += h_delta;
    }
    return to_square == king_square;
}

std::vector<ChessMove> Queen::PossibleMovement(const Board* board) const {
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

bool Queen::IsChecking(const Board* board, Square king_square) const {
    Square to_square = m_square;
    int v_delta = 0, h_delta = 0;
    if (to_square.v < king_square.v) v_delta = 1;
    else if (to_square.v > king_square.v) v_delta = -1;
    if (to_square.h < king_square.h) h_delta = 1;
    else if (to_square.h > king_square.h) h_delta = -1;
    to_square.v += v_delta;
    to_square.h += h_delta;
    while(to_square != king_square && board->IsMovable(to_square)) {
        to_square.v += v_delta;
        to_square.h += h_delta;
    }
    return to_square == king_square;
}
}