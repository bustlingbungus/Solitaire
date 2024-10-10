# Solitaire

C++ Implementation of the classic Solitaire card game.

This repository is still a work in progress
### planned changes:
- cosmetic improvements
- variable difficulty (by changing the amount of cards drawn per click)
- score tracker (by amount of time to complete the game)
- saving ongoing games and scores to local storage

## How to play
To win Solitaire, all of the cards need to be placed in the suit stacks on the right side of the board. Each of these stacks can only contain cards of the `same suit`. If a stack if empty, you may place an `ace` of the corresponding suit into the stack. Otherwise, you may only place a card onto the right side stacks if it's rank is `one greater` than the card at the top of the stack, and has the `same suit` as said card (aces are low).

The cards placed in the right side suit piles will come from the `play stacks` in the centre of the board, or the `draw pile` at the left of the board. 

Click on the hidden cards in the `draw pile` to reveal the top one. Of the face up cards in the draw pile, you may drag the `top card` into a new position. When the hidden cards run out, click the empty space where the draw pile once was to `recycle` the draw pile. (make all the cards hidden again, still in the same order). Cards may be taken out of but not added to the draw pile.

In the central `play stacks`, there will be a number of hidden cards topped by a revealed card. To reveal these hidden cards, all the revealed cards on top must be moved. You can move a valid card onto any play stack from any other `play stack`, the `draw pile`, or any `suit pile`. The placement is valid if the `top card` of the play stack's `rank` is one higher than the card being placed on it, AND the top card's `suit` is a different colour than the card being placed. 

There are seven play stacks. If one is empty, a `King` may be placed in the empty space.

You may only move `revealed` cards from play stacks. If you click on a revealed card to move it, all of the other cards above it in the stack will also be moved. When you place this held stack onto another play stack, the `entire stack` of cards will be moved, and only the top card of the stack (the one being held) needs to be a valid placement onto the target stack. Though, you may only move cards onto `other play stacks` when carrying a stack. 

## Implementation
This implementation makes use of multiple different data structures to manage the cards. The draw pile is managed with a ```circularly linked list``` that gets cycled around as cards are drawn. Each of the play stacks are managed with ```deques``` that have cards added and removed from them. And each of the suit stacks on the right of the board are represented by ```stack``` structures.

## Build Instructions
### Dependencies
- C++ Compiler
- CMake
- Installation of ```SDL2```, ```SDL2_Image```, ```SDL2_ttf```, and ```SDL2_mixer```

### Instructions
Ensure there is a 'build' folder in your directory. Change to this directory with ```cd build```. From the build folder, run these commands:
```
cmake ..
cmake --build . --verbose
```
Then, run ```cd bin``` to go to the output directory, and run ```./project.exe``` to launch the game.

On VSCode, pressing ```ctrl+shift+b``` to run the included default build tasks should also work.

When developing your own projects, be sure to go to ```CMakeLists.txt```, and on line 7, replace 'project' with the name of your project. After doing this, wou will run your project with ```./[yourProjectName].exe```. If using build tasks to automate the build process, be sure to update line 37 in ```tasks.json``` to reflect this name change.

### Build Fix:
On some installations, static linking to SDL_ttf will cause undefined reference errors. If you are having this issue, simply remove ```-static``` from the SDL_ttf link in line 26 of ```CmMakeLists.txt```. Line 26 should look like this:
```
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
```
Hopefully this fix works!