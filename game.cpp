#include "game.hpp"
#include "assets.hpp"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define LEVEL_COUNT 1

#define FRAME_LENGTH 0.15
#define TRANSITION_FRAME_LENGTH 0.1
#define TRANSITION_CLOSE_LENGTH 0.5

#define TILE_ID_EMPTY 255
#define TILE_ID_COIN 96
#define TILE_ID_PLAYER_1 64
#define TILE_ID_PLAYER_2 68
#define TILE_ID_HEART 112
#define TILE_ID_CAMERA 253
#define TILE_ID_TRANSITION 224
#define TILE_ID_FINISH 128
#define TILE_ID_LEVEL_TRIGGER 116
#define TILE_ID_ENEMY_1 80
#define TILE_ID_ENEMY_2 84
#define TILE_ID_ENEMY_3 88
#define TILE_ID_ENEMY_4 92
#define TILE_ID_ENEMY_PROJECTILE 114

#define CAMERA_SCALE_X 10
#define CAMERA_SCALE_Y 5
#define CAMERA_PAN_TIME 4

#define LEVEL_DEATH_BOUNDARY_SCALE 1.5

#define SPRITE_SIZE 8


#define ENTITY_DEATH_PARTICLE_COUNT 100
#define ENTITY_DEATH_PARTICLE_GRAVITY 40.0f
#define ENTITY_DEATH_PARTICLE_AGE 0.8f
#define ENTITY_DEATH_PARTICLE_SPEED 60.0f


#define GRAVITY 600.0f
#define GRAVITY_MAX 200.0f
#define PROJECTILE_GRAVITY 45.0f
#define PROJECTILE_GRAVITY_MAX 50.0f


#define PLAYER_MAX_HEALTH 3
#define PLAYER_MAX_JUMP 190.0f
#define PLAYER_MAX_SPEED 85.0f
#define PLAYER_IMMUNE_TIME 3.0f
#define PLAYER_ACCELERATION 400.0f

#define ENTITY_IDLE_SPEED 40.0f
#define ENTITY_PERSUIT_SPEED 65.0f
#define ENTITY_JUMP_SPEED 160.0f

#define RANGED_MAX_RANGE 64.0f
#define RANGED_RELOAD_TIME 2.0f
#define RANGED_PROJECTILE_X_VEL_SCALE 0.7f
#define RANGED_PROJECTILE_Y_VEL_SCALE 0.5f

#define PERSUIT_MAX_RANGE 48.0f

#define TEXT_FLASH_TIME 0.8f

#define TEXT_JUMP_VELOCITY 80.0f
#define TEXT_GRAVITY 250.0f

#define NO_LEVEL_SELECTED 255

using namespace blit;


// NOTE: all positions (x,y) mark TOP LEFT corner of sprites


const uint8_t SCREEN_MID_WIDTH = SCREEN_WIDTH / 2;
const uint8_t SCREEN_MID_HEIGHT = SCREEN_HEIGHT / 2;


const uint8_t SPRITE_HALF = SPRITE_SIZE / 2;
const uint8_t SPRITE_QUARTER = SPRITE_SIZE / 4;

const uint16_t SCREEN_TILE_SIZE = (SCREEN_WIDTH / SPRITE_SIZE) * (SCREEN_HEIGHT / SPRITE_SIZE);

const uint8_t enemyHealths[] = { 1, 1, 1, 1 }; // maybe obsolete, since player currently just deletes enemy if jumping on top

const std::vector<uint8_t> coinFrames = { TILE_ID_COIN, TILE_ID_COIN + 1, TILE_ID_COIN + 2, TILE_ID_COIN + 3, TILE_ID_COIN + 2, TILE_ID_COIN + 1 };

const std::vector<uint8_t> finishFrames = { TILE_ID_FINISH, TILE_ID_FINISH + 1, TILE_ID_FINISH + 2, TILE_ID_FINISH + 3, TILE_ID_FINISH + 4, TILE_ID_FINISH + 5 };

//const std::vector<uint8_t> transitionFrames = { TILE_ID_TRANSITION + 3, TILE_ID_TRANSITION + 2, TILE_ID_TRANSITION + 1, TILE_ID_TRANSITION, TILE_ID_TRANSITION + 1, TILE_ID_TRANSITION + 2, TILE_ID_TRANSITION + 3 };
const std::vector<uint8_t> transitionFramesClose = { TILE_ID_TRANSITION, TILE_ID_TRANSITION + 1, TILE_ID_TRANSITION + 2, TILE_ID_TRANSITION + 3, TILE_ID_TRANSITION + 4, TILE_ID_TRANSITION + 6, TILE_ID_TRANSITION + 7 };
const std::vector<uint8_t> transitionFramesOpen = { TILE_ID_TRANSITION + 6, TILE_ID_TRANSITION + 5, TILE_ID_TRANSITION + 4, TILE_ID_TRANSITION + 3, TILE_ID_TRANSITION + 2, TILE_ID_TRANSITION + 1, TILE_ID_TRANSITION};

const float parallaxFactorLayersX[2] = {
    0.4,
    0.2
};

const float parallaxFactorLayersY[2] = {
    0.4,
    0.2
};

const uint8_t* asset_levels[] = {
    asset_level0,
    asset_level_title,
    asset_level_char_select,
    asset_level_level_select
};

uint16_t levelDeathBoundary;

float dt;
uint32_t lastTime = 0;

Surface* background_image = Surface::load(asset_background);

bool cameraIntro = false;
uint16_t cameraStartX, cameraStartY;
uint16_t playerStartX, playerStartY;


float textFlashTimer = 0.0f;
uint8_t playerSelected = 0;

uint8_t currentLevelNumber = NO_LEVEL_SELECTED;

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

enum class GameState {
    STATE_CHARACTER_SELECT,
    STATE_MENU,
    STATE_LEVEL_SELECT,
    STATE_IN_GAME,
    STATE_LOST
};
GameState gameState = GameState::STATE_CHARACTER_SELECT;

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

