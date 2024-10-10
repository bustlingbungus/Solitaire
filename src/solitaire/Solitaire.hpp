#ifndef SOLITAIRE_HPP
#define SOLITAIRE_HPP

// sdl
#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"

// std
#include <iostream>
#include <memory>
#include <stack>
#include <cmath>
#include <deque>
#include <random>
#include <vector>

// data structs
#include "../dataStructs/CLinkedList.hpp"

// solitaire
#include "Card.hpp"
#include "StackTypes.hpp"


class Solitaire
{
    public:
        /* Assigns variables */
        Solitaire(std::shared_ptr<LWindow> window);
        /* Deallocates resources */
        ~Solitaire();

        /* Renders all cards */
        void renderCards();

        /* Reads user input */
        void getInput(const SDL_Event& e);

        /* clears all card piles */
        void clear();
        
        /* randomly shuffles the deck into draw_pile */
        void shuffle();
        /* takes cards from draw_pile and places them on the board */
        void deal();

        /* returns true if all cards are in the suit piles */
        bool victory() const;

    private:

        /* ==========  CONSTANTS  ========== */

        // the dimensions of each card
        const int CARD_WIDTH = 100, CARD_HEIGHT = 150;
        // the amount of cards that get drawn when clicking the draw pile 
        const int DRAW_AMT = 1;
        // the amount of revealed cards shown in the draw pile
        const int SHOW_AMT = 3;
        // the number of stacks in the play area
        const int NUM_PLAYSTACKS = 7;


        /* ==========  GAME VARIABLES  ========== */

        // The cards in the main play area
        std::deque<std::shared_ptr<Card>> *game_cards;
        // the cards in the draw pile
        CList<std::shared_ptr<Card>> draw_pile;
        // The cards in the suit piles
        std::stack<std::shared_ptr<Card>> suit_cards[4];

        // the region of the draw piles
        SDL_Rect hidden_draw_pile = {5, 5, CARD_WIDTH, CARD_HEIGHT},
                 shown_draw_pile = {5, CARD_HEIGHT+50, CARD_WIDTH, CARD_HEIGHT};
        // the size of the draw pile that is shown, and hidden, respectively
        int nshown = 0, nhidden;

        // the number of hidden cards on the game board
        int hidden_in_game = 0;
        
        // the positions of the top card of each game pile
        SDL_Rect *game_card_piles;
        // the positions of the suit cards piles
        SDL_Rect suit_piles[4];

        // the card the player is currently holding
        std::shared_ptr<Card> held_card = nullptr;
        // information about where the held card came from
        ReturnType held_origin;

        // whether or not the game was been won
        bool has_won = false;


        /* ==========  SDL VARIABLES  ========== */

        // The window the game gets rendered to
        std::shared_ptr<LWindow> window;


        /* ==========  HELPER FUNCTIONS  ========== */

        /* ==========  GENERAL  ========== */

        // cycles the draw pile by the draw amount
        void cycle_draw_pile();
        // checks if an x, y coordinate is with a rect region
        bool isInRegion(const int& x, const int& y, const SDL_Rect& rect) const;
        // returns the number of cards carried when pressing a game stack. 
        // -1 if not within the game stack
        int in_game_stack(const int& x, const int& y, SDL_Rect rect) const;
        // removes the held card from its stack of origin
        void removeHeld(int n_carried = 0);
        // returns true if the given card can be placed on the given game stack
        bool canBePlaced_game(const std::shared_ptr<Card>& c, const std::deque<std::shared_ptr<Card>>& dq);
        // returns true if the given card can be placed on the given suit stack
        bool canBePlaced_suit(const std::shared_ptr<Card>& c, const std::stack<std::shared_ptr<Card>>& stk, const Suit& suit);


        // transition into win state
        void win_game();

        /* ==========  INPUT  ========== */

        // checks where the mouse was left clicked, calls helper functions if anything was clicked
        void leftClick(const int& x, const int& y);
        // releases the held card at the given location
        void releaseLeft(const int& x, const int& y);

        // picks up a card from the draw pile
        void pick_from_draw_pile(const int& samt);
        // attempts to pick up a card from a game stack, returns true if successful
        bool pick_from_game_stack(const int& x, const int& y);
        // attempts to pick up a card from a suit pile 
        void pick_from_suit_pile(const int& x, const int& y);

        // attempts to release the held card on one of the game stacks
        // returns false if the card was not placed 
        bool release_on_game_stack(const int& x, const int& y);
        // attempts to release the held card on one of the suit stacks
        void release_on_suit_stack(const int& x, const int& y);

        /* ==========  RENDERING  ========== */
        
        // renders the given card if it is not held
        void render_card(std::shared_ptr<Card> card, SDL_Rect *rect);
        // renders the held card
        void render_held();
        // renders the top 3 shown cards in the draw pile, and the remaining hidden draw pile
        void render_draw_pile();
        // renders each of the game stacks
        void render_game_stacks();
        // renders all of the suit stacks
        void render_suit_stacks();
        // renders a single game stack
        void render_stack(SDL_Rect rect, std::deque<std::shared_ptr<Card>> dq, int limit = -1);
        
};

#endif