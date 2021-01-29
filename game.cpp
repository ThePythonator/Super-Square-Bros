#include "game.hpp"
#include "assets.hpp"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define TILE_ID_EMPTY 255
#define TILE_ID_COIN 24

#define CAMERA_SCALE 10

#define SPRITE_SIZE 8


#define PLAYER_MAX_HEALTH 3


#define GRAVITY 600
#define GRAVITY_MAX 200

#define PLAYER_MAX_JUMP 190
#define PLAYER_MAX_SPEED 80

using namespace blit;


// NOTE: all positions (x,y) mark TOP LEFT corner of sprites


const uint8_t SCREEN_MID_WIDTH = SCREEN_WIDTH / 2;
const uint8_t SCREEN_MID_HEIGHT = SCREEN_HEIGHT / 2;


const uint8_t SPRITE_HALF = SPRITE_SIZE / 2;


double dt;
uint32_t lastTime = 0;


const uint8_t* asset_levels[] = {
    asset_level0
};



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
    STATE_MENU,
    STATE_LEVEL_SELECT,
    STATE_IN_GAME,
    STATE_LOST
};
GameState gameState = STATE_MENU;

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

struct LevelData {
    uint16_t levelWidth, levelHeight;
} levelData;

class Camera {
public:
    float x, y;

    bool locked;

    Camera() {
        x = y = 0;
        locked = false;
    }

    void ease_to(double dt, float targetX, float targetY) {
        if (!locked) {
            x += (targetX - x) * CAMERA_SCALE * dt;
            y += (targetY - y) * CAMERA_SCALE * dt;
        }
    }
};
Camera camera;



class LevelObject {
public:
    uint16_t x, y;

    LevelObject() {
        x = y = 0;
    }

    LevelObject(uint16_t xPosition, uint16_t yPosition) {
        x = xPosition;
        y = yPosition;
    }

    virtual void update(double dt, ButtonStates buttonStates) = 0;

    virtual void render(Camera camera) = 0; 

protected:
};

class Tile : public LevelObject {
public:
    Tile() : LevelObject() {
        id = TILE_ID_EMPTY;
    }

    Tile(uint16_t xPosition, uint16_t yPosition, uint8_t tileID) : LevelObject(xPosition, yPosition) {
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
std::vector<Tile> foreground;
std::vector<Tile> background;

class Pickup : public LevelObject {
public:
    Pickup() : LevelObject() {

    }

    Pickup(uint16_t xPosition, uint16_t yPosition) : LevelObject(xPosition, yPosition) {

    }

    virtual void update(double dt, ButtonStates buttonStates) = 0;

    virtual void render(Camera camera) = 0;

protected:

};

class AnimatedPickup : public Pickup {
public:
    AnimatedPickup() : Pickup() {

    }

    AnimatedPickup(uint16_t xPosition, uint16_t yPosition, std::vector<uint8_t> frames) : Pickup(xPosition, yPosition) {

    }

    void update(double dt, ButtonStates buttonStates) {

    }

    void render(Camera camera) {

    }

protected:
    std::vector<uint8_t> frames;
};

class Coin : public AnimatedPickup {
public:
    Coin() : AnimatedPickup() {

    }

    Coin(uint16_t xPosition, uint16_t yPosition, std::vector<uint8_t> frames) : AnimatedPickup(xPosition, yPosition, frames) {

    }

    void update(double dt, ButtonStates buttonStates) {

    }

    void render(Camera camera) {

    }

protected:

};
std::vector<Coin> coins;

class Entity { // : public LevelObject
public:
    float x, y;
    uint8_t health;

    Entity() { // : LevelObject()
        x = y = 0;
        xVel = yVel = 0;

        state = IDLE;

        locked = false;

        health = 1;
    }

    Entity(uint16_t xPosition, uint16_t yPosition, uint8_t startHealth) { // : LevelObject(xPosition, yPosition)
        x = xPosition;
        y = yPosition;

        xVel = yVel = 0;

        health = startHealth;

        state = IDLE;

        locked = false;
    }

    void update(double dt, ButtonStates buttonStates) {
        if (!locked) {
            // Update gravity
            yVel += GRAVITY * dt;
            yVel = std::min(yVel, (float)GRAVITY_MAX);

            // Move entity y
            y += yVel * dt;

            // Here check collisions...
            
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground.at(i))) {
                    if (yVel > 0) {
                        // Collided from top
                        y = foreground.at(i).y - SPRITE_SIZE;
                    }
                    else {
                        // Collided from bottom
                        y = foreground.at(i).y + SPRITE_SIZE;
                    }
                    yVel = 0;
                }
            }

            // Move entity x
            x += xVel * dt;
            
            // Here check collisions...
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground.at(i))) {
                    if (xVel > 0) {
                        // Collided from left
                        x = foreground.at(i).x - SPRITE_SIZE;
                    }
                    else {
                        // Collided from right
                        x = foreground.at(i).x + SPRITE_SIZE;
                    }
                    xVel = 0;
                }
            }

            if (y > levelData.levelHeight * SPRITE_SIZE) {
                health = 0;
            }

            if (health == 0) {
                state = DEAD;
                // do something?
                x = y = 20; health = 3; yVel = -10;//remove
            }
        }
    }

    void render(Camera camera) {

    }

    bool colliding(Tile tile) {
        // Replace use of this with actual code?
        return (tile.x + SPRITE_SIZE > x && tile.x < x + SPRITE_SIZE && tile.y + SPRITE_SIZE > y && tile.y < y + SPRITE_SIZE);
    }