//struct PositionF {
//    float x, y;
//};
//
//struct PositionI {
//    int x, y;
//};

class Colour {
public:
    uint8_t r, g, b, a;

    Colour() {
        r = g = b = a = 255;
    }

    Colour(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
        a = 255;
    }

    Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};

// Particle colours
const std::vector<Colour> playerDeathParticleColours[2] = {
    { Colour(255, 255, 242), Colour(255, 204, 181), Colour(178, 53, 53) },
    { Colour(255, 255, 242), Colour(178, 214, 96), Colour(37, 124, 73) }
};
const std::vector<Colour> enemyDeathParticleColours[4] = {
    { Colour(255, 255, 242), Colour(184, 197, 216), Colour(25, 40, 102) },
    { Colour(255, 255, 242), Colour(255, 204, 181), Colour(165, 82, 139) },
    { Colour(255, 255, 242), Colour(255, 204, 181), Colour(229, 114, 57) },
    { Colour(255, 255, 242), Colour(184, 197, 216), Colour(62, 106, 178) }
};
const std::vector<Colour> levelTriggerParticleColours = { Colour(255, 255, 242), Colour(145, 224, 204), Colour(53, 130, 130) };

class Camera {
public:
    float x, y;

    bool locked;

    Camera() {
        x = y = 0;
        locked = false;
    }

    void ease_out_to(float dt, float targetX, float targetY) {
        if (!locked) {
            x += (targetX - x) * CAMERA_SCALE_X * dt;
            y += (targetY - y) * CAMERA_SCALE_Y * dt;
        }
    }

