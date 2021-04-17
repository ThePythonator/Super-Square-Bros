#include "SuperSquareBros.hpp"
#include "assets.hpp"

using namespace blit;

// Note: if want to go hires, with lores graphics:
// screen.sprite(uint16_t sprite, const Point &position, const Point &origin, float scale, uint8_t transform)
// origin can be Point(0,0) and transform can be SpriteTransform::NONE, scale is 2

// e.g. screen.sprite(id, Point(x, y), Point(0, 0), 2.0f, SpriteTransform::NONE

#define RESET_SAVE_DATA_IF_MINOR_DIFF
#define TESTING_MODE

void init_game();

const uint16_t SCREEN_WIDTH = 160;
const uint16_t SCREEN_HEIGHT = 120;

const uint8_t LEVEL_COUNT = 10;
const uint8_t LEVEL_SELECT_NUMBER = LEVEL_COUNT + 2;
const uint8_t LEVELS_PER_WORLD = 4;

const uint8_t SNOW_WORLD = 1;

const uint8_t FADE_STEP = 3;

const float FRAME_LENGTH = 0.15f;
const float TRANSITION_FRAME_LENGTH = 0.1f;
const float TRANSITION_CLOSE_LENGTH = 0.5f;

const uint16_t TILE_ID_EMPTY = 255;
const uint16_t TILE_ID_COIN = 384;
const uint16_t TILE_ID_PLAYER_1 = 192;
const uint16_t TILE_ID_PLAYER_2 = 196;
const uint16_t TILE_ID_HEART = 416;
const uint16_t TILE_ID_CAMERA = 509;
const uint16_t TILE_ID_TRANSITION = 496;
const uint16_t TILE_ID_FINISH = 432;
const uint16_t TILE_ID_LEVEL_TRIGGER = 420;
const uint16_t TILE_ID_LEVEL_BRIDGE_MIN = 144;

const uint16_t TILE_ID_ENEMY_1 = 208;
const uint16_t TILE_ID_ENEMY_2 = 212;
const uint16_t TILE_ID_ENEMY_3 = 216;
const uint16_t TILE_ID_ENEMY_4 = 220;
const uint16_t TILE_ID_ENEMY_5 = 224;
const uint16_t TILE_ID_ENEMY_6 = 228;
const uint16_t TILE_ID_ENEMY_7 = 232;
const uint16_t TILE_ID_ENEMY_8 = 236;
const uint16_t TILE_ID_ENEMY_9 = 240;

const uint16_t TILE_ID_BOSS_1 = 256;
const uint16_t TILE_ID_BOSS_2 = 264;
const uint16_t TILE_ID_BIG_BOSS = 288;

const uint16_t TILE_ID_SPIKE_BOTTOM = 480;
const uint16_t TILE_ID_SPIKE_TOP = 481;
const uint16_t TILE_ID_SPIKE_LEFT = 482;
const uint16_t TILE_ID_SPIKE_RIGHT = 483;

const uint16_t TILE_ID_ENEMY_PROJECTILE_ROCK = 472;
const uint16_t TILE_ID_ENEMY_PROJECTILE_SNOWBALL = 473;
const uint16_t TILE_ID_ENEMY_PROJECTILE_BULLET = 474;

const uint16_t TILE_ID_BOSS_PROJECTILE_ROCK = 476;
const uint16_t TILE_ID_BOSS_PROJECTILE_SNOWBALL = 477;

const uint16_t TILE_ID_HUD_LIVES = 422;
const uint16_t TILE_ID_HUD_COINS = 424;
const uint16_t TILE_ID_HUD_ENEMIES_KILLED = 425;
const uint16_t TILE_ID_HUD_TIME_TAKEN = 426;

const float CAMERA_SCALE_X = 10.0f;
const float CAMERA_SCALE_Y = 5.0f;
const float CAMERA_PAN_TIME = 7.0f;
const float CAMERA_NEW_WORLD_TIME = 3.0f;
const float NEW_WORLD_DELAY_TIME = 2.0f;
const float CAMERA_RESPAWN_LOCK_MIN = 1.0f;

const float LEVEL_DEATH_BOUNDARY_SCALE = 1.5f;

const uint8_t SPRITE_SIZE = 8;

const uint8_t SG_ICON_SIZE = SPRITE_SIZE * 4;

const float SNOW_LEVEL_GENERATE_DELAY = 0.1f;
const float SNOW_LEVEL_SELECT_GENERATE_DELAY = 0.15f;
const uint16_t SNOW_LEVEL_INIT_COUNT = 300;
const uint16_t SNOW_LEVEL_SELECT_INIT_COUNT = 200;

const float SCREEN_SHAKE_SHAKINESS = 3.0f; //pixels either way - maybe more?

const float REPEL_PLAYER_MIN = 5.0f;


const uint8_t ENTITY_DEATH_PARTICLE_COUNT = 100;
const float ENTITY_DEATH_PARTICLE_GRAVITY_X = 0.0f;
const float ENTITY_DEATH_PARTICLE_GRAVITY_Y = 60.0f;
const float ENTITY_DEATH_PARTICLE_AGE = 1.0f;
const float ENTITY_DEATH_PARTICLE_SPEED = 60.0f;

const uint8_t BOSS_DEATH_PARTICLE_COUNT = 200;
const float BOSS_DEATH_PARTICLE_GRAVITY_X = 0.0f;
const float BOSS_DEATH_PARTICLE_GRAVITY_Y = 60.0f;
const float BOSS_DEATH_PARTICLE_AGE = 1.4f;
const float BOSS_DEATH_PARTICLE_SPEED = 70.0f;

const float PLAYER_SLOW_PARTICLE_GRAVITY_X = 0.0f;
const float PLAYER_SLOW_PARTICLE_GRAVITY_Y = -30.0f;
const float PLAYER_SLOW_PARTICLE_AGE = 0.8f;
const float PLAYER_SLOW_PARTICLE_SPEED = 50.0f;
const uint8_t PLAYER_SLOW_PARTICLE_WIGGLE = 1;
const float PLAYER_SLOW_PARTICLE_SPAWN_DELAY = 0.05f;


const float GRAVITY = 600.0f;
const float GRAVITY_MAX = 190.0f;
const float PROJECTILE_GRAVITY = 55.0f;
const float PROJECTILE_GRAVITY_MAX = 100.0f;


const uint8_t PLAYER_START_LIVES = 3;
const uint8_t PLAYER_MAX_HEALTH = 3;

const float PLAYER_MAX_JUMP = 190.0f;
//const float  PLAYER_ATTACK_JUMP = 90.0f;
const float PLAYER_ATTACK_JUMP_SCALE = 0.65f;
const float PLAYER_ATTACK_JUMP_MIN = 90.0f;
const float PLAYER_MAX_SPEED = 87.0f;
const float PLAYER_IMMUNE_TIME = 2.0f;
const float PLAYER_ACCELERATION = 370.0f;
const float PLAYER_DECELERATION = 330.0f;
const float PLAYER_JUMP_COOLDOWN = 0.4f;
const float PLAYER_JUMP_MIN_AIR_TIME = 0.05f;
//const float PLAYER_JUMP_EXTRA_GRAVITY = 500.0f;
const float PLAYER_JUMP_EXTRA_DOWNSCALE = 0.92f;

const float PLAYER_SLOW_MAX_JUMP = 110.0f;
const float PLAYER_SLOW_MAX_SPEED = 15.0f;
const float PLAYER_SLOW_ACCELERATION = 150.0f;
const float PLAYER_SLOW_DECELERATION = 100.0f;
const float PLAYER_SLOW_JUMP_COOLDOWN = 0.3f;

const float ENTITY_IDLE_SPEED = 40.0f;
const float ENTITY_PURSUIT_SPEED = 55.0f;
const float ENTITY_JUMP_SPEED = 160.0f;
const float ENTITY_JUMP_COOLDOWN = 0.5f;


const float BOSS_INJURED_TIME = 0.3f;
const float BOSS_IMMUNE_TIME = 1.0f;

const float BOSS_1_IDLE_SPEED = 30.0f;
const float BOSS_1_PURSUIT_SPEED = 50.0f;
const float BOSS_1_ANGRY_SPEED = 105.0f;
const float BOSS_1_SPEED_REDUCTION_SCALE = 0.7f;
const float BOSS_1_JUMP_SPEED = 160.0f;
const float BOSS_1_ANGRY_JUMP_SPEED = 220.0f;
const float BOSS_1_JUMP_COOLDOWN = 1.5f;
const float BOSS_1_MINION_SPAWN_COOLDOWN = 1.5f;
const float BOSS_1_MINION_SPEED = 25.0f;
const float BOSS_1_MINION_SPEED_REDUCTION = 4.0f;
const float BOSS_1_JUMP_TRIGGER_MAX_RANGE = SPRITE_SIZE * 8;
const float BOSS_1_IGNORE_MIN_RANGE = SPRITE_SIZE * 9;
const float BOSS_1_INJURED_MAX_RANGE = SPRITE_SIZE * 12;
const float BOSS_1_DEATH_MAX_RANGE = SPRITE_SIZE * 16;
const float BOSS_1_RETURN_TO_SPAWN_RANGE = SPRITE_SIZE * 4;
const float BOSS_1_JUMP_SHAKE_TIME = 0.3f;
const float BOSS_1_ANGRY_JUMP_SHAKE_TIME = 0.35f;

const float BOSS_2_JUMP_SPEED = 160.0f;
const float BOSS_2_ANGRY_JUMP_SPEED = 220.0f;
const float BOSS_2_JUMP_COOLDOWN = 1.5f;
const float BOSS_2_RELOAD_TIME = 2.0f;
const float BOSS_2_RAPID_RELOAD_TIME = 0.4f;
const float BOSS_2_SUPER_RAPID_RELOAD_TIME = 0.1f;
const uint8_t BOSS_2_RAPID_SHOT_COUNT = 3;
const float BOSS_2_PROJECTILE_FLIGHT_TIME = 1.1f;
const float BOSS_2_RAPID_PROJECTILE_FLIGHT_TIME = 0.8f;
const float BOSS_2_SUPER_RAPID_PROJECTILE_FLIGHT_TIME = 0.8f;
const float BOSS_2_RESET_COOLDOWN = 2.0f;
const float BOSS_2_INJURED_TIME = 0.3f;
const float BOSS_2_IMMUNE_TIME = 1.0f;
const float BOSS_2_JUMP_TRIGGER_MAX_RANGE = SPRITE_SIZE * 10;
const float BOSS_2_IGNORE_MIN_RANGE = SPRITE_SIZE * 11;
const float BOSS_2_JUMP_SHAKE_TIME = 0.3f;
const float BOSS_2_ANGRY_JUMP_SHAKE_TIME = 0.35f;

const float BIG_BOSS_IDLE_SPEED = 30.0f;
const float BIG_BOSS_PURSUIT_SPEED = 50.0f;
const float BIG_BOSS_RETREAT_SPEED = 20.0f;
//const float BIG_BOSS_ANGRY_SPEED = 100.0f;
const float BIG_BOSS_JUMP_SPEED = 180.0f;
const float BIG_BOSS_ANGRY_JUMP_SPEED = 240.0f;
const float BIG_BOSS_JUMP_COOLDOWN = 2.0f;
const float BIG_BOSS_MINION_SPAWN_COOLDOWN = 1.5f;
const float BIG_BOSS_JUMP_TRIGGER_MAX_RANGE = SPRITE_SIZE * 10;
const float BIG_BOSS_IGNORE_MIN_RANGE = SPRITE_SIZE * 12;
const float BIG_BOSS_INJURED_MAX_RANGE = SPRITE_SIZE * 10;
//const float BOSS_1_DEATH_MAX_RANGE = SPRITE_SIZE * 16;
const float BIG_BOSS_RETURN_TO_SPAWN_RANGE = SPRITE_SIZE * 4;
const float BIG_BOSS_JUMP_SHAKE_TIME = 0.45f;
const float BIG_BOSS_ANGRY_JUMP_SHAKE_TIME = 0.5f;
const float BIG_BOSS_RELOAD_TIME = 2.3f;
const float BIG_BOSS_RAPID_RELOAD_TIME = 0.4f;
const float BIG_BOSS_PROJECTILE_FLIGHT_TIME = 1.0f;


const float RANGED_MAX_RANGE = 64.0f;
const float RANGED_RELOAD_TIME = 2.0f;
const float RANGED_PROJECTILE_X_VEL_SCALE = 0.8f;
const float RANGED_PROJECTILE_Y_VEL_SCALE = 0.5f;

const float SHOOTING_MAX_RANGE_X = 64.0f;
const float SHOOTING_MAX_RANGE_Y = 32.0f;
const float SHOOTING_RELOAD_TIME = 2.5f;
const float SHOOTING_RAPID_RELOAD_TIME = 0.5f;
const uint8_t SHOOTING_ENEMY_CLIP_SIZE = 2;

const float BULLET_PROJECTILE_SPEED = 64.0f;

const float PURSUIT_MAX_RANGE = 48.0f;

const float LEVEL_INFO_MAX_RANGE = SPRITE_SIZE * 4;

const float TEXT_FLASH_TIME = 0.8f;

const float TEXT_JUMP_VELOCITY = 80.0f;
const float TEXT_GRAVITY = 280.0f;

const uint8_t NO_LEVEL_SELECTED = 255;


const uint8_t MESSAGE_STRINGS_COUNT = 4;
const uint8_t INPUT_TYPE_COUNT = 2;

const uint16_t BYTE_SIZE = 256;


// NOTE: all positions (x,y) mark TOP LEFT corner of sprites

// NOTE: issue with rendering (tiles on left are a pixel out sometimes) is due to integers being added to floats. Something along lines of (int)floorf(camera.x) etc is recommended, but when I tried it I got strange results.
// If I implement that again, remember that all float calcs should be done, *then* casted, rather than casting each to int then adding etc


const uint16_t SCREEN_MID_WIDTH = SCREEN_WIDTH / 2;
const uint16_t SCREEN_MID_HEIGHT = SCREEN_HEIGHT / 2;


const uint8_t SPRITE_HALF = SPRITE_SIZE / 2;
const uint8_t SPRITE_QUARTER = SPRITE_SIZE / 4;

const uint16_t SCREEN_TILE_SIZE = (SCREEN_WIDTH / SPRITE_SIZE) * (SCREEN_HEIGHT / SPRITE_SIZE);

const uint8_t enemyHealths[] = { 1, 1, 1, 1, 2, 2, 2, 2, 1 };
const uint8_t bossHealths[] = { 3, 3, 3 };
const uint8_t bigBossMinions[] = { 7, 6, 4 };

const std::vector<uint16_t> coinFrames = { TILE_ID_COIN, TILE_ID_COIN + 1, TILE_ID_COIN + 2, TILE_ID_COIN + 3, TILE_ID_COIN + 2, TILE_ID_COIN + 1 };

const std::vector<uint16_t> finishFrames = { TILE_ID_FINISH, TILE_ID_FINISH + 1, TILE_ID_FINISH + 2, TILE_ID_FINISH + 3, TILE_ID_FINISH + 4, TILE_ID_FINISH + 5 };

const std::vector<uint16_t> transitionFramesClose = { TILE_ID_TRANSITION, TILE_ID_TRANSITION + 1, TILE_ID_TRANSITION + 2, TILE_ID_TRANSITION + 3, TILE_ID_TRANSITION + 4, TILE_ID_TRANSITION + 6, TILE_ID_TRANSITION + 7};
const std::vector<uint16_t> transitionFramesOpen = { TILE_ID_TRANSITION + 6, TILE_ID_TRANSITION + 5, TILE_ID_TRANSITION + 4, TILE_ID_TRANSITION + 3, TILE_ID_TRANSITION + 2, TILE_ID_TRANSITION + 1, TILE_ID_TRANSITION};

const float parallaxFactorLayersX[2] = {
    0.4f,
    0.2f
};

const float parallaxFactorLayersY[2] = {
    0.4f,
    0.2f
};

const uint8_t* asset_levels[] = {
    asset_level0,
    asset_level1,
    asset_level2,
    asset_level3,
    asset_level4,
    asset_level5,
    asset_level6,
    asset_level7,
    asset_level8,
    asset_level9,
    asset_level_title,
    asset_level_char_select,
    asset_level_level_select
};

std::vector<uint16_t> snowParticleImages = {
    464,
    465
};



const std::string messageStrings[MESSAGE_STRINGS_COUNT][INPUT_TYPE_COUNT] = {
    {
        "Press A to Start",
        "Press U to Start"
    },
    {
        "Press A to Skip",
        "Press U to Skip"
    },
    {
        "Press A",
        "Press U"
    },
    {
        "Press Y to Resume",
        "Press P to Resume"
    }
};


uint16_t levelDeathBoundary;

float dt;
uint32_t lastTime = 0;

Surface* sg_icon_image = Surface::load(asset_scorpion_games);
Surface* background_image = Surface::load(asset_background);

AudioHandler::AudioHandler audioHandler;

bool menuBack = false; // tells menu to go backwards instead of forwards.
bool gamePaused = false; // used for determining if game is paused or not.

bool coinSfxAlternator = false; // used for alternating channel used for coin pickup sfx

bool cameraIntro = false;
bool cameraRespawn = false;
bool cameraNewWorld = false;
uint16_t cameraStartX, cameraStartY;
uint16_t playerStartX, playerStartY;

//float windSpeed = 0;

float textFlashTimer = 0.0f;
uint8_t playerSelected = 0;
uint8_t pauseMenuItem = 0;
uint8_t menuItem = 0;
uint8_t settingsItem = 0;

float snowGenTimer = 0.0f;

bool slowPlayer = false;
bool dropPlayer = false;
bool repelPlayer = false;
bool bossBattle = false;

uint8_t currentLevelNumber = NO_LEVEL_SELECTED;
uint8_t currentWorldNumber = 0;

// For version handling
struct GameVersion {
    uint8_t major;
    uint8_t minor;
    uint8_t build;
} gameVersion;

// For metadata
GameMetadata metadata;

// struct to handle level data header...
// this will probably need to be revisited/reworked if 32blit-tools has *any* kind of update to it...
#pragma pack(push,1)
struct TMX16 {
    char head[4];
    uint8_t empty_tile;
    uint16_t width;
    uint16_t height;
    uint16_t layers;
    uint16_t data[];
};
#pragma pack(pop)

enum class GameState {
    STATE_SG_ICON,
    STATE_INPUT_SELECT,
    STATE_MENU,
    STATE_SETTINGS,
    STATE_CHARACTER_SELECT,
    STATE_LEVEL_SELECT,
    STATE_IN_GAME,
    STATE_LOST,
    STATE_WON
};
GameState gameState = GameState::STATE_SG_ICON;

enum InputType {
    CONTROLLER = 0,
    KEYBOARD = 1
    //NO_INPUT_TYPE
};

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

struct GameSaveData {
    uint16_t version;
    uint8_t inputType;
} gameSaveData;

