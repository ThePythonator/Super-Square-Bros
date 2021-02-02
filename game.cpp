#include "game.hpp"
#include "assets.hpp"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define FRAME_LENGTH 0.15

#define TILE_ID_EMPTY 255
#define TILE_ID_COIN 96
#define TILE_ID_PLAYER_1 64
#define TILE_ID_PLAYER_2 68
#define TILE_ID_HEART 112
#define TILE_ID_CAMERA 253
#define TILE_ID_ENEMY_1 80
#define TILE_ID_ENEMY_2 84
#define TILE_ID_ENEMY_3 88
#define TILE_ID_ENEMY_4 92

#define CAMERA_SCALE 10
#define CAMERA_PAN_TIME 3

#define SPRITE_SIZE 8


#define PLAYER_MAX_HEALTH 3


#define GRAVITY 600.0f
#define GRAVITY_MAX 200.0f

#define PLAYER_MAX_JUMP 190.0f
#define PLAYER_MAX_SPEED 80.0f

using namespace blit;


// NOTE: all positions (x,y) mark TOP LEFT corner of sprites


const uint8_t SCREEN_MID_WIDTH = SCREEN_WIDTH / 2;
const uint8_t SCREEN_MID_HEIGHT = SCREEN_HEIGHT / 2;


const uint8_t SPRITE_HALF = SPRITE_SIZE / 2;

const uint8_t enemyHealths[] = { 2, 1, 2, 2 };

const std::vector<uint8_t> coinFrames = { TILE_ID_COIN, TILE_ID_COIN + 1, TILE_ID_COIN + 2, TILE_ID_COIN + 3, TILE_ID_COIN + 2, TILE_ID_COIN + 1 };

const uint8_t* asset_levels[] = {
    asset_level0
};


float dt;
uint32_t lastTime = 0;

Surface* background_image = Surface::load(asset_background);

bool cameraIntro = false;
uint16_t cameraStartX, cameraStartY;
uint16_t finishX, finishY, playerStartX, playerStartY;




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

    void linear_to(double dt, float startX, float startY, float targetX, float targetY, float time) {
        if (!locked) {
            if (std::abs(targetX - x) < std::abs(((targetX - startX) / time) * dt)) {
                x = targetX;
            }
            else {
                x += ((targetX - startX) / time) * dt;
            }

            if (std::abs(targetY - y) < std::abs(((targetY - startY) / time) * dt)) {
                y = targetY;
            }
            else {
                y += ((targetY - startY) / time) * dt;
            }
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
    bool collected;

    Pickup() : LevelObject() {
        collected = false;
    }

    Pickup(uint16_t xPosition, uint16_t yPosition) : LevelObject(xPosition, yPosition) {
        collected = false;
    }

    virtual void update(double dt, ButtonStates buttonStates) = 0;

    virtual void render(Camera camera) = 0;

protected:

};

class AnimatedPickup : public Pickup {
public:
    AnimatedPickup() : Pickup() {
        animationTimer = 0;
        currentFrame = 0;
    }

    AnimatedPickup(uint16_t xPosition, uint16_t yPosition, std::vector<uint8_t> animationFrames) : Pickup(xPosition, yPosition) {
        animationTimer = 0;
        currentFrame = 0;

        frames = animationFrames;
    }

    void update(double dt, ButtonStates buttonStates) {
        animationTimer += dt;

        if (animationTimer >= FRAME_LENGTH) {
            animationTimer -= FRAME_LENGTH;
            currentFrame++;
            currentFrame %= frames.size();
        }
    }

    void render(Camera camera) {
        if (!collected) {
            screen.sprite(frames[currentFrame], Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
        }
    }

protected:
    double animationTimer;
    std::vector<uint8_t> frames;
    uint8_t currentFrame;
};

class Coin : public AnimatedPickup {
public:
    Coin() : AnimatedPickup() {

    }

    Coin(uint16_t xPosition, uint16_t yPosition, std::vector<uint8_t> animationFrames) : AnimatedPickup(xPosition, yPosition, animationFrames) {

    }

    void update(double dt, ButtonStates buttonStates) {
        AnimatedPickup::update(dt, buttonStates);
    }

    void render(Camera camera) {
        AnimatedPickup::render(camera);
    }

protected:

};
std::vector<Coin> coins;



class Entity {
public:
    float x, y;
    uint8_t health;
    bool locked;

    Entity() {
        x = y = 0;
        xVel = yVel = 0;

        anchorFrame = 0;

        lastDirection = 1; // 1 = right, 0 = left
        //state = IDLE;
        locked = false;

        health = 1;
    }

    Entity(uint16_t xPosition, uint16_t yPosition, uint8_t frame, uint8_t startHealth) {
        x = xPosition;
        y = yPosition;
        xVel = yVel = 0;

        anchorFrame = frame;

        lastDirection = 1;
        //state = IDLE;
        locked = false;

        health = startHealth;
    }

    void update(double dt, ButtonStates buttonStates) {

    }

    void update_collisions() {
        if (!locked) {
            // Update gravity
            yVel += GRAVITY * dt;
            yVel = std::min(yVel, (float)GRAVITY_MAX);

            // Move entity y
            y += yVel * dt;

            // Here check collisions...
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground[i])) {
                    if (yVel > 0) {
                        // Collided from top
                        y = foreground[i].y - SPRITE_SIZE;
                    }
                    else {
                        // Collided from bottom
                        y = foreground[i].y + SPRITE_SIZE;
                    }
                    yVel = 0;
                }
            }

            // Move entity x
            x += xVel * dt;

            // Here check collisions...
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground[i])) {
                    if (xVel > 0) {
                        // Collided from left
                        x = foreground[i].x - SPRITE_SIZE + 1;
                    }
                    else {
                        // Collided from right
                        x = foreground[i].x + SPRITE_SIZE - 1;
                    }
                    xVel = 0;
                }
            }

            if (y > levelData.levelHeight * SPRITE_SIZE) {
                health = 0;
                // cause particle stuff, don't reset position until particles done/ timer done
            }

            if (xVel > 0) {
                lastDirection = 1;
            }
            else if (xVel < 0) {
                lastDirection = 0;
            }
        }
    }
    
    void render(Camera camera) {
        uint8_t frame = anchorFrame;

        if (yVel < -50) {
            frame = anchorFrame + 1;
        }
        else if (yVel > 160) {
            frame = anchorFrame + 2;
        }

        if (lastDirection == 1) {
            screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
        }
        else {
            screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
        }
    }

    bool colliding(Tile tile) {
        // Replace use of this with actual code?
        return (tile.x + SPRITE_SIZE > x + 1 && tile.x < x + SPRITE_SIZE - 1 && tile.y + SPRITE_SIZE > y && tile.y < y + SPRITE_SIZE);
    }

