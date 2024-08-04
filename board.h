#pragma once
namespace Chess {
class Board {
    // v = verical, h = horizontal
    // verticals is used first due to traditional notation in chess
private:
    std::vector<SharedPiecePtr> m_chess_table;
    std::vector<ChessMove> m_move_log;
    SharedPiecePtr m_white_king, m_black_king;

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

public:
    Board();
    ~Board();
    SharedPiecePtr CreatePiecePtr(PieceInfo raw_piece);
    void AddFigure(SharedPiecePtr new_piece);
    // Returns pointer to the piece or nullptr if the square is empty
    SharedPiecePtr GetPiecePtr(Square square);
    char GetColor(Square square);
    char GetType(Square square);
    ChessMove GetLastMove();
    bool IsEmpty(Square square);
    bool Isinside(Square square);
    bool IsMovable(Square square);
    bool IsCapturable(Square square, char color);
    bool isMovableOrCapturable(Square square, char color);
    bool CheckForCheck(char king_color);
    ChessMove Move(ChessMove move, bool just_checking = false);
    void Revert(ChessMove last_move);
    void PossibleMovementChecked(SharedPiecePtr this_piece, std::vector<ChessMove>& can_move_checked);
    bool IsValidMove(SharedPiecePtr this_piece, const ChessMove& move);
};
}