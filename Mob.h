//
// Created by u238 on 5/12/23.
//

#ifndef SANDBOX_MOB_H
#define SANDBOX_MOB_H

#include "Entity.h"

//! Mob class - enemy of the player
/*!
 * Is not currently implemented due to time constraints
 */
class Mob : public Entity {
private:
    int damage;
//todo add animation
public:

    Mob();

    void hit() override;
};


#endif //SANDBOX_MOB_H
