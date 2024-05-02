//
// Created by u238 on 5/12/23.
//

#include "Player.h"

Player::Player(const GameObject::Vector2D &pos, const GameObject::Vector2D &velocity, const SDL_Rect &hitBoxTop,
               const SDL_Rect &hitBoxBot) : Entity(pos, velocity, hitBoxTop, hitBoxBot) {
    hungerLevel = 10;
    health = PLAYER_MAX_HEALTH;

}

Player::Player() {
    hungerLevel = 10;
    health = PLAYER_MAX_HEALTH;
}

void Player::hit() {
    cout << "Hit" << endl;
}

void Player::attack() {

}

void Player::heal() {
    if (health != PLAYER_MAX_HEALTH) {
        health++;
    }
}

int Player::getHealth() const {
    return health;
}

void Player::setHealth(int hp) {
    health = hp;
}

void Player::draw(SDL_Renderer *renderer) {
    Entity::draw(renderer);
}






