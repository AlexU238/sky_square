//
// Created by u238 on 5/12/23.
//

#ifndef SANDBOX_GAME_H
#define SANDBOX_GAME_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameObject.h"
#include <fstream>
#include <vector>
#include "Player.h"
#include "Block.h"
#include "MapGenerator.h"
#include "Text.h"
#include <csignal>
#include <iterator>
#include <filesystem>
#include "InputBox.h"

#define INITIAL_WIDTH 1080
#define INITIAL_HEIGHT 720
#define TILE_SIZE 25
#define DEFAULT_PLAYER_SPEED 150
#define DEFAULT_PLAYER_GRAVITY 1
#define ACCELERATION 500
#define DEFAULT_SAVE_NAME "Saves/tmpSave.level"


using namespace std;
//! Main Game class
/*!
  Controls everything that happens in the Game.
  Movement, collisions, saving, loading, destruction of blocks.
*/
class Game {
private:
    //! SDL renderer
    SDL_Renderer *renderer{};
    //! SDL window (scree)
    /*! The application window */
    SDL_Window *window{};
    //! Fonts
    /*! Title font is big (100 pixels), general is small (50 pixels) */
    TTF_Font *generalFont, *titleFont;
    //! Program running indicator
    /*! Determines if the game loop is still running. The program runs as long as the game loop is running. */
    bool isRunning;
    //! Time frames
    /*! deltaTime=time between frames,
     * currentTime=time when current frame happened,
     * previousTime=time when the previous frame happened */
    float deltaTime, currentTime, previousTime{};
    //! mouse position coordinates
    int mouseX{}, mouseY{};
    //! map generator
    MapGenerator generator;
    //! tile map vectors
    /*! gameMap=tiles that will be drawn on screen,
     * levelStructure=keeps the levelID of blocks to keep track of overall level structure
      */
    vector<Block>gameMap,levelStructure;
    //! UI element vectors
    /*! menuItems=keeps all buttons and text,
     * loadItems=keeps names of loaded save files as clickable buttons
      */
    vector<Text>menuItems, loadItems;
    //! loaded names of save items vector
    vector<string>saveFiles;
    //! coordinates of start of tile map drawing
    int mapX, mapY;
    //! Previous position of player
    /*! Used to store position of player for as part of the way to determine falling */
    int posPrev;
    //! Player
    Player  player;
    //! boolean values of different game and player states
    /*!
     * jump= if player jumped
     * canSave= open save menu
     * canLoad= open load menu
     * mouseOnL= Left mouse button clicked
     * mouseOnR= Right mouse button clicked
     * left= player moving left or not
     * moving= player is moving
     * menu= open main menu
     * allowManipulation= player can destroy or place blocks
     * canHeal= player can replenish health
     * isDead= player health is zero, game over
     */
    bool jump, canSave, canLoad, mouseOnL, mouseOnR, left{}, moving{}, menu, allowManipulation, canHeal, isDead;
    //! integer values for utility reasons
    /*!
     * mapStartX, mapStartY= default map drawing start position
     * mapWidth, mapHeight = tile map width and height
     * globalChangeX, globalChangeY = values of how much the camera moved following player
     * stPage, enPage, page = values of pages in the load menu
     */
    int mapStartX{}, mapStartY{}, mapWidth{}, mapHeight{}, globalChangeX, globalChangeY, stPage, enPage, page;
    //!Player gravity
    float gravity;
    //! Text input
    InputBox inputBox;
    //! Utility values
    /*!
     * height= how much player fell in pixels
     * frameCount= counter of frames to keep track of time passed
     */
    int height,frameCount;

public:
    //!Game constructor
    /*!
     * Initializes SDL renderer, window, image, font.
     * Assigned initial values for jump, canSave, canLoad, mouseOnL, mouseOnR, left{}, moving{}, menu, allowManipulation, canHeal, isDead,
     * mapStartX, mapStartY, mapWidth, mapHeight, globalChangeX, globalChangeY, stPage, enPage, page, gravity,height,frameCount
     * loads existing save files by calling getSaveFiles();
     * loads default map by calling loadMap();
     * initializes the ui by calling initMenuItems();
     * initializes input box
     * starts the game loop
     */
    Game();

    ~Game();

    //! Game loop
    /*!
     * While isRunning true:
     * 1. Calculate deltaTime
     * 2. Determine if the player can heal, if yes do so
     * 3. call render();
     * 4. call input();
     * 5. call update(deltaTime);
     */
    void gameLoop();

    //! Update - updates the state of objects
    /*!
     *
     * @param dT - deltaTime
     * Fills the background
     * draws map by calling drawMap();
     * keeps track of game state
     * if canSave is true call saveMenu();
     * if canLoad is true call loadMenu();
     * call turn turnOnMenuButtons(); if any of those states are true
     * if menu is true call mainMenu();
     *
     * Update the world, camera position, player position, abilities and alive state according to actions of user
     * Uses worldInteraction(); to keep track of player collisions and block destruction/placement events
     */
    void update(float dT);