struct PlayerSaveData {
    uint8_t levelReached;
};
PlayerSaveData allPlayerSaveData[2];

struct LevelSaveData {
    uint8_t score;
    uint8_t enemiesKilled;
    float time;
};
LevelSaveData allLevelSaveData[2][LEVEL_COUNT];

struct SettingsData {
    bool checkpoints = false;
    bool musicVolume = true;
    bool sfxVolume = true;
} settingsData;


// 0 slot is gameSaveData
// 1 slot is player 1 saveData
// 2..257 slots are player 1 levelData
// 258 slot is player 2 saveData
// 259..514 slots are player 2 levelData


/*int16_t get_version() {
    return VERSION_MAJOR * 256 + VERSION_MINOR * 16 + VERSION_BUILD;
}*/

uint16_t get_version(GameVersion gameVersionData) {
    return gameVersionData.major * 256 + gameVersionData.minor * 16 + gameVersionData.build;
}

GameVersion get_version_struct(uint16_t version) {
    GameVersion gameVersionData{};

    gameVersionData.major = version / 256;
    version %= 256;

    gameVersionData.minor = version / 16;
    version %= 16;

    gameVersionData.build = version;

    return gameVersionData;
}

GameVersion parse_version(std::string versionString) {
    GameVersion version{};

    uint8_t startIndex, endIndex;

    startIndex = 1;
    endIndex = versionString.find('.');

    version.major = (uint8_t)std::stoi(versionString.substr(startIndex, endIndex));

    startIndex = endIndex + 1;
    endIndex = versionString.find('.', startIndex);

    version.minor = (uint8_t)std::stoi(versionString.substr(startIndex, endIndex));

    startIndex = endIndex + 1;
    endIndex = versionString.size();

    version.build = (uint8_t)std::stoi(versionString.substr(startIndex, endIndex));

    return version;
}


void save_game_data() {
    // Write save data
    write_save(gameSaveData);
}

void save_level_data(uint8_t playerID, uint8_t levelNumber) {
    // Write level data
    write_save(allLevelSaveData[playerID][levelNumber], (playerID * (BYTE_SIZE + 1)) + 1 + levelNumber + 1);
}

void save_player_data(uint8_t playerID) {
    // Write level data
    write_save(allPlayerSaveData[playerID], (playerID * (BYTE_SIZE + 1)) + 1);
}

LevelSaveData load_level_data(uint8_t playerID, uint8_t levelNumber) {
    LevelSaveData levelSaveData;
    if (read_save(levelSaveData, (playerID * (BYTE_SIZE + 1)) + 1 + levelNumber + 1)) {
        // Success
    }
    else {
        // Set some defaults
        levelSaveData.score = 0;
        levelSaveData.enemiesKilled = 0;
        levelSaveData.time = 0.0f; // If time == 0.0f, game realises that it's a N/A time
    }
    return levelSaveData;
}

PlayerSaveData load_player_data(uint8_t playerID) {
    PlayerSaveData playerSaveData;
    if (read_save(playerSaveData, (playerID * (BYTE_SIZE + 1)) + 1)) {
        // Success
    }
    else {
        // Set some defaults
        playerSaveData.levelReached = 0;
    }
    return playerSaveData;
}

void reset_save() {
    gameSaveData.version = get_version(gameVersion);
    gameSaveData.inputType = InputType::CONTROLLER;
    save_game_data();

    allPlayerSaveData[0].levelReached = 0;
    save_player_data(0);

    allPlayerSaveData[1].levelReached = 0;
    save_player_data(1);

    for (uint8_t i = 0; i < LEVEL_COUNT; i++) {
        allLevelSaveData[0][i].score = 0;
        allLevelSaveData[0][i].enemiesKilled = 0;
        allLevelSaveData[0][i].time = 0.0f;
        save_level_data(0, i);

        allLevelSaveData[1][i].score = 0;
        allLevelSaveData[1][i].enemiesKilled = 0;
        allLevelSaveData[1][i].time = 0.0f;
        save_level_data(1, i);
    }
}


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


void render_sprite(uint16_t id, Point point) {
    //screen.sprite(id, point, Point(0, 0), 1.0f, SpriteTransform::NONE);
    screen.sprite(id, point);
}

void render_sprite(uint16_t id, Point point, SpriteTransform transform) {
    //screen.sprite(id, point, Point(0, 0), 1.0f, transform);
    screen.sprite(id, point, transform);
}


class ScreenShake {
public:
    ScreenShake() {
        set_shake(0);
        set_shakiness(0);
    }

    ScreenShake(float shakiness) {
        set_shake(0);
        set_shakiness(shakiness);
    }

    void set_shake(float shake) {
        this->shake = shake;
    }

    void set_shakiness(float shakiness) {
        this->shakiness = 100 / shakiness;
    }

    float time_to_shake(float dt) {
        if (shake > 0) {
            shake -= dt;
            if (shake < 0) {
                shake = 0;
            }
            return (rand() % 201 - 100) / shakiness;
        }
        else {
            return 0.0f;
        }
    }

protected:
    float shake;
    float shakiness;
};
ScreenShake shaker(SCREEN_SHAKE_SHAKINESS);

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
const std::vector<Colour> enemyDeathParticleColours[5] = {
    { Colour(255, 255, 242), Colour(184, 197, 216), Colour(25, 40, 102) },
    { Colour(255, 255, 242), Colour(255, 204, 181), Colour(165, 82, 139) },
    { Colour(255, 255, 242), Colour(255, 204, 181), Colour(229, 114, 57) },
    { Colour(255, 255, 242), Colour(204, 137, 124), Colour(127, 24, 75) },
    { Colour(255, 255, 242), Colour(145, 224, 204), Colour(53, 130, 130) }
};
const std::vector<Colour> bossDeathParticleColours[3] = {
    { Colour(255, 255, 242), Colour(184, 197, 216), Colour(25, 40, 102) },
    { Colour(255, 255, 242), Colour(255, 204, 181), Colour(165, 82, 139) },
    { Colour(255, 255, 242), Colour(184, 197, 216), Colour(25, 40, 102) }
};
const std::vector<Colour> levelTriggerParticleColours = { Colour(255, 255, 242), Colour(145, 224, 204), Colour(53, 130, 130) };

const std::vector<Colour> slowPlayerParticleColours = { Colour(145, 224, 204), Colour(53, 130, 130) };//Colour(255, 255, 242), 
const std::vector<Colour> repelPlayerParticleColours = { Colour(255, 235, 140), Colour(255, 199, 89) };

const Colour inputSelectColour = Colour(255, 199, 89);
const Colour hudBackground = Colour(7, 0, 14, 64);
const Colour gameBackground = Colour(62, 106, 178);
const Colour defaultWhite = Colour(255, 255, 242);
Colour splashColour = Colour(7, 0, 14, 0);

class Camera {
public:
    float x, y;
    float tempX, tempY;

    bool locked;

    Camera() {
        x = y = 0;
        locked = false;
        reset_temp();
    }

    void reset_temp() {
        tempX = tempY = 0;
    }

    void reset_timer() {
        timer = -1.0f;
    }

    bool timer_started() {
        return timer >= 0;
    }

    void start_timer() {
        timer = 0.0f;
    }

    void update_timer(float dt) {
        timer += dt;
    }

    float get_timer() {
        return timer;
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
protected:
    float timer;
};
Camera camera;


class Particle {
public:
    float x, y;
    float xVel, yVel;
    float gravityX, gravityY;
    Colour colour;
    float age;

    Particle() {
        x = y = 0;
        xVel = yVel = 0;

        gravityX = 0;
        gravityY = 0;
        colour = Colour(0, 0, 0);

        age = 0;
    }

    Particle(float xPosition, float yPosition, float xVelocity, float yVelocity, float particleGravityX, float particleGravityY, Colour particleColour) {
        x = xPosition;
        y = yPosition;
        xVel = xVelocity;
        yVel = yVelocity;

        gravityX = particleGravityX;
        gravityY = particleGravityY;
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

        xVel += gravityX * dt;
        yVel += gravityY * dt;

        x += xVel * dt;
        y += yVel * dt;
    }
};

class ImageParticle {
public:
    float x, y;
    float xVel, yVel;
    float gravityX, gravityY;
    uint16_t id;

    ImageParticle() {
        x = y = 0;
        xVel = yVel = 0;

        gravityX = 0;
        gravityY = 0;
        id = 0;
    }

    ImageParticle(float xPosition, float yPosition, float xVelocity, float yVelocity, float particleGravityX, float particleGravityY, uint16_t tileID) {
        x = xPosition;
        y = yPosition;
        xVel = xVelocity;
        yVel = yVelocity;

        gravityX = particleGravityX;
        gravityY = particleGravityY;
        id = tileID;
    }

    void render(Camera camera) {
        screen.sprite(id, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
    }

    void update(float dt) {
        xVel += gravityX * dt;
        yVel += gravityY * dt;

        x += xVel * dt;
        y += yVel * dt;
    }
};
std::vector<ImageParticle> imageParticles;


class BrownianParticle : public Particle {
public:
    BrownianParticle() : Particle() {
        angleWiggle = 1;
        angle = 0;
        speed = 0;
    }

    BrownianParticle(float xPosition, float yPosition, uint16_t angle, float speed, float particleGravityX, float particleGravityY, Colour particleColour, uint8_t angleWiggle) : Particle(xPosition, yPosition, 0, 0, particleGravityX, particleGravityY, particleColour) {
        this->angleWiggle = angleWiggle;
        this->angle = angle;
        this->speed = speed;
    }

    void update(float dt) {
        angle += (rand() % (angleWiggle * 2 + 1)) - angleWiggle;
        angle %= 360;

        xVel = std::cos((float)angle) * speed;
        yVel = std::sin((float)angle) * speed;

        Particle::update(dt);
    }

protected:
    uint8_t angleWiggle;
    uint16_t angle;
    float speed;
};



std::vector<Particle> generate_particles(float x, float y, float gravityX, float gravityY, std::vector<Colour> colours, float speed, uint8_t count) {
    std::vector<Particle> particles;

    for (uint8_t i = 0; i < count; i++) {
        float angle = rand() % 360;

        float xVel = ((rand() % 100) / 100.0f) * std::cos(angle) * speed;
        float yVel = ((rand() % 100) / 100.0f) * std::sin(angle) * speed;

        particles.push_back(Particle(x, y, xVel, yVel, gravityX, gravityY, colours[rand() % colours.size()]));
    }

    return particles;
}

BrownianParticle generate_brownian_particle(float x, float y, float gravityX, float gravityY, float speed, std::vector<Colour> colours, uint8_t wiggle) {
    uint16_t angle = rand() % 360;

    return BrownianParticle(x, y, angle, speed, gravityX, gravityY, colours[rand() % colours.size()], wiggle);
}


class Projectile {
public:
    float x, y;
    float xVel, yVel;
    uint16_t id;
    uint8_t width;

    Projectile() {
        x = y = 0;
        xVel = yVel = 0;

        id = 0;

        gravity = false;

        width = SPRITE_HALF;
    }

    Projectile(float xPosition, float yPosition, float xVelocity, float yVelocity, uint16_t tileId, bool gravity=true, uint8_t rectWidth=SPRITE_HALF) {
        x = xPosition;
        y = yPosition;
        xVel = xVelocity;
        yVel = yVelocity;
        id = tileId;
        this->gravity = gravity;
        width = rectWidth;
    }

    void update(float dt, ButtonStates buttonStates) {
        if (gravity) {
            // Update gravity
            yVel += PROJECTILE_GRAVITY * dt;
            yVel = std::min(yVel, (float)PROJECTILE_GRAVITY_MAX);
        }

        // Move entity y
        y += yVel * dt;

        // Move entity x
        x += xVel * dt;
    }

