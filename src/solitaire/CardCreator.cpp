#include "CardCreator.hpp"

CardCreator cardCreator = CardCreator();

CardCreator::CardCreator() {}

/* Deallocates resources */
CardCreator::~CardCreator()
{
    TTF_CloseFont(font);

    backTex->free();
    heart->free(); spade->free(); 
    diamond->free(); club->free(); 
    pheart->free(); pspade->free(); 
    pdiamond->free(); pclub->free(); 

    jclub->free(); jdiamond->free(); jheart->free(); jspade->free();
    qclub->free(); qdiamond->free(); qheart->free(); qspade->free();
    kclub->free(); kdiamond->free(); kheart->free(); kspade->free();

    placeholder->free(); suitlessking->free(); mouseclick->free();
}

/* Initialises the window and loads textures */
void CardCreator::Init(std::shared_ptr<LWindow> Window)
{
    window = Window;
    loadTextures("../../assets/");
}


/* the back face of cards */
std::shared_ptr<LTexture> CardCreator::backFace() const { return backTex; }

/* Creates a face texture for cards. Actually creates a texture for number cards,
 * but loads from file for face cards. */
std::shared_ptr<LTexture> CardCreator::front(const Suit& suit, const Rank& rank)
{
    auto res = std::make_shared<LTexture>(window);
    if (!res->solidColour({170,170,170,170}, 100, 150)) {
        std::cerr << "Failed to create solid colour!\n";
    }

    auto clr = std::make_unique<LTexture>(window);
    SDL_Rect r = {2, 2, 96, 146};
    clr->solidColour({255,255,255,255});
    clr->render_toTexture(res.get(), &r);
    clr->free();

    if (rank < RANK_Jack)
    {
        std::string str = (rank == RANK_Ace)? "A" : std::to_string((int)rank);
        SDL_Color col = (suit%2)?  black : red;
        auto tex = get_suit(suit);

        render_text_to_face(res, tex, str, col);

        // render the suit images onto the card face
        auto points = get_suit_points(rank);
        for (auto point : points) 
        {
            SDL_RendererFlip flip = (point[1]>75)? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
            tex->render_toTexture(res.get(), point[0]-13, point[1]-13, NULL, NULL, 0.0, NULL, flip);
        }
    }
    else
    {
        auto face = get_face(suit, rank);
        face->render_toTexture(res.get(), &r);
    }

    return res;
}

/* Creates an Icon for stack placeholders */
std::shared_ptr<LTexture> CardCreator::makePlaceholder(const Suit& suit, const Rank& rank)
{
    auto res = std::make_shared<LTexture>(window);
    res->solidColour({0,80,0,255}, 110, 160);

    placeholder->render_toTexture(res.get(), 0, 0);

    switch (rank)
    {
        case RANK_Ace:
        {
            std::shared_ptr<LTexture> suit_tex = get_suit(suit, true),
                                      Atex = std::make_shared<LTexture>(window);
            Atex->loadFromRenderedText("A", {0,120,0,255}, font);

            SDL_Rect suitrect = {
                (res->getWidth()-suit_tex->getWidth())/2, 
                (res->getHeight()-suit_tex->getHeight())/2, 
                suit_tex->getWidth(), suit_tex->getHeight()
            };
            suit_tex->render_toTexture(res.get(), &suitrect);
            
            SDL_Rect dest = {8, 0, 17, 34};
            suitrect = {8+(dest.w-(suitrect.w/2))/2, dest.h-5, suitrect.w/2, suitrect.h/2};
            Atex->render_toTexture(res.get(), &dest);
            suit_tex->render_toTexture(res.get(), &suitrect);

            dest.x = 102 - dest.w; dest.y = 160 - dest.h;
            suitrect.x = dest.x+(dest.w-suitrect.w)/2; suitrect.y = dest.y-suitrect.h;
            Atex->render_toTexture(res.get(), &dest, NULL, 180.0);
            suit_tex->render_toTexture(res.get(), &suitrect, NULL, 0.0, NULL, SDL_FLIP_VERTICAL);

            Atex->free(); suit_tex->free();
            break;
        }

        case RANK_King:
        {
            auto KTex = std::make_shared<LTexture>(window);
            KTex->loadFromRenderedText("K", {0,120,0,255}, font);

            SDL_Rect dest = {8, 0, 17, 34};
            KTex->render_toTexture(res.get(), &dest);
            dest.x = 102 - dest.w; dest.y = 160 - dest.h;
            KTex->render_toTexture(res.get(), &dest, NULL, 180.0);

            dest = {17, 18, 75, 113};
            suitlessking->render_toTexture(res.get(), &dest);

            KTex->free();
            break;
        }

        default:
        {
            int w = mouseclick->getWidth(), h = mouseclick->getHeight();
            SDL_Rect rect = {(res->getWidth()-w)/2, (res->getHeight()-h)/2, w, h};
            mouseclick->render_toTexture(res.get(), &rect);
        }
    }

    return res;
}

