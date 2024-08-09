#include "src/game_class.h"
#include <SDL2/SDL.h>

const int WIDTH = 800, HEIGHT = 600;

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* Window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    if (Window == NULL) {
        std::cout << "Could not create a window!" << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }   
    SDL_Event window_event;
    do {
        SDL_PollEvent(&window_event);
    }
    while (window_event.type != SDL_QUIT);
    SDL_DestroyWindow(Window);
    SDL_Quit();

    Chess::Game chess_game;
    chess_game.Start();
    std::cin.get();
    return EXIT_SUCCESS;
}