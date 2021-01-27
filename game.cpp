#include "game.hpp"
#include "assets.hpp"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define EMPTY_TILE_ID 255

#define CAMERA_SCALE 0.1

#define SPRITE_SIZE 8

using namespace blit;


const uint8_t SCREEN_MID_WIDTH = SCREEN_WIDTH / 2;
const uint8_t SCREEN_MID_HEIGHT = SCREEN_HEIGHT / 2;


// struct to handle level data header...
// this will probably need to be revisited/reworked if 32blit-tools has *any* kind of update to it...
#pragma pack(push,1)
struct TMX {
    char head[4];
    uint8_t empty_tile;
    uint16_t width;
    uint16_t height;
    uint16_t layers;
    uint8_t data[];
};
#pragma pack(pop)

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

    Camera() {
        x = y = 0;
    }
};

class LevelObject {
public:
    LevelObject() {
        x = y = 0;
    }

    LevelObject(int xPosition, int yPosition) {
        x = xPosition;
        y = yPosition;
    }

    virtual void update(double dt, ButtonStates buttonStates) = 0;

    virtual void render(Camera camera) = 0;

protected:
    int x, y;
};

class Tile : LevelObject {
public:
    Tile() : LevelObject() {
        id = EMPTY_TILE_ID;
    }

    Tile(int xPosition, int yPosition, uint8_t tileID) : LevelObject(xPosition, yPosition) {
        id = tileID;
    }

    void update(double dt, ButtonStates buttonStates) {

    }

    void render(Camera camera)
    {
        screen.sprite(id, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
    }

protected:
    uint8_t id;
};

class Pickup : LevelObject {
public:
    Pickup() : LevelObject() {

    }

    Pickup(int xPosition, int yPosition) : LevelObject(xPosition, yPosition) {

    }

    virtual void update(double dt, ButtonStates buttonStates) = 0;

    virtual void render(Camera camera) = 0;

protected:

};

class AnimatedPickup : Pickup {
public:
    AnimatedPickup() : Pickup() {

    }

    AnimatedPickup(int xPosition, int yPosition, std::vector<uint8_t> frames) : Pickup(xPosition, yPosition) {

    }

    void update(double dt, ButtonStates buttonStates) {

    }

    void render(Camera camera) {

    }

protected:
    std::vector<uint8_t> frames;
};

class Coin : AnimatedPickup {
public:
    Coin() : AnimatedPickup() {

    }

    Coin(int xPosition, int yPosition, std::vector<uint8_t> frames) : AnimatedPickup(xPosition, yPosition, frames) {

    }

    void update(double dt, ButtonStates buttonStates) {

    }

    void render(Camera camera) {

    }

protected:

};

class Entity : LevelObject {
public:
    Entity() : LevelObject() {

    }

    Entity(int xPosition, int yPosition) : LevelObject(xPosition, yPosition) {
        x = xPosition;
        y = yPosition;

        state = IDLE;
    }

    void update(double dt, ButtonStates buttonStates) {

    }

    void render(Camera camera) {

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
    Player() : Entity() {

    }

    Player(int xPosition, int yPosition) : Entity(xPosition, yPosition) {

    }

    void update(double dt, ButtonStates buttonStates) {
        if (buttonStates.A && state != JUMP) {
            // Jump.
            state = JUMP;
        }
    }

    void render(Camera camera) {

    }

    void update_camera(Camera camera) {
        /*camera.x += (x - camera.x) * CAMERA_SCALE;
        camera.y += (y - camera.x) * CAMERA_SCALE;*/
    }

protected:

};

class Enemy : Entity {
public:
    Enemy() : Entity() {

    }

    Enemy(int xPosition, int yPosition) : Entity(xPosition, yPosition) {

    }

    void update(double dt, ButtonStates buttonStates) {

    }

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

    Camera camera;

    Player player;
} levelObjects;



const uint8_t* asset_levels[] = {
    asset_level0
};




void render_tiles(std::vector<Tile> tiles) {
    for (int i = 0; i < tiles.size(); i++) {
        tiles.at(i).render(levelObjects.camera);
    }
}

void render_coins(std::vector<Coin> coins) {
    for (int i = 0; i < coins.size(); i++) {
        coins.at(i).render(levelObjects.camera);
    }
}

void render_enemies(std::vector<Enemy> enemies) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies.at(i).render(levelObjects.camera);
    }
}

void render_level() {
    render_tiles(levelObjects.background);
    render_tiles(levelObjects.foreground);

    render_coins(levelObjects.coins);
}

void render_entities() {
    render_enemies(levelObjects.enemies);

    levelObjects.player.render(levelObjects.camera);
}

void load_level(uint8_t levelNumber) {
    // Get a pointer to the map header
    TMX* tmx = (TMX*)asset_levels[levelNumber];

    uint16_t levelWidth = tmx->width;
    uint16_t levelHeight = tmx->height;
    uint32_t levelSize = levelWidth * levelHeight;

    levelObjects.foreground.clear();
    levelObjects.background.clear();
    levelObjects.coins.clear();
    levelObjects.enemies.clear();

    for (int i = 0; i < levelSize; i++) {
        if (tmx->data[i] != EMPTY_TILE_ID) {
            // Isn't a blank tile, let's add it to the foreground tiles
            //printf("%u %u %u \n", (i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i]);
            levelObjects.foreground.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i]));
        }
    }

    for (int i = levelSize; i < levelSize * 2; i++) {
        if (tmx->data[i] != EMPTY_TILE_ID) {
            // Isn't a blank tile, let's add it to the background tiles
            levelObjects.background.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i]));
        }
    };
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

    load_level(0); // remove later
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

    render_tiles(levelObjects.background);
    render_tiles(levelObjects.foreground);

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
}