    void linear_to(float dt, float startX, float startY, float targetX, float targetY, float time) {
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


class Particle {
public:
    float x, y;
    float xVel, yVel;
    float gravity;
    Colour colour;
    float age;

    Particle() {
        x = y = 0;
        xVel = yVel = 0;

        gravity = 0;
        colour = Colour(0, 0, 0);

        age = 0;
    }

    Particle(float xPosition, float yPosition, float xVelocity, float yVelocity, float particleGravity, Colour particleColour) {
        x = xPosition;
        y = yPosition;
        xVel = xVelocity;
        yVel = yVelocity;

        gravity = particleGravity;
        colour = particleColour;

        age = 0;
    }

    void render(Camera camera) {
        screen.pen = Pen(colour.r, colour.g, colour.b, colour.a);
        screen.pixel(Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
    }

    void update(float dt) {
        age += dt;
        colour.a = std::max(0.0f, colour.a - age * 10);

        y += gravity * dt;

        x += xVel * dt;
        y += yVel * dt;
    }
};



std::vector<Particle> generate_particles(float x, float y, float gravity, std::vector<Colour> colours, float speed, uint8_t count) {
    std::vector<Particle> particles;

    for (uint8_t i = 0; i < count; i++) {
        float angle = rand() % 360;

        float xVel = ((rand() % 100) / 100.0f) * std::cos(angle) * speed;
        float yVel = ((rand() % 100) / 100.0f) * std::sin(angle) * speed;

        particles.push_back(Particle(x, y, xVel, yVel, gravity, colours[rand() % colours.size()]));
    }

    return particles;
}



class Projectile {
public:
    float x, y;
    float xVel, yVel;
    uint8_t id;

    Projectile() {
        x = y = 0;
        xVel = yVel = 0;

        id = 0;
    }

    Projectile(float xPosition, float yPosition, float xVelocity, float yVelocity, uint8_t tileId) {
        x = xPosition;
        y = yPosition;
        xVel = xVelocity;
        yVel = yVelocity;
        id = tileId;
    }

    void update(float dt, ButtonStates buttonStates) {
        // Update gravity
        yVel += PROJECTILE_GRAVITY * dt;
        yVel = std::min(yVel, (float)PROJECTILE_GRAVITY_MAX);

        // Move entity y
        y += yVel * dt;

        // Move entity x
        x += xVel * dt;
    }

    void render(Camera camera) {
        screen.sprite(id, Point(SCREEN_MID_WIDTH + x - camera.x - SPRITE_QUARTER, SCREEN_MID_HEIGHT + y - camera.y - SPRITE_QUARTER));
        //screen.rectangle(Rect(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y, 4, 4));
    }
};
std::vector<Projectile> projectiles;



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

    virtual void update(float dt, ButtonStates buttonStates) = 0;

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

    void update(float dt, ButtonStates buttonStates) {

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
//std::vector<Tile> platforms;

class ParallaxTile : public Tile {
public:
    ParallaxTile() : Tile() {
        id = TILE_ID_EMPTY;
        layer = 0;
    }

    ParallaxTile(uint16_t xPosition, uint16_t yPosition, uint8_t tileID, uint8_t parallaxLayer) : Tile(xPosition, yPosition, tileID) {
        id = tileID;
        layer = parallaxLayer;
    }

    void update(float dt, ButtonStates buttonStates) {
        
    }

    void render(Camera camera)
    {
        //screen.sprite(id, Point(SCREEN_MID_WIDTH + x - (camera.x * parallaxFactorLayersX[layer]), SCREEN_MID_HEIGHT + y - (camera.y * parallaxFactorLayersY[layer])));
        // Not shifting sprite to center seems to give better coverage of parallax
        screen.sprite(id, Point(x - (camera.x * parallaxFactorLayersX[layer]), y - (camera.y * parallaxFactorLayersY[layer])));
    }

protected:
    uint8_t id;
    uint8_t layer;
};
std::vector<ParallaxTile> parallax;


class Pickup : public LevelObject {
public:
    bool collected;

    Pickup() : LevelObject() {
        collected = false;
    }

    Pickup(uint16_t xPosition, uint16_t yPosition) : LevelObject(xPosition, yPosition) {
        collected = false;
    }

    virtual void update(float dt, ButtonStates buttonStates) = 0;

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

    void update(float dt, ButtonStates buttonStates) {
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
    float animationTimer;
    std::vector<uint8_t> frames;
    uint8_t currentFrame;
};

class Coin : public AnimatedPickup {
public:
    Coin() : AnimatedPickup() {

    }

    Coin(uint16_t xPosition, uint16_t yPosition, std::vector<uint8_t> animationFrames) : AnimatedPickup(xPosition, yPosition, animationFrames) {

    }

    void update(float dt, ButtonStates buttonStates) {
        AnimatedPickup::update(dt, buttonStates);
    }

    void render(Camera camera) {
        AnimatedPickup::render(camera);
    }

protected:

};
std::vector<Coin> coins;

class Finish : public AnimatedPickup {
public:
    Finish() : AnimatedPickup() {

    }

    Finish(uint16_t xPosition, uint16_t yPosition, std::vector<uint8_t> animationFrames) : AnimatedPickup(xPosition, yPosition, animationFrames) {

    }

    void update(float dt, ButtonStates buttonStates) {
        AnimatedPickup::update(dt, buttonStates);
    }

    void render(Camera camera) {
        AnimatedPickup::render(camera);
    }
};
Finish finish;

class AnimatedTransition {
public:
    uint8_t x, y;
    float closedTimer;

    AnimatedTransition() {
        animationTimer = 0;
        currentFrame = 0;

        state = TransitionState::OPEN;
        x = y = 0;
        closedTimer = 0;
    }

    AnimatedTransition(uint16_t xPosition, uint16_t yPosition, std::vector<uint8_t> open, std::vector<uint8_t> close) {
        animationTimer = 0;
        currentFrame = 0;

        openingFrames = open;
        closingFrames = close;
        state = TransitionState::OPEN;
        x = xPosition;
        y = yPosition;

        closedTimer = 0;
    }

    void update(float dt, ButtonStates buttonStates) {
        if (state == TransitionState::CLOSING || state == TransitionState::OPENING) {
            animationTimer += dt;

            if (animationTimer >= TRANSITION_FRAME_LENGTH) {
                animationTimer -= TRANSITION_FRAME_LENGTH;
                currentFrame++;

                if (state == TransitionState::CLOSING) {
                    if (currentFrame == closingFrames.size()) {
                        state = TransitionState::CLOSED;
                        closedTimer = 0;
                    }
                }
                else {
                    if (currentFrame == openingFrames.size()) {
                        state = TransitionState::OPEN;
                    }
                }
            }
        }
        else if (state == TransitionState::CLOSED) {
            closedTimer += dt;
            if (closedTimer >= TRANSITION_CLOSE_LENGTH) {
                state = TransitionState::READY_TO_OPEN;
            }
        }
    }

    void render(Camera camera) {
        if (state == TransitionState::CLOSING) {
            screen.sprite(closingFrames[currentFrame], Point(x, y));
        }
        else if (state == TransitionState::OPENING) {
            screen.sprite(openingFrames[currentFrame], Point(x, y), SpriteTransform::HORIZONTAL);
        }
        else if (state == TransitionState::CLOSED || state == TransitionState::READY_TO_OPEN) {
            screen.sprite(closingFrames[closingFrames.size() - 1], Point(x, y));
        }
        else if (state == TransitionState::OPEN) {
            // Don't do anything
        }
    }

    void close() {
        state = TransitionState::CLOSING;
        animationTimer = 0;
        currentFrame = 0;
    }

    void open() {
        state = TransitionState::OPENING;
        animationTimer = 0;
        currentFrame = 0;
    }

    bool is_closed() {
        return state == TransitionState::CLOSED;
    }

    bool is_ready_to_open() {
        return state == TransitionState::READY_TO_OPEN;
    }

    bool is_open() {
        return state == TransitionState::OPEN;
    }

protected:
    enum class TransitionState {
        OPENING,
        OPEN,
        CLOSING,
        CLOSED,
        READY_TO_OPEN
    } state;

    float animationTimer;
    std::vector<uint8_t> openingFrames, closingFrames;
    uint8_t currentFrame;
};
AnimatedTransition transition[SCREEN_TILE_SIZE];






class LevelTrigger {
public:
    uint16_t x, y;
    std::vector<Particle> particles;
    uint8_t levelNumber;
    bool visible;
    bool generateParticles;

    LevelTrigger(){
        x = y = 0;
        levelNumber = 0;

        visible = true;
        generateParticles = false;

        textY = textVelY = 0;
    }

    LevelTrigger(uint16_t xPosition, uint16_t yPosition, uint8_t levelTriggerNumber) {
        x = xPosition;
        y = yPosition;

        levelNumber = levelTriggerNumber;

        visible = true;
        generateParticles = false;

        textY = textVelY = 0;
    }

    void update(float dt, ButtonStates buttonStates) {
        textVelY += TEXT_GRAVITY * dt;
        if (textY > SPRITE_SIZE) {
            textY = SPRITE_SIZE;
            textVelY = -TEXT_JUMP_VELOCITY;
        }

        textY += textVelY * dt;

        if (!visible) {

            if (generateParticles) {
                if (particles.size() == 0) {
                    generateParticles = false;
                }
                else {
                    for (uint8_t i = 0; i < particles.size(); i++) {
                        particles[i].update(dt);
                    }

                    // Remove any particles which are too old
                    particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle particle) { return (particle.age >= ENTITY_DEATH_PARTICLE_AGE); }), particles.end());
                }
            }
            else {
                // Generate particles
                // TODO: change constants?
                particles = generate_particles(x, y, ENTITY_DEATH_PARTICLE_GRAVITY, levelTriggerParticleColours, ENTITY_DEATH_PARTICLE_SPEED, ENTITY_DEATH_PARTICLE_COUNT);
                generateParticles = true;
            }
        }
    }

    void render(Camera camera) {
        if (visible) {
            screen.pen = Pen(levelTriggerParticleColours[1].r, levelTriggerParticleColours[1].g, levelTriggerParticleColours[1].b, levelTriggerParticleColours[1].a);
            screen.text(std::to_string(levelNumber + 1), minimal_font, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_HALF, SCREEN_MID_HEIGHT + y - camera.y - SPRITE_HALF * 3 + textY), true, TextAlign::center_center);
            screen.sprite(TILE_ID_LEVEL_TRIGGER, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
        }

        // Particles
        for (uint8_t i = 0; i < particles.size(); i++) {
            particles[i].render(camera);
        }
    }

    void set_active() {
        visible = false;
    }

protected:
    float textY;
    float textVelY;
};
std::vector<LevelTrigger> levelTriggers;





class Entity {
public:
    float x, y;
    uint8_t health;
    bool locked;
    std::vector<Particle> particles;
    uint8_t lastDirection;

    Entity() {
        x = y = 0;
        xVel = yVel = 0;

        anchorFrame = 0;

        lastDirection = 1; // 1 = right, 0 = left
        //state = IDLE;
        locked = false;
        deathParticles = false;

        health = 1;

        immuneTimer = 0;
    }

    Entity(uint16_t xPosition, uint16_t yPosition, uint8_t frame, uint8_t startHealth) {
        x = xPosition;
        y = yPosition;
        xVel = yVel = 0;

        anchorFrame = frame;

        lastDirection = 1;
        //state = IDLE;
        locked = false;
        deathParticles = false;

        health = startHealth;

        immuneTimer = 0;
    }

    void update(float dt, ButtonStates buttonStates) {

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

            if (xVel > 0) {
                lastDirection = 1;
            }
            else if (xVel < 0) {
                lastDirection = 0;
            }
        }
    }
    
    void render(Camera camera) {
        if (health != 0) {
            bool visible = false;

            if (immuneTimer) {
                uint16_t immuneTimer_ms = (uint16_t)(immuneTimer * 1000);
                if (immuneTimer_ms % 150 < 75) {
                    visible = true;
                }
            }
            else {
                visible = true;
            }

            if (visible) {
                uint8_t frame = anchorFrame;

                if (yVel < -50) {
                    frame = anchorFrame + 1;
                }
                else if (yVel > 160) {
                    frame = anchorFrame + 2;
                }

                /*if (immuneTimer) {
                    frame = anchorFrame + 3;
                }*/

                if (lastDirection == 1) {
                    screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                }
                else {
                    screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
                }
            }
        }

        // Particles
        for (uint8_t i = 0; i < particles.size(); i++) {
            particles[i].render(camera);
        }
    }

    bool colliding(Tile tile) {
        // Replace use of this with actual code?
        return (tile.x + SPRITE_SIZE > x + 1 && tile.x < x + SPRITE_SIZE - 1 && tile.y + SPRITE_SIZE > y && tile.y < y + SPRITE_SIZE);
    }

    void set_immune() {
        immuneTimer = PLAYER_IMMUNE_TIME;
    }

    bool is_immune() {
        return immuneTimer;
    }

protected:
    float xVel, yVel;
    uint8_t anchorFrame;
    bool deathParticles;
    float immuneTimer;
};



class Enemy : public Entity {
public:
    float* playerX;
    float* playerY;

