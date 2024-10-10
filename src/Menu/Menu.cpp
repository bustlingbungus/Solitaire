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

    font = TTF_OpenFont("../../assets/Jacquard12-Regular.ttf", 128);
    win = std::make_unique<LTexture>(window);
    win->loadFromRenderedText("VICTORY",{220,170,0,255}, font);
    menutxt = std::make_unique<LTexture>(window);
    menutxt->loadFromRenderedText("SOLITAIRE", {220,170,0,255}, font);

    TTF_CloseFont(font);
    font = TTF_OpenFont("../../assets/Jacquard12-Regular.ttf", 48);


    auto txt = std::make_unique<LTexture>(window);
    if (!txt->loadFromRenderedText("NEW GAME", {0,0,0,255}, font)) {
        std::cerr << "failed to create text!\n";
    }
    SDL_Rect rect = {
        (window->getWidth()-menutxt->getWidth())/2, 
        (window->getHeight()-menutxt->getHeight())/2 + 200, 
        txt->getWidth()*1.25, txt->getHeight()*1.25
    };
    auto tex = std::make_shared<LTexture>(window);
    tex->solidColour({255,255,255,255}, rect.w, rect.h);
    int x = (rect.w-txt->getWidth())/2, y = (rect.h-txt->getHeight())/2;
    txt->render_toTexture(tex.get(), x, y);

    auto button = std::make_shared<MenuButton>(window, tex, rect, &Menu::new_game, this);
    buttons.push_back(button);


    if (!txt->loadFromRenderedText("EXIT", {0,0,0,255}, font)) {
        std::cerr << "failed to create text!\n";
    }
    rect.x = (window->getWidth()+menutxt->getWidth())/2 - rect.w;
    tex = std::make_shared<LTexture>(window);
    tex->solidColour({255,255,255,255}, rect.w, rect.h);
    x = (rect.w-txt->getWidth())/2; y = (rect.h-txt->getHeight())/2;
    txt->render_toTexture(tex.get(), x, y);
    button = std::make_shared<MenuButton>(window, tex, rect, &Menu::exit, this);
    buttons.push_back(button);
}

/* Deallocates resources */
Menu::~Menu() {
    buttons.clear();
    win->free(); menutxt->free();
    TTF_CloseFont(font);
}

/* takes in user input. returns true for quit */
bool Menu::getInput(const SDL_Event& e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN:
            if (e.key.repeat == 0 && e.key.keysym.sym == SDLK_ESCAPE) {
                buttons_active = !buttons_active;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (buttons_active) {
                    for (auto button : buttons) button->press(x, y);
                } else {
                    solitaire->getInput(e);
                }
            }
            break;
        case SDL_MOUSEBUTTONUP: 
            solitaire->getInput(e);
            break;
    }
    return quit;
}

/* Renders the game and any menu buttons */
void Menu::render()
{
    solitaire->renderCards();

    if (solitaire->victory()) buttons_active = true;
    if (buttons_active)
    {
        for (auto button : buttons) button->render();
        if (solitaire->victory()) win->render((window->getWidth()-win->getWidth())/2,(window->getHeight()-win->getHeight())/2);
        else menutxt->render((window->getWidth()-menutxt->getWidth())/2,(window->getHeight()-menutxt->getHeight())/2);
    }
}

/* starts a new game of solitaire */
void Menu::new_game()
{
    solitaire->shuffle();
    solitaire->deal();
    buttons_active = false;
}

/* exits the game */
void Menu::exit() {
    quit = true;
}