/* renders text onto a card face */
void CardCreator::render_text_to_face(std::shared_ptr<LTexture> face, std::shared_ptr<LTexture> suitTex, const std::string& str, const SDL_Color& col) const
{
    auto txt = std::make_unique<LTexture>(window);
    if (!txt->loadFromRenderedText(str, col, font)) {
        std::cerr << "Failed to create card text!\n";
    }
    SDL_Rect dest = {3, -5, 17, 34};
    SDL_Rect suitRect = {3+(dest.w-(suitTex->getWidth()/2))/2, dest.h-5, suitTex->getWidth()/2, suitTex->getHeight()/2};
    txt->render_toTexture(face.get(), &dest);
    suitTex->render_toTexture(face.get(), &suitRect);

    dest.x = 97 - dest.w; dest.y = 155 - dest.h;
    suitRect.x = dest.x+(dest.w-suitRect.w)/2; suitRect.y = dest.y-suitRect.h;
    txt->render_toTexture(face.get(), &dest, NULL, 180.0);
    suitTex->render_toTexture(face.get(), &suitRect, NULL, 0.0, NULL, SDL_FLIP_VERTICAL);
    txt->free();
}

/* returns the points where suit images have to be placed
 * for each element, ints 0 and 1 are x y coords, and int 3 is
 * whether the image should be flipped */
std::vector<std::vector<int>> CardCreator::get_suit_points(const Rank& rank) const
{
    std::vector<std::vector<int>> res = {};
    switch (rank)
    {
        case RANK_Ace: 
            res = {{50,75}};
            break;
        case RANK_Two: 
            res = {{50,38},{50,113}};
            break;
        case RANK_Three:
            res = {{50,38},{50,75},{50,113}};
            break;
        case RANK_Four: 
            res = {{30,38},{30,113},
                   {70,38},{70,113}};
            break;
        case RANK_Five:
            res = {{30,38},{30,113},
                   {70,38},{70,113},
                   {50,75}};
            break;
        case RANK_Six:
            res = {{30,38},{30,75},{30,113},
                   {70,38},{70,75},{70,113}};
            break;
        case RANK_Seven:
            res = {{30,38},{30,75},{30,113},
                   {70,38},{70,75},{70,113},
                   {50,50}};
            break;
        case RANK_Eight:
            res = {{30,38},{30,75},{30,113},
                   {70,38},{70,75},{70,113},
                   {50,50},{50,100}};
            break;
        case RANK_Nine:
            res = {{30,30},{30,60},{30,90},{30,120},
                   {70,30},{70,60},{70,90},{70,120},
                   {50,75}};
            break;
        case RANK_Ten:
            res = {{30,30},{30,60},{30,90},{30,120},
                   {70,30},{70,60},{70,90},{70,120},
                   {50,50},{50,100}};
            break;
    }
    return res;
}

/* returns the desired suit as an image */
std::shared_ptr<LTexture> CardCreator::get_suit(const Suit& suit, const bool& is_placeholder) const
{
    std::shared_ptr<LTexture> res = nullptr;
    switch (suit)
    {
        case SUIT_Hearts: res = (is_placeholder)? pheart : heart; break;
        case SUIT_Clubs: res = (is_placeholder)? pclub : club; break;
        case SUIT_Spades: res = (is_placeholder)? pspade : spade; break;
        case SUIT_Diamonds: res = (is_placeholder)? pdiamond : diamond; break;
    }
    return res;
}

