//
// Created by u238 on 6/4/23.
//

#ifndef SANDBOX_TEXT_H
#define SANDBOX_TEXT_H


#include "Drawable.h"

//! Text UI element. Can be used as button
class Text : public Drawable{
private:
    //! Text position
SDL_Rect pos{};
    //! Text bounds
SDL_Rect bounds{};
//! Text contents
const char *message;
//! Text font
TTF_Font *font;
//! Text color
SDL_Color color{};
//! Indicator if text is clickable
bool clickable;

public:


    Text(const char *msg, TTF_Font *font, int x, int y, int w, int h, RGB coloring, bool click);

    //! Get Text position
    [[nodiscard]] const SDL_Rect *getPos() const;

    //! Get text color
    [[nodiscard]] const SDL_Color &getColor() const;

    //! Get text contents
    [[nodiscard]] const char *getMessage() const;

    //! Get text font
    [[nodiscard]] TTF_Font *getFont() const;

    //! Set text color using the RGB struct
    /*!
     *
     * @param rgb
     */
    void setColor(RGB rgb);

    //! Draw the text for user to see
    void draw(SDL_Renderer *renderer) override;

    //! Draw the text with changed message
    void draw(SDL_Renderer *renderer, const char *msg);

    //! Get the indicator if the text is clickable
    [[nodiscard]] bool isClickable() const;

    //! Set the indicator if the text is clickable
    void setClickable(bool clickState);

};


#endif //SANDBOX_TEXT_H
