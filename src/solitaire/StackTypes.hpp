#ifndef STACKS_HPP
#define STACKS_HPP

// data structures
#include "../dataStructs/CLinkedList.hpp"

// std
#include <memory>
#include <deque>

// solitaire
#include "Card.hpp"

/* the different types of card stacks */
enum StackType
{
    STACK_Draw, // 0
    STACK_Game, // 1
    STACK_Suit,  // 2
};

/* information about where the held card comes from */
struct ReturnType
{
    /* The type of stack the card comes from */
    StackType origin;
    /* If the card cam from the draw pile, the pointer in the draw pile */
    CNode<std::shared_ptr<Card>> *node = nullptr;
    /* if the card cam from a game stack or ace stack, the index of the stack it cam from */
    int stackIdx = -1;
    /* the stack of cards being held */
    std::deque<std::shared_ptr<Card>> held_dq;
};

#endif