    Enemy() : Entity() {
        enemyType = EnemyType::BASIC;
        reloadTimer = 0;

        playerX = nullptr;
        playerY = nullptr;
    }

    Enemy(uint16_t xPosition, uint16_t yPosition, uint8_t startHealth, uint8_t type) : Entity(xPosition, yPosition, TILE_ID_ENEMY_1 + type * 4, startHealth) {
        enemyType = (EnemyType)type;
        reloadTimer = 0;

        playerX = nullptr;
        playerY = nullptr;
    }

    void update(float dt, ButtonStates buttonStates) {

        if (health > 0) {
            if (reloadTimer) {
                reloadTimer -= dt;
                if (reloadTimer < 0) {
                    reloadTimer = 0;
                }
            }

            if (enemyType == EnemyType::BASIC) {
                // Consider adding acceleration?
                if (lastDirection) {
                    xVel = ENTITY_IDLE_SPEED;
                }
                else {
                    xVel = -ENTITY_IDLE_SPEED;
                }

                Entity::update_collisions();


                bool reverseDirection = true;

                float tempX = lastDirection ? x + SPRITE_HALF : x - SPRITE_HALF;
                for (uint16_t i = 0; i < foreground.size(); i++) {
                    if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE - 1) {
                        // About to be on block
                        reverseDirection = false;
                    }
                    if (foreground[i].y + SPRITE_SIZE > y && foreground[i].y < y + SPRITE_SIZE && (lastDirection ? x + SPRITE_SIZE - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                        // Walked into side of block
                        reverseDirection = true;
                        // Break because we definitely need to change direction, and don't want any other blocks resetting this to false
                        break;
                    }
                }

                if (reverseDirection) {
                    lastDirection = 1 - lastDirection;
                }
            }
            else if (enemyType == EnemyType::RANGED) {
                Entity::update_collisions();

                lastDirection = *playerX < x ? 0 : 1;

                if (std::abs(x - *playerX) < RANGED_MAX_RANGE && std::abs(y - *playerY) < RANGED_MAX_RANGE && !reloadTimer) {
                    // fire!
                    // Maybe make these values constants?
                    projectiles.push_back(Projectile(x, y, RANGED_PROJECTILE_X_VEL_SCALE * (*playerX - x), -std::abs(x - *playerX) * RANGED_PROJECTILE_Y_VEL_SCALE + (*playerY - y) * RANGED_PROJECTILE_Y_VEL_SCALE, TILE_ID_ENEMY_PROJECTILE));
                    reloadTimer = RANGED_RELOAD_TIME;
                }
            }
            else if (enemyType == EnemyType::PERSUIT) {
                // Consider adding acceleration?
                if (lastDirection) {
                    xVel = ENTITY_IDLE_SPEED;
                }
                else {
                    xVel = -ENTITY_IDLE_SPEED;
                }
                // TODO use faster speed if persuing
                Entity::update_collisions();


                if (std::abs(x - *playerX) < PERSUIT_MAX_RANGE && std::abs(y - *playerY) < PERSUIT_MAX_RANGE) {
                    // Persue!

                    lastDirection = *playerX < x ? 0 : 1;

                    bool jump = true;

                    float tempX = lastDirection ? x + SPRITE_HALF : x - SPRITE_HALF;
                    for (uint16_t i = 0; i < foreground.size(); i++) {
                        if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE - 1) {
                            // About to be on block
                            jump = false;
                        }
                        //if ((lastDirection ? x + SPRITE_SIZE - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                        //    // Walked into side of block
                        //    jump = true;
                        //    // Break because we definitely need to jump
                        //    break;
                        //}
                    }

                    if (jump) {
                        for (uint16_t i = 0; i < foreground.size(); i++) {
                            if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE > x && foreground[i].x < x + SPRITE_SIZE) {
                                // On top of block
                                // Jump
                                yVel = -ENTITY_JUMP_SPEED;
                                //state = JUMP;
                            }
                        }
                    }
                }
                else {
                    // Just patrol... (Same as basic enemy)

                    bool reverseDirection = true;

                    float tempX = lastDirection ? x + SPRITE_HALF : x - SPRITE_HALF;
                    for (uint16_t i = 0; i < foreground.size(); i++) {
                        if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE - 1) {
                            // About to be on block
                            reverseDirection = false;
                        }
                        if (foreground[i].y + SPRITE_SIZE > y && foreground[i].y < y + SPRITE_SIZE && (lastDirection ? x + SPRITE_SIZE - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                            // Walked into side of block
                            reverseDirection = true;
                            // Break because we definitely need to change direction, and don't want any other blocks resetting this to false
                            break;
                        }
                    }

                    if (reverseDirection) {
                        lastDirection = 1 - lastDirection;
                    }
                }
            }
            else if (enemyType == EnemyType::FLYING) {
                Entity::update_collisions();
            }