    void render(Camera camera) {
        render_sprite(id, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
        //screen.sprite(id, Point(SCREEN_MID_WIDTH + x - camera.x - SPRITE_QUARTER, SCREEN_MID_HEIGHT + y - camera.y - SPRITE_QUARTER));
        //screen.rectangle(Rect(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y, 4, 4));
    }

    bool is_colliding(float playerX, float playerY) {
        return x + SPRITE_HALF + width / 2 > playerX && x + SPRITE_HALF - width / 2 < playerX + SPRITE_SIZE && y + SPRITE_HALF + width / 2 > playerY && y + SPRITE_HALF - width / 2 < playerY + SPRITE_SIZE;
    }

protected:
    bool gravity;
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

    Tile(uint16_t xPosition, uint16_t yPosition, uint16_t tileID) : LevelObject(xPosition, yPosition) {
        id = tileID;
    }

    void update(float dt, ButtonStates buttonStates) {

    }

    void render(Camera camera)
    {
        //screen.sprite(id, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
        render_sprite(id, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
    }

    uint16_t get_id() {
        return id;
    }
    
protected:
    uint16_t id;
};
std::vector<Tile> foreground;
std::vector<Tile> generic_entities; // used because platforms are rendered over background, but bushes, trees etc need to be rendered over platforms
std::vector<Tile> background;
// Platforms are only collidable when falling (or travelling sideways)
std::vector<Tile> platforms;
std::vector<Tile> spikes;

class ParallaxTile : public Tile {
public:
    ParallaxTile() : Tile() {
        id = TILE_ID_EMPTY;
        layer = 0;
    }

    ParallaxTile(uint16_t xPosition, uint16_t yPosition, uint16_t tileID, uint8_t parallaxLayer) : Tile(xPosition, yPosition, tileID) {
        id = tileID;
        layer = parallaxLayer;
    }

    void update(float dt, ButtonStates buttonStates) {
        
    }

    void render(Camera camera)
    {
        //screen.sprite(id, Point(SCREEN_MID_WIDTH + x - (camera.x * parallaxFactorLayersX[layer]), SCREEN_MID_HEIGHT + y - (camera.y * parallaxFactorLayersY[layer])));
        // Not shifting sprite to center seems to give better coverage of parallax
        //screen.sprite(id, Point(x - (camera.x * parallaxFactorLayersX[layer]), y - (camera.y * parallaxFactorLayersY[layer])));
        render_sprite(id, Point(x - (camera.x * parallaxFactorLayersX[layer]), y - (camera.y * parallaxFactorLayersY[layer])));
    }

protected:
    uint16_t id;
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

    AnimatedPickup(uint16_t xPosition, uint16_t yPosition, std::vector<uint16_t> animationFrames) : Pickup(xPosition, yPosition) {
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
            //screen.sprite(frames[currentFrame], Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
            render_sprite(frames[currentFrame], Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
        }
    }

protected:
    float animationTimer;
    std::vector<uint16_t> frames;
    uint16_t currentFrame;
};

class Coin : public AnimatedPickup {
public:
    Coin() : AnimatedPickup() {

    }

    Coin(uint16_t xPosition, uint16_t yPosition, std::vector<uint16_t> animationFrames) : AnimatedPickup(xPosition, yPosition, animationFrames) {

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

    Finish(uint16_t xPosition, uint16_t yPosition, std::vector<uint16_t> animationFrames) : AnimatedPickup(xPosition, yPosition, animationFrames) {

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

    AnimatedTransition(uint16_t xPosition, uint16_t yPosition, std::vector<uint16_t> open, std::vector<uint16_t> close) {
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
            //screen.sprite(closingFrames[currentFrame], Point(x, y));
            render_sprite(closingFrames[currentFrame], Point(x, y));
        }
        else if (state == TransitionState::OPENING) {
            //screen.sprite(openingFrames[currentFrame], Point(x, y), SpriteTransform::HORIZONTAL);
            render_sprite(openingFrames[currentFrame], Point(x, y), SpriteTransform::HORIZONTAL);
        }
        else if (state == TransitionState::CLOSED || state == TransitionState::READY_TO_OPEN) {
            //screen.sprite(closingFrames[closingFrames.size() - 1], Point(x, y));
            render_sprite(closingFrames[closingFrames.size() - 1], Point(x, y));
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
    std::vector<uint16_t> openingFrames, closingFrames;
    uint16_t currentFrame;
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
                particles = generate_particles(x + SPRITE_HALF, y + SPRITE_HALF, ENTITY_DEATH_PARTICLE_GRAVITY_X, ENTITY_DEATH_PARTICLE_GRAVITY_Y, levelTriggerParticleColours, ENTITY_DEATH_PARTICLE_SPEED, ENTITY_DEATH_PARTICLE_COUNT);
                generateParticles = true;
            }
        }
    }

    void render(Camera camera) {
        if (visible) {
            screen.pen = Pen(levelTriggerParticleColours[1].r, levelTriggerParticleColours[1].g, levelTriggerParticleColours[1].b, levelTriggerParticleColours[1].a);
            screen.text(std::to_string(levelNumber + 1), minimal_font, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_HALF, SCREEN_MID_HEIGHT + y - camera.y - SPRITE_HALF * 3 + textY), true, TextAlign::center_center);
            //screen.sprite(TILE_ID_LEVEL_TRIGGER, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
            render_sprite(TILE_ID_LEVEL_TRIGGER, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
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

// Function used for vector sorting
bool level_trigger_sort_min_x(const LevelTrigger& a, const LevelTrigger& b) {
    return a.x < b.x;
}



class Entity {
public:
    float x, y;
    float xVel, yVel;
    uint8_t health;
    bool locked;
    std::vector<Particle> particles;
    uint8_t lastDirection;
    float jumpCooldown;

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
        jumpCooldown = 0;
    }

    Entity(uint16_t xPosition, uint16_t yPosition, uint16_t frame, uint8_t startHealth) {
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
        jumpCooldown = 0;
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
                    else if (yVel < 0) {
                        // Collided from bottom
                        y = foreground[i].y + SPRITE_SIZE;
                    }
                    yVel = 0;
                }
            }

            // Platforms may need work
            for (uint16_t i = 0; i < platforms.size(); i++) {
                handle_platform_collisions(platforms[i]);
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
                    else if (xVel < 0) {
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

    void jump(float jumpVel, float cooldown) {
        // Jump
        yVel = -jumpVel;
        jumpCooldown = cooldown;
    }

    bool is_on_block() {
        // Is entity on a tile?
        for (uint16_t i = 0; i < foreground.size(); i++) {
            if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE - 1 > x && foreground[i].x + 1 < x + SPRITE_SIZE) {
                // On top of block
                return true;
            }
        }

        // Is entity on a platform?
        for (uint16_t i = 0; i < platforms.size(); i++) {
            if (y + SPRITE_SIZE == platforms[i].y && platforms[i].x + SPRITE_SIZE - 1 > x && platforms[i].x + 1 < x + SPRITE_SIZE) {
                // On top of block
                return true;
            }
        }

        // Is entity on a locked LevelTrigger?
        for (uint16_t i = 0; i < levelTriggers.size(); i++) {
            if (y + SPRITE_SIZE == levelTriggers[i].y && levelTriggers[i].x + SPRITE_SIZE - 1 > x && levelTriggers[i].x + 1 < x + SPRITE_SIZE) {
                // On top of block
                if (allPlayerSaveData[playerSelected].levelReached < levelTriggers[i].levelNumber) {
                    // LevelTrigger is locked
                    return true;
                }
            }
        }

        // Not on a block
        return false;
    }

    void handle_platform_collisions(Tile platform) {
        if (colliding(platform)) {
            if (yVel > 0 && y + SPRITE_SIZE < platform.y + SPRITE_QUARTER) {
                // Collided from top
                y = platform.y - SPRITE_SIZE;
                yVel = 0;
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
                uint16_t frame = anchorFrame;

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
                    //screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                    render_sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                }
                else {
                    //screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
                    render_sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
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
    uint16_t anchorFrame;
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
        rapidfireTimer = 0;

        playerX = nullptr;
        playerY = nullptr;

        currentSpeed = ENTITY_IDLE_SPEED;

        state = 0;

        shotsLeft = 0;
    }

    Enemy(uint16_t xPosition, uint16_t yPosition, uint8_t startHealth, uint8_t type) : Entity(xPosition, yPosition, TILE_ID_ENEMY_1 + type * 4, startHealth) {
        enemyType = (EnemyType)type;
        reloadTimer = 0;
        rapidfireTimer = 0;

        playerX = nullptr;
        playerY = nullptr;

        currentSpeed = ENTITY_IDLE_SPEED;

        state = 0;

        if (enemyType == EnemyType::SHOOTING) {
            shotsLeft = SHOOTING_ENEMY_CLIP_SIZE;
        }
        else {
            shotsLeft = 0;
        }
    }

    void update(float dt, ButtonStates buttonStates) {
        if (health > 0) {
            if (reloadTimer) {
                reloadTimer -= dt;
                if (reloadTimer < 0) {
                    reloadTimer = 0;
                }
            }

            if (rapidfireTimer) {
                rapidfireTimer -= dt;
                if (rapidfireTimer < 0) {
                    rapidfireTimer = 0;
                }
            }

            if (jumpCooldown) {
                jumpCooldown -= dt;
                if (jumpCooldown < 0) {
                    jumpCooldown = 0;
                }
            }

            if (enemyType == EnemyType::BASIC || enemyType == EnemyType::ARMOURED) {
                // Consider adding acceleration?
                if (lastDirection) {
                    xVel = currentSpeed;
                }
                else {
                    xVel = -currentSpeed;
                }

                Entity::update_collisions();


                bool reverseDirection = true;

                float tempX = lastDirection ? x + SPRITE_SIZE : x - SPRITE_SIZE;

                for (uint16_t i = 0; i < platforms.size(); i++) {
                    if (y + SPRITE_SIZE == platforms[i].y && platforms[i].x + SPRITE_SIZE > tempX + 1 && platforms[i].x < tempX + SPRITE_SIZE - 1) {
                        // About to be on block
                        reverseDirection = false;
                    }
                }

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

                if (health == 1) {
                    // EnemyType::ARMOURED has helmet on, and 2 hp
                    enemyType = EnemyType::BASIC;
                    anchorFrame = TILE_ID_ENEMY_1 + (int)enemyType * 4;
                }
            }
            else if (enemyType == EnemyType::RANGED || enemyType == EnemyType::ARMOURED_RANGED) {
                Entity::update_collisions();

                lastDirection = *playerX < x ? 0 : 1;

                if (std::abs(x - *playerX) < RANGED_MAX_RANGE && std::abs(y - *playerY) < RANGED_MAX_RANGE) {
                    state = 1;
                }
                else {
                    state = 0;
                }

                if (state == 1) {
                    if (!reloadTimer) {
                        // Fire!
                        // Maybe make these values constants?
                        projectiles.push_back(Projectile(x, y, RANGED_PROJECTILE_X_VEL_SCALE * (*playerX - x), -std::abs(x - *playerX) * RANGED_PROJECTILE_Y_VEL_SCALE + (*playerY - y) * RANGED_PROJECTILE_Y_VEL_SCALE, currentWorldNumber == SNOW_WORLD || currentLevelNumber == 8 ? TILE_ID_ENEMY_PROJECTILE_SNOWBALL : TILE_ID_ENEMY_PROJECTILE_ROCK));
                        reloadTimer = RANGED_RELOAD_TIME;

                        audioHandler.play(6);
                    }
                }

                if (health == 1) {
                    // EnemyType::ARMOURED_RANGED has helmet on, and 2 hp
                    enemyType = EnemyType::RANGED;
                    anchorFrame = TILE_ID_ENEMY_1 + (int)enemyType * 4;
                }
            }
            else if (enemyType == EnemyType::PURSUIT || enemyType == EnemyType::ARMOURED_PURSUIT) {
                // Consider adding acceleration?
                if (lastDirection) {
                    xVel = currentSpeed;
                }
                else {
                    xVel = -currentSpeed;
                }

                Entity::update_collisions();


                if (std::abs(x - *playerX) < PURSUIT_MAX_RANGE && std::abs(y - *playerY) < PURSUIT_MAX_RANGE) {
                    state = 1;
                }
                else {
                    state = 0;
                }

                if (state == 0) {
                    // Just patrol... (Same as basic enemy)
                    currentSpeed = ENTITY_IDLE_SPEED;

                    bool reverseDirection = true;

                    float tempX = lastDirection ? x + SPRITE_SIZE : x - SPRITE_SIZE;

                    for (uint16_t i = 0; i < platforms.size(); i++) {
                        if (y + SPRITE_SIZE == platforms[i].y && platforms[i].x + SPRITE_SIZE > tempX + 1 && platforms[i].x < tempX + SPRITE_SIZE - 1) {
                            // About to be on block
                            reverseDirection = false;
                        }
                    }

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
                else if (state == 1) {
                    // Pursue!
                    currentSpeed = ENTITY_PURSUIT_SPEED;

                    lastDirection = *playerX < x ? 0 : 1;

                    bool shouldJump = true;

                    float tempX = lastDirection ? x + SPRITE_SIZE : x - SPRITE_SIZE;

                    for (uint16_t i = 0; i < platforms.size(); i++) {
                        if (y + SPRITE_SIZE == platforms[i].y && platforms[i].x + SPRITE_SIZE > tempX + 1 && platforms[i].x < tempX + SPRITE_SIZE - 1) {
                            // About to be on block
                            shouldJump = false;
                            break;
                        }
                    }

                    for (uint16_t i = 0; i < foreground.size(); i++) {
                        if (y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE - 1) {
                            // About to be on block
                            shouldJump = false;
                            break;
                        }
                    }
                    for (uint16_t i = 0; i < foreground.size(); i++) {
                        if ((lastDirection ? x + SPRITE_SIZE - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                            // Walked into side of block
                            shouldJump = true;
                            // Break because we definitely need to jump
                            break;
                        }
                    }

                    if (shouldJump && jumpCooldown == 0) {
                        if (is_on_block()) {
                            jump(ENTITY_JUMP_SPEED, ENTITY_JUMP_COOLDOWN);
                        }
                    }
                }

                if (health == 1) {
                    // EnemyType::ARMOURED_PURSUIT has helmet on, and 2 hp
                    enemyType = EnemyType::PURSUIT;
                    anchorFrame = TILE_ID_ENEMY_1 + (int)enemyType * 4;
                }
            }
            else if (enemyType == EnemyType::FLYING || enemyType == EnemyType::ARMOURED_FLYING) {
                // Should it be EnemyType::BOUNCING instead?
                // Consider adding acceleration?
                if (lastDirection) {
                    xVel = currentSpeed;
                }
                else {
                    xVel = -currentSpeed;
                }

                Entity::update_collisions();


                bool reverseDirection = false;
                float tempX = lastDirection ? x + SPRITE_SIZE : x - SPRITE_SIZE;
                for (uint16_t i = 0; i < foreground.size(); i++) {
                    if (foreground[i].y + SPRITE_SIZE > y && foreground[i].y < y + SPRITE_SIZE && (lastDirection ? x + SPRITE_SIZE - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                        // Walked into side of block
                        reverseDirection = true;
                        // Break because we definitely need to change direction, and don't want any other blocks resetting this to false
                        break;
                    }
                    /*if (jumpCooldown == 0 && y + SPRITE_SIZE == foreground[i].y && foreground[i].x + SPRITE_SIZE - 1 > x && foreground[i].x + 1 < x + SPRITE_SIZE) {
                        yVel = -ENTITY_JUMP_SPEED;
                        jumpCooldown = ENTITY_JUMP_COOLDOWN;
                    }*/
                }

                if (jumpCooldown == 0 && is_on_block()) {
                    jump(ENTITY_JUMP_SPEED, ENTITY_JUMP_COOLDOWN);
                }

                if (reverseDirection) {
                    lastDirection = 1 - lastDirection;
                }

                if (health == 1) {
                    // EnemyType::ARMOURED_FLYING has helmet on, and 2 hp
                    enemyType = EnemyType::FLYING;
                    anchorFrame = TILE_ID_ENEMY_1 + (int)enemyType * 4;
                }
            }
            else if (enemyType == EnemyType::SHOOTING) {
                Entity::update_collisions();

                lastDirection = *playerX < x ? 0 : 1;

                if (std::abs(x - *playerX) < SHOOTING_MAX_RANGE_X && std::abs(y - *playerY) < SHOOTING_MAX_RANGE_Y) {
                    state = 1;
                }
                else {
                    state = 0;
                }

                if (state == 1) {
                    if (!reloadTimer && !shotsLeft) {
                        shotsLeft = SHOOTING_ENEMY_CLIP_SIZE;
                    }
                    if (shotsLeft && !rapidfireTimer) {
                        // Fire!
                        // Maybe make these values constants?
                        float magnitude = std::sqrt(std::pow(*playerX - x, 2) + std::pow(*playerY - y, 2));
                        projectiles.push_back(Projectile(x, y, BULLET_PROJECTILE_SPEED * (*playerX - x) / magnitude, BULLET_PROJECTILE_SPEED * (*playerY - y) / magnitude, TILE_ID_ENEMY_PROJECTILE_BULLET, false, SPRITE_QUARTER));
                        shotsLeft--;
                        rapidfireTimer = SHOOTING_RAPID_RELOAD_TIME;

                        if (!shotsLeft) {
                            reloadTimer = SHOOTING_RELOAD_TIME;
                        }

                        audioHandler.play(6);
                    }
                }
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
                    //deathParticles = false;
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
                particles = generate_particles(x + SPRITE_HALF, y + SPRITE_HALF, ENTITY_DEATH_PARTICLE_GRAVITY_X, ENTITY_DEATH_PARTICLE_GRAVITY_Y, enemyDeathParticleColours[enemyType == EnemyType::SHOOTING ? 4 : ((uint8_t)enemyType % 4)], ENTITY_DEATH_PARTICLE_SPEED, ENTITY_DEATH_PARTICLE_COUNT);
                deathParticles = true;
                // Play enemydeath sfx
                audioHandler.play(3);
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

    void set_speed(float speed) {
        currentSpeed = speed;
    }

    uint8_t get_state() {
        return state;
    }

protected:
    enum class EnemyType {
        BASIC, // type 1
        RANGED, // type 2
        PURSUIT, // type 3
        FLYING, // type 4
        ARMOURED, // type 5
        ARMOURED_RANGED, // type 6
        ARMOURED_PURSUIT, // type 7
        ARMOURED_FLYING, // type 8
        SHOOTING // type 9
    } enemyType;

    float reloadTimer;

    // Used for SHOOTING enemy
    float rapidfireTimer;
    uint8_t shotsLeft;

    float currentSpeed;

    uint8_t state;

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


class Boss : public Enemy {
public:
    Boss() : Enemy() {
        spawnX = spawnY = 0;

        anchorFrame = TILE_ID_BOSS_1;

        currentSpeed = BOSS_1_IDLE_SPEED;

        injuredTimer = 0;
        minionsToSpawn = 0;
        dead = false;
        shakeOnLanding = 0;
    }

    Boss(uint16_t xPosition, uint16_t yPosition, uint8_t startHealth, uint8_t type) : Enemy(xPosition, yPosition, startHealth, type) {
        spawnX = xPosition;
        spawnY = yPosition;

        anchorFrame = TILE_ID_BOSS_1 + type * 8;
        if (type == 2) {
            anchorFrame += 16;
        }

        currentSpeed = BOSS_1_IDLE_SPEED;

        injuredTimer = 0;
        minionsToSpawn = 0;
        dead = false;
        shakeOnLanding = 0;
    }

    void update(float dt, ButtonStates buttonStates) {
        if (jumpCooldown) {
            jumpCooldown -= dt;
            if (jumpCooldown < 0) {
                jumpCooldown = 0;
            }
        }

        if (injuredTimer) {
            injuredTimer -= dt;
            if (injuredTimer < 0) {
                injuredTimer = 0;
            }
        }

        if (immuneTimer) {
            immuneTimer -= dt;
            if (immuneTimer < 0) {
                immuneTimer = 0;
            }
        }

        if (reloadTimer) {
            reloadTimer -= dt;
            if (reloadTimer < 0) {
                reloadTimer = 0;
            }
        }

        if (rapidfireTimer) {
            rapidfireTimer -= dt;
            if (rapidfireTimer < 0) {
                rapidfireTimer = 0;
            }
        }

        if (enemyType == EnemyType::BASIC) {
            if (lastDirection) {
                xVel = currentSpeed;
            }
            else {
                xVel = -currentSpeed;
            }

            update_collisions();

            if (state == 0) {
                // IDLE

                if (is_within_range(x, spawnX, BOSS_1_RETURN_TO_SPAWN_RANGE)) {
                    // Wait
                    currentSpeed = 0;

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;
                }
                else {
                    // Return to spawn
                    currentSpeed = BOSS_1_IDLE_SPEED;

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                    bool shouldJump = true;

                    float tempX = lastDirection ? x + SPRITE_SIZE * 2 : x - SPRITE_SIZE * 2;
                    for (uint16_t i = 0; i < foreground.size(); i++) {
                        if (y + SPRITE_SIZE * 2 == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE * 2 - 1) {
                            // About to be on block
                            shouldJump = false;
                            break;
                        }
                    }
                    for (uint16_t i = 0; i < foreground.size(); i++) {
                        if ((lastDirection ? x + SPRITE_SIZE * 2 - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                            // Walked into side of block
                            shouldJump = true;
                            // Break because we definitely need to jump
                            break;
                        }
                    }

                    if (shouldJump && !jumpCooldown) {
                        if (is_on_block()) {
                            shakeOnLanding = BOSS_1_JUMP_SHAKE_TIME;
                            jump(BOSS_1_JUMP_SPEED, BOSS_1_JUMP_COOLDOWN);
                        }
                    }
                }


                // Handle states
                if (is_within_range(x, *playerX, BOSS_1_JUMP_TRIGGER_MAX_RANGE) && is_within_range(y, *playerY, BOSS_1_JUMP_TRIGGER_MAX_RANGE)) {
                    state = 1;
                    bossBattle = true;

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;


                    // JUMP
                    if (is_on_block()) {
                        shakeOnLanding = BOSS_1_ANGRY_JUMP_SHAKE_TIME;
                        jump(BOSS_1_ANGRY_JUMP_SPEED, BOSS_1_JUMP_COOLDOWN);
                    }
                }
            }
            else if (state == 1) {
                // PURSUE

                // Only go fast once on ground
                for (uint16_t i = 0; i < foreground.size(); i++) {
                    if (y + SPRITE_SIZE * 2 == foreground[i].y && foreground[i].x + SPRITE_SIZE - 1 > x && foreground[i].x + 1 < x + SPRITE_SIZE * 2) {
                        // On block
                        currentSpeed = BOSS_1_PURSUIT_SPEED;
                    }
                }

                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                bool shouldJump = true;

                float tempX = lastDirection ? x + SPRITE_SIZE * 2 : x - SPRITE_SIZE * 2;
                for (uint16_t i = 0; i < foreground.size(); i++) {
                    if (y + SPRITE_SIZE * 2 == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE * 2 - 1) {
                        // About to be on block
                        shouldJump = false;
                        break;
                    }
                }
                for (uint16_t i = 0; i < foreground.size(); i++) {
                    if ((lastDirection ? x + SPRITE_SIZE * 2 - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                        // Walked into side of block
                        shouldJump = true;
                        // Break because we definitely need to jump
                        break;
                    }
                }


                if (shouldJump && !jumpCooldown) {
                    if (is_on_block()) {
                        shakeOnLanding = BOSS_1_JUMP_SHAKE_TIME;
                        jump(BOSS_1_JUMP_SPEED, BOSS_1_JUMP_COOLDOWN);
                    }
                }



                // Handle states
                if (!is_within_range(x, *playerX, BOSS_1_IGNORE_MIN_RANGE) || !is_within_range(y, *playerY, BOSS_1_IGNORE_MIN_RANGE)) {
                    state = 0;
                }
                else if (is_immune()) {
                    state = 2;
                    // slow down player
                    slowPlayer = true;
                }
            }
            else if (state == 2) {
                // IMMUNE

                currentSpeed = BOSS_1_ANGRY_SPEED - BOSS_1_SPEED_REDUCTION_SCALE * get_abs_center_range(x, *playerX);

                //printf("Immune state (2), current speed %f\n", currentSpeed);

                // Head away from player
                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 1 : 0;

                bool hitWall = false;
                for (uint16_t i = 0; i < foreground.size(); i++) {
                    if (foreground[i].y + SPRITE_SIZE > y && foreground[i].y < y + SPRITE_SIZE * 2 && (lastDirection ? x + SPRITE_SIZE * 2 - 1 : x - SPRITE_SIZE + 1) == foreground[i].x) {
                        // Walked into side of block
                        hitWall = true;
                        // Break because we definitely have hit wall
                        break;
                    }
                }

                //bool atEdge = true;
                //float tempX = lastDirection ? x + SPRITE_SIZE * 2 : x - SPRITE_SIZE * 2;
                //for (uint16_t i = 0; i < foreground.size(); i++) {
                //    if (y + SPRITE_SIZE * 2 == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE * 2 - 1) {
                //        // About to be on block
                //        atEdge = false;
                //        break;
                //    }
                //}

                // Handle states
                if (hitWall || /*atEdge ||*/ !is_within_range(x, *playerX, health == 0 ? BOSS_1_DEATH_MAX_RANGE : BOSS_1_INJURED_MAX_RANGE) || !is_within_range(y, *playerY, health == 0 ? BOSS_1_DEATH_MAX_RANGE : BOSS_1_INJURED_MAX_RANGE)) {
                    // NOTE: bug if you kill boss right on edge of platform or other times?
                    state = 3;
                    minionsToSpawn = 3 - health;
                    jumpCooldown = BOSS_1_MINION_SPAWN_COOLDOWN; // delay minion spawning
                }
            }
            else if (state == 3) {
                // SPAWN MINIONS

                currentSpeed = 0;//BOSS_IDLE_SPEED;

                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                // Jump whenever spawn minion
                if (!jumpCooldown && minionsToSpawn) {
                    if (is_on_block()) {
                        shakeOnLanding = BOSS_1_ANGRY_JUMP_SHAKE_TIME;

                        // Spawn minion
                        enemies.push_back(Enemy(x + SPRITE_SIZE, y + SPRITE_SIZE, enemyHealths[(uint8_t)enemyType], (uint8_t)enemyType));
                        enemies[enemies.size() - 1].lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;
                        enemies[enemies.size() - 1].set_speed(BOSS_1_MINION_SPEED - BOSS_1_MINION_SPEED_REDUCTION * (2 - health));
                        minionsToSpawn--;

                        jump(BOSS_1_ANGRY_JUMP_SPEED, BOSS_1_MINION_SPAWN_COOLDOWN);
                    }
                }

                // Handle states
                if (!jumpCooldown && !minionsToSpawn) {
                    if (health > 0) {
                        // Not dead
                        state = 1;
                        immuneTimer = 0;

                        // Unslow player
                        slowPlayer = false;
                    }
                    else {
                        // Dead
                        // Generate particles
                        particles = generate_particles(x + SPRITE_SIZE, y + SPRITE_SIZE, BOSS_DEATH_PARTICLE_GRAVITY_X, BOSS_DEATH_PARTICLE_GRAVITY_Y, bossDeathParticleColours[(uint8_t)enemyType], BOSS_DEATH_PARTICLE_SPEED, BOSS_DEATH_PARTICLE_COUNT);
                        deathParticles = true;
                        state = 4;
                        dead = true;
                    }
                }
            }
            else if (state == 4) {
                // Dead, displaying particles

                if (deathParticles) {
                    if (particles.size() == 0) {
                        // No particles left
                        deathParticles = false;

                        // Unslow player
                        slowPlayer = false;
                    }
                    else {
                        for (uint8_t i = 0; i < particles.size(); i++) {
                            particles[i].update(dt);
                        }

                        // Remove any particles which are too old
                        particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle particle) { return (particle.age >= BOSS_DEATH_PARTICLE_AGE); }), particles.end());
                    }
                }
            }
        }
        else if (enemyType == EnemyType::RANGED) {
            update_collisions();

            if (state == 0) {
                // IDLE
                currentSpeed = 0;

                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                // Handle states
                if (is_within_range(x, *playerX, BOSS_2_JUMP_TRIGGER_MAX_RANGE) && is_within_range(y, *playerY, BOSS_2_JUMP_TRIGGER_MAX_RANGE)) {
                    state = 1;
                    bossBattle = true;

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                    // JUMP
                    if (is_on_block()) {
                        shakeOnLanding = BOSS_2_ANGRY_JUMP_SHAKE_TIME;
                        jump(BOSS_2_ANGRY_JUMP_SPEED, BOSS_2_JUMP_COOLDOWN);
                    }
                }
            }
            else if (state == 1) {
                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                if (!reloadTimer) {
                    // Fire!
                    float xV = (*playerX - x) / BOSS_2_PROJECTILE_FLIGHT_TIME;
                    // yVel is broken
                    float yV = ((*playerY - y) / BOSS_2_PROJECTILE_FLIGHT_TIME) - 0.5f * PROJECTILE_GRAVITY * BOSS_2_PROJECTILE_FLIGHT_TIME;

                    //x,y should be offset to center
                    projectiles.push_back(Projectile(x + SPRITE_SIZE, y + SPRITE_SIZE, xV, yV, currentWorldNumber == SNOW_WORLD || currentLevelNumber == 8 ? TILE_ID_BOSS_PROJECTILE_SNOWBALL : TILE_ID_BOSS_PROJECTILE_ROCK, true, SPRITE_SIZE));
                    reloadTimer = BOSS_2_RELOAD_TIME;

                    audioHandler.play(6);
                }

                // Handle states
                if (!is_within_range(x, *playerX, BOSS_2_IGNORE_MIN_RANGE) || !is_within_range(y, *playerY, BOSS_2_IGNORE_MIN_RANGE)) {
                    state = 0;
                }
                else if (is_immune()) {
                    if (health == 0) {
                        state = 3;

                        // JUMP
                        if (is_on_block()) {
                            shakeOnLanding = BOSS_2_ANGRY_JUMP_SHAKE_TIME;
                            jump(BOSS_2_ANGRY_JUMP_SPEED, BOSS_2_JUMP_COOLDOWN);
                        }

                        shotsLeft = BOSS_2_RAPID_SHOT_COUNT * 3;

                        // Make player drop through floor
                        dropPlayer = true;
                    }
                    else {
                        state = 2;

                        // JUMP
                        if (is_on_block()) {
                            shakeOnLanding = BOSS_2_ANGRY_JUMP_SHAKE_TIME;
                            jump(BOSS_2_ANGRY_JUMP_SPEED, BOSS_2_JUMP_COOLDOWN);
                        }

                        shotsLeft = BOSS_2_RAPID_SHOT_COUNT + (3 - health);

                        // Make player drop through floor
                        dropPlayer = true;
                    }
                }
            }
            else if (state == 2) {
                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                if (is_on_block()) {
                    if (shotsLeft && !reloadTimer) {
                        // Fire!
                        float xV = ((*playerX - x) / BOSS_2_RAPID_PROJECTILE_FLIGHT_TIME) * (1.15f - shotsLeft / 20);
                        // yVel is broken
                        float yV = ((*playerY - y) / BOSS_2_RAPID_PROJECTILE_FLIGHT_TIME) - 0.5f * PROJECTILE_GRAVITY * BOSS_2_RAPID_PROJECTILE_FLIGHT_TIME;

                        //x,y should be offset to center
                        projectiles.push_back(Projectile(x + SPRITE_SIZE, y + SPRITE_SIZE, xV, yV, currentWorldNumber == SNOW_WORLD || currentLevelNumber == 8 ? TILE_ID_BOSS_PROJECTILE_SNOWBALL : TILE_ID_BOSS_PROJECTILE_ROCK, true, SPRITE_SIZE));
                        reloadTimer = BOSS_2_RAPID_RELOAD_TIME;

                        audioHandler.play(6);

                        shotsLeft--;
                    }
                }
                
                if (!shotsLeft && !reloadTimer) {
                    // Not dead
                    state = 0;
                    immuneTimer = 0;
                    reloadTimer = BOSS_2_RESET_COOLDOWN;

                    // Unslow player
                    slowPlayer = false;
                }
            }
            else if (state == 3) {
                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;

                if (is_on_block()) {
                    if (shotsLeft && !reloadTimer) {
                        // Fire!
                        float tX = x - (SPRITE_SIZE * BOSS_2_RAPID_SHOT_COUNT * 3) + SPRITE_SIZE * shotsLeft * 2;
                        float xV = ((tX - x) / BOSS_2_SUPER_RAPID_PROJECTILE_FLIGHT_TIME);
                        // yVel is broken
                        float yV = ((*playerY - y) / BOSS_2_SUPER_RAPID_PROJECTILE_FLIGHT_TIME) - 0.5f * PROJECTILE_GRAVITY * BOSS_2_SUPER_RAPID_PROJECTILE_FLIGHT_TIME;

                        //x,y should be offset to center
                        projectiles.push_back(Projectile(x, y, xV, yV, currentWorldNumber == SNOW_WORLD || currentLevelNumber == 8 ? TILE_ID_BOSS_PROJECTILE_SNOWBALL : TILE_ID_BOSS_PROJECTILE_ROCK, true, SPRITE_SIZE));
                        reloadTimer = BOSS_2_SUPER_RAPID_RELOAD_TIME;

                        audioHandler.play(6);

                        shotsLeft--;
                    }
                }

                if (!shotsLeft && !reloadTimer) {
                    // Dead
                    // Generate particles
                    particles = generate_particles(x + SPRITE_SIZE, y + SPRITE_SIZE, BOSS_DEATH_PARTICLE_GRAVITY_X, BOSS_DEATH_PARTICLE_GRAVITY_Y, bossDeathParticleColours[(uint8_t)enemyType], BOSS_DEATH_PARTICLE_SPEED, BOSS_DEATH_PARTICLE_COUNT);
                    deathParticles = true;
                    state = 4;
                    dead = true;
                }
            }
            else if (state == 4) {
                // Dead, displaying particles

                if (deathParticles) {
                    if (particles.size() == 0) {
                        // No particles left
                        deathParticles = false;

                        // Unslow player
                        slowPlayer = false;
                    }
                    else {
                        for (uint8_t i = 0; i < particles.size(); i++) {
                            particles[i].update(dt);
                        }

                        // Remove any particles which are too old
                        particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle particle) { return (particle.age >= BOSS_DEATH_PARTICLE_AGE); }), particles.end());
                    }
                }
            }
        }
        else if (enemyType == EnemyType::PURSUIT) {
            // Use persuit tag for giant boss, even though it's more like a BASIC v2.0
            if (lastDirection) {
                xVel = currentSpeed;
            }
            else {
                xVel = -currentSpeed;
            }

            update_collisions();

            if (state == 0) {
                // IDLE

                if (is_within_range(x, spawnX, BIG_BOSS_RETURN_TO_SPAWN_RANGE)) {
                    // Wait
                    currentSpeed = 0;

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE * 2 ? 0 : 1;
                }
                else {
                    // Return to spawn
                    currentSpeed = BIG_BOSS_IDLE_SPEED;

                    lastDirection = spawnX < x + SPRITE_SIZE * 2 ? 0 : 1;
                }


                // Handle states
                if (is_within_range(x, *playerX, BIG_BOSS_JUMP_TRIGGER_MAX_RANGE) && is_within_range(y, *playerY, BIG_BOSS_JUMP_TRIGGER_MAX_RANGE)) {
                    state = 1;
                    bossBattle = true;

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE * 2 ? 0 : 1;

                    shotsLeft = 0;


                    // JUMP
                    if (is_on_block()) {
                        shakeOnLanding = BIG_BOSS_ANGRY_JUMP_SHAKE_TIME;
                        jump(BIG_BOSS_ANGRY_JUMP_SPEED, BIG_BOSS_JUMP_COOLDOWN);
                    }
                }
            }
            else if (state == 1) {
                // PURSUE

                if ((*playerY < y - SPRITE_SIZE * 4 && std::abs((*playerX + SPRITE_HALF) - (x + SPRITE_SIZE * 2)) < SPRITE_SIZE * 6) || std::abs((*playerX + SPRITE_HALF) - (x + SPRITE_SIZE * 2)) > SPRITE_SIZE * 9) {
                    // Player is a bit above boss, FIRE!
                    currentSpeed = 0;

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE * 2 ? 0 : 1;

                    if (!reloadTimer && !shotsLeft) {
                        shotsLeft = 3;
                    }

                    if (is_on_block()) {
                        if (!rapidfireTimer && shotsLeft) {
                            // Fire!
                            float xV = ((*playerX - x - SPRITE_HALF * 3) / BIG_BOSS_PROJECTILE_FLIGHT_TIME) * (1.1f - shotsLeft / 20);
                            // yVel is broken
                            float yV = ((*playerY - y - SPRITE_HALF * 3) / BIG_BOSS_PROJECTILE_FLIGHT_TIME) - 0.5f * PROJECTILE_GRAVITY * BIG_BOSS_PROJECTILE_FLIGHT_TIME;

                            // x,y are offset to center
                            projectiles.push_back(Projectile(x + SPRITE_SIZE * 2, y + SPRITE_SIZE, xV, yV, currentWorldNumber == SNOW_WORLD || currentLevelNumber == 8 ? TILE_ID_BOSS_PROJECTILE_SNOWBALL : TILE_ID_BOSS_PROJECTILE_ROCK, true, SPRITE_SIZE));
                            
                            rapidfireTimer = BIG_BOSS_RAPID_RELOAD_TIME;
                            shotsLeft--;

                            audioHandler.play(6);
                        }
                    }

                    if (!shotsLeft && !reloadTimer) {
                        reloadTimer = BIG_BOSS_RELOAD_TIME;
                    }
                }
                else if (std::abs((*playerX + SPRITE_HALF) - (x + SPRITE_SIZE * 2)) > SPRITE_SIZE * 4 && !reloadTimer) {
                    // Only go fast once on ground
                    if (is_on_block()) {
                        currentSpeed = BIG_BOSS_PURSUIT_SPEED;
                    }

                    lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE * 2 ? 0 : 1;

                    bool shouldStop = true;

                    float tempX = lastDirection ? x + SPRITE_SIZE * 2 : x - SPRITE_SIZE * 2;
                    for (uint16_t i = 0; i < foreground.size(); i++) {
                        if (y + SPRITE_SIZE * 4 == foreground[i].y && foreground[i].x + SPRITE_SIZE > tempX + 1 && foreground[i].x < tempX + SPRITE_SIZE * 4 - 1) {
                            // About to be on block
                            shouldStop = false;
                            break;
                        }
                    }
                    if (shouldStop) {
                        currentSpeed = 0;
                    }
                }
                else if (std::abs((*playerX + SPRITE_HALF) - (x + SPRITE_SIZE * 2)) < SPRITE_SIZE * 5 && *playerY > y) {
                    // Only go fast once on ground
                    if (is_on_block()) {
                        currentSpeed = BIG_BOSS_PURSUIT_SPEED;

                        if (!jumpCooldown) {
                            shakeOnLanding = BIG_BOSS_JUMP_SHAKE_TIME;
                            jump(BIG_BOSS_JUMP_SPEED, BIG_BOSS_JUMP_COOLDOWN);
                        }
                    }
                }
                else {
                    currentSpeed = 0;
                }

                // Handle states
                if (!is_within_range(x, *playerX, BIG_BOSS_IGNORE_MIN_RANGE) || !is_within_range(y, *playerY, BIG_BOSS_IGNORE_MIN_RANGE)) {
                    state = 0;
                }
                else if (is_immune()) {
                    state = 2;
                    // slow down player
                    //slowPlayer = true;
                }
            }
            else if (state == 2) {
                // SELF DEFENCE

                // Push player away, in direction of spawn (i.e. furthest distance)

                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE * 2 ? 1 : 0;

                repelPlayer = true;

                currentSpeed = BIG_BOSS_RETREAT_SPEED;

                // Handle states
                if (!is_within_range(x, *playerX, BIG_BOSS_INJURED_MAX_RANGE) || !is_within_range(y, *playerY, BIG_BOSS_INJURED_MAX_RANGE)) {
                    state = 3;
                    minionsToSpawn = 1;
                    repelPlayer = false;
                }

            }
            else if (state == 3) {
                // SPAWN MINIONS

                currentSpeed = 0;

                lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE * 2 ? 0 : 1;

                // Jump whenever spawn minion
                if (!jumpCooldown && minionsToSpawn) {
                    if (is_on_block()) {
                        shakeOnLanding = BIG_BOSS_ANGRY_JUMP_SHAKE_TIME;

                        // Spawn minion
                        enemies.push_back(Enemy(x + SPRITE_SIZE * 2, y + SPRITE_SIZE * 2, enemyHealths[bigBossMinions[health]], bigBossMinions[health]));
                        enemies[enemies.size() - 1].lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE * 2 ? 0 : 1;
                        enemies[enemies.size() - 1].set_player_position(playerX, playerY);
                        //enemies[enemies.size() - 1].set_speed(BIG_BOSS_MINION_SPEED - BIG_BOSS_MINION_SPEED_REDUCTION * (2 - health));
                        minionsToSpawn--;

                        jump(BIG_BOSS_ANGRY_JUMP_SPEED, BIG_BOSS_MINION_SPAWN_COOLDOWN);
                    }
                }

                // Handle states
                if (!jumpCooldown && !minionsToSpawn) {
                    if (health > 0) {
                        // Not dead
                        state = 1;
                        immuneTimer = 0;

                        // Unslow player
                        slowPlayer = false;
                    }
                    else {
                        // Dead
                        // Generate particles
                        particles = generate_particles(x + SPRITE_SIZE * 2, y + SPRITE_SIZE * 2, BOSS_DEATH_PARTICLE_GRAVITY_X, BOSS_DEATH_PARTICLE_GRAVITY_Y, bossDeathParticleColours[(uint8_t)enemyType], BOSS_DEATH_PARTICLE_SPEED, BOSS_DEATH_PARTICLE_COUNT);
                        deathParticles = true;
                        state = 4;
                        dead = true;
                    }
                }
            }
            else if (state == 4) {
                // Dead, displaying particles

                if (deathParticles) {
                    if (particles.size() == 0) {
                        // No particles left
                        deathParticles = false;

                        // Unslow player
                        slowPlayer = false;
                    }
                    else {
                        for (uint8_t i = 0; i < particles.size(); i++) {
                            particles[i].update(dt);
                        }

                        // Remove any particles which are too old
                        particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle particle) { return (particle.age >= BOSS_DEATH_PARTICLE_AGE); }), particles.end());
                    }
                }
            }
        }

        if (y > levelDeathBoundary) {
            health = 0;
            xVel = yVel = 0;
        }
    }

    bool is_on_block() {
        if (is_big()) {
            // Allow boss to jump on tiles
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (y + SPRITE_SIZE * 4 == foreground[i].y && foreground[i].x + SPRITE_SIZE - 1 > x && foreground[i].x + 1 < x + SPRITE_SIZE * 4) {
                    // On top of block
                    return true;
                }
            }

            // Allow boss to jump on platforms
            for (uint16_t i = 0; i < platforms.size(); i++) {
                if (y + SPRITE_SIZE * 4 == platforms[i].y && platforms[i].x + SPRITE_SIZE - 1 > x && platforms[i].x + 1 < x + SPRITE_SIZE * 4) {
                    // On top of block
                    return true;
                }
            }
        }
        else {
            // Allow boss to jump on tiles
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (y + SPRITE_SIZE * 2 == foreground[i].y && foreground[i].x + SPRITE_SIZE - 1 > x && foreground[i].x + 1 < x + SPRITE_SIZE * 2) {
                    // On top of block
                    return true;
                }
            }

            // Allow boss to jump on platforms
            for (uint16_t i = 0; i < platforms.size(); i++) {
                if (y + SPRITE_SIZE * 2 == platforms[i].y && platforms[i].x + SPRITE_SIZE - 1 > x && platforms[i].x + 1 < x + SPRITE_SIZE * 2) {
                    // On top of block
                    return true;
                }
            }
        }

        // Boss didn't jump
        return false;

    }

    void set_immune() {
        immuneTimer = BOSS_IMMUNE_TIME;
        lastDirection = *playerX + SPRITE_HALF < x + SPRITE_SIZE ? 0 : 1;
    }

    bool is_immune() {
        if (enemyType == EnemyType::BASIC) {
            return Entity::is_immune() || state >= 2;
        }
        else if (enemyType == EnemyType::RANGED) {
            return Entity::is_immune() || state >= 2;
        }
        else if (enemyType == EnemyType::PURSUIT) {
            return Entity::is_immune() || state >= 2;
        }
        else {
            // Catch-all
            return false;
        }
    }

    void set_injured() {
        injuredTimer = BOSS_INJURED_TIME;
        set_immune();
    }

    uint8_t get_size() {
        if (is_big()) {
            return SPRITE_SIZE * 4;
        }
        else {
            return SPRITE_SIZE * 2;
        }
    }

    bool is_big() {
        return enemyType == EnemyType::PURSUIT;
    }

    bool is_within_range(float me, float them, float range) {
        //return get_range(me, them) < range;
        return get_abs_center_range(me, them) < range;
    }

    //float get_range(float me, float them) {
    //    if (them > me) {
    //        // They are right
    //        return them - (me + SPRITE_SIZE * 2);
    //    }
    //    else {
    //        // They are left
    //        return me - (them + SPRITE_SIZE);
    //    }
    //}

    float get_center_range(float me, float them) {
        return (me + SPRITE_SIZE) - (them + SPRITE_HALF);
    }

    float get_abs_center_range(float me, float them) {
        return std::abs(get_center_range(me, them));
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
                        y = foreground[i].y - SPRITE_SIZE * (is_big() ? 4 : 2);
                        if (shakeOnLanding) {
                            shaker.set_shake(shakeOnLanding);
                            shakeOnLanding = 0;
                        }
                    }
                    else if (yVel < 0) {
                        // Collided from bottom
                        y = foreground[i].y + SPRITE_SIZE;
                    }
                    yVel = 0;
                }
            }

            // Platforms may need work
            if (!is_big()) {
                for (uint16_t i = 0; i < platforms.size(); i++) {
                    if (colliding(platforms[i])) {
                        if (yVel > 0 && y + SPRITE_SIZE * 2 < platforms[i].y + SPRITE_QUARTER) {
                            // Collided from top
                            y = platforms[i].y - SPRITE_SIZE * 2;
                            if (shakeOnLanding) {
                                shaker.set_shake(shakeOnLanding);
                                shakeOnLanding = 0;
                            }
                            yVel = 0;
                        }
                    }
                }
            }

            // Move entity x
            x += xVel * dt;

            // Here check collisions...
            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground[i])) {
                    if (xVel > 0) {
                        // Collided from left
                        x = foreground[i].x - SPRITE_SIZE * (is_big() ? 4 : 2) + 1;
                    }
                    else if (xVel < 0) {
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
        if (!dead) {
            if (is_big()) {
                uint16_t frame = anchorFrame;

                //if (true) { //state != 0 //health < 3
                //    frame = anchorFrame + 8 + 16 * 4;
                //}

                if (yVel < -50) {
                    frame = anchorFrame + 4;
                }
                else if (yVel > 160) {
                    frame = anchorFrame + 8;
                }

                if (injuredTimer) {
                    frame = anchorFrame + 12;
                }

                if (lastDirection == 1) {
                    for (uint8_t i = 0; i < 4; i++) {
                        for (uint8_t j = 0; j < 4; j++) {
                            render_sprite(frame + i + j * 16, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_SIZE * (3 - i), SCREEN_MID_HEIGHT + y - camera.y + SPRITE_SIZE * j), SpriteTransform::HORIZONTAL);
                        }
                    }
                }
                else {
                    for (uint8_t i = 0; i < 4; i++) {
                        for (uint8_t j = 0; j < 4; j++) {
                            render_sprite(frame + i + j * 16, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_SIZE * i, SCREEN_MID_HEIGHT + y - camera.y + SPRITE_SIZE * j));
                        }
                    }
                }
            }
            else {
                uint16_t frame = anchorFrame;

                if (yVel < -50) {
                    frame = anchorFrame + 2;
                }
                else if (yVel > 160) {
                    frame = anchorFrame + 4;
                }

                if (injuredTimer) {
                    frame = anchorFrame + 6;
                }

                if (lastDirection == 1) {
                    //screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                    render_sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_SIZE, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                    render_sprite(frame + 1, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                    render_sprite(frame + 16, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_SIZE, SCREEN_MID_HEIGHT + y - camera.y + SPRITE_SIZE), SpriteTransform::HORIZONTAL);
                    render_sprite(frame + 17, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y + SPRITE_SIZE), SpriteTransform::HORIZONTAL);
                }
                else {
                    //screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
                    render_sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
                    render_sprite(frame + 1, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_SIZE, SCREEN_MID_HEIGHT + y - camera.y));
                    render_sprite(frame + 16, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y + SPRITE_SIZE));
                    render_sprite(frame + 17, Point(SCREEN_MID_WIDTH + x - camera.x + SPRITE_SIZE, SCREEN_MID_HEIGHT + y - camera.y + SPRITE_SIZE));
                }
            }
        }

