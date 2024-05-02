//
// Created by u238 on 5/12/23.
//

#ifndef SANDBOX_GAMEOBJECT_H
#define SANDBOX_GAMEOBJECT_H

#include <iostream>
#include <SDL2/SDL_image.h>
#include "Drawable.h"


using namespace std;

//! Abstract class
class GameObject : public Drawable {

public:

    SDL_Rect destination{};

    SDL_Rect src{};

    SDL_Texture *texture{};

    //! Struct to easier way to keep track of the position
    typedef struct {
        float x;
        float y;
    } Vector2D;

    //! Default constructor
    GameObject();

    ~GameObject() override = 0;

    //! Get position of where the object will be drawn
    [[nodiscard]] const SDL_Rect &getDestination() const;

    //! Get source of texture of the object
    [[nodiscard]] const SDL_Rect &getSrc() const;

    //! Set position of where the object will be drawn and the size of the drawn object
    void setDestination(int x, int y, int w, int h);

    //! Set position of where the object will be drawn
    void setDestination(int x, int y);

    //! Set source of texture of the object
    void setSrc(int x, int y, int w, int h);

    //!Load the texture for the object
    void setTexture(string filename, SDL_Renderer *renderer);

    //! Get texture of the object
    [[nodiscard]] SDL_Texture *getTexture() const;

    //! Draw the object
    void draw(SDL_Renderer *renderer) override;
};


#endif //SANDBOX_GAMEOBJECT_H
