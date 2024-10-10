#define SDL_MAIN_HANDLED

// sdl
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "sdl/LWindow.hpp"
#include "sdl/LTexture.hpp"
#include "sdl/LAudio.hpp"

// std
#include <iostream>
#include <memory>

// solitaire
#include "solitaire/Card.hpp"
#include "solitaire/Solitaire.hpp"
#include "solitaire/CardCreator.hpp"

int main()
{
    // window dimensions
    int WIDTH = 1280, HEIGHT = 720;
    auto window = std::make_shared<LWindow>(WIDTH, HEIGHT, "Solitaire");

    cardCreator.Init(window);
    // create solitaire game
    Solitaire solitaire(window);
    solitaire.shuffle();
    solitaire.deal();

    SDL_Event e;
    bool quit = false;
    // main window loop
    while (!quit)
    {
        // handle events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
            window->handleEvent(e);
            solitaire.getInput(e);
        }

        // render when not minimised
        if (!window->isMinimized())
        {
            // clear the window
            SDL_SetRenderDrawColor(window->gRenderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(window->gRenderer);

            // render cards
            solitaire.renderCards();

            // update the window
            SDL_RenderPresent(window->gRenderer);
        }
    }

    return 0;
}