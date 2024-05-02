//
// Created by u238 on 5/12/23.
//


#include "Game.h"

Game::Game() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
    }
    if (SDL_CreateWindowAndRenderer(INITIAL_WIDTH, INITIAL_HEIGHT, 0, &window, &renderer) < 0) {
        cerr << "Failed to create window and renderer: " << SDL_GetError() << endl;
    }
    SDL_SetWindowTitle(window, "SkySquare");
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    generalFont = TTF_OpenFont("Fonts/alpha_echo.ttf", 50);
    titleFont = TTF_OpenFont("Fonts/alpha_echo.ttf", 100);
    isRunning = jump = allowManipulation = menu = canHeal = true;
    canSave = canLoad = mouseOnL = mouseOnR = isDead = false;
    mapX = mapY = globalChangeX = globalChangeY = 0;
    deltaTime = currentTime = 0.0f;
    stPage = page = posPrev = 0;
    enPage = 5;
    gravity = DEFAULT_PLAYER_GRAVITY;
    getSaveFiles();
    loadMap();  //default world
    initMenuItems();
    inputBox = InputBox("", {255, 255, 0}, generalFont, 265, 250, renderer);
    height = frameCount = 0;
    gameLoop();
}

Game::~Game() {
    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::gameLoop() {
    while (isRunning) {
        currentTime = static_cast<float >(SDL_GetTicks());

        deltaTime = (currentTime - previousTime) / 1000;
        frameCount++;

        if ((frameCount / 60) > 100 && canHeal) {
            player.heal();
            frameCount = 0;
        }

        previousTime = currentTime;

        render();
        input();
        update(deltaTime);

    }

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 126, 192, 238, 255);
    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = INITIAL_WIDTH;
    rect.h = INITIAL_HEIGHT;
    SDL_RenderFillRect(renderer, &rect);

    drawMap();

    //uncomment the hit box
//    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
//    SDL_RenderFillRect(renderer, player.getHitBoxTop());
//    SDL_RenderFillRect(renderer, player.getHitBoxBot());
    if (!isDead) {
        player.draw(renderer);
    }

    if (!menu) {
        string tmpStr = to_string(player.getHealth());
        menuItems[10].draw(renderer, tmpStr.c_str());
        menuItems[11].draw(renderer);
    }

    if (isDead) {
        menuItems[12].draw(renderer);
    }

    if (menu) {
        showMenu();
    }

    if (canSave) {
        showSaveMenu();
    }

    if (canLoad) {
        showLoadMenu(stPage, enPage, page);
    }

    //60 frames per second
    int timerFPS = static_cast<int>((float) SDL_GetTicks() - deltaTime);
    if (timerFPS < (1000 / 60)) { // 1000/60 milisec to sec
        SDL_Delay(1000 / 60 - timerFPS);
    }
    SDL_RenderPresent(renderer);
}


