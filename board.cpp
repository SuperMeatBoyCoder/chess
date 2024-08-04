#include "board.h"
#include "chess_piece.h"

namespace Chess {

Board::Board() {
        m_chess_table.resize(73, nullptr);
        for (const PieceInfo raw_piece : default_board_config) {
            AddFigure(CreatePiecePtr(raw_piece));
        }
        log("Board was constructed");
}

Board::~Board() {
    log("Board was deconstructed");
}

void Board::AddFigure(SharedPiecePtr new_piece) {
    Square square;
    square = new_piece->GetPosition();
    m_chess_table[square] = new_piece;
    if (new_piece->GetType() == 'K') {
        if (new_piece->GetColor() == 'W')
            m_white_king = new_piece;
        else
            m_black_king = new_piece;
    }
}

SharedPiecePtr Board::GetPiecePtr(Square square) {
    return m_chess_table[square];
}

char Board::GetColor(Square square) {
    return m_chess_table[square]->GetColor();
}

char Board::GetType(Square square) {
    return m_chess_table[square]->GetType();
}

ChessMove Board::GetLastMove() {
    return m_move_log.back();
}

bool Board::IsEmpty(Square square) {
    return m_chess_table[square] == nullptr;
}

bool Board::Isinside(Square square) {
    return 1 <= square.v && square.v <= 8 && 1 <= square.h && square.h <= 8;
}

bool Board::IsMovable(Square square) {
    return Isinside(square) && IsEmpty(square);
}

bool Board::IsCapturable(Square square, char color) {
    return Isinside(square) && !IsEmpty(square) && GetColor(square) != color;
}

bool Board::isMovableOrCapturable(Square square, char color) {
    return Isinside(square) && (IsEmpty(square) || GetColor(square) != color);
}

bool Board::CheckForCheck(char king_color) {
    Square king_square;
    if (king_color == 'B') {
        king_square = m_black_king->GetPosition();
    }
    else {
        king_square = m_white_king->GetPosition();
    }
    for (int v = 1; v <= 8; v++) {
        for (int h = 1; h <= 8; h++) {
            SharedPiecePtr this_piece = GetPiecePtr({v, h});
            if (this_piece != nullptr && this_piece->GetColor() != king_color && this_piece->IsChecking(this, king_square))
                return true;
        }
    }
    return false;
}

ChessMove Board::Move(ChessMove move, bool just_checking) {
    auto [end_square, captured_piece, moving_piece, special, done] = move;
    Square start_square = moving_piece->GetPosition();
    assert(!done);

    switch (special) {
    case NORMAL_MOVE:
        break;
    case SHORT_CASTLE:
        end_square.v = 7;
        Move({{6, end_square.h}, nullptr, GetPiecePtr({8, end_square.h})}, true);
        break;
    case LONG_CASTLE:
        end_square.v = 3;
        Move({{4, end_square.h}, nullptr, GetPiecePtr({1, end_square.h})}, true);
        break;
    case EN_PASSANT:
        m_chess_table[end_square.v * 8 + start_square.h] = nullptr;
        break;
    case PROMOTION:
        // in this case I expect moving_piece to be a new piece
        assert(moving_piece->GetType() != 'P');
        break;
    default:
        throw "invalid special move";
    }

    moving_piece->UpdatePosition(end_square);
    moving_piece->times_moved++;
    m_chess_table[start_square] = nullptr;
    m_chess_table[end_square] = moving_piece;

    move.square = start_square;
    move.done = true;

    if (!just_checking) {
        m_move_log.push_back(move);
    }
    return move;
}

void Board::Revert(ChessMove last_move) {
    auto [start_square, captured_piece, moving_piece, special, done] = last_move;
    Square end_square = moving_piece->GetPosition();
    assert(done);

    switch (special) {
    case NORMAL_MOVE:
        break;
    case SHORT_CASTLE:
        end_square.v = 7;
        Revert({{8, end_square.h}, nullptr, GetPiecePtr({6, end_square.h}), NORMAL_MOVE, true});
        break;
    case LONG_CASTLE:
        end_square.v = 3;
        Revert({{1, end_square.h}, nullptr, GetPiecePtr({4, end_square.h}), NORMAL_MOVE, true});
        break;
    case EN_PASSANT:
        m_chess_table[end_square.v * 8 + start_square.h] = captured_piece;
        captured_piece = nullptr;
        break;
    case PROMOTION:
        // in this case I expect moving_piece to be a pawn
        assert(moving_piece->GetType() == 'P');
        break;
    default:
        throw "invalid special move in revert";
    }

    m_chess_table[end_square] = captured_piece;
    moving_piece->UpdatePosition(start_square);
    moving_piece->times_moved--;
    m_chess_table[start_square] = moving_piece;
}

// writes possible moves in the provided vector
void Board::PossibleMovementChecked(SharedPiecePtr this_piece, std::vector<ChessMove>& can_move_checked) {
    Square piece_square = this_piece->GetPosition();
    for (ChessMove& move : this_piece->PossibleMovement(this)) {
        move.moving_piece = this_piece;
        if (IsValidMove(this_piece, move))
            can_move_checked.push_back(move);
    }
}

bool Board::IsValidMove(SharedPiecePtr this_piece, const ChessMove& move) {
    ChessMove helper_move = move;
    if (helper_move.special == PROMOTION)
        // promotion being just a movement of pawn can't influence check to the king
        helper_move.special = NORMAL_MOVE;
    else if (helper_move.special == SHORT_CASTLE) {
        // castle is possible only if starting square, end square and squre between them are not attacked.
        // here we check starting square and square between
        helper_move.special = NORMAL_MOVE;
        for (int v : {5, 6}) {
            helper_move.square.v = v;
            if (!IsValidMove(this_piece, helper_move))
                return false;
        }
        helper_move.square.v = 7;
        helper_move.special = SHORT_CASTLE;
    }
    else if (helper_move.special == LONG_CASTLE) {
        // same as short castle
        helper_move.special = NORMAL_MOVE;
        for (int v : {4, 5}) {
            helper_move.square.v = v;
            if (!IsValidMove(this_piece, helper_move))
                return false;
        }
        helper_move.square.v = 5;
        helper_move.special = SHORT_CASTLE;
    }
    helper_move = Move(helper_move, true);
    bool valid = true;
    if (CheckForCheck(this_piece->GetColor()))
        valid = false;
    Revert(helper_move);
    return valid;
}
}
