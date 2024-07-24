#pragma once
#include "chess_piece.h"

struct chess_move {
    std::shared_ptr<ChessPiece> moved;
    int start_v, start_h;
    std::shared_ptr<ChessPiece> captured;
};

class Board {
private:
    std::vector<std::vector<std::shared_ptr<ChessPiece>>> chess_table;
    std::vector<chess_move> move_log;
    std::vector<std::shared_ptr<ChessPiece>>* white_pieces;
    std::vector<std::shared_ptr<ChessPiece>>* black_pieces;
    std::shared_ptr<ChessPiece> white_king, black_king;
public:
    Board(std::vector<std::shared_ptr<ChessPiece>>* white, std::vector<std::shared_ptr<ChessPiece>>* black,
          std::shared_ptr<ChessPiece> w_king, std::shared_ptr<ChessPiece> b_king) :
          white_pieces(white), black_pieces(black), white_king(w_king), black_king(b_king) {
        chess_table.resize(9, std::vector<std::shared_ptr<ChessPiece>>(9, nullptr));
    }

    void AddFigure(std::shared_ptr<ChessPiece> f) {
        int v, h;
        std::tie(v, h) = f->GetPosition();
        chess_table[v][h] = f;
    }

    bool IsEmpty(int v, int h) {
        if (!Isinside(v, h)) return true;
        return chess_table[v][h] == nullptr;
    }

    std::shared_ptr<ChessPiece> GetFigure(int v, int h) {
        return chess_table[v][h];
    }

    std::string GetColor(int v, int h) {
        if (IsEmpty(v, h)) return "-";
        return chess_table[v][h]->GetColor();
    }

    std::string GetType(int v, int h) {
        if (IsEmpty(v, h)) return "-";
        return chess_table[v][h]->GetType();
    }

    bool Isinside(int v, int h) {
        return 1 <= v && v <= 8 && 1 <= h && h <= 8;
    }

    bool CheckForCheck(std::string king_color) {
        int king_v, king_h;
        if (king_color == "Black") {
            std::tie(king_v, king_h) = black_king->GetPosition();
            for (size_t i = 0; i < white_pieces->size(); i++) {
                if (white_pieces->at(i)->IsChecking(this, king_v, king_h)) return true;
            }
        }
        else {
            std::tie(king_v, king_h) = white_king->GetPosition();
            for (size_t i = 0; i < black_pieces->size(); i++) {
                if (black_pieces->at(i)->IsChecking(this, king_v, king_h)) return true;
            }
        }
        return false;
    }

    void Move(int start_v, int start_h, int end_v, int end_h) {
        std::shared_ptr<ChessPiece> f = chess_table[start_v][start_h];
        f->UpdatePosition(end_v, end_h);
        f->times_moved++;
        if (chess_table[end_v][end_h] != nullptr) chess_table[end_v][end_h]->captured = true;
        move_log.push_back({f, start_v, start_h, chess_table[end_v][end_h]});
        chess_table[end_v][end_h] = f;
        chess_table[start_v][start_h] = nullptr;
    }

    void Move(std::shared_ptr<ChessPiece> f, int end_v, int end_h) {
        int start_v, start_h;
        std::tie(start_v, start_h) = f->GetPosition();
        Move(start_v, start_h, end_v, end_h);
    }

    void Revert() {
        chess_move last_move = move_log.back();
        move_log.pop_back();
        int end_v, end_h;
        std::tie(end_v, end_h) = last_move.moved->GetPosition();
        last_move.moved->UpdatePosition(last_move.start_v, last_move.start_h);
        last_move.moved->times_moved--;
        if (last_move.captured != nullptr) last_move.captured->captured = false;
        chess_table[last_move.start_v][last_move.start_h] = last_move.moved;
        chess_table[end_v][end_h] = last_move.captured;
    }
};