            if (y > levelDeathBoundary) {
                health = 0;
                xVel = yVel = 0;
            }
        }

        if (health == 0) {
            //state = DEAD;

            if (deathParticles) {
                if (particles.size() == 0) {
                    // No particles left

                    health = 3; //remove?

                    // Reset player position
                    yVel = 0;
                    lastDirection = 1;
                    x = playerStartX;
                    y = playerStartY;

                    deathParticles = false;
                }
                else {
                    for (uint8_t i = 0; i < particles.size(); i++) {
                        particles[i].update(dt);
                    }

                    // Remove any particles which are too old
                    particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle particle) { return (particle.age >= ENTITY_DEATH_PARTICLE_AGE); }), particles.end());
                }
            }
            else {
                // Generate particles
                particles = generate_particles(x, y, ENTITY_DEATH_PARTICLE_GRAVITY, enemyDeathParticleColours[(uint8_t)enemyType], ENTITY_DEATH_PARTICLE_SPEED, ENTITY_DEATH_PARTICLE_COUNT);
                deathParticles = true;
            }
        }
    }

    void render(Camera camera) {
        Entity::render(camera);
    }

    void set_player_position(float* x, float* y) {
        playerX = x;
        playerY = y;
    }

protected:
    enum class EnemyType {
        BASIC, // type 1
        RANGED, // type 2
        PERSUIT, // type 3
        FLYING // type 4
    } enemyType;

    float reloadTimer;

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
    uint8_t id;

    Player() : Entity() {
        score = 0;
        id = 0;
    }

    Player(uint16_t xPosition, uint16_t yPosition, uint8_t colour) : Entity(xPosition, yPosition, TILE_ID_PLAYER_1 + colour * 4, PLAYER_MAX_HEALTH) {
        score = 0;
        id = colour;
    }

    void update(float dt, ButtonStates buttonStates) {
        if (immuneTimer) {
            immuneTimer -= dt;
            if (immuneTimer < 0) {
                immuneTimer = 0;
            }
        }

        if (health > 0) {

            //xVel = 0; // remove later? - change to fast acceleration?

            if (buttonStates.A) {
                for (uint16_t i = 0; i < foreground.size(); i++) {
                    if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE > x && foreground[i].x < x + SPRITE_SIZE) {
                        // On top of block
                        // Jump
                        yVel = -PLAYER_MAX_JUMP;
                        //state = JUMP;
                    }
                }
            }


            if (buttonStates.LEFT) {
                xVel -= PLAYER_ACCELERATION * dt;
                if (xVel < -PLAYER_MAX_SPEED) {
                    xVel = -PLAYER_MAX_SPEED;
                }
            }
            else if (buttonStates.RIGHT) {
                xVel += PLAYER_ACCELERATION * dt;
                if (xVel > PLAYER_MAX_SPEED) {
                    xVel = PLAYER_MAX_SPEED;
                }
            }
            else {
                if (xVel > 0) {
                    xVel -= PLAYER_ACCELERATION * dt;
                    if (xVel < 0) {
                        xVel = 0;
                    }
                }
                else if (xVel < 0) {
                    xVel += PLAYER_ACCELERATION * dt;
                    if (xVel > 0) {
                        xVel = 0;
                    }
                }
            }

            uint8_t coinCount = coins.size();

            // Remove coins if player jumps on them
            coins.erase(std::remove_if(coins.begin(), coins.end(), [this](Coin coin) { return (coin.x + SPRITE_SIZE > x && coin.x < x + SPRITE_SIZE && coin.y + SPRITE_SIZE > y && coin.y < y + SPRITE_SIZE); }), coins.end());

            // Add points to player score (1 point per coin which has been deleted)
            score += coinCount - coins.size();

            // Remove enemies if player jumps on them
            //NOTE: HOW DO I IMPLEMENT HEALTH THEN?
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy enemy) { return (enemy.health == 0 && enemy.particles.size() == 0); }), enemies.end());

            //enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [this](Enemy enemy) { return (enemy.x + SPRITE_SIZE > x && enemy.x < x + SPRITE_SIZE && enemy.y == y + SPRITE_SIZE); }), enemies.end());

            for (uint8_t i = 0; i < enemies.size(); i++) {
                if (enemies[i].x + SPRITE_SIZE > x && enemies[i].x < x + SPRITE_SIZE && enemies[i].y == y + SPRITE_SIZE && enemies[0].health) {
                    enemies[i].health = 0; // or --?
                }
            }

            update_collisions();


            if (y > levelDeathBoundary) {
                health = 0;
                xVel = yVel = 0;
            }
        }


        if (health == 0) {
            //state = DEAD;

            if (deathParticles) {
                if (particles.size() == 0) {
                    // No particles left, reset values which need to be

                    deathParticles = false;

                    // Reset player position and health, maybe remove all this?
                    health = 3;
                    yVel = -PLAYER_MAX_JUMP;
                    lastDirection = 1;
                    x = playerStartX;
                    y = playerStartY;

                    // Make player immune when respawning?
                    //set_immune();
                }
                else {
                    for (uint8_t i = 0; i < particles.size(); i++) {
                        particles[i].update(dt);
                    }

                    // Remove any particles which are too old
                    particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle particle) { return (particle.age >= ENTITY_DEATH_PARTICLE_AGE); }), particles.end());
                }
            }
            else {
                // Generate particles
                particles = generate_particles(x, y, ENTITY_DEATH_PARTICLE_GRAVITY, playerDeathParticleColours[id], ENTITY_DEATH_PARTICLE_SPEED, ENTITY_DEATH_PARTICLE_COUNT);
                deathParticles = true;
            }
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

            for (uint16_t i = 0; i < levelTriggers.size(); i++) {
                if (levelTriggers[i].visible && colliding(levelTriggers[i])) {
                    if (yVel > 0) {
                        // Collided from top
                        y = levelTriggers[i].y - SPRITE_SIZE;
                        yVel = 0;
                        levelTriggers[i].set_active();
                    }
                }
            }

            for (uint16_t i = 0; i < enemies.size(); i++) {
                if (enemies[i].health && colliding(enemies[i])) {
                    if (yVel > 0) {
                        // Collided from top
                        y = enemies[i].y - SPRITE_SIZE;
                        yVel = 0;
                    }
                    //else {
                    //    // Collided from bottom
                    //    y = enemies[i].y + SPRITE_SIZE;
                    //}
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

            for (uint16_t i = 0; i < levelTriggers.size(); i++) {
                if (levelTriggers[i].visible && colliding(levelTriggers[i])) {
                    if (xVel > 0) {
                        // Collided from left
                        x = levelTriggers[i].x - SPRITE_SIZE;
                    }
                    else {
                        // Collided from right
                        x = levelTriggers[i].x + SPRITE_SIZE;
                    }
                    xVel = 0;
                }
            }

            //for (uint16_t i = 0; i < enemies.size(); i++) {
            //    if (colliding(enemies[i]) && enemies[i].health) {
            //        if (xVel > 0) {
            //            // Collided from left
            //            x = enemies[i].x - SPRITE_SIZE;
            //        }
            //        else {
            //            // Collided from right
            //            x = enemies[i].x + SPRITE_SIZE;
            //        }
            //        xVel = 0;
            //    }
            //}

            if (!immuneTimer) {
                for (uint16_t i = 0; i < enemies.size(); i++) {
                    if (colliding(enemies[i]) && enemies[i].health) {
                        health--;
                        set_immune();
                    }
                }
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

    bool colliding(LevelTrigger levelTrigger) {
        // Replace use of this with actual code?
        return (levelTrigger.x + SPRITE_SIZE > x && levelTrigger.x < x + SPRITE_SIZE && levelTrigger.y + SPRITE_SIZE > y && levelTrigger.y < y + SPRITE_SIZE);
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




void open_transition() {
    for (uint16_t i = 0; i < SCREEN_TILE_SIZE; i++) {
        transition[i].open();
    }
}

void close_transition() {
    for (uint16_t i = 0; i < SCREEN_TILE_SIZE; i++) {
        transition[i].close();
    }
}

void render_transition() {
    for (uint16_t i = 0; i < SCREEN_TILE_SIZE; i++) {
        transition[i].render(camera);
    }
}

void update_transition(float dt, ButtonStates buttonStates) {
    for (uint16_t i = 0; i < SCREEN_TILE_SIZE; i++) {
        transition[i].update(dt, buttonStates);
    }
}


void render_tiles(std::vector<Tile> tiles) {
    for (uint32_t i = 0; i < tiles.size(); i++) {
        tiles[i].render(camera);
    }
}

void render_parallax(std::vector<ParallaxTile> parallax) {
    for (uint32_t i = 0; i < parallax.size(); i++) {
        parallax[i].render(camera);
    }
}

void render_coins() {
    for (uint8_t i = 0; i < coins.size(); i++) {
        coins[i].render(camera);
    }
}

void render_enemies() {
    for (uint8_t i = 0; i < enemies.size(); i++) {
        enemies[i].render(camera);
    }
}

void render_level_triggers() {
    for (uint8_t i = 0; i < levelTriggers.size(); i++) {
        levelTriggers[i].render(camera);
    }
}

void render_projectiles() {
    for (uint8_t i = 0; i < projectiles.size(); i++) {
        projectiles[i].render(camera);
    }
}

void render_finish() {
    finish.render(camera);
}

void render_background() {
    screen.blit(background_image, Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), Point(0, 0), false);
}

void render_level() {
    render_parallax(parallax);

    render_tiles(background);
    render_tiles(foreground);

    render_coins();
}

void render_entities() {
    render_enemies();

    player.render(camera);

    render_projectiles();
}

void render_hud() {
    screen.pen = Pen(255, 255, 255);

    // Player health
    for (uint8_t i = 0; i < player.health; i++) {
        screen.sprite(TILE_ID_HEART, Point(2 + i * SPRITE_SIZE, 2));
    }

    // Player score
    blit::screen.text(std::to_string(player.score), minimal_font, Point(SCREEN_WIDTH - 2, 2), true, blit::TextAlign::top_right);
}

void load_level(uint8_t levelNumber) {
    uint8_t levelTriggerCount = 0;

    // Variables for finding start and finish positions
    uint16_t finishX, finishY;

    playerStartX = playerStartY = 0;
    cameraStartX = cameraStartY = 0;
    finishX = finishY = 0;


    // Get a pointer to the map header
    TMX* tmx = (TMX*)asset_levels[levelNumber];

    uint16_t levelWidth = tmx->width;
    uint16_t levelHeight = tmx->height;
    uint32_t levelSize = levelWidth * levelHeight;

    levelData.levelWidth = levelWidth;
    levelData.levelHeight = levelHeight;

    levelDeathBoundary = levelData.levelHeight * SPRITE_SIZE * LEVEL_DEATH_BOUNDARY_SCALE;

    foreground.clear();
    background.clear();
    parallax.clear();
    coins.clear();
    enemies.clear();
    levelTriggers.clear();

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
        else if (tmx->data[i + levelSize] == TILE_ID_FINISH) {
            finishX = (i % levelWidth) * SPRITE_SIZE;
            finishY = (i / levelWidth) * SPRITE_SIZE;
        }
        else if (tmx->data[i + levelSize] == TILE_ID_LEVEL_TRIGGER) {
            levelTriggers.push_back(LevelTrigger((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, levelTriggerCount++));
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


    // maybe adjust position of tile so that don't need to bunch all up in corner while designing level

    // go backwards through parallax layers so that rendering is correct

    for (int i = 0; i < levelSize; i++) {
        if (tmx->data[i + levelSize * 3] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else {
            parallax.push_back(ParallaxTile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i + levelSize * 3], 1));
        }
    }

    for (int i = 0; i < levelSize; i++) {
        if (tmx->data[i + levelSize * 2] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else {
            parallax.push_back(ParallaxTile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[i + levelSize * 2], 0));
        }
    }


    // Reset player attributes
    player = Player(playerStartX, playerStartY, playerSelected);

    finish = Finish(finishX, finishY, finishFrames);
    
    // Reset camera position
    camera.x = cameraStartX;
    camera.y = cameraStartY;

    for (uint8_t i = 0; i < enemies.size(); i++) {
        enemies[i].set_player_position(&player.x, &player.y);
    }
}

void start_level() {
    player.locked = true;
    cameraIntro = true;

    open_transition();
}

void render_character_select() {
    render_background();

    render_level();

    render_entities();

    if (playerSelected) {
        screen.sprite(TILE_ID_PLAYER_1, Point(SCREEN_MID_WIDTH + playerStartX - camera.x, SCREEN_MID_HEIGHT + playerStartY - camera.y), SpriteTransform::HORIZONTAL);
    }
    else {
        screen.sprite(TILE_ID_PLAYER_2, Point(SCREEN_MID_WIDTH + playerStartX - camera.x + SPRITE_SIZE * 7, SCREEN_MID_HEIGHT + playerStartY - camera.y));
    }


    screen.pen = Pen(255, 255, 255);
    screen.text("Select Player", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
        screen.text("Press A to Start", minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 10), true, TextAlign::center_center);
    }
}

void render_menu() {
    render_background();

    render_level();

    render_entities();


    screen.pen = Pen(255, 255, 255);
    screen.text("Super Square Bros.", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
        screen.text("Press A to Start", minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 10), true, TextAlign::center_center);
    }
}

void render_level_select() {
    render_background();

    render_level();

    render_entities();

    render_level_triggers();


    screen.pen = Pen(255, 255, 255);
    screen.text("Select level", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);
}

void render_game() {
    render_background();

    render_level();

    render_finish();

    render_entities();

    render_hud();
}



void update_enemies(float dt, ButtonStates buttonStates) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].update(dt, buttonStates);
    }
}

