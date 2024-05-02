//
// Created by u238 on 5/12/23.
//

#ifndef SANDBOX_BLOCK_H
#define SANDBOX_BLOCK_H

#include "GameObject.h"

//! Block object class
class Block : public GameObject {
private:

    unsigned int blockTypeID{};

    unsigned int levelID{};

    bool isSolid = false;

public:

    //! Default constructor
    Block() = default;

    //! Default destructor
    ~Block() override = default;

    //! get Id of block regarding the texture
    [[nodiscard]] unsigned int getTypeId() const;

    //! set Id of block regarding the texture
    void setTypeId(unsigned int p_id);

    //! set Id of block on tile map of the generated world
    void setLevelId(unsigned int p_id);

    //! get Id of block on tile map of the generated world
    [[nodiscard]] unsigned int getLevelId() const;

    //! get indicator if there can be collision with this block
    [[nodiscard]] bool isItSolid() const;

    //! set indicator if there can be collision with this block
    void setSolid(bool p_isSolid);

    //! get block position
    [[nodiscard]] SDL_Rect getPos() const;

    //!draw inherited from GameObject
    void draw(SDL_Renderer *renderer) override;

};


#endif //SANDBOX_BLOCK_H