    //! Render - provides visuals to user
    /*!
     * Fills the background
     * draws map by calling drawMap();
     * if isDead is false - draws player
     * if isDead is true - draws the end game title
     * if menu is false draws player visible ui
     * if manu is true calls showMenu();
     * if canLoad is true calls showLoadMenu(stPage, enPage, page);
     * if casSave is true calls showSaveMenu();
     * keeps track of frames per second, limits them at 60
     * Renders the contents of renderer by SDL_RenderPresent(renderer);
     */
    void render();

    //! Input - provides responses to user input
    /*!
     * Responses to user manipulations are:
     * Text input - active only when the save menu is active, uses keyboard;
     * 'LMB' on the cross of the application window will close it;
     * 'ESC' key - calls main menu or starts new game if the main menu is active.
     * 'A' key - makes player move left
     * 'D' key - makes player move right
     * 'SPACEBAR' key - makes player to move up in a burst of speed (jump)
     * 'BACKSPACE' key - deletes the last current character in the input box
     * 'LMB' on a block - destroy block
     * 'RMB' on empty space - place block
     *
     * Gets cursor coordinates
     */
    void input();

    //! Check if the coordinates are in 50 pixel radius away form player
    /*!
     *
     * @param x - x axis coordinate
     * @param y - y axis coordinate
     * @return bool
     *
     * uses the sqrt(X^2 + X^2) formula to determine the distance
     * X is the subtraction of player x position from the the parameter x
     * Y is the subtraction of player y position from the the parameter y
     * return true if the result of equation is less then 50
     */
    bool inRange(int x, int y);

    //! Load default game state
    /*!
     * initializes the player, his initial position, height,width,texture,hitbox
     * first looks for default save file, and fills the gameMap and levelStructure values if such file present.
     * this was made to allow map presets
     * if the default file not found uses mapGenerator to create such and load the values from it
     */
    void loadMap();

    //!Draws the textures for the map
    void drawMap();

    //! check if the bottom hitbox of the entity collides with the blocks under it
    /*!
     * @param a - entity
     * @param b - block
     * @return - boolean
     */
    static bool entityCollisionFloor(Entity& a, const Block& b);

    //! check if the top hitbox of the entity collides with blocks under it
    /*!
    * @param a - entity
    * @param b - block
    * @return - boolean
    */
    static bool entityCollisionTop(Entity& a, const Block& b);

    //! check if the top hitbox collides with the blocks to the left or right of it
    /*!
    * @param a - entity
    * @param b - block
    * @return - boolean
    */
    static bool entityCollisionWalls(Entity& a, const Block& b);

    //! check collision with coordinates
    /*!
     *
     * @param x - x coordinate
     * @param y - y coordinate
     * @param b - block
     * @return boolen
     */
    static bool collision(int x, int y, const Block& b);

    //! check collision with Text elements
    /*!
     *
     * @param x
     * @param y
     * @param b
     * @return
     */
    static bool ui_collision(int x, int y, const Text& b);

    //! camera movement with player
    void scroll(int x, int y);

    //! pause the game
    /*!
     *  make player and mob states false
     *  make gravity zero
     */
    void pause();

    //! unpause the game
    /*!
     * revert changes made by pause();
     */
    void unpause();

    //! Initialize ui items
    void initMenuItems();

    //! draw the main menu on screen
    void showMenu();

    //! draw the save menu on screen
    void saveMenu();

    //! draw the load menu on screen
    void loadMenu();

    //! make main menu buttons clickable
    void turnOnMenuButtons();

    //! make main menu buttons not clickable
    void turnOffMenuButtons();

    //! keep track of main menu functionality
    /*!
     * if the cursor collides with the clickable ui element and the LMB is pressed the element will be active
     * Buttons:
     * 1. New game. Just unpauses default game, the main menu becomes not active
     * 2. Save. Save menu becomes active
     * 3. Load. Load menu becomes active
     * 4. Exit. Shuts down the program.
     */
    void mainMenu();

    //! keep track of save menu functionality
    /*!
     * if the cursor collides with the clickable ui element and the LMB is pressed the element will be active
     * Buttons:
     * 1. OK. Calls saveGame(const char* filename); with the input box content as parameter
     * 2. Chancel. Save menu becomes non active
     * 3. Text input. Replaces the current content of the input box with the user input
     */
    void showSaveMenu();

    //! keep track of effect of player on world
    /*!
     * checks collisions and mouse-block interactions
     */
    void worldInteraction();

    //! Load existing save files
    void getSaveFiles();

    //! draws the load menu onto screen
    /*!
     *
     * @param st
     * @param en
     * @param page
     *
     * input parameters are pages of load menu that allow to keep track of on which save files the user is currently looking at
     * pressing buttons NEXT and PREVIOUS on screen increment and decrement the page values
     */
    void showLoadMenu(int st, int en, int page);

    //! Saves the state of objects in game to .level file
    /*!
     *
     * @param filename
     *
     * Writes player current position, health and levelStructure to the .level file
     */
    void saveGame(const char* filename);

    //! Change the state of the objects in game to that of a file
    /*!
     *
     * @param filename
     *
     * Takes the values from a chosen file and applies them to objects in game
     */
    void loadGame(const char* filename);

    //! Shut down the game loop
    void exitGame();

    //! Actions to take when player is dead
    void dead();

};


#endif //SANDBOX_GAME_H
