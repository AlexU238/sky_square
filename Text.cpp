//
// Created by u238 on 6/4/23.
//

#include <iostream>
#include "Text.h"

Text::Text(const char *msg, TTF_Font *font, int x, int y, int w, int h, RGB coloring, bool click) : message(msg),
                                                                                                    font(font),
                                                                                                    clickable(click) {
    pos = {x, y, w, h};
    bounds = {x, y, w, h};
    color.r = coloring.r;
    color.g = coloring.g;
    color.b = coloring.b;
    color.a = 255;
}


void Text::draw(SDL_Renderer *renderer) {
    SDL_Surface *surf;
    SDL_Texture *tex;
    surf = TTF_RenderText_Solid(font, message, color);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    pos.h = surf->h;
    pos.w = surf->w;
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, nullptr, &pos);
    SDL_DestroyTexture(tex);
}

void Text::draw(SDL_Renderer *renderer, const char *msg) {
    SDL_Surface *surf;
    SDL_Texture *tex;
    surf = TTF_RenderText_Solid(font, msg, color);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    pos.h = surf->h;
    pos.w = surf->w;
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, nullptr, &pos);
    SDL_DestroyTexture(tex);
}

void Text::setColor(Text::RGB rgb) {
    color.r = rgb.r;
    color.g = rgb.g;
    color.b = rgb.b;
    color.a = 255;
}

bool Text::isClickable() const {
    return clickable;
}

void Text::setClickable(bool clickState) {
    Text::clickable = clickState;
}

const SDL_Rect *Text::getPos() const {
    return &bounds;
}

const SDL_Color &Text::getColor() const {
    return color;
}

const char *Text::getMessage() const {
    return message;
}


TTF_Font *Text::getFont() const {
    return font;
}




