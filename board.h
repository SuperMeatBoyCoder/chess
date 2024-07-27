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
        file_log << "Board was constructed\n";
    }
    
    ~Board() {
        file_log << "Board was deconstructed\n";
    }

    void AddFigure(std::shared_ptr<ChessPiece> new_piece) {
        int v, h;
        std::tie(v, h) = new_piece->GetPosition();
        chess_table[v][h] = new_piece;
        if (new_piece->GetType() == 'K') {
            if (new_piece->GetColor() == 'W')
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
        return chess_table[v][h]->GetColor();
    }

    char GetType(int v, int h) {
        return chess_table[v][h]->GetType();
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
                if (!IsEmpty(v, h) && chess_table[v][h]->GetColor() != king_color && chess_table[v][h]->IsChecking(this, king_v, king_h))
                    return true;
            }
        }
        return false;
    }
    
    chess_move Move(std::shared_ptr<ChessPiece> moving_piece, possible_move move, bool log_needed = true) {
        auto [start_v, start_h] = moving_piece->GetPosition();
        auto [end_v, end_h, special] = move;

        moving_piece->UpdatePosition(end_v, end_h);
        moving_piece->times_moved++;

        chess_move this_move;

        switch (special) {
        // normal move
        case 0:
            this_move = {moving_piece, start_v, start_h, chess_table[end_v][end_h]};
            break;
        // Castle
        case 1:
            break;
        //En Passaunt
        case 2:
            this_move = {moving_piece, start_v, start_h, chess_table[end_v][start_h], 2};
            chess_table[end_v][start_h] = nullptr;
            break;
        default:
            throw "invalid special move";
        }

        chess_table[start_v][start_h] = nullptr;
        chess_table[end_v][end_h] = moving_piece;

        if (log_needed) {
            move_log.push_back(this_move);
        }
        return this_move;
    }

    void Revert(chess_move last_move) {
        int start_v = last_move.start_v;
        int start_h = last_move.start_h;
        auto [end_v, end_h] = last_move.moved->GetPosition();
        last_move.moved->UpdatePosition(start_v, start_h);
        chess_table[start_v][start_h] = last_move.moved;
        last_move.moved->times_moved--;

        switch (last_move.special) {
        // normal move
        case 0:
            chess_table[end_v][end_h] = last_move.captured;
            break;
        // Castle
        case 1:
            break;
        //En Passaunt
        case 2:
            chess_table[end_v][end_h] = nullptr;
            chess_table[end_v][start_h] = last_move.captured;
            break;
        default:
            throw "invalid special move in revert";
        }
    }

    // most heavy code

    // writes possible moves in the provided vector
    void PossibleMovementChecked(std::shared_ptr<ChessPiece> this_figure, std::vector<possible_move>& can_move_checked) {
        int piece_v, piece_h;
        std::tie(piece_v, piece_h) = this_figure->GetPosition();
        for (possible_move move : this_figure->PossibleMovement(this)) {
            chess_move this_move = Move(this_figure, move, false);
            if (!CheckForCheck(this_figure->GetColor()))
                can_move_checked.push_back(move);
            Revert(this_move);
        }
    }
};