        // Particles
        for (uint8_t i = 0; i < particles.size(); i++) {
            particles[i].render(camera);
        }
    }

    bool colliding(Tile tile) {
        if (is_big()) {
            return (tile.x + SPRITE_SIZE > x + 1 && tile.x < x + SPRITE_SIZE * 4 - 1 && tile.y + SPRITE_SIZE > y && tile.y < y + SPRITE_SIZE * 4);
        }
        else {
            return (tile.x + SPRITE_SIZE > x + 1 && tile.x < x + SPRITE_SIZE * 2 - 1 && tile.y + SPRITE_SIZE > y && tile.y < y + SPRITE_SIZE * 2);
        }
    }

    bool spawning_minions() {
        return minionsToSpawn;
    }

    bool is_dead() {
        return dead;
    }

    bool particles_left() {
        return deathParticles;
    }

    void reset() {
        dead = false;
        particles.clear();
        shakeOnLanding = false;
        x = spawnX;
        y = spawnY;
        injuredTimer = 0;
        minionsToSpawn = 0;
        state = 0;
        health = bossHealths[(uint8_t)enemyType];
    }

protected:
    float injuredTimer;
    uint8_t minionsToSpawn;

    uint16_t spawnX, spawnY;
    bool dead;
    float shakeOnLanding;
};
std::vector<Boss> bosses; // used for levels where there is a boss.

