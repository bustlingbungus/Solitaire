#include "Solitaire.hpp"

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
        held_card->render(&hrect, (held_origin.held_dq.empty())? 15.0 : 0.0);
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
    drawstk->render(hidden_draw_pile.x-5, hidden_draw_pile.y-5);
    if (!draw_pile.empty()) 
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
    playstk->render(rect.x-5, rect.y-5);
    for (int i=0; i<limit && !dq.empty(); i++)
    {
        auto card = dq.back();
        render_card(card, &rect);
        rect.y += 50;
        dq.pop_back();
    }
}

/* renders all of the suit stacks */
void Solitaire::render_suit_stacks()
{
    for (int i=0; i<4; i++)
    {
        SDL_Rect *rect = suit_piles + i;
        suitplaceholders[i]->render(rect->x-5, rect->y-5);
        auto stk = suit_cards[i];
        if (!stk.empty()) {
            auto card = stk.top();
            render_card(card, rect);
            if (card == held_card && stk.size()>1) {
                stk.pop();
                render_card(stk.top(), rect);
            }
        }
    }
}