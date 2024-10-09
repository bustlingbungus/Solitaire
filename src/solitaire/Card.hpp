#ifndef CARD_HPP
#define CARD_HPP

// sdl
#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"

// std
#include <iostream>
#include <memory>

enum Suit
{
    SUIT_Hearts,    // 0
    SUIT_Clubs,     // 1
    SUIT_Diamonds,  // 2
    SUIT_Spades,    // 3
    SUIT_MAX        // 4
};

enum Rank
{
    RANK_MIN,   // 0
    RANK_Ace,   // 1
    RANK_Two,   // 2
    RANK_Three, // 3
    RANK_Four,  // 4
    RANK_Five,  // 5
    RANK_Six,   // 6
    RANK_Seven, // 7
    RANK_Eight, // 8
    RANK_Nine,  // 9
    RANK_Ten,   // 10
    RANK_Jack,  // 11
    RANK_Queen, // 12
    RANK_King,  // 13
    RANK_MAX    // 14
};



class Card
{
    public:
        /* Assigns variables */
        Card(Suit suit, Rank rank, std::shared_ptr<LWindow> window);
        /* Deallocates resources */
        ~Card();

        /* Renders the card within the given rect */
        void render(SDL_Rect *rect, const double& rotation = 0.0) const;

        /* Toggles revealed on/off */
        void flip();

        /* Returns true if the card is facing the screen */
        bool isRevealed() const;

        Suit getSuit() const;
        Rank getRank() const;

    private:

        /* The window the card will be rendered to */
        std::shared_ptr<LWindow> window;

        /* Whether or no the card face can be seen */
        bool is_revealed = false;

        /* The card's suit and rank */
        Suit suit;
        Rank rank;

        std::unique_ptr<LTexture> bg0, bg1;
};

/* Creates a texture for a card based on its suit and rank.
 * Actually creates a texture for number cards, but loads from file for face cards */
class CardCreator
{

};


#endif