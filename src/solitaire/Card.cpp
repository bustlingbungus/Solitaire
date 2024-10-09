#include "Card.hpp"

/* Assigns variables */
Card::Card(Suit suit, Rank rank, std::shared_ptr<LWindow> window)
: suit(suit), rank(rank), window(window)
{
    bg0 = std::make_unique<LTexture>(window);
    bg1 = std::make_unique<LTexture>(window);
    bg0->solidColour({0,0,70,255});
    bg1->solidColour({255,255,255,255});
}

/* Deallocates resources */
Card::~Card() {}

/* Renders the card within the given rect */
void Card::render(SDL_Rect *rect, const double& rotation) const
{
    if (is_revealed) 
    {
        bg1->render(rect, NULL, rotation);
        auto tex = std::make_unique<LTexture>(window);
        tex->loadFromRenderedText("Suit: "+std::to_string(suit),{0,0,0,255});
        tex->render(rect->x+5, rect->y+5, NULL, NULL, rotation);
        tex->loadFromRenderedText("Rank: "+std::to_string(rank),{0,0,0,255});
        tex->render(rect->x+5, rect->y+5+tex->getHeight(), NULL, NULL, rotation);
        tex->free();
    }
    else 
    {
        bg0->render(rect, NULL, rotation);
    }
}

/* Returns true if the card is facing the screen */
bool Card::isRevealed() const { return is_revealed; }

/* Toggles revealed on/off */
void Card::flip() {
    is_revealed = !is_revealed;
}

Suit Card::getSuit() const { return suit; }
Rank Card::getRank() const { return rank; }