void Game::input() {
    SDL_Event e;
    SDL_StartTextInput();

    while (SDL_PollEvent(&e)) {

        if (e.type == SDL_TEXTINPUT && canSave) {
            inputBox.type(e, renderer);
        }

        switch (e.type) {
            case SDL_QUIT:
                cout << "Quitting" << endl;
                exitGame();
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (menu) {
                            menu = false;
                            unpause();
                        } else {
                            // isRunning = false;
                            menu = true;
                        }
                        break;
                    case SDLK_a:
                        player.setVelocityX(-1 * DEFAULT_PLAYER_SPEED);
                        left = true;
                        moving = true;
                        break;
                    case SDLK_d:
                        player.setVelocityX(DEFAULT_PLAYER_SPEED);
                        left = false;
                        moving = true;
                        break;
                    case SDLK_SPACE:
                        if (!jump) {
                            player.setVelocityY(-1 * ACCELERATION);
                            jump = true;
                        }
                        break;
                    case SDLK_BACKSPACE:
                        inputBox.deleteBackChar(renderer);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                    case SDLK_d:
                        moving = false;
                        player.setVelocityX(0);
                        break;
                    case SDLK_SPACE:
                        player.setVelocityY(0);
                        break;
                    case SDLK_p:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (e.button.button) {
                    case SDL_BUTTON_LEFT:
                        mouseOnL = true;
                        break;
                    case SDL_BUTTON_RIGHT:
                        mouseOnR = true;
                        break;

                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (e.button.button) {
                    case SDL_BUTTON_LEFT:
                        mouseOnL = false;
                        break;
                    case SDL_BUTTON_RIGHT:
                        mouseOnR = false;
                        break;

                    default:
                        break;
                }
                break;
        }

        //todo idle animation

        SDL_GetMouseState(&mouseX, &mouseY);
    }

}


void Game::update(float dT) {


    if (canSave) {
        saveMenu();
    } else if (canLoad) {
        loadMenu();
    } else {
        turnOnMenuButtons();
    }

    if (menu) {
        mainMenu();
    }

    worldInteraction();

    player.incrementVelocityYBy(gravity); // gravity influence
    player.changePosX(player.getVelocity().x * dT);
    player.changePosY(player.getVelocity().y * dT);


    bool scrollDown = false;
    if (player.getPos().x < 100) {
        player.setPosX(110);
        player.setPosY(player.getPos().y);
        scroll(1, 0);
    }
    if (player.getPos().x > INITIAL_WIDTH - 100) {
        player.setPosX(INITIAL_WIDTH - 101);
        player.setPosY(player.getPos().y);
        scroll(-1, 0);
    }
    if (player.getPos().y < 50) {
        player.setPosX(player.getPos().x);
        player.setPosY(50);
        scroll(0, 1);
    }
    if (player.getPos().y > INITIAL_HEIGHT - 200) {
        player.setPosX(player.getPos().x);
        player.setPosY(INITIAL_HEIGHT - 200);
        scrollDown = true;
        scroll(0, -1);
    }

    player.setHitBoxBotX(static_cast<int>(player.getPos().x));
    player.setHitBoxBotY(static_cast<int>(player.getPos().y));
    player.setHitBoxTopX(static_cast<int>(player.getPos().x));
    player.setHitBoxTopY(static_cast<int>(player.getPos().y - static_cast<float>(player.getHitBoxBot()->h)));
    player.setDestination(player.getHitBoxTop()->x, player.getHitBoxTop()->y);


    if (player.getHitBoxTop()->y != posPrev) {
        if ((posPrev - player.getHitBoxTop()->y) < 0) {
            height++;
        }
        posPrev = player.getHitBoxTop()->y;
    }

    if (scrollDown) {
        height++;
    }

    if (globalChangeY < (-25000)) {
        player.setHealth(0);
    }

    if (player.getHealth() <= 0) {
        dead();
    }
}


void Game::loadMap() {
    player = Player({250, 150}, {0, 0}, {0, 0, 25, 25}, {0, 0, 25, 25});
    player.setDestination(player.getHitBoxTop()->x, player.getHitBoxTop()->y, 25, 50);
    player.setTexture("Images/entity.png", renderer);
    player.setSrc(26, 0, 25, 50);

    unsigned int totalLevel = 0;
    const char *filename = DEFAULT_SAVE_NAME;
    Block tmp;
    tmp.setTexture("Images/blocks.png", renderer);
    int current;
    here:
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Failed to find the map file. Generating new map..." << endl;
        generator.generateNewMap(filename, 0.7);
        in.close();
        cout << "Map generation successful." << endl;
        goto here;
    }
    in >> mapWidth;
    in >> mapHeight;
    in >> mapStartX;
    in >> mapStartY;
    int gameObjects = 0;
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            if (in.eof()) {
                cerr << "Reached end of map file too soon." << endl;
                return;
            }
            in >> current;

            tmp.setTypeId(current);
            tmp.setLevelId(totalLevel);
            totalLevel++;
            if (current) {
                tmp.setSrc((current - 1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
                tmp.setDestination((j * TILE_SIZE) + mapStartX, (i * TILE_SIZE) + mapStartY, TILE_SIZE, TILE_SIZE);
                if (current == 1 || current == 2) { tmp.setSolid(true); }
                gameMap.push_back(tmp);
                gameObjects++;
            }
            levelStructure.push_back(tmp);
        }
    }
    in.close();
}


