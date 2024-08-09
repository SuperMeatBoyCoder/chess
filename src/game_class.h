#pragma once
#include "board.h"
#include "pieces.h"

namespace Chess {
class Game {
private:
    
    Board board;
    bool running = false;
    int move = 1;
    const char* colors = "BW";

    void Render();
    ChessPiece* InputPiece();
    ChessMove InputMove(ChessPiece* this_piece);

public:
    Game();
    ~Game();
    void Update();
    void Start();
    bool IsRunning();
    //for threads
    void End();
};
}