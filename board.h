#pragma once
#include "chess_piece.h"

class Board {
private:
    std::vector<std::vector<std::shared_ptr<ChessPiece>>> chess_table;
    std::vector<chess_move> move_log;
    std::shared_ptr<ChessPiece> white_king, black_king = nullptr;
public:
    Board() {
        chess_table.resize(9, std::vector<std::shared_ptr<ChessPiece>>(9, nullptr));
    }

    void AddFigure(std::shared_ptr<ChessPiece> new_piece) {
        int v, h;
        std::tie(v, h) = new_piece->GetPosition();
        chess_table[v][h] = new_piece;
        if (new_piece->figure_type == "King") {
            if (new_piece->color == 'W')
                white_king = new_piece;
            else
                black_king = new_piece;
        }
    }

    bool IsEmpty(int v, int h) {
        return chess_table[v][h] == nullptr;
    }

    std::shared_ptr<ChessPiece> GetFigurePtr(int v, int h) {
        return chess_table[v][h];
    }

    char GetColor(int v, int h) {
        return chess_table[v][h]->color;
    }

    std::string GetType(int v, int h) {
        return chess_table[v][h]->figure_type;
    }

    chess_move GetLastMove() {
        return move_log.back();
    }

    bool Isinside(int v, int h) {
        return 1 <= v && v <= 8 && 1 <= h && h <= 8;
    }

    bool IsMovable(int v, int h) {
        return Isinside(v, h) && IsEmpty(v, h);
    }

    bool IsCapturable(int v, int h, char color) {
        return Isinside(v, h) && !IsEmpty(v, h) && GetColor(v, h) != color;
    }

    bool isMovableOrCapturable(int v, int h, char color) {
        return Isinside(v, h) && (IsEmpty(v, h) || GetColor(v, h) != color);
    }

    bool CheckForCheck(char king_color) {
        int king_v, king_h;
        if (king_color == 'B') {
            std::tie(king_v, king_h) = black_king->GetPosition();
        }
        else {
            std::tie(king_v, king_h) = white_king->GetPosition();
        }
        for (int v = 1; v <= 8; v++) {
            for (int h = 1; h <= 8; h++) {
                if (!IsEmpty(v, h) && chess_table[v][h]->color != king_color && chess_table[v][h]->IsChecking(*this, king_v, king_h))
                    return true;
            }
        }
        return false;
    }

    void Move(int start_v, int start_h, int end_v, int end_h) {
        std::shared_ptr<ChessPiece> moving_piece = chess_table[start_v][start_h];
        moving_piece->UpdatePosition(end_v, end_h);
        moving_piece->times_moved++;
        move_log.push_back({moving_piece, start_v, start_h, chess_table[end_v][end_h]});
        chess_table[end_v][end_h] = moving_piece;
        chess_table[start_v][start_h] = nullptr;
    }

    void Move(std::shared_ptr<ChessPiece> moving_piece, int end_v, int end_h) {
        int start_v, start_h;
        std::tie(start_v, start_h) = moving_piece->GetPosition();
        Move(start_v, start_h, end_v, end_h);
    }

    void Revert() {
        chess_move last_move = move_log.back();
        move_log.pop_back();
        int end_v, end_h;
        std::tie(end_v, end_h) = last_move.moved->GetPosition();
        last_move.moved->UpdatePosition(last_move.start_v, last_move.start_h);
        last_move.moved->times_moved--;
        chess_table[last_move.start_v][last_move.start_h] = last_move.moved;
        chess_table[end_v][end_h] = last_move.captured;
    }
};