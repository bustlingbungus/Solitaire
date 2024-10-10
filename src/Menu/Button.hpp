#ifndef BUTTON_HPP
#define BUTTON_HPP

// sdl
#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"

// std 
#include <iostream>

class Button
{
    public:
        /* Assigns variables */
        Button(std::shared_ptr<LWindow> window, std::shared_ptr<LTexture> tex, SDL_Rect rect);
        /* Deallocates resources */
        ~Button();

        /* Returns true if x, y is on the button */
        virtual bool press(const int& x, const int& y) const;

        /* renders the button onto the window */
        virtual void render();

    private:

        /* The window the button is rendered to */
        std::shared_ptr<LWindow> window = nullptr;

        /* the texture that gets rendered to the window */
        std::shared_ptr<LTexture> tex = nullptr;

        /* the button's hitbox */
        SDL_Rect rect = {0,0,0,0};
};

#endif