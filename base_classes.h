#pragma once
#include "global_config.h"

class Board;

class Figure {
protected:
    int vertical = -1, horizontal = -1;
    std::string figure_type = "-", color;
public:
    Figure(int v, int h, std::string c, std::string f_type) :
           vertical(v), horizontal(h), color(c), figure_type(f_type) {
        file_log << "Figure of type " + figure_type + " was constructed\n";
    }

    virtual ~Figure() {
        file_log << "Figure of type " + figure_type + " was deconstructed\n";
    }

    std::pair<int, int> GetPosition() {
        return std::pair(vertical, horizontal);
    }

    std::string GetColor() {
        return color;
    }
    
    std::string GetType() {
        return figure_type;
    }

    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) {
        return {};
    }
};

struct chess_move {
    std::string piece_type = "-";
    int start_v, start_h, end_v, end_h;
    std::shared_ptr<Figure> captured = nullptr;
};

class Board {
private:
    std::vector<std::vector<std::shared_ptr<Figure>>> chess_table;
    std::vector<chess_move> move_log;
    std::pair<int, int> white_king = std::pair(5, 1), black_king = std::pair(5, 8);
    std::vector<std::shared_ptr<Figure>>* white_pieces;
    std::vector<std::shared_ptr<Figure>>*  black_pieces;
public:
    Board(std::vector<std::shared_ptr<Figure>>* white, std::vector<std::shared_ptr<Figure>>* black) :
          white_pieces(white), black_pieces(black) {
        chess_table.resize(9, std::vector<std::shared_ptr<Figure>>(9, nullptr));
    }

    void AddFigure(std::shared_ptr<Figure> f, int v, int h) {
        chess_table[v][h] = f;
    }

    bool IsEmpty(int v, int h) {
        return chess_table[v][h] == nullptr;
    }

    std::shared_ptr<Figure> GetFigure(int v, int h) {
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