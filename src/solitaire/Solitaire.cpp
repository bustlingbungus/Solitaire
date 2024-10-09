#include "Solitaire.hpp"

/* Assigns variables */
Solitaire::Solitaire(std::shared_ptr<LWindow> window)
: window(window)
{
    shuffle();
    game_cards = new std::deque<std::shared_ptr<Card>>[NUM_PLAYSTACKS];
    game_card_piles = new SDL_Rect[NUM_PLAYSTACKS];
    deal();

    SDL_Rect rect = {(10*CARD_WIDTH)+175,5,CARD_WIDTH,CARD_HEIGHT};
    for (int i=0; i<4; i++) {
        suit_piles[i] = rect;
        rect.y += CARD_HEIGHT + 25;
    }
}

/* Deallocates resources */
Solitaire::~Solitaire() 
{
    delete [] game_cards;
    delete [] game_card_piles;
}

void Solitaire::renderCards()
{
    render_draw_pile();
    render_game_stacks();
    render_suit_stacks();
    render_held();
}

/* renders the given card if it is not held */
void Solitaire::render_card(std::shared_ptr<Card> card, SDL_Rect *rect) {
    if (card != held_card) card->render(rect);
}

/* renders the held card */
void Solitaire::render_held()
{
    if (held_card != nullptr)
    {
        // get the coordinates of the mouse
        int x, y;
        SDL_GetMouseState(&x, &y);
        // the rect to render the held card into
        SDL_Rect hrect = {x-(CARD_WIDTH/2), y-(CARD_HEIGHT/2), CARD_WIDTH, CARD_HEIGHT};
        held_card->render(&hrect, 15.0);
        for (auto dq = held_origin.held_dq; !dq.empty(); dq.pop_back())
        {
            hrect.y += 50;
            dq.back()->render(&hrect);
        }
    }
}

/* renders the top 3 shown cards in the draw pile, and the remaining hidden draw pile */
void Solitaire::render_draw_pile()
{
    SDL_Rect shown = shown_draw_pile;
    int rshown = std::min(nshown, SHOW_AMT);
    auto head = draw_pile.back_ptr();

    for (int i=0; i<=rshown; i++)
    {
        auto card = draw_pile.back();
        draw_pile.advance();
        if (i<rshown) {
            render_card(card, &shown);
            shown.y += 50;
        } else if (nhidden > 0) render_card(card, &hidden_draw_pile);
    }
    draw_pile.setCursor(head);
}

/* renders all of the suit stacks */
void Solitaire::render_suit_stacks()
{
    for (int i=0; i<4; i++)
    {
        if (suit_cards[i].empty()) {
            auto tex = std::make_unique<LTexture>(window);
            tex->solidColour({50,50,50,255});
            tex->render(suit_piles+i);
            tex->free();
        } else suit_cards[i].top()->render(suit_piles+i);
    }
}

/* renders each of the game stacks */
void Solitaire::render_game_stacks() {
    for (int i=0; i<NUM_PLAYSTACKS; i++) {
        int lim = -1;
        if (held_card != nullptr && held_origin.origin == STACK_Game && held_origin.stackIdx == i) {
            lim = game_cards[i].size() - held_origin.held_dq.size();
        }
        render_stack(game_card_piles[i], game_cards[i], lim);
    }
}

/* renders a single game stack */
void Solitaire::render_stack(SDL_Rect rect, std::deque<std::shared_ptr<Card>> dq, int limit)
{
    if (limit == -1) limit = dq.size();
    rect.y -= 50 * (dq.size()-1);
    for (int i=0; i<limit && !dq.empty(); i++)
    {
        auto card = dq.back();
        render_card(card, &rect);
        rect.y += 50;
        dq.pop_back();
    }
}

/* randomly shuffles the deck into draw_pile */
void Solitaire::shuffle()
{
    // create a vector with all cards
    std::vector<std::shared_ptr<Card>> deck;

    for (int suit = 0; suit < SUIT_MAX; suit++) {
        for (int rank = 1; rank < RANK_MAX; rank++) {
            auto card = std::make_shared<Card>((Suit)suit, (Rank)rank, window);
            deck.push_back(card);
        }
    }

    // randomly shuffle into draw_pile until empty
    while (!deck.empty())
    {
        int idx = rand() % (deck.size());
        auto card = deck.begin() + idx;
        draw_pile.insert_front(*card);
        deck.erase(card);
    }
}

