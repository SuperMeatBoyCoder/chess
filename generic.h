#pragma once
#include "global_config.h"

class Figure {
protected:
    int vertical = -1, horizontal = -1;
    std::string figure_type = "-", color;
public:
    Figure(int v, int h, std::string c, std::string f_type) :
           vertical(v), horizontal(h), color(c), figure_type(f_type) {
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
    
    std::string GetType() {
        return figure_type;
    }
};

struct chess_move {
    std::string piece_type = "-";
    int start_v, start_h, end_v, end_h;
    Figure* captured = nullptr;
};

class Board {
private:
    std::vector<std::vector<Figure*>> chess_table;
    std::vector<chess_move> move_log;
    std::pair<int, int> white_king = std::pair(5, 1), black_king = std::pair(5, 8);
public:
    Board() {
        chess_table.resize(9, std::vector<Figure*>(9, nullptr));
    }

    void AddFigure(Figure* f, int v, int h) {
        chess_table[v][h] = f;
    }

    bool IsEmpty(int v, int h) {
        return chess_table[v][h] == nullptr;
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
        //placeholder
        return false;
    }

    void Move(int start_v, int start_h, int end_v, int end_h) {
        if (!chess_table[start_v][start_h]) {
            throw "InvalidMovement";
        }
        move_log.push_back({chess_table[start_v][start_h]->GetType(), start_v, start_h, end_v, end_h, chess_table[end_v][end_h]});
        chess_table[end_v][end_h] = chess_table[start_v][start_h];
        chess_table[start_v][start_h] = nullptr;
    }

    void Revert() {
        if (move_log.empty()) {
            throw "InvalidRevert";
        }
        chess_move last_move = move_log.back();
        
    }
};