void reset_bosses() {
    for (uint8_t i = 0; i < bosses.size(); i++) {
        bosses[i].reset();
    }
}

class Player : public Entity {
public:
    uint8_t score;
    uint8_t enemiesKilled;
    float levelTimer;
    uint8_t id;
    uint8_t lives;

    Player() : Entity() {
        score = 0;
        enemiesKilled = 0;
        levelTimer = 0.0f;

        id = 0;

        lives = PLAYER_START_LIVES;

        slowPlayerParticleTimer = 0.0f;
        airTime = 0.0f;
    }

    Player(uint16_t xPosition, uint16_t yPosition, uint8_t colour) : Entity(xPosition, yPosition, TILE_ID_PLAYER_1 + colour * 4, PLAYER_MAX_HEALTH) {
        score = 0;
        enemiesKilled = 0;
        levelTimer = 0.0f;

        id = colour;

        lives = PLAYER_START_LIVES;

        slowPlayerParticleTimer = 0.0f;
        airTime = 0.0f;
    }

    void update(float dt, ButtonStates buttonStates) {
        if (immuneTimer) {
            immuneTimer -= dt;
            if (immuneTimer < 0) {
                immuneTimer = 0;
            }
        }

        if (jumpCooldown) {
            jumpCooldown -= dt;
            if (jumpCooldown < 0) {
                jumpCooldown = 0;
            }
        }

        if (health > 0) {

#ifdef TESTING_MODE
            health = 3;

            if (buttonStates.X) {
                x = finish.x - SPRITE_SIZE * 2;
                y = finish.y;
            }
#endif // TESTING_MODE

            if (!locked) {
                levelTimer += dt;

                if (is_on_block()) {
                    airTime = 0.0f;
                }
                else {
                    airTime += dt;
                }

                if (buttonStates.A && jumpCooldown == 0 && airTime < PLAYER_JUMP_MIN_AIR_TIME) {
                    jump(slowPlayer ? PLAYER_SLOW_MAX_JUMP : PLAYER_MAX_JUMP, slowPlayer ? PLAYER_SLOW_JUMP_COOLDOWN : PLAYER_JUMP_COOLDOWN);
                    if (gameState != GameState::STATE_CHARACTER_SELECT) {
                        // Play jump sfx
                        audioHandler.play(1);
                    }
                }
                else if (!buttonStates.A && yVel < 0) {
                    yVel *= PLAYER_JUMP_EXTRA_DOWNSCALE;
                }

                if (buttonStates.LEFT) {
                    if (slowPlayer) {
                        xVel -= PLAYER_SLOW_ACCELERATION * dt;
                        if (xVel < -PLAYER_SLOW_MAX_SPEED) {
                            xVel = -PLAYER_SLOW_MAX_SPEED;
                        }
                    }
                    else {
                        xVel -= PLAYER_ACCELERATION * dt;
                        if (xVel < -PLAYER_MAX_SPEED) {
                            xVel = -PLAYER_MAX_SPEED;
                        }
                    }
                }
                else if (buttonStates.RIGHT) {
                    if (slowPlayer) {
                        xVel += PLAYER_SLOW_ACCELERATION * dt;
                        if (xVel > PLAYER_SLOW_MAX_SPEED) {
                            xVel = PLAYER_SLOW_MAX_SPEED;
                        }
                    }
                    else {
                        xVel += PLAYER_ACCELERATION * dt;
                        if (xVel > PLAYER_MAX_SPEED) {
                            xVel = PLAYER_MAX_SPEED;
                        }
                    }
                }
                else {
                    if (xVel > 0) {
                        if (slowPlayer) {
                            xVel -= PLAYER_SLOW_DECELERATION * dt;
                        }
                        else {
                            xVel -= PLAYER_DECELERATION * dt;
                        }

                        if (xVel < 0) {
                            xVel = 0;
                        }
                    }
                    else if (xVel < 0) {
                        if (slowPlayer) {
                            xVel += PLAYER_SLOW_DECELERATION * dt;
                        }
                        else {
                            xVel += PLAYER_DECELERATION * dt;
                        }

                        if (xVel > 0) {
                            xVel = 0;
                        }
                    }
                }
            }

            uint8_t coinCount = coins.size();

            // Remove coins if player jumps on them
            coins.erase(std::remove_if(coins.begin(), coins.end(), [this](Coin coin) { return (coin.x + SPRITE_SIZE > x && coin.x < x + SPRITE_SIZE && coin.y + SPRITE_SIZE > y && coin.y < y + SPRITE_SIZE); }), coins.end());

            // Add points to player score (1 point per coin which has been deleted)
            score += coinCount - coins.size();

            if (coinCount != coins.size()) {
                // Must have picked up a coin
                // Play coin sfx
                audioHandler.play(coinSfxAlternator ? 0 : 2);
                coinSfxAlternator = !coinSfxAlternator;
            }


            uint8_t enemyCount = enemies.size();// + bosses.size();

            // Remove enemies if no health left
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy enemy) { return (enemy.health == 0 && enemy.particles.size() == 0); }), enemies.end());
            bosses.erase(std::remove_if(bosses.begin(), bosses.end(), [](Boss boss) { return (boss.is_dead() && !boss.particles_left()); }), bosses.end());

            enemiesKilled += enemyCount - enemies.size();// - bosses.size();

            update_collisions();


            if (!is_immune()) {
                for (uint8_t i = 0; i < spikes.size(); i++) {
                    if (colliding(spikes[i])) {
                        if ((spikes[i].get_id() == TILE_ID_SPIKE_BOTTOM && y + SPRITE_SIZE >= spikes[i].y + SPRITE_HALF) ||
                            (spikes[i].get_id() == TILE_ID_SPIKE_TOP && y <= spikes[i].y + SPRITE_HALF) ||
                            (spikes[i].get_id() == TILE_ID_SPIKE_LEFT && x <= spikes[i].x + SPRITE_HALF) ||
                            (spikes[i].get_id() == TILE_ID_SPIKE_RIGHT && x + SPRITE_SIZE >= spikes[i].x + SPRITE_HALF)) {

                            health -= 1;
                            set_immune();
                            break;
                        }
                    }
                }
            }

            if (y > levelDeathBoundary) {
                health = 0;
                xVel = yVel = 0;
            }
        }

        if (slowPlayer || repelPlayer) {
            slowPlayerParticleTimer += dt;
            if (slowPlayerParticleTimer >= PLAYER_SLOW_PARTICLE_SPAWN_DELAY) {
                slowPlayerParticleTimer -= PLAYER_SLOW_PARTICLE_SPAWN_DELAY;
                slowParticles.push_back(generate_brownian_particle(x + SPRITE_HALF, y + SPRITE_HALF, PLAYER_SLOW_PARTICLE_GRAVITY_X, PLAYER_SLOW_PARTICLE_GRAVITY_Y, PLAYER_SLOW_PARTICLE_SPEED, repelPlayer ? repelPlayerParticleColours : slowPlayerParticleColours, PLAYER_SLOW_PARTICLE_WIGGLE));
            }
        }
        else {
            slowPlayerParticleTimer = 0.0f;
        }

        for (uint16_t i = 0; i < slowParticles.size(); i++) {
            slowParticles[i].update(dt);
        }
        // Remove any particles which are too old
        slowParticles.erase(std::remove_if(slowParticles.begin(), slowParticles.end(), [](BrownianParticle particle) { return (particle.age >= PLAYER_SLOW_PARTICLE_AGE); }), slowParticles.end());



        if (health == 0) {
            //state = DEAD;

            if (deathParticles) {
                if (particles.size() == 0) {
                    // No particles left, reset values which need to be

                    deathParticles = false;

                    // If player has lives left, respawn
                    if (lives) {
                        // Reset player position and health, maybe remove all this?
                        health = 3;
                        xVel = yVel = 0;
                        lastDirection = 1;
                        x = playerStartX;
                        y = playerStartY;

                        // Stop player from moving while respawning
                        cameraRespawn = true;
                        locked = true;
                        slowPlayer = false;
                        dropPlayer = false;
                        repelPlayer = false;
                        bossBattle = false;

                        reset_bosses();

                        // Make player immune when respawning?
                        //set_immune();

                        // Remove immunity when respawning
                        immuneTimer = 0;
                    }
                }
                else {
                    for (uint8_t i = 0; i < particles.size(); i++) {
                        particles[i].update(dt);
                    }

                    // Remove any particles which are too old
                    particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle particle) { return (particle.age >= ENTITY_DEATH_PARTICLE_AGE); }), particles.end());
                }
            }
            else if (lives) {
                // Generate particles
                particles = generate_particles(x + SPRITE_HALF, y + SPRITE_HALF, ENTITY_DEATH_PARTICLE_GRAVITY_X, ENTITY_DEATH_PARTICLE_GRAVITY_Y, playerDeathParticleColours[id], ENTITY_DEATH_PARTICLE_SPEED, ENTITY_DEATH_PARTICLE_COUNT);
                deathParticles = true;

                // Reduce player lives by one
                lives--;

                audioHandler.play(5);
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

            // Enemies first
            for (uint16_t i = 0; i < enemies.size(); i++) {
                if (enemies[i].health && colliding(enemies[i])) {
                    if (y + SPRITE_SIZE < enemies[i].y + SPRITE_QUARTER) {
                        // Collided from top
                        y = enemies[i].y - SPRITE_SIZE;

                        if (yVel > 0 || enemies[i].yVel < 0) { // && !enemies[i].is_immune()
                            //yVel = -PLAYER_ATTACK_JUMP;
                            yVel = -std::max(yVel * PLAYER_ATTACK_JUMP_SCALE, PLAYER_ATTACK_JUMP_MIN);

                            // Take health off enemy
                            enemies[i].health--;

                            // Play enemy injured sfx
                            audioHandler.play(4);

                            if (enemies[i].yVel < 0) {
                                // Enemy is jumping
                                // Stop enemy's jump
                                enemies[i].yVel = 0;
                            }
                        }
                    }
                }
            }

            if (!dropPlayer) {
                for (uint16_t i = 0; i < bosses.size(); i++) {
                    if (!bosses[i].is_dead() && colliding(bosses[i])) {
                        if (y + SPRITE_SIZE < bosses[i].y + SPRITE_QUARTER) {
                            // Collided from top
                            y = bosses[i].y - SPRITE_SIZE;

                            if (yVel > 0 && !bosses[i].is_immune() && bosses[i].health) {
                                //yVel = -PLAYER_ATTACK_JUMP;
                                yVel = -std::max(yVel * PLAYER_ATTACK_JUMP_SCALE, PLAYER_ATTACK_JUMP_MIN);

                                // Take health off enemy
                                bosses[i].health--;

                                if (bosses[i].yVel < 0) {
                                    // Enemy is jumping
                                    // Stop enemy's jump
                                    //bosses[i].yVel = 0;
                                }

                                bosses[i].set_injured();
                            }
                        }
                    }
                }
            }

            for (uint16_t i = 0; i < foreground.size(); i++) {
                if (colliding(foreground[i])) {
                    if (yVel > 0 && y + SPRITE_SIZE < foreground[i].y + SPRITE_HALF) {
                        // Collided from top
                        y = foreground[i].y - SPRITE_SIZE;
                        dropPlayer = false; // stop player falling through platforms (only used in boss #2 currently)
                    }
                    else if (yVel < 0 && y + SPRITE_SIZE > foreground[i].y + SPRITE_HALF) {
                        // Collided from bottom
                        y = foreground[i].y + SPRITE_SIZE;
                    }
                    yVel = 0;
                }
            }

            // Platforms may need work
            if (!dropPlayer) {
                for (uint16_t i = 0; i < platforms.size(); i++) {
                    handle_platform_collisions(platforms[i]);
                }
            }

            for (uint16_t i = 0; i < levelTriggers.size(); i++) {
                if (levelTriggers[i].visible && colliding(levelTriggers[i])) {
                    if (yVel > 0 && y + SPRITE_SIZE < levelTriggers[i].y + SPRITE_HALF) {
                        if (allPlayerSaveData[playerSelected].levelReached >= levelTriggers[i].levelNumber) {
                            // Level is unlocked

                            // Collided from top
                            y = levelTriggers[i].y - SPRITE_SIZE;
                            //yVel = -PLAYER_ATTACK_JUMP;
                            yVel = -std::max(yVel * PLAYER_ATTACK_JUMP_SCALE, PLAYER_ATTACK_JUMP_MIN);

                            levelTriggers[i].set_active();

                            // Play sfx
                            audioHandler.play(4);
                        }
                        else {
                            // Level is locked, act as a solid object

                            // Collided from top
                            y = levelTriggers[i].y - SPRITE_SIZE;
                            yVel = 0;
                        }
                    }
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
                    else if (xVel < 0) {
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
                    else if (xVel < 0) {
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

            if (!immuneTimer && !dropPlayer) {
                for (uint16_t i = 0; i < enemies.size(); i++) {
                    if (colliding(enemies[i]) && enemies[i].health) {
                        health--;
                        set_immune();
                    }
                }

                for (uint16_t i = 0; i < bosses.size(); i++) {
                    if (colliding(bosses[i]) && bosses[i].health) {
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

            if (x < -SCREEN_MID_WIDTH) {
                x = -SCREEN_MID_WIDTH;
            }
            else if (x > levelData.levelWidth * SPRITE_SIZE + SCREEN_MID_WIDTH) {
                x = levelData.levelWidth * SPRITE_SIZE + SCREEN_MID_WIDTH;
            }
        }
    }

    void render(Camera camera) {
        // Particles
        for (uint8_t i = 0; i < slowParticles.size(); i++) {
            slowParticles[i].render(camera);
        }

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
                uint16_t frame = anchorFrame;

                if (yVel < -50) {
                    frame = anchorFrame + 1;
                }
                else if (yVel > 160) {
                    frame = anchorFrame + 2;
                }

                if (lastDirection == 1) {
                    //screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                    render_sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y), SpriteTransform::HORIZONTAL);
                }
                else {
                    //screen.sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
                    render_sprite(frame, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
                }


                if (slowPlayer || repelPlayer) {
                    render_sprite(448, Point(SCREEN_MID_WIDTH + x - camera.x, SCREEN_MID_HEIGHT + y - camera.y));
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

    bool colliding(Enemy enemy) {
        // Replace use of this with actual code?
        return (enemy.x + SPRITE_SIZE > x && enemy.x < x + SPRITE_SIZE && enemy.y + SPRITE_SIZE > y && enemy.y < y + SPRITE_SIZE);
    }

    bool colliding(Boss boss) {
        if (boss.is_big()) {
            return (boss.x + SPRITE_SIZE * 4 > x && boss.x < x + SPRITE_SIZE && boss.y + SPRITE_SIZE * 4 > y && boss.y < y + SPRITE_SIZE);
        }
        else {
            return (boss.x + SPRITE_SIZE * 2 > x && boss.x < x + SPRITE_SIZE && boss.y + SPRITE_SIZE * 2 > y && boss.y < y + SPRITE_SIZE);
        }
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
    std::vector<BrownianParticle> slowParticles;
    float slowPlayerParticleTimer;
    float airTime;
};
Player player;


void background_rect(uint8_t position) {
    if (position) {
        screen.pen = Pen(hudBackground.r, hudBackground.g, hudBackground.b, hudBackground.a);
        screen.rectangle(Rect(0, SCREEN_HEIGHT - (SPRITE_SIZE + 12), SCREEN_WIDTH, SPRITE_SIZE + 12));
    }
    else {
        screen.pen = Pen(hudBackground.r, hudBackground.g, hudBackground.b, hudBackground.a);
        screen.rectangle(Rect(0, 0, SCREEN_WIDTH, SPRITE_SIZE + 12));
    }
}

void display_stats() {
    screen.text("Coins collected:", minimal_font, Point(SPRITE_SIZE, SCREEN_MID_HEIGHT - SPRITE_SIZE * 2), true, TextAlign::center_left);
    screen.text("Enemies defeated:", minimal_font, Point(SPRITE_SIZE, SCREEN_MID_HEIGHT), true, TextAlign::center_left);
    screen.text("Time taken:", minimal_font, Point(SPRITE_SIZE, SCREEN_MID_HEIGHT + SPRITE_SIZE * 2), true, TextAlign::center_left);


    screen.text(std::to_string(player.score), minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE * 2, SCREEN_MID_HEIGHT - SPRITE_SIZE * 2), true, TextAlign::center_right);
    screen.text(std::to_string(player.enemiesKilled), minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE * 2, SCREEN_MID_HEIGHT), true, TextAlign::center_right);
    //screen.text(std::to_string((int)player.levelTimer), minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE * 2, SCREEN_MID_HEIGHT + SPRITE_SIZE * 2), true, TextAlign::center_right);

    // Trim player.levelTimer to 2dp
    std::string levelTimerString = std::to_string(player.levelTimer);
    levelTimerString = levelTimerString.substr(0, levelTimerString.find('.') + 3);
    screen.text(levelTimerString, minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE * 2, SCREEN_MID_HEIGHT + SPRITE_SIZE * 2), true, TextAlign::center_right);


    render_sprite(TILE_ID_HUD_COINS, Point(SCREEN_WIDTH - SPRITE_HALF * 3, SCREEN_MID_HEIGHT - SPRITE_HALF * 5));
    render_sprite(TILE_ID_HUD_ENEMIES_KILLED, Point(SCREEN_WIDTH - SPRITE_HALF * 3, SCREEN_MID_HEIGHT - SPRITE_HALF));
    render_sprite(TILE_ID_HUD_TIME_TAKEN, Point(SCREEN_WIDTH - SPRITE_HALF * 3, SCREEN_MID_HEIGHT + SPRITE_HALF * 3));
}





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

void render_bosses() {
    for (uint8_t i = 0; i < bosses.size(); i++) {
        bosses[i].render(camera);
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

void render_image_particles() {
    for (uint16_t i = 0; i < imageParticles.size(); i++) {
        imageParticles[i].render(camera);
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

    if (dropPlayer) {
        screen.alpha = 128;
    }
    render_tiles(platforms);
    screen.alpha = 255;

    render_tiles(generic_entities);
    render_tiles(spikes);
    render_tiles(foreground);

    render_coins();
}

void render_entities() {
    render_bosses();
    render_enemies();

    player.render(camera);

    render_projectiles();
}

void render_particles() {
    render_image_particles();
}

void render_hud() {
    screen.pen = Pen(hudBackground.r, hudBackground.g, hudBackground.b, hudBackground.a);
    screen.rectangle(Rect(0, 0, SCREEN_WIDTH, SPRITE_HALF * 2 + 2));


    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);

    // Player health
    for (uint8_t i = 0; i < PLAYER_MAX_HEALTH; i++) {
        if (i < player.health) {
            render_sprite(TILE_ID_HEART, Point(2 + i * SPRITE_SIZE, 2));
            //screen.sprite(TILE_ID_HEART, Point(2 + i * SPRITE_SIZE, 2));
        }
        else {
            render_sprite(TILE_ID_HEART + 1, Point(2 + i * SPRITE_SIZE, 2));
            //screen.sprite(TILE_ID_HEART + 1, Point(2 + i * SPRITE_SIZE, 2));
        }
    }

    // Player score
    blit::screen.text(std::to_string(player.score), minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE - 2, 2), true, blit::TextAlign::top_right);

    render_sprite(TILE_ID_HUD_COINS, Point(SCREEN_WIDTH - SPRITE_SIZE, 2));
    //screen.sprite(TILE_ID_HUD_COINS, Point(SCREEN_WIDTH - SPRITE_SIZE, 2));


    // Player lives
    blit::screen.text(std::to_string(player.lives), minimal_font, Point(2 + 6 * SPRITE_SIZE - 2, 2), true, blit::TextAlign::top_right);

    render_sprite(TILE_ID_HUD_LIVES + playerSelected, Point(2 + 6 * SPRITE_SIZE, 2));
    //screen.sprite(TILE_ID_HUD_LIVES + playerSelected, Point(2 + 6 * SPRITE_SIZE, 2));
}

void render_nearby_level_info() {
    for (uint8_t i = 0; i < levelTriggers.size(); i++) {
        if (std::abs(player.x - levelTriggers[i].x) < LEVEL_INFO_MAX_RANGE && std::abs(player.y - levelTriggers[i].y) < LEVEL_INFO_MAX_RANGE) {
            background_rect(1);


            screen.pen = Pen(levelTriggerParticleColours[1].r, levelTriggerParticleColours[1].g, levelTriggerParticleColours[1].b);

            // Level number
            screen.text("Level " + std::to_string(levelTriggers[i].levelNumber + 1), minimal_font, Point(SPRITE_HALF, SCREEN_HEIGHT - 9 - SPRITE_HALF), true, TextAlign::center_left);


            screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);

            if (allPlayerSaveData[playerSelected].levelReached < levelTriggers[i].levelNumber) {
                // Level is locked
                screen.text("Level locked", minimal_font, Point(SCREEN_WIDTH - SPRITE_HALF, SCREEN_HEIGHT - 9 + SPRITE_HALF), true, TextAlign::center_right);
            }
            else if (allPlayerSaveData[playerSelected].levelReached == levelTriggers[i].levelNumber) {
                // Level is unlocked and has not been completed
                screen.text("No highscores", minimal_font, Point(SCREEN_WIDTH - SPRITE_HALF, SCREEN_HEIGHT - 9 + SPRITE_HALF), true, TextAlign::center_right);
            }
            else {
                // Level is unlocked and has been completed

                if (allLevelSaveData[playerSelected][levelTriggers[i].levelNumber].time == 0.0f) {
                    // If time == 0.0f, something's probably wrong (like no save slot for that level, but still save slot for saveData worked)

                    screen.text("Error loading highscores", minimal_font, Point(SCREEN_WIDTH - SPRITE_HALF, SCREEN_HEIGHT - 9 + SPRITE_HALF), true, TextAlign::center_right);
                }
                else {
                    screen.text(std::to_string(allLevelSaveData[playerSelected][levelTriggers[i].levelNumber].score), minimal_font, Point(SCREEN_WIDTH - SPRITE_HALF * 25, SCREEN_HEIGHT - 9 + SPRITE_HALF), true, TextAlign::center_right);
                    screen.text(std::to_string(allLevelSaveData[playerSelected][levelTriggers[i].levelNumber].enemiesKilled), minimal_font, Point(SCREEN_WIDTH - SPRITE_HALF * 16, SCREEN_HEIGHT - 9 + SPRITE_HALF), true, TextAlign::center_right);

                    // Trim time to 2dp
                    std::string timeString = std::to_string(allLevelSaveData[playerSelected][levelTriggers[i].levelNumber].time);
                    timeString = timeString.substr(0, timeString.find('.') + 3);
                    screen.text(timeString, minimal_font, Point(SCREEN_WIDTH - SPRITE_HALF * 4, SCREEN_HEIGHT - 9 + SPRITE_HALF), true, TextAlign::center_right);


                    render_sprite(TILE_ID_HUD_COINS, Point(SCREEN_WIDTH - SPRITE_HALF * 24, SCREEN_HEIGHT - 9));
                    render_sprite(TILE_ID_HUD_ENEMIES_KILLED, Point(SCREEN_WIDTH - SPRITE_HALF * 15, SCREEN_HEIGHT - 9));
                    render_sprite(TILE_ID_HUD_TIME_TAKEN, Point(SCREEN_WIDTH - SPRITE_HALF * 3, SCREEN_HEIGHT - 9));
                }
            }

            //screen.text(std::to_string(saveData.scores[levelTriggers[i].levelNumber]), minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
        }
    }
}

void load_level(uint8_t levelNumber) {
    snowGenTimer = 0.0f;

    // Variables for finding start and finish positions
    uint16_t finishX, finishY;

    playerStartX = playerStartY = 0;
    cameraStartX = cameraStartY = 0;
    finishX = finishY = 0;


    // Get a pointer to the map header
    TMX16* tmx = (TMX16*)asset_levels[levelNumber];

    uint16_t levelWidth = tmx->width;
    uint16_t levelHeight = tmx->height;
    uint32_t levelSize = levelWidth * levelHeight;

    levelData.levelWidth = levelWidth;
    levelData.levelHeight = levelHeight;

    levelDeathBoundary = levelData.levelHeight * SPRITE_SIZE * LEVEL_DEATH_BOUNDARY_SCALE;

    foreground.clear();
    generic_entities.clear();
    background.clear();
    platforms.clear();
    parallax.clear();
    coins.clear();
    enemies.clear();
    bosses.clear();
    levelTriggers.clear();
    projectiles.clear();
    imageParticles.clear();
    spikes.clear();

    // Foreground Layer
    for (uint32_t i = 0; i < levelSize; i++) {
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

    // Background Layer
    for (uint32_t i = 0; i < levelSize; i++) {
        uint32_t index = i + levelSize * 3;

        if (tmx->data[index] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else {
            // Background tiles are non-solid. If semi-solidity (can jump up but not fall through) is required, use platforms (will be a separate layer).
            background.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[index]));
        }
    }

    // Entity Spawns Layer
    for (uint32_t i = 0; i < levelSize; i++) {
        uint32_t index = i + levelSize;

        if (tmx->data[index] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else if (tmx->data[index] == TILE_ID_PLAYER_1) {
            playerStartX = (i % levelWidth) * SPRITE_SIZE;
            playerStartY = (i / levelWidth) * SPRITE_SIZE;
        }
        else if (tmx->data[index] == TILE_ID_CAMERA) {
            cameraStartX = (i % levelWidth) * SPRITE_SIZE;
            cameraStartY = (i / levelWidth) * SPRITE_SIZE;
        }
        else if (tmx->data[index] == TILE_ID_FINISH) {
            finishX = (i % levelWidth) * SPRITE_SIZE;
            finishY = (i / levelWidth) * SPRITE_SIZE;
        }
        else if (tmx->data[index] == TILE_ID_LEVEL_TRIGGER) {
            levelTriggers.push_back(LevelTrigger((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, 0));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_1) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[0], 0));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_2) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[1], 1));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_3) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[2], 2));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_4) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[3], 3));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_5) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[4], 4));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_6) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[5], 5));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_7) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[6], 6));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_8) {
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[7], 7));
        }
        else if (tmx->data[index] == TILE_ID_ENEMY_9) {
            // A ninth enemy!?
            enemies.push_back(Enemy((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, enemyHealths[8], 8));
        }
        else if (tmx->data[index] == TILE_ID_BOSS_1) {
            bosses.push_back(Boss((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, bossHealths[0], 0));
        }
        else if (tmx->data[index] == TILE_ID_BOSS_2) {
            bosses.push_back(Boss((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, bossHealths[1], 1));
        }
        else if (tmx->data[index] == TILE_ID_BIG_BOSS) {
            bosses.push_back(Boss((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, bossHealths[2], 2));
        }
        else if (tmx->data[index] == TILE_ID_SPIKE_BOTTOM ||
            tmx->data[index] == TILE_ID_SPIKE_TOP ||
            tmx->data[index] == TILE_ID_SPIKE_LEFT ||
            tmx->data[index] == TILE_ID_SPIKE_RIGHT) {

            spikes.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[index]));
        }
        else if (tmx->data[index] < BYTE_SIZE) {
            // Don't create tiles for bosses etc (no terrain/non-enemy entity tiles are >=256)
            // Background tiles are non-solid
            generic_entities.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[index]));
        }
    }

    // Sort levelTriggers by x and relabel
    std::sort(levelTriggers.begin(), levelTriggers.end(), level_trigger_sort_min_x);
    for (uint8_t i = 0; i < levelTriggers.size(); i++) {
        levelTriggers[i].levelNumber = i;
    }

    // Platform Layer
    for (uint32_t i = 0; i < levelSize; i++) {
        uint32_t index = i + levelSize * 2;

        if (tmx->data[index] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else if (levelNumber == LEVEL_SELECT_NUMBER && tmx->data[index] >= TILE_ID_LEVEL_BRIDGE_MIN) {
            // For now, only load these into normal collection of platforms if player has unlocked a level after this location
            if (allPlayerSaveData[playerSelected].levelReached == LEVEL_COUNT || (i % levelWidth) * SPRITE_SIZE < levelTriggers[allPlayerSaveData[playerSelected].levelReached].x) {
                platforms.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[index]));//,PLATFORM_BOUNCE
            }
        }
        else {
            // Background tiles are non-solid. If semi-solidity (can jump up but not fall through) is required, use platforms (will be a separate layer).
            platforms.push_back(Tile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[index]));
        }
    }

    // maybe adjust position of tile so that don't need to bunch all up in corner while designing level

    // go backwards through parallax layers so that rendering is correct

    // Parallax Background Layer
    for (uint32_t i = 0; i < levelSize; i++) {
        uint32_t index = i + levelSize * 5;

        if (tmx->data[index] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else {
            parallax.push_back(ParallaxTile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[index], 1));
        }
    }

    // Parallax Foreground Layer
    for (uint32_t i = 0; i < levelSize; i++) {
        uint32_t index = i + levelSize * 4;

        if (tmx->data[index] == TILE_ID_EMPTY) {
            // Is a blank tile, don't do anything
        }
        else {
            parallax.push_back(ParallaxTile((i % levelWidth) * SPRITE_SIZE, (i / levelWidth) * SPRITE_SIZE, tmx->data[index], 0));
        }
    }


    // Reset player attributes
    player = Player(playerStartX, playerStartY, playerSelected);

    finish = Finish(finishX, finishY, finishFrames);
    
    // Reset camera position
    camera.x = cameraStartX;
    camera.y = cameraStartY;

    // Set player position pointers
    for (uint8_t i = 0; i < enemies.size(); i++) {
        enemies[i].set_player_position(&player.x, &player.y);
    }
    for (uint8_t i = 0; i < bosses.size(); i++) {
        bosses[i].set_player_position(&player.x, &player.y);
    }


    // Check there aren't any levelTriggers which have levelNumber >= LEVEL_COUNT
    levelTriggers.erase(std::remove_if(levelTriggers.begin(), levelTriggers.end(), [](LevelTrigger levelTrigger) { return levelTrigger.levelNumber >= LEVEL_COUNT; }), levelTriggers.end());

    // Prep snow particles (create some so that it isn't empty to start with)
    if (gameState == GameState::STATE_LEVEL_SELECT) {
        for (uint16_t i = 0; i < SNOW_LEVEL_SELECT_INIT_COUNT; i++) {
            // Generate snow particles
            // Get random position
            float xVel = rand() % 3 - 1;
            float yVel = rand() % 5 + 8;
            uint16_t startX = (levelTriggers[(SNOW_WORLD * LEVELS_PER_WORLD) - 1].x + levelTriggers[SNOW_WORLD * LEVELS_PER_WORLD].x) / 2;
            //uint16_t endX = (levelTriggers[((SNOW_WORLD + 1) * LEVELS_PER_WORLD) - 1].x + levelTriggers[(SNOW_WORLD + 1) * LEVELS_PER_WORLD].x) / 2;
            uint16_t endX = (levelTriggers[((SNOW_WORLD + 1) * LEVELS_PER_WORLD)].x + levelTriggers[((SNOW_WORLD + 1) * LEVELS_PER_WORLD) + 1].x) / 2;
            float x = (rand() % (endX - startX)) + startX;
            float y = (rand() % (levelData.levelHeight * SPRITE_SIZE + SCREEN_HEIGHT)) - SCREEN_MID_HEIGHT;
            //if ((x > levelTriggers[SNOW_WORLD * LEVELS_PER_WORLD].x && x < levelTriggers[(SNOW_WORLD + 1) * LEVELS_PER_WORLD].x) || rand() % 2 == 0) {
            if ((x > levelTriggers[SNOW_WORLD * LEVELS_PER_WORLD].x && x < levelTriggers[(SNOW_WORLD + 1) * LEVELS_PER_WORLD].x) || rand() % 2 == 0) {
                // At edges, only make a half as many particles
                imageParticles.push_back(ImageParticle(x, y, xVel, yVel, 0, 0, snowParticleImages[rand() % snowParticleImages.size()]));
            }
        }
    }
    else if (currentWorldNumber == SNOW_WORLD || currentLevelNumber == 8) {
        // Generate snow particles
        for (uint16_t i = 0; i < SNOW_LEVEL_INIT_COUNT; i++) {
            // Get random position
            // Change vel later (use wind?)
            float xVel = rand() % 5 - 2;
            float yVel = rand() % 5 + 8;
            float x = (rand() % (levelData.levelWidth * SPRITE_SIZE + SCREEN_WIDTH)) - SCREEN_MID_WIDTH;
            float y = (rand() % (levelData.levelHeight * SPRITE_SIZE + SCREEN_HEIGHT)) - SCREEN_MID_HEIGHT;
            imageParticles.push_back(ImageParticle(x, y, xVel, yVel, 0, 0, snowParticleImages[rand() % snowParticleImages.size()]));
        }
    }
}

