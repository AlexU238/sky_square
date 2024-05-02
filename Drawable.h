//
// Created by u238 on 6/10/23.
//

#ifndef SANDBOX_DRAWABLE_H
#define SANDBOX_DRAWABLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//! Interface for classes that need to be graphically drawn
class Drawable {

public:
    Drawable();

    virtual ~Drawable();

    //! struct for easier input of colors
    typedef struct {
        int r, g, b;
    } RGB;

    //! pure virtual draw method
    virtual void draw(SDL_Renderer *renderer) = 0;
};


#endif //SANDBOX_DRAWABLE_H