//drawing textures as they come onto the screen
void Game::drawMap() {
    for (Block &i: gameMap) {
        if (i.getPos().x >= mapX - TILE_SIZE && i.getPos().y >= mapY - TILE_SIZE &&
            i.getPos().x <= mapX + INITIAL_WIDTH + TILE_SIZE && i.getPos().y <= mapY + INITIAL_HEIGHT + TILE_SIZE)
            i.draw(renderer);
    }
}

void Game::scroll(int x, int y) {
    globalChangeX += x;
    globalChangeY += y;
    for (auto &i: gameMap) {
        i.setDestination(i.getPos().x + x, i.getPos().y + y);
    }
}

bool Game::collision(int x, int y, const Block &b) {
    if (y >= b.getPos().y &&
        y <= b.getPos().y + b.getPos().h &&
        x >= b.getPos().x &&
        x <= b.getPos().x + b.getPos().w
            ) {
        return true;
    }
    return false;
}

bool Game::entityCollisionFloor(Entity &a, const Block &b) {
    if (a.getPos().y + static_cast<float>(a.getHitBoxBot()->h) >= static_cast<float>(b.getPos().y) &&
        a.getPos().y <= static_cast<float>(b.getPos().y + b.getPos().h)
        &&
        a.getPos().x + static_cast<float>(a.getHitBoxBot()->w) - 3 > static_cast<float>(b.getPos().x) &&
        a.getPos().x + 3 < static_cast<float>(b.getPos().x + b.getPos().w)
            ) {
        return true;
    }
    return false;
}

bool Game::entityCollisionTop(Entity &a, const Block &b) {
    if (a.getHitBoxTop()->y + a.getHitBoxTop()->h >= b.getPos().y &&
        a.getHitBoxTop()->y <= b.getPos().y + b.getPos().h &&
        a.getHitBoxTop()->x + a.getHitBoxTop()->w - 2 >= b.getPos().x &&
        a.getHitBoxTop()->x + 2 <= b.getPos().x + b.getPos().w
            ) {
        return true;
    }
    return false;
}

bool Game::entityCollisionWalls(Entity &a, const Block &b) {
    if (a.getHitBoxTop()->y + a.getHitBoxTop()->h + 2 >= b.getPos().y &&
        a.getHitBoxTop()->y + 2 <= b.getPos().y + b.getPos().h &&
        a.getHitBoxTop()->x + a.getHitBoxTop()->w >= b.getPos().x &&
        a.getHitBoxTop()->x <= b.getPos().x + b.getPos().w
            ) {
        return true;
    }
    return false;
}


void Game::showMenu() {
    pause();

    for (int i = 0; i < 5; i++) {
        menuItems[i].draw(renderer);
    }
}

void Game::showSaveMenu() {
    //SDL_RenderCopy(renderer, textImg, nullptr, &pos);
    inputBox.draw(renderer);
    menuItems[5].draw(renderer);
    menuItems[6].draw(renderer);
}

void Game::pause() {
    canHeal = false;
    allowManipulation = false;
    player.setVelocityX(0);
    player.setVelocityY(0);
    gravity = 0;
}

void Game::unpause() {
    allowManipulation = true;
    canHeal = true;
    gravity = DEFAULT_PLAYER_GRAVITY;
}