protected:
    bool locked;
    enum EntityState {
        IDLE,
        WALK,
        //RUN,
        JUMP,
        //CROUCH,
        //INJURED,
        DEAD
    } state;

    float xVel, yVel;
};

class Player : public Entity {
public:
    Player() : Entity() {

    }

    Player(uint16_t xPosition, uint16_t yPosition) : Entity(xPosition, yPosition, PLAYER_MAX_HEALTH) {

    }

    void update(double dt, ButtonStates buttonStates) {

        xVel = 0; // remove later? - change to fast acceleration?

        if (buttonStates.A) {
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (y + SPRITE_SIZE == foreground.at(i).y && foreground.at(i).x + SPRITE_SIZE > x && foreground.at(i).x < x + SPRITE_SIZE) {
                    // On top of block
                    // Jump
                    yVel = -PLAYER_MAX_JUMP;
                    state = JUMP;
                }
            }
        }


        if (buttonStates.LEFT) {
            xVel -= PLAYER_MAX_SPEED;
        }
        if (buttonStates.RIGHT) {
            xVel += PLAYER_MAX_SPEED;
        }

        Entity::update(dt, buttonStates);
    }

    void render(Camera camera) {
        screen.sprite(40, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y)); //remove
    }

protected:

};
Player player;

class Enemy : public Entity {
public:
    Enemy() : Entity() {

    }

    Enemy(uint16_t xPosition, uint16_t yPosition, uint8_t startHealth) : Entity(xPosition, yPosition, startHealth) {

    }

    void update(double dt, ButtonStates buttonStates) {
        Entity::update(dt, buttonStates);
    }

    void render(Camera camera) {

    }

protected:

};
std::vector<Enemy> enemies;

//maybe inherit from entity to create specific enemy types, but all should fit into a vector<Enemy> - same interface for each


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
    render_tiles(background);
    render_tiles(foreground);

    render_coins(coins);
}

void render_entities() {
    render_enemies(enemies);

    player.render(camera);
}

