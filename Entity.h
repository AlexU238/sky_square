//
// Created by u238 on 5/12/23.
//

#ifndef SANDBOX_ENTITY_H
#define SANDBOX_ENTITY_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "GameObject.h"

using namespace std;

//! abstract Entity class
/*!
  Entity is any playable and non-playable characters in game
*/
class Entity : public GameObject {
private:

    Vector2D pos{};

    Vector2D velocity{};

    SDL_Rect hitBoxTop{};

    SDL_Rect hitBoxBot{};

public:

    Entity();

    Entity(const Vector2D &pos, const Vector2D &velocity, const SDL_Rect &hitBoxTop, const SDL_Rect &hitBoxBot);

    virtual void hit() = 0;

    void draw(SDL_Renderer *renderer) override;

    //! Get central position of entity
    /*! It is a relative position of entity */
    [[nodiscard]] Vector2D getPos() const;

    //! get the top half of entity
    /*! Every entity consists of two halves to navigate interactions with the world*/
    [[nodiscard]] SDL_Rect *getHitBoxTop();

    //! get the bottom half of entity
    [[nodiscard]] SDL_Rect *getHitBoxBot();

    //! Get speed of entity
    /*! indicates how fast the entity moves on screen */
    [[nodiscard]] Vector2D getVelocity() const;

    //! Increments the vertical speed of the entity
    /*! takes float as a parameter. the result is the current value of velocity on Y axis plus the input parameter */
    void incrementVelocityYBy(float vel);

    void setVelocityY(float vel);

    void setVelocityX(float vel);

    void changePosX(float byX);

    void changePosY(float byY);

    void setPosX(float newX);

    void setPosY(float newY);

    void setHitBoxTopX(int x);

    void setHitBoxTopY(int y);

    void setHitBoxBotX(int x);

    void setHitBoxBotY(int y);


};


#endif //SANDBOX_ENTITY_H