void update_level_triggers(float dt, ButtonStates buttonStates) {
    for (int i = 0; i < levelTriggers.size(); i++) {
        levelTriggers[i].update(dt, buttonStates);
        if (!levelTriggers[i].visible && levelTriggers[i].particles.size() == 0) {
            currentLevelNumber = levelTriggers[i].levelNumber;
        }
    }

    levelTriggers.erase(std::remove_if(levelTriggers.begin(), levelTriggers.end(), [](LevelTrigger levelTrigger) { return (!levelTrigger.visible && levelTrigger.particles.size() == 0); }), levelTriggers.end());
}

void update_coins(float dt, ButtonStates buttonStates) {
    for (int i = 0; i < coins.size(); i++) {
        coins[i].update(dt, buttonStates);
    }
}


void update_projectiles(float dt, ButtonStates buttonStates) {
    for (uint8_t i = 0; i < projectiles.size(); i++) {
        projectiles[i].update(dt, buttonStates);
    }

    //  Allow enemies to get hit?
    /*for (uint8_t i = 0; i < enemies.size(); i++) {

    }*/

    if (!player.is_immune()) {
        uint8_t projectileCount = projectiles.size();
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile projectile) { return (projectile.x + SPRITE_HALF > player.x && projectile.x < player.x + SPRITE_SIZE && projectile.y + SPRITE_HALF > player.y && projectile.y < player.y + SPRITE_SIZE); }), projectiles.end());
        if (projectileCount - projectiles.size() > 0) {
            player.health -= 1;
            player.set_immune();
        }
    }
    

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile projectile) { return (std::abs(projectile.x - player.x) > SCREEN_TILE_SIZE || std::abs(projectile.y - player.y) > SCREEN_HEIGHT); }), projectiles.end());

}