protected:
    float xVel, yVel;
    uint8_t anchorFrame;
    uint8_t lastDirection;
};

class Enemy : public Entity {
public:

    Enemy() : Entity() {
        enemyType = basic;
    }

    Enemy(uint16_t xPosition, uint16_t yPosition, uint8_t startHealth, uint8_t type) : Entity(xPosition, yPosition, TILE_ID_ENEMY_1 + type * 4, startHealth) {
        enemyType = (EnemyType)type;
    }

    void update(double dt, ButtonStates buttonStates) {
        Entity::update_collisions();

        if (enemyType == basic) {
            /*
            bool reverseDirection = false;

            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (y + SPRITE_SIZE == foreground[i].y) {
                    // maybe rework this (checks if is about to not be on a block), since it's v. rushed
                    if (lastDirection == 1) {
                        if (foreground[i].x + SPRITE_SIZE < x + SPRITE_SIZE && foreground[i].x + SPRITE_SIZE * 2 > x + SPRITE_SIZE) {
                            reverseDirection = true;
                        }
                    }
                    else {
                        if (foreground[i].x > x && foreground[i].x - SPRITE_SIZE < x) {
                            reverseDirection = true;
                        }
                    }
                }
            }

            if (reverseDirection) {
                lastDirection = 1 - lastDirection;
            }

            if (lastDirection) {
                xVel = PLAYER_MAX_SPEED;
            }
            else {
                xVel = -PLAYER_MAX_SPEED;
            }*/
        }
    }

    void render(Camera camera) {
        Entity::render(camera);
    }

protected:
    enum EnemyType {
        basic, // type 1
        ranged, // type 2
        persuit, // type 3
        flying // type 4
    } enemyType;

    //enum EntityState {
    //    IDLE,
    //    WALK,
    //    //RUN,
    //    JUMP,
    //    //CROUCH,
    //    //INJURED,
    //    DEAD
    //} state;
};
std::vector<Enemy> enemies;


class Player : public Entity {
public:
    uint8_t score;

    Player() : Entity() {
        score = 0;
    }

