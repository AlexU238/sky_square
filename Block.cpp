//
// Created by u238 on 5/12/23.
//

#include "Block.h"


void Block::setTypeId(unsigned int p_id) {
    blockTypeID = p_id;
}

unsigned int Block::getTypeId() const {
    return blockTypeID;
}

void Block::setLevelId(unsigned int p_id) {
    levelID = p_id;
}

unsigned int Block::getLevelId() const {
    return levelID;
}

bool Block::isItSolid() const {
    return isSolid;
}

void Block::setSolid(bool p_isSolid) {
    isSolid = p_isSolid;
}

SDL_Rect Block::getPos() const {
    return getDestination();
}

void Block::draw(SDL_Renderer *renderer) {
    GameObject::draw(renderer);
}



