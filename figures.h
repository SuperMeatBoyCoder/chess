#pragma once
#include "base_classes.h"

class Pawn : public Figure {
private:
    bool moved = false;
public:
    Pawn(int v, int h, std::string c) : Figure(v, h, c, "Pawn") {}

    std::vector<std::pair<int, int>> PossibleMovement(Board* board) override {
        std::vector<std::pair<int, int>> can_move;
        int move_delta = 1;
        if (color == "Black")
            move_delta = -1;
        int to_horizontal = horizontal + move_delta;

        // funny stuff: pawns can't be limited by the border when going straight
        if (board->IsEmpty(vertical, to_horizontal)) {
            can_move.push_back(std::pair(vertical, to_horizontal));
            if (!moved && board->IsEmpty(vertical, to_horizontal + move_delta)) {
                can_move.push_back(std::pair(vertical, to_horizontal + move_delta));
            }
        }
        for (int campture_delta : {-1, 1}) {
            if (!board->IsEmpty(vertical + campture_delta, to_horizontal) &&
                board->Isinside(vertical + campture_delta, to_horizontal) &&
                board->GetColor(vertical + campture_delta, to_horizontal) != color) {
                can_move.push_back(std::pair(vertical + campture_delta, to_horizontal));
            } 
        }
        std::vector<std::pair<int, int>> can_move_checked;
        // TODO
        /*
        for (std::pair<int, int> move : can_move) {
            if (!board->CheckForCheck(color))
                can_move_checked.push_back(move);
        }
        */
        can_move_checked = can_move;
        /*
        if (!(board->IsEmpty(vertical - 1, to_horizontal)) &&
            board->Isinside(vertical - 1, to_horizontal) &&
            board->GetColor(vertical - 1, to_horizontal) != color) {
            can_move.push_back(std::pair(vertical - 1, to_horizontal));
        }
        if (!(board->IsEmpty(vertical + 1, to_horizontal)) &&
            board->Isinside(vertical + 1, to_horizontal) &&
            board->GetColor(vertical + 1, to_horizontal) != color) {
            can_move.push_back(std::pair(vertical + 1, to_horizontal));
        }
        */
        return can_move_checked;
    }

    bool CanMove(Board* board, std::vector<Figure*> white_pieces, std::vector<Figure*> black_pieces, int end_v, int end_h) {
        std::vector<std::pair<int, int>> can_move = this->PossibleMovement(board);
        std::pair<int, int> need = std::pair(end_v, end_h);
        return std::find(can_move.begin(), can_move.end(), need) != can_move.end();
    }
};