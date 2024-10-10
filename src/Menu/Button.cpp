#include "Button.hpp"

/* Assign variables */
Button::Button(std::shared_ptr<LWindow> window, std::shared_ptr<LTexture> tex, SDL_Rect rect)
: window(window), tex(tex), rect(rect) {}

/* Deallocates resources */
Button::~Button()
{
    tex->free();
}

/* Returns true if x, y is on the button */
bool Button::press(const int& x, const int& y) const {
    return (x>=rect.x && x<=rect.x+rect.w) && (y>=rect.y && y<=rect.y+rect.h);
}

/* renders the button onto the window */
void Button::render() {
    tex->render(&rect);
}