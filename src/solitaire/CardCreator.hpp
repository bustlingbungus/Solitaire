#ifndef CARD_CREATOR_HPP
#define CARD_CREATOR_HPP

// sdl
#include "../sdl/LWindow.hpp"
#include "../sdl/LTexture.hpp"

// std
#include <memory>
#include <iostream>
#include <vector>

// solitaire
#include "Card.hpp"

/* Used for creating card textures */
class CardCreator
{
    public:

        CardCreator();
        /* Deallocates resources */
        ~CardCreator();

        /* Initialises the window and loads textures */
        void Init(std::shared_ptr<LWindow> Window);

        /* the back face of cards */
        std::shared_ptr<LTexture> backFace() const;

        /* Creates a face texture for cards. Actually creates a texture for number cards,
         * but loads from file for face cards. */
        std::shared_ptr<LTexture> front(const Suit& suit, const Rank& rank);

    private:


        /* colours to be used */
        SDL_Color red = {255,0,0,255}, black = {0,0,0,255};

        /* The window for texture creation */
        std::shared_ptr<LWindow> window = nullptr;

        /* The textures used for card creation */
        std::shared_ptr<LTexture> backTex = nullptr,
            heart = nullptr, spade = nullptr, diamond = nullptr, club = nullptr,
            jclub = nullptr, jdiamond = nullptr, jheart = nullptr, jspade = nullptr,
            qclub = nullptr, qdiamond = nullptr, qheart = nullptr, qspade = nullptr,
            kclub = nullptr, kdiamond = nullptr, kheart = nullptr, kspade = nullptr;

        /* The font used for number cards */
        TTF_Font *font = nullptr;

        /* ==========  HELPER FUNCTIONS  ========== */

        // Loads required assets from file
        void loadTextures(std::string assets_dir);

        // renders text onto a card face
        void render_text_to_face(std::shared_ptr<LTexture> face, std::shared_ptr<LTexture> suitTex, const std::string& str, const SDL_Color& col) const;

        // returns the points where suit images have to be placed
        // for each element, ints 0 and 1 are x y coords, and int 3 is
        // whether the image should be flipped
        std::vector<std::vector<int>> get_suit_points(const Rank& rank) const;
        // returns the desired suit image
        std::shared_ptr<LTexture> get_suit(const Suit& suit) const;

        // returns the desired face texture
        std::shared_ptr<LTexture> get_face(const Suit& suit, const Rank& rank) const;
};

extern CardCreator cardCreator;

#endif