void update_character_select(float dt, ButtonStates buttonStates) {
    // Dummy states is used to make selected player continually jump (sending A key pressed).
    ButtonStates dummyStates = { 0 };
    dummyStates.A = 1;
    player.update(dt, dummyStates);

    if (buttonStates.A == 2 && transition[0].is_open()) {
        close_transition();
    }

    if (buttonStates.RIGHT && !playerSelected) {
        playerSelected = 1;
        player = Player(player.x + SPRITE_SIZE * 7, playerStartY, 1);
        player.lastDirection = 0;
    }
    else if (buttonStates.LEFT && playerSelected) {
        playerSelected = 0;
        player = Player(playerStartX, playerStartY, 0);
    }

    if (transition[0].is_ready_to_open()) {
        gameState = GameState::STATE_MENU;

        load_level(LEVEL_COUNT);

        open_transition();
    }
}

void update_menu(float dt, ButtonStates buttonStates) {
    //player.update(dt, buttonStates);

    update_coins(dt, buttonStates);

    //finish.update(dt, buttonStates);

    // Button handling

    if (buttonStates.A == 2 && transition[0].is_open()) {
        close_transition();
    }

    if (transition[0].is_ready_to_open()) {
        gameState = GameState::STATE_LEVEL_SELECT;

        // Load level select level
        load_level(LEVEL_COUNT + 2);

        open_transition();
    }
}