void reset_level_vars() {
    cameraRespawn = false;
    bossBattle = false;
    slowPlayer = false;
    dropPlayer = false;
    repelPlayer = false;

    gamePaused = false;
    pauseMenuItem = 0;
}

void start_level(uint8_t levelNumber) {
    gameState = GameState::STATE_IN_GAME;

    // Load coin sfx slot into select sfx slot
    audioHandler.load(0, 2);

    // Load level
    load_level(levelNumber);


    // Make sure player attribute setting is done after load_level call, since player is reassigned in that method
    player.locked = true;
    cameraIntro = true;
    reset_level_vars();

    open_transition();
}

void start_input_select() {
    gameState = GameState::STATE_INPUT_SELECT;

    open_transition();
}

void start_character_select() {
    gameState = GameState::STATE_CHARACTER_SELECT;

    // Load character select level
    load_level(LEVEL_COUNT + 1);

    if (playerSelected) {
        player = Player(playerStartX + SPRITE_SIZE * 7, playerStartY, 1);
    }
    else {
        player = Player(playerStartX, playerStartY, 0);
    }

    open_transition();
}

void start_menu() {
    gameState = GameState::STATE_MENU;

    menuItem = 0;

    // Load menu level
    load_level(LEVEL_COUNT);

    open_transition();
}