/* takes cards from draw_pile and places them on the board */
void Solitaire::deal()
{
    int amt = 1;
    for (int i=0; i<NUM_PLAYSTACKS; i++) {
        auto dq = game_cards[i];
        for (int j=0; j<amt; j++)
        {
            auto card = draw_pile.front();
            draw_pile.remove_front();
            dq.emplace_front(card);
        }
        SDL_Rect rect = {(i*(CARD_WIDTH+25))+(2*CARD_WIDTH), (amt*50)-45, CARD_WIDTH, CARD_HEIGHT};
        game_card_piles[i] = rect;

        dq.front()->flip();
        game_cards[i] = dq;
        amt++;
    }
    nhidden = draw_pile.size();
}

/* Reads user input */
void Solitaire::getInput(const SDL_Event& e)
{
    switch (e.type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            // find coordinates of mouse click
            int x, y;
            SDL_GetMouseState(&x, &y);
            // if a vlid mouse button is pressed call the appropriate function
            switch (e.button.button)
            {
                case SDL_BUTTON_LEFT: leftClick(x, y); break;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            // find coordinates of mouse
            int x, y;
            SDL_GetMouseState(&x, &y);
            // if a vlid mouse button is pressed call the appropriate function
            switch (e.button.button)
            {
                case SDL_BUTTON_LEFT: releaseLeft(x, y); break;
            }
            break;
        }
    }
}

/* cycles the draw pile by the draw amount */
void Solitaire::cycle_draw_pile()
{
    if (nhidden == 0)
    {
        nshown = 0;
        nhidden = draw_pile.size();
        for (int i=0; i<nhidden; i++) {
            draw_pile.front()->flip();
            draw_pile.advance();
        }
        draw_pile.advance(SHOW_AMT-1);
    }
    else
    {
        int offset = (nshown<SHOW_AMT)? std::max(0,nshown-1) : SHOW_AMT-1;
        auto start = draw_pile.at(offset);
        int transfer = std::min(DRAW_AMT, nhidden);
        for (int i=0; i<transfer; i++) {
            start->elem->flip();
            start = start->next;
        }
        int advance = 0;
        if (nshown < SHOW_AMT) {
            if (nshown==0) advance = 1;
            else if ((SHOW_AMT-nshown)<DRAW_AMT) advance = (nshown+transfer) - SHOW_AMT;
        } else advance = transfer;
        draw_pile.advance(advance);

        nshown += transfer; nhidden -= transfer;
    }
}

/* checks if an x, y coordinate is with a rect region */
bool Solitaire::isInRegion(const int& x, const int& y, const SDL_Rect& rect) const {
    return (x >= rect.x && x <= rect.x+rect.w) && (y >= rect.y && y <= rect.y+rect.h);
}

/* returns the number of cards carried when pressing a game stack. 
 * -1 if not within the game stack */
int Solitaire::in_game_stack(const int& x, const int& y, SDL_Rect rect) const
{
    if (y<5 || y>rect.y+rect.h || x<rect.x || x>rect.x+rect.w) return -1;

    int res = 0;
    while (!isInRegion(x, y, rect))
    {
        rect.y -= 50;
        res++;
    }
    return res;
}

/* checks where the mouse was left clicked, calls helper functions if anything was clicked */
void Solitaire::leftClick(const int& x, const int& y)
{
    // find the rect representing the shown draw pile
    SDL_Rect srect = shown_draw_pile;
    int samt = std::min(nshown, SHOW_AMT);
    srect.y += (samt-1) * 50;

    // advance the draw pile
    if (isInRegion(x, y, hidden_draw_pile)) cycle_draw_pile();
    else if (isInRegion(x, y, srect) && samt > 0) pick_from_draw_pile(samt);
    // see if a suit card is picked up
    else if (!pick_from_game_stack(x, y)) {
        // otherwise, attempt to pick from a suit stack
        pick_from_suit_pile(x, y);
    }
}

/* picks up a card from the draw pile */
void Solitaire::pick_from_draw_pile(const int& samt)
{
    CNode<std::shared_ptr<Card>> *node;
    if (samt == 1) node = draw_pile.at(draw_pile.size()-2);
    else if (samt == 2) node = draw_pile.back_ptr();
    else node = draw_pile.at(samt-3);
    held_card = node->next->elem;
    held_origin.origin = STACK_Draw;
    held_origin.node = node;
}

