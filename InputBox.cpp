//
// Created by u238 on 6/10/23.
//

#include "InputBox.h"

#include <utility>

InputBox::InputBox() = default;


InputBox::InputBox(string text, RGB rgb, TTF_Font *font, int x, int y, SDL_Renderer *renderer) : text(std::move(text)),
                                                                                                 font(font), x(x),
                                                                                                 y(y) {
    color.r = rgb.r;
    color.g = rgb.g;
    color.b = rgb.b;
    color.a = 255;
    dest = TTF_RenderText_Solid(font, "NewWorld", color);
    texture = SDL_CreateTextureFromSurface(renderer, dest);
    pos = {x, y, dest->w, dest->h};
    SDL_FreeSurface(dest);
    SDL_RenderCopy(renderer, texture, nullptr, &pos);
}

void InputBox::draw(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &pos);
}

void InputBox::type(SDL_Event e, SDL_Renderer *renderer) {
    text += e.text.text;
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    dest = TTF_RenderText_Solid(font, text.c_str(), color);
    if (dest) {
        texture = SDL_CreateTextureFromSurface(renderer, dest);
        pos.w = dest->w;
        pos.h = dest->h;
        SDL_FreeSurface(dest);
    }
}

const string &InputBox::getText() const {
    return text;
}

void InputBox::clear() {
    text.clear();
}

void InputBox::deleteBackChar(SDL_Renderer *renderer) {
    if (!text.empty()) {
        text.pop_back();
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
        dest = TTF_RenderText_Solid(font, text.c_str(), color);
        if (dest) {
            texture = SDL_CreateTextureFromSurface(renderer, dest);
            pos.w = dest->w;
            pos.h = dest->h;
            SDL_FreeSurface(dest);
        }
    }
}








