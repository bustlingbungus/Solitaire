#ifndef MENU_HPP
#define MENU_HPP

// sdl
#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"

// std
#include <iostream>
#include <vector>

// solitaire
#include "../solitaire/Solitaire.hpp"

#include "Button.hpp"

/* A menu for managing a solitaire game */
class Menu
{
    public:
        /* Assign variables */
        Menu(std::shared_ptr<LWindow> window);
        /* Deallocate resources */
        ~Menu();

        /* Renders the game and any menu buttons */
        void render();

        /* takes in user input. returns true for quit */
        bool getInput(const SDL_Event& e);

    private:

        /* ==========  MENU BUTTON CLASS DEFINITION  ========== */
        class MenuButton : public Button
        {
            public:
                // Assigns variables 
                MenuButton(std::shared_ptr<LWindow> window, std::shared_ptr<LTexture> tex, SDL_Rect rect, void (Menu::*func)(), Menu *menu);
                // Deallocates resources
                ~MenuButton();
                // executes function if the button was pressed 
                virtual bool press(const int& x, const int& y) const;
            private:
                // the function that gets executed on button press
                void (Menu::*func)();
                // the menu the button belongs to 
                Menu *menu = nullptr;
        };


        /* ==========  MENU VARIABLES  ========== */

        /* the window the game is rendered to */
        std::shared_ptr<LWindow> window = nullptr;

        /* the actual solitaire game */
        std::unique_ptr<Solitaire> solitaire = nullptr;

        /* a vector of menu buttons */
        std::vector<std::shared_ptr<MenuButton>> buttons;
        /* Whether to show the buttons, and take input for buttons */
        bool buttons_active = true;

        /* text to render */
        std::unique_ptr<LTexture> win = nullptr, menutxt = nullptr;

        TTF_Font *font = nullptr;

        /* Starts a new game of solitaire */
        void new_game();
        /* exits the game */
        void exit();
        bool quit = false;
};

#endif