    Player(uint16_t xPosition, uint16_t yPosition, uint8_t colour) : Entity(xPosition, yPosition, TILE_ID_PLAYER_1 + colour * 4, PLAYER_MAX_HEALTH) {
        score = 0;
    }

    void update(double dt, ButtonStates buttonStates) {

        xVel = 0; // remove later? - change to fast acceleration?

        if (buttonStates.A) {
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE > x + 1 && foreground[i].x < x + SPRITE_SIZE - 1) {
                    // On top of block
                    // Jump
                    yVel = -PLAYER_MAX_JUMP;
                    //state = JUMP;
                }
            }
        }


        if (buttonStates.LEFT) {
            xVel -= PLAYER_MAX_SPEED;
        }
        if (buttonStates.RIGHT) {
            xVel += PLAYER_MAX_SPEED;
        }


        //for (uint16_t i = 0; i < coins.size(); i++) {
        //    if (!coins[i].collected && coins[i].x + SPRITE_SIZE > x && coins[i].x < x + SPRITE_SIZE && coins[i].y + SPRITE_SIZE > y && coins[i].y < y + SPRITE_SIZE) {
        //        // Hit coin, add 1 to player score
        //        coins[i].collected = true;
        //        score++;

        //    }
        //}

        uint8_t coinCount = coins.size();

        // Remove coins if player jumps on them
        coins.erase(std::remove_if(coins.begin(), coins.end(), [this](Coin coin) { return (coin.x + SPRITE_SIZE > x && coin.x < x + SPRITE_SIZE && coin.y + SPRITE_SIZE > y && coin.y < y + SPRITE_SIZE); }), coins.end());
        
        // Add points to player score (1 point per coin which has been deleted)
        score += coinCount - coins.size();

        // Remove enemies if player jumps on them
        //NOTE: HOW DO I IMPLEMENT HEALTH THEN?
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [this](Enemy enemy) { return (enemy.x + SPRITE_SIZE > x && enemy.x < x + SPRITE_SIZE && enemy.y == y + SPRITE_SIZE); }), enemies.end());

        update_collisions();

        if (health == 0) {
            //state = DEAD;
            // do something?
            health = 3; //remove?
            yVel = 0;
            lastDirection = 1;
            x = playerStartX;
            y = playerStartY;
        }
    }

    void update_collisions() {
        if (!locked) {
            // Update gravity
            yVel += GRAVITY * dt;
            yVel = std::min(yVel, GRAVITY_MAX);

            // Move entity y
            y += yVel * dt;

            // Here check collisions...

            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground[i])) {
                    if (yVel > 0) {
                        // Collided from top
                        y = foreground[i].y - SPRITE_SIZE;
                    }
                    else {
                        // Collided from bottom
                        y = foreground[i].y + SPRITE_SIZE;
                    }
                    yVel = 0;
                }
            }

            for (uint16_t i = 0; i < enemies.size(); i++) {
                if (colliding(enemies[i])) {
                    if (yVel > 0) {
                        // Collided from top
                        y = enemies[i].y - SPRITE_SIZE;
                    }
                    else {
                        // Collided from bottom
                        y = enemies[i].y + SPRITE_SIZE;
                    }
                    yVel = 0;
                }
            }

            // Move entity x
            x += xVel * dt;

            // Here check collisions...
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground[i])) {
                    if (xVel > 0) {
                        // Collided from left
                        x = foreground[i].x - SPRITE_SIZE + 1;
                    }
                    else {
                        // Collided from right
                        x = foreground[i].x + SPRITE_SIZE - 1;
                    }
                    xVel = 0;
                }
            }

            for (uint16_t i = 0; i < enemies.size(); i++) {
                if (colliding(enemies[i])) {
                    if (xVel > 0) {
                        // Collided from left
                        x = enemies[i].x - SPRITE_SIZE;
                    }
                    else {
                        // Collided from right
                        x = enemies[i].x + SPRITE_SIZE;
                    }
                    xVel = 0;
                }
            }



            if (y > levelData.levelHeight * SPRITE_SIZE) {
                health = 0;
                // cause particle stuff, don't reset position until particles done/ timer done
            }

            if (xVel > 0) {
                lastDirection = 1;
            }
            else if (xVel < 0) {
                lastDirection = 0;
            }
        }
    }

    void render(Camera camera) {
        Entity::render(camera);
    }

    bool colliding(Tile tile) {
        // Replace use of this with actual code?
        return (tile.x + SPRITE_SIZE > x + 1 && tile.x < x + SPRITE_SIZE - 1 && tile.y + SPRITE_SIZE > y && tile.y < y + SPRITE_SIZE);
    }

    bool colliding(Enemy enemy) {
        // Replace use of this with actual code?
        return (enemy.x + SPRITE_SIZE > x && enemy.x < x + SPRITE_SIZE && enemy.y + SPRITE_SIZE > y && enemy.y < y + SPRITE_SIZE);
    }

