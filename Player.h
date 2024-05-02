//
// Created by u238 on 5/12/23.
//

#ifndef SANDBOX_PLAYER_H
#define SANDBOX_PLAYER_H

#include "Entity.h"

#define PLAYER_MAX_HEALTH 10

//! Player- user playable character
class Player : public Entity {
private:
    //! Current tool used by player - not implemented
    string currentTool;
    //! Hunger level indicator - food not implemented
    int hungerLevel;
    //! Player inventory - not implemented
    vector<string> inventory;

    //! Player heath indicator
    int health;
//todo add animation
public:
    Player();

    Player(const Vector2D &pos, const Vector2D &velocity, const SDL_Rect &hitBoxTop, const SDL_Rect &hitBoxBot);

    //! Deal damage -not implemented, because the mobs are not implemented
    void attack();

    //! Receive damage -not implemented, because the mobs are not implemented
    void hit() override;

    //! increment player health by 1 when called
    void heal();

    [[nodiscard]] int getHealth() const;

    //! Get value of Player current heath
    void setHealth(int hp);

    //! draw player for user to see
    void draw(SDL_Renderer *renderer) override;

};


#endif //SANDBOX_PLAYER_H