void load_level(uint8_t levelNumber) {
    // Variables for finding start and finish positions
    uint16_t finishX, finishY, startX, startY;
    finishX = finishY = startX = startY = 0;

    // Get a pointer to the map header
    TMX* tmx = (TMX*)asset_levels[levelNumber];

    uint16_t levelWidth = tmx->width;
    uint16_t levelHeight = tmx->height;
    uint32_t levelSize = levelWidth * levelHeight;

    levelData.levelWidth = levelWidth;
    levelData.levelHeight = levelHeight;

    foreground.clear();
    background.clear();
    coins.clear();
    enemies.clear();

    for (int i = 0; i < levelSize; i++) {
        if (tmx->data[i] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else if (tmx->data[i] == TILE_ID_COIN) {

        }
        else {
            foreground.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i]));
        }
    }

    for (int i = 0; i < levelSize; i++) {
        if (tmx->data[i + levelSize] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        /*else if (tmx->data[i + levelSize] == TILE_ID_COIN) {

        }*/
        else {
            background.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i + levelSize]));
        }
    }

    // Reset player attributes
    player = Player(startX, startY);
}

void render_menu() {
    screen.text("Super Square Bros.", minimal_font, Point(SCREEN_MID_WIDTH, 20), true, TextAlign::center_center);
    screen.text("Press A to Start", minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 10), true, TextAlign::center_center);
}

void render_game() {
    render_level();

    render_entities();
}

void update_menu(double dt, ButtonStates buttonStates) {
    if (buttonStates.A == 2) {
        gameState = STATE_IN_GAME; // change to LEVEL_SELECT later

        load_level(0); // move to update_level later
    }
}

void update_game(double dt, ButtonStates buttonStates) {
    player.update(dt, buttonStates);


    camera.ease_to(dt, player.x, player.y);
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

    if (gameState == STATE_MENU) {
        render_menu();
    }
    else if (gameState == STATE_LEVEL_SELECT) {

    }
    else if (gameState == STATE_IN_GAME) {
        render_game();
    }
    else if (gameState == STATE_LOST) {

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
    // Get dt
    dt = (time - lastTime) / 1000.0;
    lastTime = time;

    // Update buttonStates
    if (buttons & Button::A) {
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

    if (buttons & Button::B) {
        if (buttonStates.B == 2) {
            buttonStates.B = 1;
        }
        else {
            buttonStates.B = 2;
        }
    }
    else {
        buttonStates.B = 0;
    }

    if (buttons & Button::X) {
        if (buttonStates.X == 2) {
            buttonStates.X = 1;
        }
        else {
            buttonStates.X = 2;
        }
    }
    else {
        buttonStates.X = 0;
    }

    if (buttons & Button::Y) {
        if (buttonStates.Y == 2) {
            buttonStates.Y = 1;
        }
        else {
            buttonStates.Y = 2;
        }
    }
    else {
        buttonStates.Y = 0;
    }

    if (buttons & Button::DPAD_UP) {
        if (buttonStates.UP == 2) {
            buttonStates.UP = 1;
        }
        else {
            buttonStates.UP = 2;
        }
    }
    else {
        buttonStates.UP = 0;
    }

    if (buttons & Button::DPAD_DOWN) {
        if (buttonStates.DOWN == 2) {
            buttonStates.DOWN = 1;
        }
        else {
            buttonStates.DOWN = 2;
        }
    }
    else {
        buttonStates.DOWN = 0;
    }

    if (buttons & Button::DPAD_LEFT) {
        if (buttonStates.LEFT == 2) {
            buttonStates.LEFT = 1;
        }
        else {
            buttonStates.LEFT = 2;
        }
    }
    else {
        buttonStates.LEFT = 0;
    }

    if (buttons & Button::DPAD_RIGHT) {
        if (buttonStates.RIGHT == 2) {
            buttonStates.RIGHT = 1;
        }
        else {
            buttonStates.RIGHT = 2;
        }
    }
    else {
        buttonStates.RIGHT = 0;
    }


    // Update game
    if (gameState == STATE_MENU) {
        update_menu(dt, buttonStates);
    }
    else if (gameState == STATE_LEVEL_SELECT) {

    }
    else if (gameState == STATE_IN_GAME) {
        update_game(dt, buttonStates);
    }
    else if (gameState == STATE_LOST) {

    }
}