/* returns the desired face texture */
std::shared_ptr<LTexture> CardCreator::get_face(const Suit& suit, const Rank& rank) const
{
    switch (suit)
    {
        case SUIT_Clubs:
        switch (rank)
        {
            case RANK_Jack: return jclub;
            case RANK_Queen: return qclub;
            case RANK_King: return kclub;
        }
        break;

        case SUIT_Diamonds:
        switch (rank)
        {
            case RANK_Jack: return jdiamond;
            case RANK_Queen: return qdiamond;
            case RANK_King: return kdiamond;
        }
        break;
        
        case SUIT_Hearts:
        switch (rank)
        {
            case RANK_Jack: return jheart;
            case RANK_Queen: return qheart;
            case RANK_King: return kheart;
        }
        break;
        
        case SUIT_Spades:
        switch (rank)
        {
            case RANK_Jack: return jspade;
            case RANK_Queen: return qspade;
            case RANK_King: return kspade;
        }
        break;
    }
    return nullptr;
}

/* Loads required textures from file */
void CardCreator::loadTextures(std::string assets_dir)
{
    // load images from file
    std::string filename = assets_dir + "Back.png";
    backTex = std::make_shared<LTexture>(window);
    if (!backTex->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }

    filename = assets_dir + "suits/heart.png";
    heart = std::make_shared<LTexture>(window);
    if (!heart->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "suits/spade.png";
    spade = std::make_shared<LTexture>(window);
    if (!spade->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "suits/diamond.png";
    diamond = std::make_shared<LTexture>(window);
    if (!diamond->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "suits/club.png";
    club = std::make_shared<LTexture>(window);
    if (!club->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "placeholders/heart.png";
    pheart = std::make_shared<LTexture>(window);
    if (!pheart->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "placeholders/spade.png";
    pspade = std::make_shared<LTexture>(window);
    if (!pspade->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "placeholders/diamond.png";
    pdiamond = std::make_shared<LTexture>(window);
    if (!pdiamond->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "placeholders/club.png";
    pclub = std::make_shared<LTexture>(window);
    if (!pclub->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }

    filename = assets_dir + "faces/jclub.png";
    jclub = std::make_shared<LTexture>(window);
    if (!jclub->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/jdiamond.png";
    jdiamond = std::make_shared<LTexture>(window);
    if (!jdiamond->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/jheart.png";
    jheart = std::make_shared<LTexture>(window);
    if (!jheart->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/jspade.png";
    jspade = std::make_shared<LTexture>(window);
    if (!jspade->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    
    filename = assets_dir + "faces/qclub.png";
    qclub = std::make_shared<LTexture>(window);
    if (!qclub->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/qdiamond.png";
    qdiamond = std::make_shared<LTexture>(window);
    if (!qdiamond->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/qheart.png";
    qheart = std::make_shared<LTexture>(window);
    if (!qheart->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/qspade.png";
    qspade = std::make_shared<LTexture>(window);
    if (!qspade->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }

    filename = assets_dir + "faces/kclub.png";
    kclub = std::make_shared<LTexture>(window);
    if (!kclub->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/kdiamond.png";
    kdiamond = std::make_shared<LTexture>(window);
    if (!kdiamond->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/kheart.png";
    kheart = std::make_shared<LTexture>(window);
    if (!kheart->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "faces/kspade.png";
    kspade = std::make_shared<LTexture>(window);
    if (!kspade->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }

    filename = assets_dir + "placeholders/placeholder.png";
    placeholder = std::make_shared<LTexture>(window);
    if (!placeholder->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "placeholders/king.png";
    suitlessking = std::make_shared<LTexture>(window);
    if (!suitlessking->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }
    filename = assets_dir + "placeholders/pointer.png";
    mouseclick = std::make_shared<LTexture>(window);
    if (!mouseclick->loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename <<"!\n";
    }

    filename = assets_dir + "Jersey10-Regular.ttf";
    font = TTF_OpenFont(filename.c_str(), 18);
}