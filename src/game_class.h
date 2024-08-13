#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "board.h"
#include "pieces.h"
#include <SDL2/SDL.h>

namespace Chess {
class Game {
private:
    
    Board board;
    bool running = false;
    int move = 1;
    const char* colors = "BW";
    SDL_Window* Window;
    const int WIDTH, HEIGHT;

    void Render();
    ChessPiece* InputPiece();
    ChessMove InputMove(ChessPiece* this_piece);

public:
    Game(const int W, const int H);
    ~Game();
    void Update();
    void Start();
    bool IsRunning();
    //for threads
    void End();
};
}
#endif // GAME_CLASS_H