protected:
    //enum EntityState {
    //    IDLE,
    //    WALK,
    //    //RUN,
    //    JUMP,
    //    //CROUCH,
    //    //INJURED,
    //    DEAD
    //} state;
};
Player player;


void render_tiles(std::vector<Tile> tiles) {
    for (int i = 0; i < tiles.size(); i++) {
        tiles[i].render(camera);
    }
}

void render_coins(std::vector<Coin> coins) {
    for (int i = 0; i < coins.size(); i++) {
        coins[i].render(camera);
    }
}

void render_enemies(std::vector<Enemy> enemies) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].render(camera);
    }
}

void render_background() {
    screen.blit(background_image, Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), Point(0, 0), false);
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

void render_hud() {
    // Player health
    for (uint8_t i = 0; i < player.health; i++) {
        screen.sprite(TILE_ID_HEART, Point(2 + i * SPRITE_SIZE, 2));
    }

    // Player score
    blit::screen.text(std::to_string(player.score), minimal_font, Point(SCREEN_WIDTH - 2, 2), true, blit::TextAlign::top_right);
}

void load_level(uint8_t levelNumber) {
    // Variables for finding start and finish positions
    finishX = finishY = playerStartX = playerStartY = 0;

    cameraStartX = cameraStartY = 0;

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
            coins.push_back(Coin((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, coinFrames));
        }
        else {
            foreground.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i]));
        }
    }

    for (int i = 0; i < levelSize; i++) {
        if (tmx->data[i + levelSize] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else if (tmx->data[i + levelSize] == TILE_ID_PLAYER_1) {
            playerStartX = (i % levelWidth) * SPRITE_SIZE;
            playerStartY = (i / levelWidth) * SPRITE_SIZE;
        }
        else if (tmx->data[i + levelSize] == TILE_ID_CAMERA) {
            cameraStartX = (i % levelWidth) * SPRITE_SIZE;
            cameraStartY = (i / levelWidth) * SPRITE_SIZE;
        }
        else if (tmx->data[i + levelSize] == TILE_ID_ENEMY_1) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[0], 0));
        }
        else if (tmx->data[i + levelSize] == TILE_ID_ENEMY_2) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[1], 1));
        }
        else if (tmx->data[i + levelSize] == TILE_ID_ENEMY_3) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[2], 2));
        }
        else if (tmx->data[i + levelSize] == TILE_ID_ENEMY_4) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[3], 3));
        }
        else {
            background.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i + levelSize]));
        }
    }

    // Reset player attributes
    player = Player(playerStartX, playerStartY, 0); // change colour param later
    
    // Reset camera position
    camera.x = cameraStartX;
    camera.y = cameraStartY;
}

void start_level() {
    player.locked = true;
    cameraIntro = true;
}

void render_menu() {
    screen.text("Super Square Bros.", minimal_font, Point(SCREEN_MID_WIDTH, 20), true, TextAlign::center_center);
    screen.text("Press A to Start", minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 10), true, TextAlign::center_center);
}

void render_game() {
    render_background();

    render_level();

    render_entities();

    render_hud();
}

void update_menu(double dt, ButtonStates buttonStates) {
    if (buttonStates.A == 2) {
        gameState = STATE_IN_GAME; // change to LEVEL_SELECT later

        load_level(0); // move to update_level later

        start_level();
    }
}

void update_game(double dt, ButtonStates buttonStates) {
    player.update(dt, buttonStates);

    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].update(dt, buttonStates);
    }


    for (int i = 0; i < coins.size(); i++) {
        coins[i].update(dt, buttonStates);
    }

    if (cameraIntro) {
        camera.linear_to(dt, cameraStartX, cameraStartY, player.x, player.y, CAMERA_PAN_TIME);
        //camera.ease_to(dt/5, player.x, player.y);

        if (player.x == camera.x && player.y == camera.y) {
            cameraIntro = false;
            player.locked = false;
        }
    }
    else {
        camera.ease_to(dt, player.x, player.y);
    }
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

    //printf("%f\n", dt);

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