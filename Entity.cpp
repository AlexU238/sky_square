//
// Created by u238 on 5/12/23.
//

#include "Entity.h"

Entity::Entity() = default;

Entity::Entity(const GameObject::Vector2D &pos, const GameObject::Vector2D &velocity, const SDL_Rect &hitBoxTop,
               const SDL_Rect &hitBoxBot) : pos(pos), velocity(velocity), hitBoxTop(hitBoxTop),
                                            hitBoxBot(hitBoxBot) {}

void Entity::hit() {

}

GameObject::Vector2D Entity::getPos() const {
    return pos;
}

SDL_Rect *Entity::getHitBoxTop() {
    return &hitBoxTop;
}

SDL_Rect *Entity::getHitBoxBot() {
    return &hitBoxBot;
}

GameObject::Vector2D Entity::getVelocity() const {
    return velocity;
}

void Entity::incrementVelocityYBy(float vel) {
    velocity.y += vel;
}

void Entity::setVelocityY(float vel) {
    velocity.y = vel;
}

void Entity::setVelocityX(float vel) {
    velocity.x = vel;
}

void Entity::changePosX(float byX) {
    pos.x += byX;
}

void Entity::changePosY(float byY) {
    pos.y += byY;
}

void Entity::setPosX(float newX) {
    pos.x = newX;
}

void Entity::setPosY(float newY) {
    pos.y = newY;
}

void Entity::setHitBoxTopX(int x) {
    hitBoxTop.x = x;
}

void Entity::setHitBoxTopY(int y) {
    hitBoxTop.y = y;
}

void Entity::setHitBoxBotX(int x) {
    hitBoxBot.x = x;
}

void Entity::setHitBoxBotY(int y) {
    hitBoxBot.y = y;
}

void Entity::draw(SDL_Renderer *renderer) {
    GameObject::draw(renderer);
}