void update_level_select(float dt, ButtonStates buttonStates) {
    player.update(dt, buttonStates);

    update_enemies(dt, buttonStates);

    update_level_triggers(dt, buttonStates);

    //update_coins(dt, buttonStates);

    //update_projectiles(dt, buttonStates);

    //finish.update(dt, buttonStates);

    // Button handling

    if (currentLevelNumber != NO_LEVEL_SELECTED && transition[0].is_open()) {
        close_transition();
    }


    if (transition[0].is_ready_to_open()) {
        gameState = GameState::STATE_IN_GAME;

        load_level(currentLevelNumber);
        // TODO: reset currentLevelNumber at some point later
        //currentLevelNumber = NO_LEVEL_SELECTED;

        start_level();
    }
    else if (transition[0].is_open()) {
        camera.ease_out_to(dt, player.x, player.y);
    }
}

void update_game(float dt, ButtonStates buttonStates) {
    player.update(dt, buttonStates);

    update_enemies(dt, buttonStates);

    update_coins(dt, buttonStates);

    update_projectiles(dt, buttonStates);

    finish.update(dt, buttonStates);


    if (player.x + SPRITE_SIZE > finish.x + 3 && player.x < finish.x + SPRITE_SIZE - 3 && player.y + SPRITE_SIZE > finish.y + 4 && player.y < finish.y + SPRITE_SIZE) {
        // lock player to finish
        player.x = finish.x;
        player.y = finish.y - 1;
    }

    //if (player.x + SPRITE_SIZE > finish.x - SPRITE_SIZE && player.x < finish.x + SPRITE_SIZE * 2 && player.y + SPRITE_SIZE > finish.y - SPRITE_SIZE && player.y < finish.y + SPRITE_SIZE * 2) {
    //    // 'pull' player to finish

    //    player.x += (finish.x - player.x) * 4 * dt;
    //    player.y += (finish.y - player.y) * 4 * dt;
    //}
    if (transition[0].is_open()) {
        if (cameraIntro) {
            //camera.linear_to(dt, cameraStartX, cameraStartY, player.x, player.y, CAMERA_PAN_TIME);
            camera.linear_to(dt, cameraStartX, cameraStartY, player.x, player.y, CAMERA_PAN_TIME);

            //camera.ease_to(dt/5, player.x, player.y);

            if (player.x == camera.x && player.y == camera.y) {
                cameraIntro = false;
                player.locked = false;
                // Make player immune when spawning?
                //player.set_immune();
            }
        }
        else {
            camera.ease_out_to(dt, player.x, player.y);
        }
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


    // Load character select level
    load_level(LEVEL_COUNT + 1);

    // Populate transition array
    for (int y = 0; y < SCREEN_HEIGHT / SPRITE_SIZE; y++) {
        for (int x = 0; x < SCREEN_WIDTH / SPRITE_SIZE; x++) {
            transition[y * (SCREEN_WIDTH / SPRITE_SIZE) + x] = AnimatedTransition(x * SPRITE_SIZE, y * SPRITE_SIZE, transitionFramesOpen, transitionFramesClose);
        }
    }
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

    if (gameState == GameState::STATE_CHARACTER_SELECT) {
        render_character_select();
    }
    else if (gameState == GameState::STATE_MENU) {
        render_menu();
    }
    else if (gameState == GameState::STATE_LEVEL_SELECT) {
        render_level_select();
    }
    else if (gameState == GameState::STATE_IN_GAME) {
        render_game();
    }
    else if (gameState == GameState::STATE_LOST) {

    }

    render_transition();

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

    textFlashTimer += dt;
    if (textFlashTimer >= TEXT_FLASH_TIME) {
        textFlashTimer -= TEXT_FLASH_TIME;
    }

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
    if (gameState == GameState::STATE_CHARACTER_SELECT) {
        update_character_select(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_MENU) {
        update_menu(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_LEVEL_SELECT) {
        update_level_select(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_IN_GAME) {
        update_game(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_LOST) {

    }

    update_transition(dt, buttonStates);
}