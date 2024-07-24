#pragma once
#include "global_config.h"

class Board;

class Figure {
protected:
    int vertical = -1, horizontal = -1;
    std::string figure_type = "-", color;
    bool captured = false;
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
    
    void UpdatePosition(int v, int h) {
        vertical = v;
        horizontal = h;
    }

    std::string GetColor() {
        return color;
    }
    
    std::string GetType() {
        return figure_type;
    }

    void SetCapture(bool cap) {
        captured = cap;
    }

    virtual std::vector<std::pair<int, int>> PossibleMovement(Board* board) {
        return {};
    }

    virtual bool IsChecking(Board* board, int end_v, int end_h) {
        return false;
    }
};