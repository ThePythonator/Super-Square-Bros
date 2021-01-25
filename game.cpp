#include "game.hpp"
#include "assets.hpp"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

using namespace blit;

enum GameState {
    MENU_STATE,
    LEVEL_SELECT_STATE,
    IN_GAME_STATE,
    LOST_STATE
};
GameState gameState = MENU_STATE;

struct ButtonStates {
    uint8_t UP;
    uint8_t DOWN;
    uint8_t LEFT;
    uint8_t RIGHT;
    uint8_t A;
    uint8_t B;
    uint8_t X;
    uint8_t Y;
    uint8_t JOYSTICK;
};
ButtonStates buttonStates = { 0 };

struct SaveData {
    uint32_t highscore;
} saveData;

struct Camera {
    float x, y;
} camera;

class LevelObject {
public:
    LevelObject() {
        x = y = 0;
    }

    virtual void update(double dt, ButtonStates buttonStates) = 0;

    virtual void render(Camera camera) = 0;

protected:
    int x, y;
};

class Tile : LevelObject {
public:
    Tile(int xPosition, int yPosition, uint8_t tileID) {
        x = xPosition;
        y = yPosition;

        id = tileID;
    }

    void render(Camera camera)
    {
        screen.sprite(id, Point(camera.x - x, camera.y - y));
    }

protected:
    uint8_t id;
};

class Pickup : LevelObject {
public:
    void render(Camera camera) {

    }

protected:

};

class Coin : Pickup {
public:
    void render(Camera camera) {

    }

protected:

};

class Entity : LevelObject {
public:
    Entity(int xPosition, int yPosition) {
        x = xPosition;
        y = yPosition;

        state = IDLE;
    }

protected:
    enum EntityState {
        IDLE,
        WALK,
        RUN,
        JUMP,
        CROUCH,
        INJURED,
        DEAD
    } state;
};

class Player : Entity {
public:
    void update(double dt, ButtonStates buttonStates) {
        if (buttonStates.A && state != JUMP) {
            // Jump.
            state = JUMP;
        }
    }

    void render(Camera camera) {

    }

protected:

};

class Enemy : Entity {
public:
    void render(Camera camera) {

    }

protected:

};

//maybe inherit from entity to create specific enemy types, but all should fit into a vector<Enemy> - same interface for each

struct LevelObjects {
    std::vector<Tile> foreground;
    std::vector<Tile> background;

    std::vector<Coin> coins;

    std::vector<Enemy> enemies;

    Player player;
} levelObjects;

void load_level() {
    levelObjects.foreground.clear();
    levelObjects.background.clear();
    levelObjects.coins.clear();
    levelObjects.enemies.clear();
    // for tileid in tile ids
    // levelObjects.foreground(i) = tile
    // wehen loading tiles, take one off id to get index
}

void render_tiles(std::vector<Tile> tiles) {
    for (int i = 0; i < tiles.size(); i++) {
        tiles.at(i).render(camera);
    }
}

void render_coins(std::vector<Coin> coins) {
    for (int i = 0; i < coins.size(); i++) {
        coins.at(i).render(camera);
    }
}

void render_enemies(std::vector<Enemy> enemies) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies.at(i).render(camera);
    }
}

void render_level() {
    render_tiles(levelObjects.background);
    render_tiles(levelObjects.foreground);

    render_coins(levelObjects.coins);
}

void render_entities() {
    render_enemies(levelObjects.enemies);

    levelObjects.player.render(camera);
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    set_screen_mode(ScreenMode::lores);

    screen.sprites = Surface::load(asset_sprites);
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();

    // draw some text at the top of the screen
    screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(255, 255, 255);

    if (gameState == MENU_STATE) {
        screen.text("Press A to Start", minimal_font, Point(SCREEN_WIDTH / 2, 4), true, TextAlign::center_center);
    }
    else if (gameState == LEVEL_SELECT_STATE) {

    }
    else if (gameState == IN_GAME_STATE) {

    }
    else if (gameState == LOST_STATE) {

    }

    screen.pen = Pen(0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
    if (buttons.pressed & Button::A) {
        if (buttonStates.A == 2) {
            buttonStates.A = 1;
        }
        else {
            buttonStates.A = 2;
        }
    }
    else {
        buttonStates.A = 0;
    }

    printf("%d", asset_assets_level01_tmx[0]);
}