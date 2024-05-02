//
// Created by u238 on 5/12/23.
//

#include "GameObject.h"

GameObject::GameObject() = default;

GameObject::~GameObject() = default;

const SDL_Rect &GameObject::getDestination() const {
    return destination;
}

const SDL_Rect &GameObject::getSrc() const {
    return src;
}

void GameObject::setDestination(int x, int y, int w, int h) {
    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;
}

void GameObject::setDestination(int x, int y) {
    destination.x = x;
    destination.y = y;
}

void GameObject::setSrc(int x, int y, int w, int h) {
    src.x = x;
    src.y = y;
    src.w = w;
    src.h = h;
}

void GameObject::setTexture(string filename, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(filename.data());
    if (!surface) { cout << "could not load texture" << SDL_GetError() << endl; }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

SDL_Texture *GameObject::getTexture() const {
    return texture;
}

void GameObject::draw(SDL_Renderer *renderer) {
    SDL_Rect dest = destination;
    SDL_Rect sr = src;
    SDL_RenderCopyEx(renderer, getTexture(), &sr, &dest, 0, nullptr, SDL_FLIP_NONE);
}