void Game::initMenuItems() {
    //title
    menuItems.push_back(Text("SkySquare", titleFont, 0, 0, 281, 100, {0, 0, 0}, false));

    //main manu
    menuItems.push_back(Text("New", generalFont, 0, 150, 100, 50, {0, 0, 0}, true));
    menuItems.push_back(Text("Save", generalFont, 0, 250, 100, 50, {0, 0, 0}, true));
    menuItems.push_back(Text("Load", generalFont, 0, 350, 120, 50, {0, 0, 0}, true));
    menuItems.push_back(Text("Exit", generalFont, 0, 450, 100, 50, {0, 0, 0}, true));

    //save menu
    menuItems.push_back(Text("Cancel", generalFont, 250, 300, 180, 50, {0, 0, 0}, true));
    menuItems.push_back(Text("OK", generalFont, 450, 300, 60, 50, {0, 0, 0}, true));

    //load menu
    menuItems.push_back(Text("Next", generalFont, 600, 500, 120, 50, {0, 0, 0}, true));
    menuItems.push_back(Text("Previous", generalFont, 300, 500, 240, 50, {0, 0, 0}, true));
    menuItems.push_back(Text("Cancel", generalFont, 400, 600, 240, 50, {0, 0, 0}, true));

    //player health indicator
    menuItems.push_back(Text("10 health", titleFont, 150, 0, 120, 100, {255, 0, 0}, false));
    menuItems.push_back(Text("HP:", titleFont, 0, 0, 60, 100, {255, 0, 0}, false));

    //player death menu
    menuItems.push_back(
            Text("YOU DIED", titleFont, INITIAL_WIDTH / 2, INITIAL_HEIGHT / 2, 281, 100, {255, 0, 0}, false));

}

void Game::saveGame(const char *filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << player.getPos().x << " " << player.getPos().y << endl;
        outputFile << player.getHealth() << endl;
        outputFile << mapWidth << " " << mapHeight << endl;
        outputFile << mapStartX + globalChangeX << " " << mapStartY + globalChangeY << endl;
        int j = 0;
        for (const auto &i: levelStructure) {
            outputFile << i.getTypeId() << " ";
            j++;
            if (j == mapWidth) {
                j = 0;
                outputFile << endl;
            }
        }
        outputFile.close();
    }
    cout << "Game saved" << endl;
}


void Game::loadGame(const char *filename) {
    gameMap.clear();
    unsigned int totalLevel = 0;
    float posX, posY;
    int HP;
    cout << "Loading map: " << filename << endl;
    Block tmpBlock;
    tmpBlock.setTexture("Images/blocks.png", renderer);
    int current;
    here:
    ifstream in(filename);

    if (!in.is_open()) {

        cerr << "Failed to find the map file. Generating new map..." << filename << endl;
        generator.generateNewMap(filename, 0.7);
        in.close();
        cout << "Map generation successful." << endl;
        goto here;
    }
    in >> posX;
    in >> posY;
    player.setPosX(posX);
    player.setPosY(posY);
    in >> HP;
    player.setHealth(HP);
    in >> mapWidth;
    in >> mapHeight;
    in >> mapStartX;
    in >> mapStartY;
    int gameObjects = 0;
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            if (in.eof()) {
                cerr << "Reached end of map file too soon." << endl;
                return;
            }
            in >> current;

            tmpBlock.setTypeId(current);
            tmpBlock.setLevelId(totalLevel);
            totalLevel++;
            if (current) {
                tmpBlock.setSrc((current - 1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
                tmpBlock.setDestination((j * TILE_SIZE) + mapStartX, (i * TILE_SIZE) + mapStartY, TILE_SIZE, TILE_SIZE);
                if (current == 1 || current == 2) { tmpBlock.setSolid(true); }
                gameMap.push_back(tmpBlock);
                gameObjects++;
            }
            levelStructure.push_back(tmpBlock);
        }
    }
    in.close();
    cout << "Entering world..." << endl;
}

void Game::exitGame() {
    isRunning = false;
    if (std::remove(DEFAULT_SAVE_NAME) != 0) {
        cerr << "Failed to delete the file." << endl;
    }
}

bool Game::ui_collision(int x, int y, const Text &b) {
    if (y >= b.getPos()->y &&
        y <= b.getPos()->y + b.getPos()->h &&
        x >= b.getPos()->x &&
        x <= b.getPos()->x + b.getPos()->w
            ) {
        return true;
    }
    return false;
}