void start_settings() {
    gameState = GameState::STATE_SETTINGS;

    settingsItem = 0;

    open_transition();
}

void start_level_select() {
    bool wonLevel = gameState == GameState::STATE_WON;

    gameState = GameState::STATE_LEVEL_SELECT;

    // Load level select level
    load_level(LEVEL_SELECT_NUMBER);


    if (currentLevelNumber != NO_LEVEL_SELECTED) {
        // Must have just attempted a level
        // Place player next to finished level, on right if just finished, on left if failed.
        for (uint8_t i = 0; i < levelTriggers.size(); i++) {
            if (levelTriggers[i].levelNumber == currentLevelNumber) {
                playerStartX = wonLevel ? levelTriggers[i].x + SPRITE_HALF * 3 : levelTriggers[i].x - SPRITE_HALF * 3;
                playerStartY = levelTriggers[i].y;
                player.x = playerStartX;
                player.y = playerStartY;
                camera.x = player.x;
                camera.y = player.y;
            }
        }
    }
    else {
        // Must have just come from title/menu screen
        for (uint8_t i = 0; i < levelTriggers.size(); i++) {
            if (levelTriggers[i].levelNumber == allPlayerSaveData[playerSelected].levelReached - 1) {
                playerStartX = levelTriggers[i].x + SPRITE_HALF * 3;
                playerStartY = levelTriggers[i].y;
                player.x = playerStartX;
                player.y = playerStartY;
                camera.x = player.x;
                camera.y = player.y;
            }
        }
    }

    // Make sure player attribute setting is done after load_level call, since player is reassigned in that method
    player.locked = true;
    reset_level_vars();

    open_transition();

    // Reset currentLevelNumber so no level is pre-selected
    currentLevelNumber = NO_LEVEL_SELECTED;
}

void start_game_lost() {
    gameState = GameState::STATE_LOST;

    open_transition();
}

void start_game_won() {
    gameState = GameState::STATE_WON;

    if (currentLevelNumber == allPlayerSaveData[playerSelected].levelReached) {
        // Just completed level for first time
        allPlayerSaveData[playerSelected].levelReached = currentLevelNumber + 1;

        if (currentLevelNumber % LEVELS_PER_WORLD == LEVELS_PER_WORLD - 1 || currentLevelNumber == 8) {
            // Level was last level in world or was level 9
            cameraNewWorld = true;
            player.locked = true;
            camera.reset_temp();
        }
    }

    if (allLevelSaveData[playerSelected][currentLevelNumber].score < player.score) {
        allLevelSaveData[playerSelected][currentLevelNumber].score = player.score;
    }
    if (allLevelSaveData[playerSelected][currentLevelNumber].enemiesKilled < player.enemiesKilled) {
        allLevelSaveData[playerSelected][currentLevelNumber].enemiesKilled = player.enemiesKilled;
    }
    if (allLevelSaveData[playerSelected][currentLevelNumber].time > player.levelTimer || allLevelSaveData[playerSelected][currentLevelNumber].time == 0.0f) {
        allLevelSaveData[playerSelected][currentLevelNumber].time = player.levelTimer;
    }

    //save_game_data();
    save_player_data(playerSelected);
    save_level_data(playerSelected, currentLevelNumber);

    open_transition();
}


void render_sg_icon() {
    screen.stretch_blit(sg_icon_image, Rect(0, 0, SG_ICON_SIZE, SG_ICON_SIZE), Rect(SCREEN_MID_WIDTH - SG_ICON_SIZE, SCREEN_MID_HEIGHT - SG_ICON_SIZE, SG_ICON_SIZE * 2, SG_ICON_SIZE * 2));
}

void render_input_select() {
    render_background();

    background_rect(0);
    background_rect(1);


    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    screen.text("Select Input Method", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    screen.pen = gameSaveData.inputType == InputType::CONTROLLER ? Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b) : Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    screen.text("Controller/32Blit", minimal_font, Point(SCREEN_MID_WIDTH, 50), true, TextAlign::center_center);

    screen.pen = gameSaveData.inputType == InputType::KEYBOARD ? Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b) : Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    screen.text("Keyboard", minimal_font, Point(SCREEN_MID_WIDTH, 70), true, TextAlign::center_center);


    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);

    if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
        screen.text(messageStrings[0][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
    }
}

void render_character_select() {
    render_background();

    render_level();

    render_entities();

    if (playerSelected) {
        render_sprite(TILE_ID_PLAYER_1, Point(SCREEN_MID_WIDTH + playerStartX - camera.x, SCREEN_MID_HEIGHT + playerStartY - camera.y), SpriteTransform::HORIZONTAL);
        //screen.sprite(TILE_ID_PLAYER_1, Point(SCREEN_MID_WIDTH + playerStartX - camera.x, SCREEN_MID_HEIGHT + playerStartY - camera.y), SpriteTransform::HORIZONTAL);
    }
    else {
        render_sprite(TILE_ID_PLAYER_2, Point(SCREEN_MID_WIDTH + playerStartX - camera.x + SPRITE_SIZE * 7, SCREEN_MID_HEIGHT + playerStartY - camera.y));
        //screen.sprite(TILE_ID_PLAYER_2, Point(SCREEN_MID_WIDTH + playerStartX - camera.x + SPRITE_SIZE * 7, SCREEN_MID_HEIGHT + playerStartY - camera.y));
    }


    background_rect(0);
    background_rect(1);

    screen.pen = Pen(hudBackground.r, hudBackground.g, hudBackground.b, hudBackground.a);
    screen.rectangle(Rect(0, SCREEN_HEIGHT - (SPRITE_SIZE + 12 + 12), SCREEN_WIDTH, 12));

    screen.pen = Pen(levelTriggerParticleColours[1].r, levelTriggerParticleColours[1].g, levelTriggerParticleColours[1].b);
    screen.text("Player " + std::to_string(playerSelected + 1) + " (Save " + std::to_string(playerSelected + 1)  + ")", minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 10 - 12), true, TextAlign::center_center);

    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    screen.text("Select Player", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
        screen.text(messageStrings[0][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
    }
}

void render_menu() {
    render_background();

    render_level();

    render_entities();


    background_rect(0);
    background_rect(1);

    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    screen.text("Super Square Bros.", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    if (menuItem == 0) {
        screen.pen = Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b);
    }
    screen.text("Play", minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT - 10), true, TextAlign::center_center);

    if (menuItem == 0) {
        screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    }
    else {
        screen.pen = Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b);
    }
    screen.text("Settings", minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT + 10), true, TextAlign::center_center);

    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);

    if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
        screen.text(messageStrings[0][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
    }
}

void render_settings() {
    render_background();

    background_rect(0);

    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    screen.text("Settings", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    screen.text("Checkpoints:", minimal_font, Point(SPRITE_SIZE, SCREEN_MID_HEIGHT - SPRITE_SIZE * 3), true, TextAlign::center_left);
    screen.text("Music:", minimal_font, Point(SPRITE_SIZE, SCREEN_MID_HEIGHT - SPRITE_SIZE), true, TextAlign::center_left);
    screen.text("SFX:", minimal_font, Point(SPRITE_SIZE, SCREEN_MID_HEIGHT + SPRITE_SIZE), true, TextAlign::center_left);


    if (settingsItem == 0) {
        screen.pen = Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b);
    }
    screen.text(settingsData.checkpoints ? "On" : "Off", minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE * 2, SCREEN_MID_HEIGHT - SPRITE_SIZE * 3), true, TextAlign::center_right);

    if (settingsItem == 1) {
        screen.pen = Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b);
    }
    else {
        screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    }
    screen.text(settingsData.musicVolume ? "On" : "Off", minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE * 2, SCREEN_MID_HEIGHT - SPRITE_SIZE), true, TextAlign::center_right);

    if (settingsItem == 2) {
        screen.pen = Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b);
    }
    else {
        screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    }
    screen.text(settingsData.sfxVolume ? "On" : "Off", minimal_font, Point(SCREEN_WIDTH - SPRITE_SIZE * 2, SCREEN_MID_HEIGHT + SPRITE_SIZE), true, TextAlign::center_right);


    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
}

void render_level_select() {
    render_background();

    render_level();

    render_entities();

    render_level_triggers();

    render_particles();


    background_rect(0);

    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
    screen.text("Select level", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    render_nearby_level_info();
}

void render_game() {
    render_background();

    render_level();

    if (bosses.size() == 0) {
        render_finish();
    }

    render_entities();

    render_particles();


    if (gamePaused) {
        screen.pen = Pen(gameBackground.r, gameBackground.g, gameBackground.b, hudBackground.a); // use hudBackground.a to make background semi transparent
        screen.rectangle(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        screen.rectangle(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        screen.rectangle(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)); // repeat to make darker

        //screen.pen = Pen(hudBackground.r, hudBackground.g, hudBackground.b, hudBackground.a);
        //screen.rectangle(Rect(0, SPRITE_SIZE + 12, SCREEN_WIDTH, SCREEN_HEIGHT - (SPRITE_SIZE + 12))); // do it twice to make it darker

        background_rect(0);
        background_rect(1);


        //screen.pen = Pen(hudBackground.r, hudBackground.g, hudBackground.b, hudBackground.a);
        //screen.text(messageStrings[3][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);

        screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
        display_stats();
        screen.text("Game Paused", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

        if (pauseMenuItem == 0) {
            screen.pen = Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b);
        }
        screen.text("Resume", minimal_font, Point(SCREEN_MID_WIDTH - SPRITE_SIZE * 4, SCREEN_HEIGHT - 9), true, TextAlign::center_center);

        if (pauseMenuItem == 1) {
            screen.pen = Pen(inputSelectColour.r, inputSelectColour.g, inputSelectColour.b);
        }
        else {
            screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
        }
        screen.text("Exit", minimal_font, Point(SCREEN_MID_WIDTH + SPRITE_SIZE * 4, SCREEN_HEIGHT - 9), true, TextAlign::center_center);

        /*if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
            screen.text(messageStrings[3][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
        }*/
    }
    else if (cameraIntro) {
        // Level <num> message
        background_rect(0);
        screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
        screen.text("Level " + std::to_string(currentLevelNumber + 1), minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

        // Press <key> to skip intro message
        background_rect(1);
        if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
            screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);
            screen.text(messageStrings[1][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
        }
    }
    else {
        render_hud();
    }
}



void render_game_lost() {
    render_background();

    background_rect(0);
    background_rect(1);

    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);

    screen.text("Level failed.", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    display_stats();

    if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
        screen.text(messageStrings[2][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
    }
}


void render_game_won() {
    render_background();

    background_rect(0);
    background_rect(1);

    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);

    screen.text("Level complete!", minimal_font, Point(SCREEN_MID_WIDTH, 10), true, TextAlign::center_center);

    display_stats();

    if (textFlashTimer < TEXT_FLASH_TIME * 0.6f) {
        screen.text(messageStrings[2][gameSaveData.inputType], minimal_font, Point(SCREEN_MID_WIDTH, SCREEN_HEIGHT - 9), true, TextAlign::center_center);
    }
}

void update_enemies(float dt, ButtonStates buttonStates) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].update(dt, buttonStates);
    }
}

void update_bosses(float dt, ButtonStates buttonStates) {
    for (int i = 0; i < bosses.size(); i++) {
        bosses[i].update(dt, buttonStates);
    }

    if (bosses.size() == 1 && repelPlayer) {
        player.xVel += std::max(std::abs(((player.x + SPRITE_HALF) - (bosses[0].x + bosses[0].get_size() / 2)) * 0.2f), REPEL_PLAYER_MIN) * ((player.x + SPRITE_HALF < bosses[0].x + bosses[0].get_size() / 2) ? -1 : 1);
    }
}

void update_level_triggers(float dt, ButtonStates buttonStates) {
    for (int i = 0; i < levelTriggers.size(); i++) {
        levelTriggers[i].update(dt, buttonStates);
        if (!levelTriggers[i].visible && levelTriggers[i].particles.size() == 0) {
            currentLevelNumber = levelTriggers[i].levelNumber;
            currentWorldNumber = currentLevelNumber / LEVELS_PER_WORLD;
        }
    }

    levelTriggers.erase(std::remove_if(levelTriggers.begin(), levelTriggers.end(), [](LevelTrigger levelTrigger) { return (!levelTrigger.visible && levelTrigger.particles.size() == 0); }), levelTriggers.end());
}

void update_coins(float dt) {
    for (int i = 0; i < coins.size(); i++) {
        coins[i].update(dt, buttonStates);
    }
}


void update_projectiles(float dt) {
    for (uint8_t i = 0; i < projectiles.size(); i++) {
        projectiles[i].update(dt, buttonStates);
    }

    //  Allow enemies to get hit?
    /*for (uint8_t i = 0; i < enemies.size(); i++) {

    }*/

    if (!player.is_immune()) {
        uint8_t projectileCount = projectiles.size();
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile projectile) { return projectile.is_colliding(player.x, player.y); }), projectiles.end());
        if (projectileCount - projectiles.size() > 0) {
            player.health -= 1;
            player.set_immune();
        }
    }
    

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile projectile) { return (std::abs(projectile.x - player.x) > SCREEN_TILE_SIZE || std::abs(projectile.y - player.y) > SCREEN_HEIGHT); }), projectiles.end());

}

void update_particles(float dt) {
    for (uint16_t i = 0; i < imageParticles.size(); i++) {
        imageParticles[i].update(dt);
    }

    if (gameState == GameState::STATE_LEVEL_SELECT) {
        snowGenTimer += dt;
        while (snowGenTimer >= SNOW_LEVEL_SELECT_GENERATE_DELAY) {
            snowGenTimer -= SNOW_LEVEL_SELECT_GENERATE_DELAY;
            // Generate snow particles
            // Get random position
            float xVel = rand() % 3 - 1;
            float yVel = rand() % 5 + 8;
            uint16_t startX = (levelTriggers[(SNOW_WORLD * LEVELS_PER_WORLD) - 1].x + levelTriggers[SNOW_WORLD * LEVELS_PER_WORLD].x) / 2;
            //uint16_t endX = (levelTriggers[((SNOW_WORLD + 1) * LEVELS_PER_WORLD) - 1].x + levelTriggers[(SNOW_WORLD + 1) * LEVELS_PER_WORLD].x) / 2;
            uint16_t endX = (levelTriggers[((SNOW_WORLD + 1) * LEVELS_PER_WORLD)].x + levelTriggers[((SNOW_WORLD + 1) * LEVELS_PER_WORLD) + 1].x) / 2;
            float x = (rand() % (endX - startX)) + startX;
            if ((x > levelTriggers[SNOW_WORLD * LEVELS_PER_WORLD].x && x < levelTriggers[(SNOW_WORLD + 1) * LEVELS_PER_WORLD].x) || rand() % 2 == 0) {
                // At edges, only make a half as many particles
                imageParticles.push_back(ImageParticle(x, 0, xVel, yVel, 0, 0, snowParticleImages[rand() % snowParticleImages.size()]));
            }
        }
    }
    else if (currentWorldNumber == SNOW_WORLD || currentLevelNumber == 8) {
        snowGenTimer += dt;
        while (snowGenTimer >= SNOW_LEVEL_GENERATE_DELAY) {
            snowGenTimer -= SNOW_LEVEL_GENERATE_DELAY;
            // Generate snow particles
            // Get random position
            float xVel = rand() % 3 - 1;
            float yVel = rand() % 5 + 8;
            float x = (rand() % (levelData.levelWidth * SPRITE_SIZE + SCREEN_WIDTH)) - SCREEN_MID_WIDTH;
            imageParticles.push_back(ImageParticle(x, 0, xVel, yVel, 0, 0, snowParticleImages[rand() % snowParticleImages.size()]));
        }
    }

    imageParticles.erase(std::remove_if(imageParticles.begin(), imageParticles.end(), [](ImageParticle particle) { return particle.y > levelDeathBoundary; }), imageParticles.end());
}

void update_sg_icon(float dt, ButtonStates buttonStates) {
    if (splashColour.a == 255) {
        // Init game
        gameState = GameState::STATE_INPUT_SELECT;
        init_game();
    }
    else if (splashColour.a > 0) {
        if (splashColour.a <= 255 - FADE_STEP) {
            splashColour.a += FADE_STEP;
        }
        else {
            splashColour.a = 255;
        }
    }
    else {
        if (buttonStates.A || buttonStates.B || buttonStates.X || buttonStates.Y || !audioHandler.is_playing(7)) {
            splashColour.a = FADE_STEP;
        }
    }
}

