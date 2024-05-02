//
// Created by u238 on 6/10/23.
//

#ifndef SANDBOX_INPUTBOX_H
#define SANDBOX_INPUTBOX_H

#include "Drawable.h"
#include <string>

using namespace std;

//! UI element for user input
class InputBox : public Drawable{
private:
    //! Input box message
    string text;
    //! Position of the input box
    SDL_Rect pos{};
    //! The text in graphical representation
    SDL_Texture *texture{};
    //! Position of the text
    SDL_Surface *dest{};
    //! Color of the text
    SDL_Color color{};
    //! Text font
    TTF_Font *font{};
    //! Top right corner of the box
    int x{}, y{};
public:

    InputBox();

    //! Constructor takes RGB for easier specification of color
    InputBox(string text, RGB rgb, TTF_Font *font, int x, int y, SDL_Renderer *renderer);

    ~InputBox() override = default;

    //! Draw the input for user to see
    void draw(SDL_Renderer *renderer) override;

    //! Take and process the input form the keyboard
    void type(SDL_Event e, SDL_Renderer *renderer);

    //! Get the Input box message contents
    [[nodiscard]] const string &getText() const;

    //! Clear the input box contents
    void clear();

    //! Delete the last character in the input box message
    void deleteBackChar(SDL_Renderer *renderer);
};


#endif //SANDBOX_INPUTBOX_H
