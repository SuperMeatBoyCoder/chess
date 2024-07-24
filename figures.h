#include "global_config.h"
#include <algorithm>

class Board {
private:
    std::vector<std::vector<Figure*>> chess_table;
    std::pair<int, int> white_king = std::pair(5, 1), black_king = std::pair(5, 8);
public:
    Board() {
        chess_table.resize(9, std::vector<Figure*>(9, nullptr));
    }

    bool IsEmpty(int v, int h) {
        return chess_table[v][h] == nullptr;
    }

    std::string GetColor(int v, int h) {
        return chess_table[v][h]->GetColor();
    }

    bool Isinside(int v, int h) {
        return 1 <= v && v <= 8 && 1 <= h && h <= 8;
    }

    bool CheckForCheck(int v, int h) {
        
    }
};

class Figure {
protected:
    int vertical = -1, horizontal = -1;
    std::string figure_type = "-", color;
    Board* board;
public:
    Figure(Board* b, int v, int h, std::string c, std::string f_type) :
           board(b), vertical(v), horizontal(h), color(c), figure_type(f_type) {
        log("Figure of type " + figure_type + " was constructed");
    }

    ~Figure() {
        log("Figure of type " + figure_type + " was deconstructed");
    }

    std::pair<int, int> GetPosition() {
        return std::pair(vertical, horizontal);
    }

    std::vector<std::pair<int, int>> PossibleMovement() {
        return {};
    }

    std::string GetColor() {
        return color;
    }
};

class Pawn : public Figure {
private:
    bool moved = false;
public:
    Pawn(Board* b, int v, int h, std::string c) : Figure(b, v, h, c, "Pawn") {}

    std::vector<std::pair<int, int>> PossibleMovement() {
        std::vector<std::pair<int, int>> can_move;
        int move_delta = 1;
        if (color == "black")
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
            if (!(board->IsEmpty(vertical + campture_delta, to_horizontal)) &&
                board->Isinside(vertical + campture_delta, to_horizontal) &&
                board->GetColor(vertical + campture_delta, to_horizontal) != color) {
                can_move.push_back(std::pair(vertical + campture_delta, to_horizontal));
            } 
        }
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
        return can_move;
    }

    bool CanMove(int v, int h) {
        std::vector<std::pair<int, int>> can_move = this->PossibleMovement();
        std::pair<int, int> need = std::pair(v, h);
        return std::find(can_move.begin(), can_move.end(), need) != can_move.end();
    }
};