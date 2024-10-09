#include "Card.hpp"
#include "CardCreator.hpp"

/* Assigns variables */
Card::Card(Suit suit, Rank rank, std::shared_ptr<LWindow> window)
: suit(suit), rank(rank), window(window)
{
    back = cardCreator.backFace();
    face = cardCreator.front(suit, rank);
}

/* Deallocates resources */
Card::~Card() 
{
    face->free();
}

/* Renders the card within the given rect */
void Card::render(SDL_Rect *rect, const double& rotation) const
{
    if (is_revealed) face->render(rect, NULL, rotation);
    else back->render(rect, NULL, rotation);
}

/* Returns true if the card is facing the screen */
bool Card::isRevealed() const { return is_revealed; }

/* Toggles revealed on/off */
void Card::flip() {
    is_revealed = !is_revealed;
}

Suit Card::getSuit() const { return suit; }
Rank Card::getRank() const { return rank; }