void Game::getSaveFiles() {

    filesystem::path savesDirectory = "Saves";
    if (std::filesystem::exists(savesDirectory) && std::filesystem::is_directory(savesDirectory)) {
        for (const auto &entry: std::filesystem::directory_iterator(savesDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".level") {
                saveFiles.push_back(entry.path().string());
            }
        }
    } else {
        cerr << "The directory does not exist or is not accessible." << endl;
        return;
    }
    int j = 0;
    for (const auto &saveFile: saveFiles) {
        if (j == 5) {
            j = 0;
        }
        loadItems.push_back(
                Text(saveFile.c_str(), generalFont, 300, 200 + j * 60, static_cast<int>(saveFile.size() * 30), 50,
                     {0, 0, 0}, false));
        j++;
    }

}

void Game::showLoadMenu(int st, int en, int p) {
    for (auto &loadItem: loadItems) {
        loadItem.setClickable(false);
    }

    menuItems[9].draw(renderer);
    size_t allSize = loadItems.size();
    allSize = allSize - 5 * p;

    if (page) {
        menuItems[8].setClickable(true);
        menuItems[8].draw(renderer);
    } else {
        menuItems[8].setClickable(false);
    }
    if (allSize <= 5) {
        for (size_t i = st; i < loadItems.size(); i++) {
            loadItems[i].draw(renderer);
            loadItems[i].setClickable(true);
        }
    }
    if (allSize > 5) {
        menuItems[7].draw(renderer);
        menuItems[7].setClickable(true);
        for (int i = st; i < en; i++) {
            loadItems[i].draw(renderer);
            loadItems[i].setClickable(true);
        }
    } else {
        menuItems[7].setClickable(false);
    }

}

bool Game::inRange(int x, int y) {
    auto centerBlockX = static_cast<float >(x + 12);
    auto centerBlockY = static_cast<float >(y + 12);
    auto centerPlayerX = static_cast<float >(player.getHitBoxTop()->x + 20);
    auto centerPlayerY = static_cast<float >(player.getHitBoxTop()->y + 20);
    auto X = (centerBlockX - centerPlayerX);
    auto Y = (centerBlockY - centerPlayerY);
    float dis = sqrt(X * X + Y * Y);
    return dis < 50;
}

void Game::dead() {
    menu = true;
    isDead = true;
}

void Game::saveMenu() {
    turnOffMenuButtons();
    for (int i = 5; i < 7; i++) {
        if (ui_collision(mouseX, mouseY, menuItems[i])) {
            menuItems[i].setColor({255, 255, 0});
            if (mouseOnL) {
                if (i == 5) {
                    canSave = false;
                }
                if (i == 6) {
                    if (inputBox.getText().empty()) {
                        saveGame("Saves/NewWorld.level");
                    } else {
                        string toSave = "Saves/";
                        toSave += inputBox.getText();
                        toSave += ".level";
                        saveGame(toSave.c_str());
                        inputBox.clear();
                        toSave.clear();
                    }
                    canSave = false;
                }
                loadItems.clear();
                getSaveFiles();
                mouseOnL = false;
            }
        } else {
            menuItems[i].setColor({0, 0, 0});
        }
    }
}

void Game::turnOnMenuButtons() {
    for (int i = 1; i < 5; i++) {
        menuItems[i].setClickable(true);
    }
}

void Game::loadMenu() {
    turnOffMenuButtons();
    for (auto &loadItem: loadItems) {
        if (ui_collision(mouseX, mouseY, loadItem)) {
            loadItem.setColor({255, 255, 0});
            if (mouseOnL && loadItem.isClickable()) {
                loadGame(loadItem.getMessage());
                canLoad = false;
                menu = false;
                isDead = false;
                unpause();
                mouseOnL = false;
            }
        } else {
            loadItem.setColor({0, 0, 0});
        }
    }
    for (int i = 7; i < 10; i++) {
        if (ui_collision(mouseX, mouseY, menuItems[i])) {
            menuItems[i].setColor({255, 255, 0});
            if (mouseOnL) {
                switch (i) {
                    case 7:
                        if (menuItems[7].isClickable()) {
                            stPage += 5;
                            enPage += 5;
                            page++;
                        }
                        mouseOnL = false;
                        break;
                    case 8:
                        if (menuItems[8].isClickable()) {
                            stPage -= 5;
                            enPage -= 5;
                            page--;
                        }
                        mouseOnL = false;
                        break;
                    case 9:
                        canLoad = false;
                        mouseOnL = false;
                        break;
                    default:
                        mouseOnL = false;
                        break;
                }
            }
        } else {
            menuItems[i].setColor({0, 0, 0});
        }

    }
}

