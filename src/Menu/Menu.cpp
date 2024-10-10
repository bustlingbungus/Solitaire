#include "Menu.hpp"

/* ==========  MENU BUTTON  ========== */

/* Assigns variables */
Menu::MenuButton::MenuButton(std::shared_ptr<LWindow> window, std::shared_ptr<LTexture> tex, SDL_Rect rect, void (Menu::*func)(), Menu *menu)
: Button(window, tex, rect), func(func), menu(menu) {}

/* deallocates resources  */
Menu::MenuButton::~MenuButton() {
    Button::~Button();  
}

/* executes function if the button was pressed */
bool Menu::MenuButton::press(const int& x, const int& y) const {
    bool pressed = Button::press(x, y);
    if (pressed) (menu->*func)();
    return pressed;
}


/* ==========  MENU  ========== */

/* Assign variables */
Menu::Menu(std::shared_ptr<LWindow> window)
: window(window)
{
    solitaire = std::make_unique<Solitaire>(window);
    solitaire->shuffle();
    solitaire->deal();

    win = std::make_unique<LTexture>(window);
    win->loadFromRenderedText("VICTORY",{0,255,0,255});
    menutxt = std::make_unique<LTexture>(window);
    menutxt->loadFromRenderedText("SOLITAIRE", {0,255,0,255});
}

/* Deallocates resources */
Menu::~Menu() {
    buttons.clear();
    win->free(); menutxt->free();
}

/* Renders the game and any menu buttons */