void update_input_select(float dt, ButtonStates buttonStates) {
    if (splashColour.a > 0) {
        if (splashColour.a >= FADE_STEP) {
            splashColour.a -= FADE_STEP;
        }
        else {
            splashColour.a = 0;
        }
    }
    else {
        if (transition[0].is_ready_to_open()) {
            start_menu();
        }
        else if (transition[0].is_open()) {
            if (gameSaveData.inputType == InputType::CONTROLLER) {
                if (buttonStates.DOWN) {
                    gameSaveData.inputType = InputType::KEYBOARD;

                    audioHandler.play(0);
                }
            }
            else if (gameSaveData.inputType == InputType::KEYBOARD) {
                if (buttonStates.UP) {
                    gameSaveData.inputType = InputType::CONTROLLER;

                    audioHandler.play(0);
                }
            }


            if (buttonStates.A == 2) {
                audioHandler.play(0);

                close_transition();

                // Save inputType
                save_game_data();
            }
        }
    }
}

void update_character_select(float dt, ButtonStates buttonStates) {
    // Dummy states is used to make selected player continually jump (sending A key pressed).
    ButtonStates dummyStates = { 0 };
    dummyStates.A = 2;
    player.update(dt, dummyStates);


    if (transition[0].is_ready_to_open()) {
        if (menuBack) {
            menuBack = false;
            start_menu();
        }
        else {
            start_level_select();
        }
    }
    else if (transition[0].is_open()) {
        if (buttonStates.RIGHT && !playerSelected) {
            audioHandler.play(0);

            playerSelected = 1;
            player = Player(playerStartX + SPRITE_SIZE * 7, playerStartY, 1);
            player.lastDirection = 0;
        }
        else if (buttonStates.LEFT && playerSelected) {
            audioHandler.play(0);

            playerSelected = 0;
            player = Player(playerStartX, playerStartY, 0);
            player.lastDirection = 1;
        }

        if (buttonStates.A == 2) {
            audioHandler.play(0);

            close_transition();
        }
        else if (buttonStates.Y == 2) {
            audioHandler.play(0);

            menuBack = true;
            close_transition();
        }
    }
}

void update_menu(float dt, ButtonStates buttonStates) {
    update_coins(dt);


    if (splashColour.a > 0) {
        if (splashColour.a >= FADE_STEP) {
            splashColour.a -= FADE_STEP;
        }
        else {
            splashColour.a = 0;
        }
    }
    else {
        if (transition[0].is_ready_to_open()) {
            if (menuBack) {
                menuBack = false;
                start_input_select();
            }
            else {
                if (menuItem == 0) {
                    start_character_select();
                }
                else {
                    start_settings();
                }
            }
        }
        else if (transition[0].is_open()) {
            if (buttonStates.A == 2) {
                audioHandler.play(0);

                close_transition();
            }
            else if (buttonStates.Y == 2) {
                audioHandler.play(0);

                menuBack = true;
                close_transition();
            }
            else if (buttonStates.UP == 2 && menuItem == 1) {
                menuItem = 0;
                audioHandler.play(0);
            }
            else if (buttonStates.DOWN == 2 && menuItem == 0) {
                menuItem = 1;
                audioHandler.play(0);
            }
        }
    }
}

void update_settings(float dt, ButtonStates buttonStates) {
    if (transition[0].is_ready_to_open()) {
        if (menuBack) {
            menuBack = false;
            start_menu();
        }
        else {
            if (menuItem == 0) {
                start_character_select();
            }
            else {
                start_settings();
            }
        }
    }
    else if (transition[0].is_open()) {
        if (buttonStates.A == 2) {
            audioHandler.play(0);

            close_transition();
        }
        else if (buttonStates.Y == 2) {
            audioHandler.play(0);

            menuBack = true;
            close_transition();
        }
        else if (buttonStates.UP == 2 && settingsItem) {
            settingsItem--;
            audioHandler.play(0);
        }
        else if (buttonStates.DOWN == 2 && settingsItem < 2) {
            settingsItem++;
            audioHandler.play(0);
        }
    }
}

void update_level_select(float dt, ButtonStates buttonStates) {
    player.update(dt, buttonStates);

    update_enemies(dt, buttonStates);

    update_level_triggers(dt, buttonStates);

    update_particles(dt);


    // Button handling

    if (transition[0].is_ready_to_open()) {
        if (menuBack) {
            menuBack = false;
            start_character_select();
        }
        else {
            start_level(currentLevelNumber);
        }
    }
    else if (transition[0].is_open()) {
        if (cameraNewWorld) {
            if (camera.tempX == 0.0f && camera.tempY == 0.0f) {
                camera.tempX = camera.x;
                camera.tempY = camera.y;
            }
            camera.linear_to(dt, camera.tempX, camera.tempY, levelTriggers[allPlayerSaveData[playerSelected].levelReached].x, levelTriggers[allPlayerSaveData[playerSelected].levelReached].y, CAMERA_NEW_WORLD_TIME);

            if (levelTriggers[allPlayerSaveData[playerSelected].levelReached].x == camera.x && levelTriggers[allPlayerSaveData[playerSelected].levelReached].y == camera.y) {
                if (camera.timer_started()) {
                    camera.update_timer(dt);
                    if (camera.get_timer() > NEW_WORLD_DELAY_TIME) {
                        // Animation finished
                        cameraNewWorld = false;
                        camera.reset_temp();
                        camera.reset_timer();
                    }
                }
                else {
                    camera.start_timer();
                }
            }
        }
        else if (cameraRespawn) {
            camera.ease_out_to(dt, player.x, player.y);

            if (std::abs(player.x - camera.x) < CAMERA_RESPAWN_LOCK_MIN && std::abs(player.x - camera.x) < CAMERA_RESPAWN_LOCK_MIN) {
                // for respawns
                cameraRespawn = false;
                player.locked = false;

                // to stop player completely dying
                player.lives = 3;
            }
        }
        else {
            camera.ease_out_to(dt, player.x, player.y);

            player.locked = false;
        }


        if (currentLevelNumber != NO_LEVEL_SELECTED) {
            close_transition();
            player.locked = true;
        }
        else if (buttonStates.Y == 2) {
            audioHandler.play(0);

            menuBack = true;
            close_transition();
            player.locked = true;
        }
    }
}

void update_game(float dt, ButtonStates buttonStates) {
    if (gamePaused) {
        if (pauseMenuItem == 0) {
            if (buttonStates.RIGHT == 2) {
                pauseMenuItem = 1;
                audioHandler.play(0);
            }
        }
        else if (pauseMenuItem == 1) {
            if (buttonStates.LEFT == 2) {
                pauseMenuItem = 0;
                audioHandler.play(0);
            }
        }
    }
    else {
        // Game isn't paused, update it.

        player.update(dt, buttonStates);

        update_enemies(dt, buttonStates);

        update_bosses(dt, buttonStates);

        update_coins(dt);

        update_projectiles(dt);

        update_particles(dt);

        if (bosses.size() == 0) {
            // Only show finish if there are no bosses

            // Need to rework finish
            finish.update(dt, buttonStates);

            if (std::abs(player.x - finish.x) < SPRITE_HALF * 3 && std::abs(player.y - finish.y) < SPRITE_HALF) {
                //// lock player to finish
                //player.x = finish.x;
                //player.y = finish.y - 1;

                if (std::abs(player.x - finish.x) < SPRITE_HALF) {
                    player.x = finish.x;
                    player.y = finish.y;
                }
                else {
                    if (player.x < finish.x) {
                        player.xVel += std::max(SPRITE_SIZE * 2 - (finish.x - player.x), (float)SPRITE_SIZE) * 0.4f;
                    }
                    else {
                        player.xVel -= std::max(SPRITE_SIZE * 2 - (player.x - finish.x), (float)SPRITE_SIZE) * 0.4f;
                    }
                }
            }
        }

    }
    //if (player.x + SPRITE_SIZE > finish.x - SPRITE_SIZE && player.x < finish.x + SPRITE_SIZE * 2 && player.y + SPRITE_SIZE > finish.y - SPRITE_SIZE && player.y < finish.y + SPRITE_SIZE * 2) {
    //    // 'pull' player to finish

    //    player.x += (finish.x - player.x) * 4 * dt;
    //    player.y += (finish.y - player.y) * 4 * dt;
    //}

    if (transition[0].is_ready_to_open()) {
        if (pauseMenuItem == 1) {
            // Player exited level
            start_level_select();
        }
        else if (player.lives) {
            // Player completed level
            start_game_won();
        }
        else {
            // Player failed level
            start_game_lost();
        }

        // Unload coin sfx, load select sfx file back into select sfx slot
        audioHandler.load(0, 0);
    }
    else if (transition[0].is_open()) {
        if (gamePaused) {
            if (buttonStates.A == 2) {
                audioHandler.play(0);

                if (pauseMenuItem == 0) {
                    // Unpause game
                    gamePaused = false;

                    // Load coin sfx file into select sfx slot
                    audioHandler.load(0, 2);
                }
                else if (pauseMenuItem == 1) {
                    // Exit level
                    close_transition();
                }
            }
        }
        else {
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

                if (buttonStates.A == 2) {
                    cameraIntro = false;
                    cameraRespawn = true; // goes to player faster
                }
            }
            else if (cameraRespawn) {
                camera.ease_out_to(dt, player.x, player.y);

                if (std::abs(player.x - camera.x) < CAMERA_RESPAWN_LOCK_MIN && std::abs(player.x - camera.x) < CAMERA_RESPAWN_LOCK_MIN) {
                    // for respawns
                    cameraRespawn = false;
                    player.locked = false;
                }
            }
            else {
                /*if (freezePlayer) {
                    for (uint8_t i = 0; i < bosses.size(); i++) {
                        if (bosses[i].get_state() == 2 || bosses[i].get_state() == 3 || bosses[i].is_dead()) {
                            foundBoss = true;
                            camera.ease_out_to(dt, bosses[i].x - (bosses[i].x - player.x) / 2, bosses[i].y - (bosses[i].y - player.y) / 2);
                            break;
                        }
                    }
                }*/
                if (bossBattle) {
                    if (bosses.size() == 1) {
                        // Keep both player and boss on screen
                        camera.ease_out_to(dt, bosses[0].x - (bosses[0].get_center_range(bosses[0].x, player.x) * 0.65f) + SPRITE_SIZE, bosses[0].y - (bosses[0].get_center_range(bosses[0].y, player.y) * 0.65f) + SPRITE_SIZE);
                    }
                    else {
                        bossBattle = false;
                    }
                }
                // This can't be else {}, since bossBattle is modified in above statements
                if (!bossBattle) {
                    camera.ease_out_to(dt, player.x, player.y);
                }


                // Handle level end
                if (bosses.size() == 0 && player.x + SPRITE_SIZE > finish.x + 3 && player.x < finish.x + SPRITE_SIZE - 3 && player.y + SPRITE_SIZE > finish.y + 4 && player.y < finish.y + SPRITE_SIZE) {
                    close_transition();
                }

                // Handle player life
                if (player.lives == 0 && player.particles.size() == 0) {
                    close_transition();
                }
            }
        }


        // Allow player to toggle pause game (if game is paused, selecting 'resume' also does same thing
        if (buttonStates.Y == 2) {
            if (gamePaused) {
                audioHandler.play(0);
                // Load coin sfx into select sfx slot
                audioHandler.load(0, 2);
                gamePaused = false;
            }
            else {
                gamePaused = true;
                pauseMenuItem = 0;
                // Unload coin sfx, put select sfx back into select sfx slot
                audioHandler.load(0, 0);
                audioHandler.play(0);
            }
        }
    }
}

void update_game_lost(float dt, ButtonStates buttonStates) {
    if (transition[0].is_ready_to_open()) {
        start_level_select();
    }
    else if (transition[0].is_open()) {
        if (buttonStates.A == 2) {
            audioHandler.play(0);

            close_transition();
        }
    }
}


void update_game_won(float dt, ButtonStates buttonStates) {
    if (transition[0].is_ready_to_open()) {
        start_level_select();
    }
    else if (transition[0].is_open()) {
        if (buttonStates.A == 2) {
            audioHandler.play(0);

            close_transition();
        }
    }
}

void init_game() {
    bool success = read_save(gameSaveData);

    // Load save data
    // Attempt to load the first save slot.
    if (success) {
        bool reset = false;

        if (get_version_struct(gameSaveData.version).major != gameVersion.major) {
            reset = true;
        }
#ifdef RESET_SAVE_DATA_IF_MINOR_DIFF
        else if (get_version_struct(gameSaveData.version).minor != gameVersion.minor) {
            reset = true;
        }
#endif

        if (reset) {
            GameVersion saveDataVersion = get_version_struct(gameSaveData.version);

            printf("Warning: Saved game data is out of date, save version is %d (v%d.%d.%d), but firmware version is %d (v%d.%d.%d)\n", gameSaveData.version, saveDataVersion.major, saveDataVersion.minor, saveDataVersion.build, get_version(gameVersion), gameVersion.major, gameVersion.minor, gameVersion.build);
            printf("Resetting save data...\n");

            success = false;
            reset_save();
        }
    }

    if (success) {
        GameVersion saveDataVersion = get_version_struct(gameSaveData.version);
        printf("Save data loaded, save version: %d (v%d.%d.%d)\n", gameSaveData.version, saveDataVersion.major, saveDataVersion.minor, saveDataVersion.build);

        // Loaded sucessfully!
        gameState = GameState::STATE_MENU;

        // Load menu level
        load_level(LEVEL_COUNT);

    }
    else {
        // No save file or it failed to load, set up some defaults.
        gameSaveData.version = get_version(gameVersion);

        gameSaveData.inputType = InputType::CONTROLLER;

        // gameState is by default set to STATE_INPUT_SELECT

#ifdef TARGET_32BLIT_HW
        // If it's a 32blit, don't bother asking
        gameState = GameState::STATE_MENU;

        // Load menu level
        load_level(LEVEL_COUNT);

        // Save inputType
        save_game_data();
#endif
    }
}

void load_audio() {
    // NOTE: CURRENTLY ISSUE WITH LEAVING PAUSE MENU, blip AUDIO IS PLAYED, BUT THEN NEW SOUND IS LOADED IN, STOPPING PLAYBACK.

    // Set volume to a default
    audioHandler.set_volume(0x5000);
    // Sfx
    audioHandler.load(0, asset_sound_select, asset_sound_select_length);
    audioHandler.load(1, asset_sound_jump, asset_sound_jump_length);
    audioHandler.load(2, asset_sound_coin, asset_sound_coin_length);
    audioHandler.load(3, asset_sound_enemydeath, asset_sound_enemydeath_length);
    audioHandler.load(4, asset_sound_enemyinjured, asset_sound_enemyinjured_length);
    audioHandler.load(5, asset_sound_playerdeath, asset_sound_playerdeath_length);
    audioHandler.load(6, asset_sound_enemythrow, asset_sound_enemythrow_length);
    // Music
    audioHandler.load(7, asset_music_splash, asset_music_splash_length);

    // Start splash music playing
    audioHandler.play(7);

    // Note: to play sfx0, call audioHandler.play(0)
    // For music, need to load sound when changing (i.e. audioHandler.load(7, asset_music_<music>, asset_music_<music>_length); audioHandler.play(7, 0b11);
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

    load_audio();

    // Load metadata
    metadata = get_metadata();
    gameVersion = parse_version(metadata.version);
    printf("Loaded metadata. Game version: %d (v%d.%d.%d)\n", get_version(gameVersion), gameVersion.major, gameVersion.minor, gameVersion.build);

    // Populate transition array
    for (uint8_t y = 0; y < SCREEN_HEIGHT / SPRITE_SIZE; y++) {
        for (uint8_t x = 0; x < SCREEN_WIDTH / SPRITE_SIZE; x++) {
            transition[y * (SCREEN_WIDTH / SPRITE_SIZE) + x] = AnimatedTransition(x * SPRITE_SIZE, y * SPRITE_SIZE, transitionFramesOpen, transitionFramesClose);
        }
    }


    allPlayerSaveData[0] = load_player_data(0);
    allPlayerSaveData[1] = load_player_data(1);

    // Load level data
    for (uint8_t i = 0; i < LEVEL_COUNT; i++) {
        allLevelSaveData[0][i] = load_level_data(0, i);
    }
    for (uint8_t i = 0; i < LEVEL_COUNT; i++) {
        allLevelSaveData[1][i] = load_level_data(1, i);
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
    screen.pen = Pen(splashColour.r, splashColour.g, splashColour.b);
    screen.clear();

    // draw some text at the top of the screen
    screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(defaultWhite.r, defaultWhite.g, defaultWhite.b);

    if (gameState == GameState::STATE_SG_ICON) {
        render_sg_icon();
    }
    else if (gameState == GameState::STATE_INPUT_SELECT) {
        render_input_select();
    }
    else if (gameState == GameState::STATE_MENU) {
        render_menu();
    }
    else if (gameState == GameState::STATE_SETTINGS) {
        render_settings();
    }
    else if (gameState == GameState::STATE_CHARACTER_SELECT) {
        render_character_select();
    }
    else if (gameState == GameState::STATE_LEVEL_SELECT) {
        render_level_select();
    }
    else if (gameState == GameState::STATE_IN_GAME) {
        render_game();
    }
    else if (gameState == GameState::STATE_LOST) {
        render_game_lost();
    }
    else if (gameState == GameState::STATE_WON) {
        render_game_won();
    }

    render_transition();

    if (splashColour.a != 0) {
        screen.pen = Pen(splashColour.r, splashColour.g, splashColour.b, splashColour.a);
        screen.clear();
    }
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

    // Update buttonStates
    if (buttons & Button::A) {
        if (buttonStates.A == 2) {
            buttonStates.A = 1;
        }
        else if (buttonStates.A == 0) {
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
        else if (buttonStates.B == 0) {
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
        else if (buttonStates.X == 0) {
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
        else if (buttonStates.Y == 0) {
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
        else if (buttonStates.UP == 0) {
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
        else if (buttonStates.DOWN == 0) {
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
        else if (buttonStates.LEFT == 0) {
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
        else if (buttonStates.RIGHT == 0) {
            buttonStates.RIGHT = 2;
        }
    }
    else {
        buttonStates.RIGHT = 0;
    }


    // Update game
    if (gameState == GameState::STATE_SG_ICON) {
        update_sg_icon(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_INPUT_SELECT) {
        update_input_select(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_MENU) {
        update_menu(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_SETTINGS) {
        update_settings(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_CHARACTER_SELECT) {
        update_character_select(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_LEVEL_SELECT) {
        update_level_select(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_IN_GAME) {
        update_game(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_LOST) {
        update_game_lost(dt, buttonStates);
    }
    else if (gameState == GameState::STATE_WON) {
        update_game_won(dt, buttonStates);
    }

    update_transition(dt, buttonStates);

    // Screen shake
    camera.x += shaker.time_to_shake(dt);
    camera.y += shaker.time_to_shake(dt);

    audioHandler.update();
}