void Game::turnOffMenuButtons() {
    for (int i = 1; i < 5; i++) {
        menuItems[i].setClickable(false);
    }
}

void Game::mainMenu() {
    for (int i = 1; i < 5; i++) {
        if (ui_collision(mouseX, mouseY, menuItems[i]) && menuItems[i].isClickable()) {
            menuItems[i].setColor({255, 255, 0});
            if (mouseOnL) {
                switch (i) {
                    case 1:
                        if (isDead) {
                            gameMap.clear();
                            loadMap();
                            isDead = false;
                        }
                        menu = false;
                        unpause();
                        mouseOnL = false;
                        break;
                    case 2:
                        canSave = true;
                        break;
                    case 3:
                        canLoad = true;
                        break;
                    case 4:
                        exitGame();
                        break;
                    default:
                        mouseOnL = false;
                        break;
                }

            }
        } else {
            menuItems[i].setColor({0, 0, 0});
        }
    }
}

void Game::worldInteraction() {

    if (mouseOnR && allowManipulation) {
        int gMapX = mouseX / 25;
        int gMapY = mouseY / 25;
        int gridX = (mouseX - globalChangeX) / 25;
        int gridY = (mouseY - globalChangeY) / 25;
        int foundLevelId = (gridY * 1000) + gridX;

        if (!levelStructure[foundLevelId].getTypeId() && inRange(mouseX-12, mouseY-12)) {
            Block tmp;
            tmp.setTexture("Images/blocks.png", renderer);
            tmp.setSrc(0, 0, TILE_SIZE, TILE_SIZE);
            tmp.setDestination((gMapX * TILE_SIZE) + mapStartX, (gMapY * TILE_SIZE) + mapStartY, TILE_SIZE,
                               TILE_SIZE);
            tmp.setSolid(true);
            tmp.setLevelId(foundLevelId);
            gameMap.push_back(tmp);
            levelStructure[foundLevelId].setTypeId(1);
            drawMap();
        }
        mouseOnR = false;
    }

    for (size_t i = 0; i < gameMap.size(); i++) {

        if (collision(mouseX, mouseY, gameMap[i]) && mouseOnL && allowManipulation) {
            if (gameMap[i].isItSolid() && inRange(gameMap[i].getPos().x, gameMap[i].getPos().y)) {
                levelStructure[gameMap[i].getLevelId()].setTypeId(0);
                gameMap.erase(gameMap.begin() + i);
                mouseOnL = false;
            }
        }


        if (entityCollisionTop(player, gameMap[i])) {
            if (gameMap[i].isItSolid()) {
                player.setVelocityY(0);
            }
        }

        if (entityCollisionWalls(player, gameMap[i])) {
            if (gameMap[i].isItSolid() && !jump) {
                player.setVelocityX(0);
                if (moving) {
                    if (left) {
                        player.setVelocityX(1);
                    } else {
                        player.setVelocityX(-1);
                    }
                }
            }
        }

        if (entityCollisionFloor(player, gameMap[i])) {
            if (gameMap[i].isItSolid()) {
                if (height != 0) {
                    player.setHealth(player.getHealth() - (height / 250));
                    height = 0;
                }
                player.setPosY(static_cast<float >(gameMap[i].getPos().y - player.getHitBoxBot()->h - 1));
                player.setVelocityY(0);
                if (jump) {
                    jump = false;
                }
            }
        }

    }
}




