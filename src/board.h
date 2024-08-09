#pragma once
#include "structs.h"
namespace Chess {
class Board {
    // v = verical, h = horizontal
    // verticals is used first due to traditional notation in chess
private:
    std::vector<std::unique_ptr<ChessPiece>> m_all_pieces;
    std::vector<ChessPiece*> m_chess_table;
    std::map<const ChessPiece*, std::vector<ChessMove>> m_moves_table;
    std::vector<ChessMove> m_move_log;
    ChessPiece* m_white_king;
    ChessPiece* m_black_king;

    const std::vector<PieceInfo> default_board_config = {
        {1, 2, 'W', 'P'}, {2, 2, 'W', 'P'}, {3, 2, 'W', 'P'}, {4, 2, 'W', 'P'},
        {5, 2, 'W', 'P'}, {6, 2, 'W', 'P'}, {7, 2, 'W', 'P'}, {8, 2, 'W', 'P'},
        {1, 7, 'B', 'P'}, {2, 7, 'B', 'P'}, {3, 7, 'B', 'P'}, {4, 7, 'B', 'P'},
        {5, 7, 'B', 'P'}, {6, 7, 'B', 'P'}, {7, 7, 'B', 'P'}, {8, 7, 'B', 'P'},
        {2, 1, 'W', 'N'}, {7, 1, 'W', 'N'}, {2, 8, 'B', 'N'}, {7, 8, 'B', 'N'},
        {3, 1, 'W', 'B'}, {6, 1, 'W', 'B'}, {3, 8, 'B', 'B'}, {6, 8, 'B', 'B'},
        {1, 1, 'W', 'R'}, {8, 1, 'W', 'R'}, {1, 8, 'B', 'R'}, {8, 8, 'B', 'R'},
        {4, 1, 'W', 'Q'}, {4, 8, 'B', 'Q'},
        {5, 1, 'W', 'K'}, {5, 8, 'B', 'K'},
        };

    void PossibleMovementChecked(ChessPiece* this_piece, std::vector<ChessMove>& can_move_checked);

public:
    Board();
    ~Board();
    ChessPiece* CreatePiecePtr(PieceInfo raw_piece);
    void AddFigure(ChessPiece* new_piece);
    // Returns pointer to the piece or nullptr if the square is empty
    ChessPiece* GetPiecePtr(Square square) const;
    char GetColor(Square square) const;
    char GetType(Square square) const;
    ChessMove GetLastMove() const;
    bool IsEmpty(Square square) const;
    bool Isinside(Square square) const;
    bool IsMovable(Square square) const;
    bool IsCapturable(Square square, char color) const;
    bool isMovableOrCapturable(Square square, char color) const;
    bool CheckForCheck(char king_color) const;
    ChessMove Move(ChessMove move, bool just_checking = false);
    void Revert(ChessMove last_move);
    const std::vector<ChessMove>& PossibleMovement(const ChessPiece* this_piece);
    bool IsValidMove(ChessPiece* this_piece, const ChessMove& move);
    // Finds and stores all possible moves, returns false if checkmate
    bool FindAllMoves(char moving_color);
};
}