/* attempts to pick up a card from a game stack, returns true if successful */
bool Solitaire::pick_from_game_stack(const int& x, const int& y)
{
    for (int i=0; i<NUM_PLAYSTACKS; i++)
    {
        int num = in_game_stack(x, y, game_card_piles[i]); 
        // pick up the top card of the selected stack
        if (num != -1)
        {
            auto dq = game_cards[i];
            auto card = dq.at(num);
            if (card->isRevealed()) {
                held_card = card;

                held_origin.origin = STACK_Game;
                held_origin.stackIdx = i;
                for (num--;num>=0;num--) {
                    held_origin.held_dq.emplace_front(dq.at(num));
                }
                return true;
            }
        }
    }
    return false;
}

/* attempts to pick up a card from a suit pile */
void Solitaire::pick_from_suit_pile(const int& x, const int& y)
{
    for (int i=0; i<4; i++)
    {
        if (isInRegion(x, y, suit_piles[i]) && !suit_cards[i].empty())
        {
            held_card = suit_cards[i].top();
            held_origin.origin = STACK_Suit;
            held_origin.stackIdx = i;
        }
    }
}

/* releases the held card at the given location */
void Solitaire::releaseLeft(const int& x, const int& y)
{
    if (held_card != nullptr) {
        if (!release_on_game_stack(x, y)) {
            release_on_suit_stack(x, y);
        }
        held_card = nullptr;
        held_origin.held_dq.clear();
    }

}

/* attempts to release the held card on one of the game stacks
 * returns false if the card was not placed */
bool Solitaire::release_on_game_stack(const int& x, const int& y)
{
    for (int i=0; i<NUM_PLAYSTACKS; i++)
    {
        // place the held card in the deque it's hovering over
        if (isInRegion(x, y, game_card_piles[i]))
        {
            // check for valid placement
            if (canBePlaced_game(held_card, game_cards[i]))
            {
                game_cards[i].emplace_front(held_card);
                game_card_piles[i].y += 50;
                int n_carried = held_origin.held_dq.size();
                while (!held_origin.held_dq.empty()) {
                    game_cards[i].emplace_front(held_origin.held_dq.back());
                    game_card_piles[i].y += 50;
                    held_origin.held_dq.pop_back();
                }
                removeHeld(n_carried);
            }
            return true;
        }
    }
    return false;
}

/* attempts to release the held card on one of the suit stacks */
void Solitaire::release_on_suit_stack(const int& x, const int& y)
{
    for (int i = 0; i < 4; i++) {
        if (isInRegion(x, y, suit_piles[i])) {
            if (canBePlaced_suit(held_card, suit_cards[i], (Suit)i))
            {
                suit_cards[i].push(held_card);
                removeHeld();
            }
        }
    }
}

/* returns true if the given card can be placed on the given game stack */
bool Solitaire::canBePlaced_game(const std::shared_ptr<Card>& c, const std::deque<std::shared_ptr<Card>>& dq)
{
    if (dq.empty()) return c->getRank() == RANK_King;
    else if (dq.front()->getRank() == c->getRank()+1) {
        return (dq.front()->getSuit()%2) != (c->getSuit()%2);
    }
    return false;
}

/* returns true if the given card can be placed on the given suit stack */
bool Solitaire::canBePlaced_suit(const std::shared_ptr<Card>& c, const std::stack<std::shared_ptr<Card>>& stk, const Suit& suit)
{
    if (!held_origin.held_dq.empty() || c->getSuit() != suit) return false;
    if (stk.empty()) return c->getRank() == RANK_Ace;
    else return stk.top()->getRank() == c->getRank()-1;    
}

/* removes the held card from its stack of origin */
void Solitaire::removeHeld(int n_carried)
{
    switch (held_origin.origin)
    {
        case STACK_Draw:
            draw_pile.remove(held_origin.node);
            nshown--;
            // set the cursor back 1
            if (nshown >= SHOW_AMT) draw_pile.setCursor(draw_pile.at(draw_pile.size()-2));
            break;

        case STACK_Game:
        {
            auto dq = game_cards[held_origin.stackIdx];
            for (int i=0; i<=n_carried; i++) dq.pop_front();
            if (!dq.empty()) {
                auto card = dq.front();
                if (!card->isRevealed()) card->flip();
            }
            game_cards[held_origin.stackIdx] = dq;
            game_card_piles[held_origin.stackIdx].y -= 50*++n_carried;
            break;
        }

        case STACK_Suit:
            suit_cards[held_origin.stackIdx].pop();
            break;
    }
}