#include "src/Utils/Arduboy2Ext.h"
#include <ArduboyTones.h>
#include <EEPROM.h>
#include <math.h>

#include "src/Entities/Enemy.h"
#include "src/Entities/Player.h"
#include "src/Entities/Obstacle.h"
#include "src/Entities/Bullet.h"
#include "src/Entities/EnemyBullet.h"
#include "src/Utils/Sequences.h"
#include "src/Utils/Sounds.h"
#include "src/Utils/Enums.h"
#include "FixedPoints.h"
#include "FixedPointsCommon.h"

#include "src/Images/Images_Enemy.h"
#include "src/Images/Images_Boat.h"
#include "src/Images/Images_Explosions.h"
#include "src/Images/Images_Obstacles.h"
#include "src/Images/Images_Player.h"
#include "src/Images/Images_Scoreboard.h"
#include "src/Images/Images_Splash.h"
#include "src/Images/Images_Scenery.h"
#include "src/Images/Images_Arrays.h"
#include "src/Images/Images_Scenery.h"
#include "src/Utils/EEPROM_Utils.h"

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);

ARDUBOY_NO_USB

#include "src/Fonts/Fonts.h"
#ifdef HIGH_SCORES
#include "src/Utils/HighScoreEditor.h"

HighScore highScore;
uint8_t alternate = 0;
#endif

const uint8_t* const missions[] =        { mission_00, mission_01, mission_02, mission_03, mission_04 };
const int8_t* const formations[] =       { formation_00, formation_01, formation_02, formation_03, formation_04, formation_06, formation_05, 
                                           formation_06, formation_07, formation_08, formation_09, formation_10, formation_11, formation_12, 
                                           formation_13 };
const int8_t* const sequences[] =        { seq_00, seq_01, seq_02, seq_03 };

const int8_t obstacleLaunchDelayInc[] =  { OBSTACLE_LAUNCH_DELAY_INC_L0, OBSTACLE_LAUNCH_DELAY_INC_L1, OBSTACLE_LAUNCH_DELAY_INC_L2 };
const int8_t frameRateInc[] =            { FRAME_RATE_INC_L0, FRAME_RATE_INC_L1, FRAME_RATE_INC_L2 };

const SQ7x8 obstacleBulletsValueDec[] =  { BULLETS_DECREMENT_L0, BULLETS_DECREMENT_L1, BULLETS_DECREMENT_L2 };
const SQ7x8 obstacleFuelValueDec[] =     { FUEL_DECREMENT_L0, FUEL_DECREMENT_L1, FUEL_DECREMENT_L2 };
const SQ7x8 obstacleHealthValueDec[] =   { HEALTH_DECREMENT_L0, HEALTH_DECREMENT_L1, HEALTH_DECREMENT_L2 };

const SQ7x8 playerHitBulletDec[] =       { PLAYER_HIT_BULLET_DEC_NORM, PLAYER_HIT_BULLET_DEC_DOUBLE, PLAYER_HIT_BULLET_DEC_NORM, PLAYER_HIT_BULLET_DEC_NORM };  // Index positions defined by EnemyType

Player player = { player_images };

Enemy enemies[NUMBER_OF_ENEMIES] = {
  { EnemyType::Fighter1, enemy_fighter1_images },
  { EnemyType::Fighter1, enemy_fighter1_images },
  { EnemyType::Fighter1, enemy_fighter1_images },
  { EnemyType::Fighter1, enemy_fighter1_images },
  { EnemyType::Fighter1, enemy_fighter1_images },
  { EnemyType::Fighter1, enemy_fighter1_images },
};

Obstacle obstacle = { -20, 24, fuel, 0, 0 };

uint8_t playerBulletIdx = 0;
uint8_t enemyBulletIdx = 0;

Bullet playerBullets[PLAYER_BULLETS_MAX];
EnemyBullet enemyBullets[ENEMY_BULLETS_MAX];
uint8_t enemyBulletsMax[] = { ENEMY_BULLETS_MAX_L1, ENEMY_BULLETS_MAX_L2, ENEMY_BULLETS_MAX_L3 };

uint16_t obstacleLaunchCountdown = OBSTACLE_LAUNCH_DELAY_MIN;
uint8_t enemyShotCountdown = 5;
uint8_t level = 0;

uint8_t mission = 0;                                        // Mission currently being played
uint8_t missionIdx = 0;                                     // Byte index within current mission
uint8_t mission_formations = 0;                             // Number of formations in the current mission.
uint8_t mission_formations_left = 0;                        // Number of formations left within current mission.
uint8_t formation = 0;
uint8_t introState;
uint8_t intro;
uint16_t frameRate = INIT_FRAME_RATE;
uint8_t restart = 0;

uint16_t obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
uint16_t obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;

SQ7x8 obstacleBulletsValue = BULLETS_MAX;
SQ7x8 obstacleHealthValue = HEALTH_MAX;
SQ7x8 obstacleFuelValue = FUEL_MAX;

uint8_t sceneryRestrictions = SCENERY_NONE; 
SceneryGround upperSceneryPosition;
SceneryGround lowerSceneryPosition;
SceneryItem sceneryItems[NUMBER_OF_SCENERY_ITEMS];

GameState gameState = GameState::Intro_Init;

void initSceneryItems() {
  sceneryItems[0] = { 128, 20, SceneryElement::Boat2 };
  sceneryItems[1] = { 182, 25, SceneryElement::Wave1 };
  sceneryItems[2] = { 236, 30, SceneryElement::Wave2 };
  sceneryItems[3] = { 290, 35, SceneryElement::Boat };
}

bool renderBulletsAbove = true;
uint8_t bulletCountdown = 0;
#ifdef USE_LEDS             
uint8_t ledCountdown = 0;
#endif

// PLAYDATE
void launchPlayerBullet(const uint8_t x, const uint8_t y, const Direction direction);
void launchScenery(int16_t xOffset, uint8_t yPos, SceneryGround *sceneryItem);
Direction aimAtPlayer(const uint8_t enemyIdx);
void renderScenery_AbovePlanes();
void renderScenery(const uint8_t frame);
void drawFlyingPair();
void checkForObstacleCollision();
void launchObstacle();
void checkForEnemiesShot();
void checkForPlayerShot();
void checkForPlayerEnemyCollision();
void gameLoop();
void launchMission(bool firstFormation, const uint8_t *missionRef);
void checkCanEnemyShoot();
void launchFormation(const int8_t *formation);
void renderScenery_BelowPlanes();
void renderScoreboard();
Direction getAimDirection(const int8_t deltaX, const int8_t deltaY, const SQ15x16 heading);

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the player's active bullets ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderPlayerBullets() {

    for (uint8_t i = 0; i < PLAYER_BULLETS_MAX; ++i) {

        playerBullets[i].move();

        if (playerBullets[i].getEnabled()) {
            Sprites::drawPlusMask(playerBullets[i].getX() - 1, playerBullets[i].getY() - 1, bullet_img, static_cast<uint8_t>(BulletImage::Single));
        }

    }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the enemies' active bullets ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderEnemyBullets() {

    for (uint8_t i = 0; i < enemyBulletsMax[level]; ++i) {

        enemyBullets[i].move();

        if (enemyBullets[i].getEnabled()) {

            uint8_t x = enemyBullets[i].getX() - 1;
            uint8_t y = enemyBullets[i].getY() - 1;

            Sprites::drawPlusMask(x, y, bullet_img, static_cast<uint8_t>(enemyBullets[i].getBulletImage()));

        }

    }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the obstacle if it is active ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderObstacle() {

    if (obstacle.getEnabled()) {

        obstacle.move();
        obstacle.renderImage();

    }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the active enemies ..  Boats are handled differently as clouds should always be rendered above them.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderEnemies(bool renderBoats) {

    for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

        if (!renderBoats && enemies[i].getEnemyType() == EnemyType::Boat) { continue; }
        if (renderBoats && enemies[i].getEnemyType()  != EnemyType::Boat) { continue; }

        enemies[i].move();
        Rect enemyRect = enemies[i].getRect();

        if (enemies[i].getEnabled() && enemies[i].getDelayStart() == 0
            && enemyRect.x + enemyRect.width > 0 && enemyRect.x < WIDTH
            && enemyRect.y + enemyRect.height >= 0 && enemyRect.y < HEIGHT ) {

            if (enemies[i].getEnemyType() == EnemyType::Boat) {

                enemies[i].setTurretDirection(aimAtPlayer(i));

            }

            enemies[i].renderImage();

        }

    }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move player off the screen ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void renderEndOfMission() {

    for (uint8_t i = 0; i < 128; ++i) {

        renderScenery(i % 2);
        moveAndRenderObstacle();
        renderScenery_BelowPlanes();
        moveAndRenderEnemies(false);

#ifdef PLAYER_US
        Sprites::drawExternalMask(player.getX().getInteger() + i, player.getY().getInteger(), p38_0, p38_mask_0, (i % 6 < 3), 0);
#endif

#ifdef PLAYER_JAPANESE
        Sprites::drawExternalMask(player.getX().getInteger() + i, player.getY().getInteger(), zero_0, zero_mask_0, (i % 6 < 3), 0);
#endif

        renderScenery_AbovePlanes();

        renderScoreboard();
        arduboy.display(true);
        delay(20);

    }

}


/* ----------------------------------------------------------------------------
 *  Render score board gauge.
 * ----------------------------------------------------------------------------
 */
uint16_t scoreFlash;

void renderScoreboadGauge(const uint8_t imageX, const uint8_t imageY, const uint8_t *image, const uint8_t scoreboardY, const uint8_t value) {

    Sprites::drawOverwrite(imageX, imageY, image, 0);

    if ((value <= 4 && scoreFlash >= (SCOREBOARD_FLASH_MAX / 2)) || value > 4) {
        for (uint8_t i = 0; i < (value); i += 2) {
            arduboy.drawLine(imageX, scoreboardY + i, WIDTH, scoreboardY + i);
        }
    }

}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Render the score board.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void renderScoreboard() {


    // Increment the frame count

    ++scoreFlash;        if (scoreFlash > SCOREBOARD_FLASH_MAX) { scoreFlash = 0; }
    if (bulletCountdown > 0) bulletCountdown--;

    uint16_t player_score = player.getScore();


    // Clear the board space ..

    arduboy.fillRect(SCOREBOARD_OUTER_RECT_X, SCOREBOARD_OUTER_RECT_Y, SCOREBOARD_OUTER_RECT_WIDTH, SCOREBOARD_OUTER_RECT_HEIGHT, BLACK);
    arduboy.drawLine(SCOREBOARD_INNER_RECT_X, SCOREBOARD_INNER_RECT_Y, SCOREBOARD_INNER_RECT_X, HEIGHT, WHITE);


    // Score ..
    {
        uint8_t player_score_digits[4] = {};
        extractDigits(player_score_digits, player_score);

        for (uint8_t i = 0, y2 = 15; i < 4; ++i, y2 = y2 - 5) {
            Sprites::drawExternalMask(SCOREBOARD_KILLS_X, y2, digits, digit_mask, player_score_digits[i], 0);
        }

    }

    renderScoreboadGauge(SCOREBOARD_HEALTH_BAR_X, SCOREBOARD_HEALTH_BAR_Y, health_gauge, SCOREBOARD_HEALTH_BAR_TOP, (player.getHealth() < 0 ? 0 : player.getHealth().getInteger()));
    renderScoreboadGauge(SCOREBOARD_FUEL_BAR_X, SCOREBOARD_FUEL_BAR_Y, fuel_gauge, SCOREBOARD_FUEL_BAR_TOP, (player.getFuel() < 0 ? 0 : player.getFuel().getInteger()));


    // Render ammo gauge ..

    if (bulletCountdown > 0) {

        uint8_t bullets = player.getBullets();
        uint8_t x = player.getX().getInteger();
        uint8_t y = player.getY().getInteger();

        if (y < 7  && renderBulletsAbove)  { renderBulletsAbove = false;}
        if (y > 42 && !renderBulletsAbove) { renderBulletsAbove = true;}

        int8_t yOffset = (renderBulletsAbove ? -6 : 19);

        Sprites::drawExternalMask(x, y + yOffset, ammo_gauge, ammo_gauge_mask, 0, 0);

        if ((bullets <= 16 && scoreFlash >= (SCOREBOARD_FLASH_MAX / 2)) || bullets > 16) {

            for (int i = 0, xOffset = x; i < bullets; i = i + 10, xOffset = xOffset + 2) {

                Sprites::drawExternalMask(xOffset, y + yOffset + 2, ammo_gauge_item, ammo_gauge_item_mask, 0, 0);

            }

        }

    }

}


void renderScenery(const uint8_t frame) {


    // Draw scenery elements ..

    for (uint8_t x = 0; x < NUMBER_OF_SCENERY_ITEMS; x++) {

        switch (sceneryItems[x].element) {

            case SceneryElement::Wave1:
                if (arduboy.getFrameCount(5 + x) != 0) {
                    Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, wave_01, 0);
                }
                break;

            case SceneryElement::Wave2:
                if (arduboy.getFrameCount(5 + x) != 0) {
                    Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, wave_02, 0);
                }
                break;

            case SceneryElement::Boat:
                Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, sail_boat_01, 0);
                break;

            case SceneryElement::Boat2:
                Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, sail_boat_02, 0);
                break;

            case SceneryElement::Island1:
                Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, island_01, 0);
                break;

            case SceneryElement::Island2:
                Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, island_02, 0);
                break;

#ifdef USE_ISLAND_3
            case SceneryElement::Island3:
                Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, island_03, 0);
                break;
#endif

            default: break;

        }

    }


    // Draw ground ..

    if (upperSceneryPosition.enabled) { Sprites::drawOverwrite(upperSceneryPosition.x, upperSceneryPosition.y, ground_upper, upperSceneryPosition.image); }
    if (lowerSceneryPosition.enabled) { Sprites::drawOverwrite(lowerSceneryPosition.x, lowerSceneryPosition.y, ground_lower, lowerSceneryPosition.image); }

    if (frame == 0) {


        // Update scenery element positions ..

        for (uint8_t x = 0; x < NUMBER_OF_SCENERY_ITEMS; x++) {

            sceneryItems[x].x--;

#ifdef USE_ISLAND_3
#define NUMBER_OF_ELEMENTS 9
#else
#define NUMBER_OF_ELEMENTS 8
#endif
#define NUMBER_OF_COMMON_ELEMENTS (static_cast<uint8_t>(SceneryElement::Cloud_BelowPlanes) + 1)

            if (sceneryItems[x].x < -90) {

                sceneryItems[x].x = 162;
                SceneryElement previousElement = (x > 0 ? sceneryItems[x - 1].element : sceneryItems[NUMBER_OF_SCENERY_ITEMS - 1].element);
                uint8_t element = 0;

                switch (previousElement) {

                    case SceneryElement::Boat ... SceneryElement::Wave2:
                        element = random(
                                (sceneryRestrictions == 0 ? static_cast<uint8_t>(SceneryElement::Boat) : static_cast<uint8_t>(SceneryElement::Wave1)),
                                (sceneryRestrictions == 0 ? NUMBER_OF_ELEMENTS : NUMBER_OF_COMMON_ELEMENTS));
                        break;

                    default:
                        element = random(0, NUMBER_OF_COMMON_ELEMENTS);
                        break;

                }

                switch (static_cast<SceneryElement>(element)) {

                    case SceneryElement::Boat ... SceneryElement::Wave2:
                        sceneryItems[x].element = static_cast<SceneryElement>(element);
                        sceneryItems[x].y = random((upperSceneryPosition.enabled ? upperSceneryPosition.y + 24 : 4), (lowerSceneryPosition.enabled ? lowerSceneryPosition.y - 24 : HEIGHT - 24));
                        break;

                    case SceneryElement::Cloud_AbovePlanes ... SceneryElement::Cloud_BelowPlanes:
                        sceneryItems[x].element = static_cast<SceneryElement>(element);
                        sceneryItems[x].y = random(-16, HEIGHT - 16);
                        break;

                    default: //case SceneryElement::Island1 ... SceneryElement::Island3:
                        sceneryItems[x].element = static_cast<SceneryElement>(element);
                        sceneryItems[x].y = random((upperSceneryPosition.enabled ? upperSceneryPosition.y + 24 : -6), (lowerSceneryPosition.enabled ? lowerSceneryPosition.y - 38 : HEIGHT - 16));

                        if (!upperSceneryPosition.enabled)  { launchScenery(random(200, 250), 0, &upperSceneryPosition); }
                        if (!lowerSceneryPosition.enabled)  { launchScenery(random(200, 250), 53, &lowerSceneryPosition); }

                        break;

                }

            }

        }


        // Update ground positions ..

        if (upperSceneryPosition.enabled) {
            upperSceneryPosition.x--;
            if (upperSceneryPosition.x < -90) {
                upperSceneryPosition.enabled = false;
            }
        }
        else {
            if (random(0, 40) == 0) { launchScenery(162, 0, &upperSceneryPosition); }
        }

        if (lowerSceneryPosition.enabled) {
            lowerSceneryPosition.x--;
            if (lowerSceneryPosition.x < -90) {
                lowerSceneryPosition.enabled = false;
            }
        }
        else {
            if (random(0, 40) == 0) { launchScenery(162, 53, &lowerSceneryPosition); }
        }

    }

}

void launchScenery(int16_t xOffset, uint8_t yPos, SceneryGround *sceneryItem) {

    sceneryItem->enabled = true;
    sceneryItem->x = xOffset;
    sceneryItem->y = yPos;
    sceneryItem->image = random(0, 2);

}

void renderScenery_BelowPlanes() {


    // Draw scenery elements ..

    for (uint8_t x = 0; x < NUMBER_OF_SCENERY_ITEMS; x++) {

        switch (sceneryItems[x].element) {

            case SceneryElement::Cloud_BelowPlanes:
                Sprites::drawExternalMask(sceneryItems[x].x, sceneryItems[x].y, cloud, cloud_Mask, 0, 0);
                break;

            default: break;

        }

    }

}


void renderScenery_AbovePlanes() {


    // Draw scenery elements ..

    for (uint8_t x = 0; x < NUMBER_OF_SCENERY_ITEMS; x++) {

        switch (sceneryItems[x].element) {

            case SceneryElement::Cloud_AbovePlanes:
                Sprites::drawExternalMask(sceneryItems[x].x, sceneryItems[x].y, cloud, cloud_Mask, 0, 0);
                break;

            default: break;

        }

    }

}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Introduction loop initialisation ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void introInit() {

    mission = 0;
    missionIdx = 0;
    introState = 0;
    restart = 0;
    gameState = GameState::Intro_Loop;
    sound.tones(score);
    player.initGame();

    obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
    obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;
    obstacleBulletsValue = BULLETS_MAX;
    obstacleHealthValue = HEALTH_MAX;
    obstacleFuelValue = FUEL_MAX;

    frameRate = INIT_FRAME_RATE;
    arduboy.setFrameRate(frameRate);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Credits loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void credits_loop() {

    Sprites::drawOverwrite(11, 10, credits_01, 0);
    for (uint8_t x = 0; x < 64; x = x + 13) {
        Sprites::drawOverwrite(x + 8, 22, credits_02, 0);
    }
    Sprites::drawOverwrite(0, 48, splash_lower, 0);

    arduboy.drawHorizontalDottedLine(4, 69, 3);
    arduboy.drawHorizontalDottedLine(4, 69, 32);

    Sprites::drawOverwrite(75, 3, filmote_01, 0);
    Sprites::drawOverwrite(91, 6, filmote_02, 0);
    Sprites::drawOverwrite(75, 17, pharap_01, 0);
    Sprites::drawOverwrite(91, 23, pharap_02, 0);
    Sprites::drawOverwrite(75, 35, vampirics_01, 0);
    Sprites::drawOverwrite(91, 40, vampirics_02, 0);
    Sprites::drawOverwrite(23, 38, splash_press_a, 0);

    if (intro < 210) {

        drawFlyingPair();

    }

    if (arduboy.justPressed(A_BUTTON)) {
#ifndef HIGH_SCORES
        gameState = GameState::Intro_Init;
#else
        arduboy.pollButtons();
        gameState = GameState::Save_Score;
#endif
    }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Introduction loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
enum class IntroState : uint8_t {
    PressA,
    Level1,
    Level2,
    Level3
};

enum class ArrowState : uint8_t {
    None,
    Lower,
    Upper
};

#define SPLASH_ARROW_MAX 7
#define SPLASH_PRESS_A_X 23
#define SPLASH_PRESS_A_Y 15
#define SPLASH_LEVEL_CAPTION_X SPLASH_PRESS_A_X + 2
#define SPLASH_LEVEL_CAPTION_Y SPLASH_PRESS_A_Y + 5
#define SPLASH_LEVEL_NUMBER_X SPLASH_LEVEL_CAPTION_X + 1
#define SPLASH_LEVEL_NUMBER_Y SPLASH_LEVEL_CAPTION_Y + 21
#define SPLASH_LEVEL_ARROW_UP_X SPLASH_LEVEL_CAPTION_X + 10
#define SPLASH_LEVEL_ARROW_UP_Y 28
#define SPLASH_LEVEL_ARROW_DOWN_X SPLASH_LEVEL_CAPTION_X - 6
#define SPLASH_LEVEL_ARROW_DOWN_Y 28

void introLoop() {

    Sprites::drawOverwrite(5, 19, title, 0);
    Sprites::drawOverwrite(71, 19, return_to_midway, 0);
    Sprites::drawOverwrite(0, -2, splash_upper, 0);
    Sprites::drawOverwrite(0, 50, splash_lower, 0);

    {
        IntroState state = static_cast<IntroState>((introState & 0b11100000) >> 5);
        ArrowState arrowState = static_cast<ArrowState>((introState & 0b00011000) >> 3);
        uint8_t arrowDelay = introState & 0b00000111;

        switch (state) {

            case IntroState::PressA:
                Sprites::drawOverwrite(83, 38, splash_press_a, 0);
                break;

            case IntroState::Level1 ... IntroState::Level3:
            {
                uint8_t leftX = 77;
                uint8_t rightX = 115;

                switch (arrowState) {

                    case ArrowState::None:
                        break;

                    case ArrowState::Lower:
                        leftX = leftX - 2;
                        break;

                    case ArrowState::Upper:
                        rightX = rightX + 2;
                        break;

                }

                //Sprites::drawSelfMasked(94, downX, arrow_down, 0);
                //Sprites::drawSelfMasked(94, upX, arrow_up, 0);
                Sprites::drawSelfMasked(leftX, 40, arrow_left, 0);
                Sprites::drawSelfMasked(rightX, 40, arrow_right, 0);

                Sprites::drawOverwrite(85, 39, level_select, 0);
                Sprites::drawOverwrite(106, 40, font4x6_Full, 27 + level);

            }

                if (arrowDelay > 0) {
                    arrowDelay--;
                    introState = (introState & 0b11111000) | arrowDelay;
                    if (arrowDelay == 0) {
                        introState = introState & 0b11100000;
                    }
                }

                break;

        }

    }

    //arduboy.display(true);

    {

        uint8_t pressed = arduboy.pressedButtons();
        uint8_t justPressed = arduboy.justPressedButtons();
        IntroState state = static_cast<IntroState>((introState & 0b11100000) >> 5);

        if (justPressed & RIGHT_BUTTON) {

            switch (state) {

                case IntroState::PressA:
                    introState = ((static_cast<uint8_t>(IntroState::Level1) + level) << 5) | (static_cast<uint8_t>(ArrowState::None) << 3) | SPLASH_ARROW_MAX;
                    break;

                case IntroState::Level1:
                    introState = (static_cast<uint8_t>(IntroState::Level2) << 5) | (static_cast<uint8_t>(ArrowState::Upper) << 3) | SPLASH_ARROW_MAX;
                    level++;
                    EEPROM.put(Constants::EEPROM_Level, level);
                    EEPROM_Utils::checkSum(true);
                    break;

                case IntroState::Level2:
                    introState = (static_cast<uint8_t>(IntroState::Level3) << 5) | (static_cast<uint8_t>(ArrowState::Upper) << 3) | SPLASH_ARROW_MAX;
                    level++;
                    EEPROM.put(Constants::EEPROM_Level, level);
                    EEPROM_Utils::checkSum(true);
                    break;

                default: break;

            }

        }

        if (justPressed & LEFT_BUTTON) {

            switch (state) {

                case IntroState::PressA:
                    introState = ((static_cast<uint8_t>(IntroState::Level1) + level) << 5) | (static_cast<uint8_t>(ArrowState::None) << 3) | SPLASH_ARROW_MAX;
                    break;

                case IntroState::Level2:
                    introState = (static_cast<uint8_t>(IntroState::Level1) << 5) | (static_cast<uint8_t>(ArrowState::Lower) << 3) | SPLASH_ARROW_MAX;
                    level--;
                    EEPROM.put(Constants::EEPROM_Level, level);
                    EEPROM_Utils::checkSum(true);
                    break;

                case IntroState::Level3:
                    introState = (static_cast<uint8_t>(IntroState::Level2) << 5) | (static_cast<uint8_t>(ArrowState::Lower) << 3) | SPLASH_ARROW_MAX;
                    level--;
                    EEPROM.put(Constants::EEPROM_Level, level);
                    EEPROM_Utils::checkSum(true);
                    break;

                default: break;

            }

        }

        if (justPressed & B_BUTTON) {
            if (state != IntroState::PressA) {
                introState = 0;
            }
            else {
                gameState = GameState::Credits_Init;
            }
        }

        if (justPressed & A_BUTTON) {

            gameState = GameState::Game_Init;
            mission = 0;

        }


        // Restart ?

        // if (pressed & DOWN_BUTTON) {

        //   if (restart < UPLOAD_DELAY) {
        //     restart++;
        //   }
        //   else {
        //     arduboy.exitToBootloader();
        //   }

        // }
        // else {
        //   restart = 0;
        // }

    }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Initialize a new game ready for play.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void gameInit() {

    for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {
        enemies[i].setEnabled(false);
    }

    for (uint8_t i = 0; i < PLAYER_BULLETS_MAX; ++i) {
        playerBullets[i].setEnabled(false);
    }

    for (uint8_t i = 0; i < ENEMY_BULLETS_MAX; ++i) {
        enemyBullets[i].setEnabled(false);
    }

    {
        bool initConsumables = false;

        if (level < 2)                   { initConsumables = true; }
        if (level == 2 && mission == 60) { initConsumables = true; }

        initSceneryItems();
        player.initMission(initConsumables);
        obstacle.setEnabled(false);

    }

    sound.tones(mission_start);
    intro = 80;
    gameState = GameState::Game_Loop;

    obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
    obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;
    obstacleBulletsValue = BULLETS_MAX;
    obstacleHealthValue = HEALTH_MAX;
    obstacleFuelValue = FUEL_MAX;

    frameRate = INIT_FRAME_RATE;
    arduboy.setFrameRate(frameRate);

}

#ifdef HIGH_SCORES

uint8_t clearScores = 0;


// --------------------------------------------------------------------------------------
//  Render High Score screen ..
//
#define HIGH_SCORE_DIST_BETWEEN_ROWS 9
void renderHighScore(HighScore &highScore) {

    uint8_t xOffset = 75;
    uint8_t yOffset = 4;


    Sprites::drawOverwrite(4, 7, flying_aces, 0);
    Sprites::drawOverwrite(0, 48, splash_lower, 0);

    arduboy.drawHorizontalDottedLine(2, 67, 3);
    arduboy.drawHorizontalDottedLine(2, 67, 32);

    Sprites::drawOverwrite(6, 40, flying_aces_stars, 0);
    Sprites::drawOverwrite(40, 40, flying_aces_stars, 0);
    Sprites::drawOverwrite(25, 31, flying_aces_p38, 0);

    for (uint8_t x = 0; x < MAX_NUMBER_OF_SCORES; x++) {

        xOffset = 76;
        Slot slot = EEPROM_Utils::getSlot(x);

        Sprites::drawOverwrite(xOffset, yOffset, font4x6_Full, slot.getChar0());
        Sprites::drawOverwrite(xOffset + 6, yOffset, font4x6_Full, slot.getChar1());
        Sprites::drawOverwrite(xOffset + 12, yOffset, font4x6_Full, slot.getChar2());


        // Score ..
        {
            uint8_t digits[6] = {};
            extractDigits(digits, slot.getScore());

            for (uint8_t i = 0, x2 = xOffset + 47; i < 6; ++i, x2 -= 5) {
                Sprites::drawSelfMasked(x2, yOffset, font4x6_Full, (digits[i] + 26));
            }

        }

        yOffset = yOffset + HIGH_SCORE_DIST_BETWEEN_ROWS;

    }

    if (highScore.getSlotNumber() != DO_NOT_EDIT_SLOT) {

        xOffset = 76;
        yOffset = 4;
        alternate++;

        if (alternate < 15) {

            //Sprites::drawOverwrite(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS) - 1, yOffset - 4, arrow, 0);
            //Sprites::drawOverwrite(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS) - 1, yOffset + 59, arrow, 0);

        }
        else if (alternate > 30) {

            alternate = 0;

        }

        if (highScore.getCharIndex() == 0) {
            arduboy.fillRect(xOffset - 1, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS) - 1, 6, 8, WHITE);
            Sprites::drawErase(xOffset, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), font4x6_Full, highScore.getChar(0));
        }
        else {
            Sprites::drawOverwrite(xOffset, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), font4x6_Full, highScore.getChar(0));
        }

        if (highScore.getCharIndex() == 1) {
            arduboy.fillRect(xOffset + 5, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS) - 1, 6, 8, WHITE);
            Sprites::drawErase(xOffset + 6, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), font4x6_Full, highScore.getChar(1));
        }
        else {
            Sprites::drawOverwrite(xOffset + 6, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), font4x6_Full, highScore.getChar(1));
        }

        if (highScore.getCharIndex() == 2) {
            arduboy.fillRect(xOffset + 11, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS) - 1, 6, 8, WHITE);
            Sprites::drawErase(xOffset + 12, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), font4x6_Full, highScore.getChar(2));
        }
        else {
            Sprites::drawOverwrite(xOffset + 12, yOffset + (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), font4x6_Full, highScore.getChar(2));
        }

        // if (highScore.getCharIndex() == 1) {
        //   arduboy.fillRect(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS) - 1, yOffset + 5, 8, 6, WHITE);
        //   Sprites::drawErase(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), yOffset + 6, font4x6_Full, highScore.getChar(1));
        // }
        // else {
        //   Sprites::drawOverwrite(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), yOffset + 6, font4x6_Full, highScore.getChar(1));
        // }

        // if (highScore.getCharIndex() == 2) {
        //   arduboy.fillRect(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS) - 1, yOffset + 11, 8, 6, WHITE);
        //   Sprites::drawErase(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), yOffset + 12, font4x6_Full, highScore.getChar(2));
        // }
        // else {
        //   Sprites::drawOverwrite(xOffset - (highScore.getSlotNumber() * HIGH_SCORE_DIST_BETWEEN_ROWS), yOffset + 12, font4x6_Full, highScore.getChar(2));
        // }


        // Handle buttons ..

        {
            uint8_t justPressed = arduboy.justPressedButtons();
            uint8_t charIndex = highScore.getCharIndex();

            if (justPressed & DOWN_BUTTON)          { highScore.decChar(charIndex); }
            if (justPressed & UP_BUTTON)            { highScore.incChar(charIndex); }
            if (justPressed & LEFT_BUTTON)          { highScore.decCharIndex(); }
            if (justPressed & RIGHT_BUTTON)         { highScore.incCharIndex(); }

            if (justPressed & A_BUTTON) {

                EEPROM_Utils::writeChars(highScore.getSlotNumber(), highScore);
                highScore.disableEditting();

            }

        }

    }
    else {

        uint8_t pressed = arduboy.pressedButtons();


        // Clear scores ..

        if ((pressed & UP_BUTTON) && (pressed & DOWN_BUTTON)) {

            clearScores++;

            switch (clearScores) {

                case 21 ... 60:
#ifdef USE_LEDS
                    arduboy.setRGBled(128 - (clearScores * 2), 0, 0);
#endif
                    break;

                case 61:
                    clearScores = 0;
#ifdef USE_LEDS
                    arduboy.setRGBled(0, 0, 0);
#endif
                    EEPROM_Utils::initEEPROM(true);
                    sound.tone(NOTE_C6, 100);
                    return;

            }

        }
        else {

            if (clearScores > 0) {

#ifdef USE_LEDS
                arduboy.setRGBled(0, 0, 0);
#endif
                clearScores = 0;

            }

        }

        {
            uint8_t justPressed = arduboy.justPressedButtons();

            if ((justPressed & A_BUTTON) || (justPressed & B_BUTTON)) {
#ifdef USE_LEDS
                arduboy.setRGBled(0, 0, 0);
#endif
                gameState = GameState::Intro_Init;
            }

        }

    }

    //arduboy.display(true);

}

#endif

/* -----------------------------------------------------------------------------------------------------------------------------
 *  End of mission / game loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void endOfSequence_Render(bool endOfLevel) {

    arduboy.drawHorizontalDottedLine(4, 69, 3);
    arduboy.drawHorizontalDottedLine(4, 69, 32);

    if (gameState == GameState::End_Of_Mission) {

        Sprites::drawOverwrite(5, 8, mission_successful, 0);

    }
    else {

        if (endOfLevel) {
            Sprites::drawOverwrite(6, 8, level_complete, 0);
        }
        else {
            Sprites::drawOverwrite(6, 8, game_over, 0);
        }

    }

    Sprites::drawOverwrite(0, 48, splash_lower, 0);

}

void drawFlyingPair() {

    Sprites::drawExternalMask(intro - 80, 10, FlyBy_P38, FlyBy_P38_Mask, 0, 0);
    Sprites::drawExternalMask(intro - 38, 06, FlyBy_Zero, FlyBy_Zero_Mask, 0, 0);
    Sprites::drawExternalMask(intro - 20, 14, FlyBy_Zero, FlyBy_Zero_Mask, 0, 0);
    intro++;

}

void endOfSequence(const uint8_t level) {

    bool endOfLevel = false;

    if (level == 0 && mission == 30) { gameState = GameState::End_Of_Game; endOfLevel = true; }
    if (level == 1 && mission == 60) { gameState = GameState::End_Of_Game; endOfLevel = true; }

#ifndef HIGH_SCORES
    uint16_t high = EEPROM.read(EEPROM_SCORE + (level * 2));
#else
    uint16_t high = EEPROM_Utils::getHighScore();
#endif

    if (player.getScore() > high) {
#ifndef HIGH_SCORES
        EEPROM.put(Constants::EEPROM_Score + (level * 2)), player.getScore());
    EEPROM_Utils::checkSum(true);
#endif
        high = player.getScore();
    }

    endOfSequence_Render(endOfLevel);

    if (gameState != GameState::End_Of_Game) {
        Sprites::drawOverwrite(84, 5, usaf, 0);
    }

    // Score ..
    {
        uint8_t y = (gameState == GameState::End_Of_Game ? 3 : 28);
        Sprites::drawOverwrite(90, y, score_img, 0);

        uint8_t digits[4] = {};
        extractDigits(digits, player.getScore());

        for (uint8_t i = 0, x = 108; i < 4; ++i, x -= 6) {
            Sprites::drawSelfMasked(x, y + 10, font4x6_Full, digits[i] + 26);
        }

    }

    // Total ..
    if (gameState == GameState::End_Of_Game) {

        uint8_t digits[4] = {};
        Sprites::drawOverwrite(82, 27, highScore_img, 0);
        extractDigits(digits, high);

        for (uint8_t i = 0, x = 108; i < 4; ++i, x -= 6) {
            Sprites::drawSelfMasked(x, 37, font4x6_Full, digits[i] + 26);
        }

    }

    if (intro < 210) {

        drawFlyingPair();

    }

    Sprites::drawOverwrite(23, 38, splash_press_a, 0);


    // Handle key presses ..
    {
        uint8_t justPressed = arduboy.justPressedButtons();
        uint8_t pressed = arduboy.pressedButtons();

        if (gameState == GameState::End_Of_Mission) {
            if (justPressed & A_BUTTON) { gameState = GameState::Game_Init; }
        }
        else {
#ifndef HIGH_SCORES
            if ((pressed & UP_BUTTON) && (pressed & DOWN_BUTTON)) { EEPROM_Utils::initEEPROM(true); player.setScore(0); high = 0; }
      if (justPressed & A_BUTTON) { gameState = GameState::Intro_Init; }
#else
            if (justPressed & A_BUTTON) { gameState = GameState::Save_Score; }
#endif

        }

    }

}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Setup
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void setup() {

  EEPROM_Utils::initEEPROM(false);

  arduboy.begin();
  sound.begin();
  arduboy.audio.begin();
  
  obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
  obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;
  obstacleBulletsValue = BULLETS_MAX;
  obstacleHealthValue = HEALTH_MAX;
  obstacleFuelValue = FUEL_MAX;

  frameRate = INIT_FRAME_RATE;
  level = EEPROM.read(Constants::EEPROM_Level);
  
  arduboy.setFrameRate(frameRate);
  arduboy.initRandomSeed();

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Main loop
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void loop() {
  sound.callback();
  if (!(arduboy.nextFrame())) return;

  arduboy.clear();
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::Intro_Init:
      introInit();
      //break;

    case GameState::Intro_Loop:
      introLoop();
      break;

    case GameState::Game_Init:
      gameInit();
      //break;

    case GameState::Game_Loop:
      gameLoop();
      break;

    case GameState::End_Of_Mission:
    case GameState::End_Of_Game:
      endOfSequence(level);
      break;
    
    case GameState::Credits_Init:
      intro = 0;
      gameState = GameState::Credits_Loop;
    
    case GameState::Credits_Loop:
      credits_loop();
      break;

    #ifdef HIGH_SCORES
    case GameState::Save_Score:
      highScore.reset();
      highScore.setSlotNumber(EEPROM_Utils::saveScore(player.getScore()));
      gameState = GameState::High_Score;
      // break; Fall-through intentional.

    case GameState::High_Score:
      renderHighScore(highScore);
      break;
    #endif

  }

  arduboy.display();


  // Distinguish any LEDs..

  #ifdef USE_LEDS             
  if (ledCountdown > 0) {
    ledCountdown--;
    if (ledCountdown == 0) {
      arduboy.setRGBled(0, 0, 0);
    }
  }
  #endif

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Let's play !
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void gameLoop() {

  renderScenery(arduboy.getFrameCount(2));
  
  switch (intro) {

    case 80: 
      upperSceneryPosition.enabled = false;
      lowerSceneryPosition.enabled = false;

    case 2 ... 79:
      {
        uint8_t x = 26;
        Sprites::drawOverwrite(x + 3, 30, mission_number, 0);

        uint8_t digits[3] = {};
        extractDigits(digits, mission + 1);

        for (uint8_t i = 0, xOffset = x + 63; i < 3; ++i, xOffset -= 6) {
          Sprites::drawSelfMasked(xOffset, 31, font4x6_Full, digits[i] + 26);
        }

        arduboy.drawHorizontalDottedLine(x, x + 68, 26);
        arduboy.drawHorizontalDottedLine(x, x + 68, 41);

      }

      intro--;
      break;

    case 1:
      launchMission(true, missions[mission % NUMBER_OF_MISSIONS]);
      intro--;
      break;

    case 0:
      break;
      
  }


  if (player.getFuel() > 0 && player.getHealth() > 0 && !player.inRoll()) {
    
    uint8_t pressed = arduboy.pressedButtons();
    uint8_t justPressed = arduboy.justPressedButtons();
    
    player.decFuel(FUEL_DECREMENT);

    
    // Handle player movement ..
    
    if ((pressed & UP_BUTTON) && player.getY() > PLAYER_MOVEMENT_INC_UP)                                     { player.setY(player.getY() - PLAYER_MOVEMENT_INC_UP); }
    if ((pressed & DOWN_BUTTON) && player.getY() < HEIGHT - PLAYER_HEIGHT)                                   { player.setY(player.getY() + PLAYER_MOVEMENT_INC_DOWN); }
    if ((pressed & LEFT_BUTTON) && player.getX() > PLAYER_MOVEMENT_INC_LEFT)                                 { player.setX(player.getX() - PLAYER_MOVEMENT_INC_LEFT); }
    if ((pressed & RIGHT_BUTTON) && player.getX() < WIDTH - PLAYER_WIDTH - SCOREBOARD_OUTER_RECT_WIDTH)      { player.setX(player.getX() + PLAYER_MOVEMENT_INC_RIGHT); }
  
    if (justPressed & B_BUTTON)                                                                              { player.startRoll(); }
                                                                                                                    
    if (justPressed & A_BUTTON) {

      if (player.getBullets() > 0) {
        
        if (!sound.playing()) sound.tones(machine_gun_player);

        uint8_t playerX = player.getX().getInteger();
        uint8_t playerY = player.getY().getInteger();

        launchPlayerBullet(playerX, playerY, Direction::East);
        
        if (player.getPowerUp() && player.getBullets() > 1) {
        
          player.decPowerUp();
          launchPlayerBullet(playerX, playerY, Direction::NorthEast);
          launchPlayerBullet(playerX, playerY, Direction::SouthEast);
          
        }

      }
  
    }

    if (!intro) {

      checkForObstacleCollision();
      checkForEnemiesShot();
  
      if (!player.inRoll()) {
  
        checkForPlayerShot();
        checkForPlayerEnemyCollision();
    
      }
  
  
      // Enemy takes a shot?
  
      --enemyShotCountdown;
      if (enemyShotCountdown == 0) {
  
        checkCanEnemyShoot();
  
      }
  
  
      // Should we launch another obstacle?
  
      --obstacleLaunchCountdown;
      if (obstacleLaunchCountdown == 0 && !obstacle.getEnabled()) {

        launchObstacle();
  
      }

    }

  }


  // Render the screen ..

  moveAndRenderPlayerBullets();
  moveAndRenderEnemyBullets();
  moveAndRenderEnemies(true);
  moveAndRenderObstacle();
  renderScenery_BelowPlanes();
  moveAndRenderEnemies(false);
  player.renderImage(arduboy.getFrameCountHalf(6));
  renderScenery_AbovePlanes();

  renderScoreboard();
  //arduboy.display(true);


  // New wave ?

  if (intro == 0 && player.getHealth() > 0) {
      
    bool newFormation = true;
    for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

      if (enemies[i].getEnabled()) {
        newFormation = false;
        break;
      }

    }
  
    if (newFormation) {

      if (mission_formations_left > 0) {

        launchMission(false, missions[mission % NUMBER_OF_MISSIONS]);
  
      }
      else {

        ++mission;
        intro = 40;
        sound.tones(mission_success);
        gameState = GameState::End_Of_Mission;
        renderEndOfMission();
        
      }
  
    }

  }


  // End of life or mission?

  if ((player.getHealth() <= 0 || player.getFuel() <= 0) && !sound.playing()) {
    sound.tones(mission_failure);    
  }
  
  if (!player.getEnabled()) {
    gameState = GameState::End_Of_Game;
  }

}



/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch player bullet.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchPlayerBullet(const uint8_t x, const uint8_t y, const Direction direction) {

  playerBullets[playerBulletIdx].setX(x + PLAYER_WIDTH);
  playerBullets[playerBulletIdx].setY(y + PLAYER_HEIGHT_HALF);
  playerBullets[playerBulletIdx].setDirection(direction);
  playerBullets[playerBulletIdx].setEnabled(true);
  player.decBullets();
  ++playerBulletIdx;
  if (playerBulletIdx == PLAYER_BULLETS_MAX) playerBulletIdx = 0;
  bulletCountdown = MAX_BULLET_COUNTDOWN;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch a new obstacle.
 *  
 *  If the player is running out of fuel or health, launch one of those first.
 *  
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchObstacle() {

  ObstacleType type = (ObstacleType)random((uint8_t)ObstacleType::First, (uint8_t)ObstacleType::Count);;
  SQ7x8 minValue = 0;
  SQ7x8 maxValue = 0;

  const uint8_t *bitmap = nullptr;

  if (player.getFuel() <= 4) {
    type = ObstacleType::Fuel;
  }
  else if (player.getHealth() <= 4) {
    type = ObstacleType::Health;
  }
  
  switch (type) {

    case ObstacleType::Bullets:
      minValue = (obstacleBulletsValue / (SQ7x8)2);
      maxValue = obstacleBulletsValue;
      bitmap = bullets;
      break;

    case ObstacleType::Fuel:
      minValue = (obstacleFuelValue / (SQ7x8)2);
      maxValue = obstacleFuelValue;
      bitmap = fuel;
      break;

    case ObstacleType::Health:
      minValue = (obstacleHealthValue / (SQ7x8)2);
      maxValue = obstacleHealthValue;
      bitmap = health;
      break;

    case ObstacleType::PowerUp:
      minValue = (obstacleBulletsValue / (SQ7x8)2);
      maxValue = obstacleBulletsValue;
      bitmap = power_up;
      break;
      
    case ObstacleType::Count:
      break;
     
  }

  obstacle.setObstacleType(type);
  obstacle.setEnabled(true);
  obstacle.setX(WIDTH);
  obstacle.setY(random(0, 51));
  obstacle.setSpeed(randomSFixed<7,8>(1, 2));
  obstacle.setValue(randomSFixed<7,8>(minValue, maxValue));
  obstacle.setBitmap(bitmap);

  obstacleLaunchCountdown = random(obstacleLaunchDelayMin, obstacleLaunchDelayMax);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch a new mission.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchMission(bool firstFormation, const uint8_t *missionRef) {

  if (firstFormation) {
    missionIdx = 0;
    mission_formations_left = missionRef[missionIdx++];
    mission_formations = mission_formations_left;
  }

  {
    uint8_t formationPlusScenery = missionRef[missionIdx++];
    formation = formationPlusScenery & SCENERY_MASK_NONE;
    sceneryRestrictions = formationPlusScenery & SCENERY_MASK_RESTRICTIONS;
  }

  launchFormation(formations[formation]);
  --mission_formations_left;

  if (firstFormation) {

    frameRate = frameRate + frameRateInc[level];
    obstacleLaunchDelayMin = obstacleLaunchDelayMin + obstacleLaunchDelayInc[level];
    obstacleLaunchDelayMax = obstacleLaunchDelayMax + obstacleLaunchDelayInc[level];

    if (obstacleBulletsValue > BULLETS_MIN)  obstacleBulletsValue = obstacleBulletsValue - obstacleBulletsValueDec[level];
    if (obstacleHealthValue > HEALTH_MIN)    obstacleHealthValue = obstacleHealthValue - obstacleHealthValueDec[level];
    if (obstacleFuelValue > FUEL_MIN)        obstacleFuelValue = obstacleFuelValue - obstacleFuelValueDec[level];

    arduboy.setFrameRate(frameRate);

  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch a new obstacle.
 *  
 *  If the player is running out of fuel or health, launch one of those first.
 *  
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchFormation(const int8_t *formation) {

  int16_t enemyIdx = 0;


  // Disable all enemies ..

  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {
    enemies[i].setEnabled(false);
  }


  // Retrieve number of enemies to launch ..

  uint8_t number_of_enemies = formation[enemyIdx];
  ++enemyIdx;

  for (uint8_t i = 0; i < number_of_enemies; ++i) {

    switch ((EnemyType)formation[enemyIdx]) {

      case EnemyType::Fighter1:
        enemies[i] = { EnemyType::Fighter1, enemy_fighter1_images };
        break;

      case EnemyType::Fighter2:
        enemies[i] = { EnemyType::Fighter2, enemy_fighter2_images };
        break;

      case EnemyType::Bomber:
        enemies[i] = { EnemyType::Bomber, enemy_medium_images };
        break;

      case EnemyType::Boat:
        enemies[i] = { EnemyType::Boat, enemy_boat };
        break;

      case EnemyType::Count:
        break;
      
    }

    enemies[i].setStartingPos(sequences[formation[enemyIdx + 1]]);
    enemies[i].setOffsetX(formation[enemyIdx + 2]);
    enemies[i].setOffsetY(formation[enemyIdx + 3]);
    enemies[i].setDelayStart(formation[enemyIdx + 4]);
    enemies[i].setInvertX((formation[enemyIdx + 5]) == 1);
    enemies[i].setInvertY((formation[enemyIdx + 6]) == 1);
    enemies[i].setHealth(formation[enemyIdx + 7]);
    enemies[i].setEnabled(true);
    enemies[i].move();

    enemyIdx = enemyIdx + 8;

  }

};


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Is the nominated enemy facing the player?
 * -----------------------------------------------------------------------------------------------------------------------------
 */
bool isAimingAtPlayer(const uint8_t enemyIdx) {
  
  const int8_t playerX = player.getX().getInteger() + PLAYER_WIDTH_HALF;
  const int8_t playerY = player.getY().getInteger() + PLAYER_HEIGHT_HALF;

  const int16_t enemyX = enemies[enemyIdx].getX().getInteger() + (enemies[enemyIdx].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_CENTER_X : enemies[enemyIdx].getWidth() / 2);
  const int16_t enemyY = enemies[enemyIdx].getY().getInteger() + (enemies[enemyIdx].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_CENTER_Y : enemies[enemyIdx].getHeight() / 2);

  const int16_t deltaX = playerX - enemyX;
  const int16_t deltaY = playerY - enemyY;

  SQ15x16 heading = AIM_Q2_MAX;
  if (deltaX != 0) heading = abs((SQ15x16)deltaY / (SQ15x16)deltaX);

  Direction direction = (enemies[enemyIdx].getEnemyType() == EnemyType::Boat ? enemies[enemyIdx].getTurretDirection() : enemies[enemyIdx].getDirection());

  if (deltaY <= 0) {
    
    if (deltaX > 0) {
  
            if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::East)             return true;
       else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::NorthEast)        return true;
  
    }
    
    if (deltaX < 0) {
  
           if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::West)              return true;
      else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::NorthWest)         return true;
  
    }
  
    if (heading >= AIM_Q2_MIN && heading <= AIM_Q2_MAX && direction == Direction::North)                    return true;

  }
  
  if (deltaY > 0) {
    
    if (deltaX > 0) {

           if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::East)              return true;
      else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::SouthEast)         return true;
    
    }
    
    if (deltaX < 0) {
  
           if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::West)              return true;
      else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::SouthWest)         return true;
      
    }
    
    if (heading >= AIM_Q2_MIN && heading <= AIM_Q2_MAX && direction == Direction::South)                    return true;
  
  }
  
  return false;
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Rotate the boat's turret to aim at the player.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
Direction aimAtPlayer(const uint8_t enemyIdx) {

  int8_t playerX = player.getX().getInteger() + PLAYER_WIDTH_HALF;
  int8_t playerY = player.getY().getInteger() + PLAYER_HEIGHT_HALF;

  int16_t enemyX = enemies[enemyIdx].getX().getInteger() + ENEMY_BOAT_TURRENT_X + 5;
  int16_t enemyY = enemies[enemyIdx].getY().getInteger() + (enemies[enemyIdx].getHeight() / 2);

  int16_t deltaX = playerX - enemyX;
  int16_t deltaY = playerY - enemyY;

  SQ15x16 heading = (SQ15x16)ROTATE_Q2_MAX;
  if (deltaX != 0) heading = abs((SQ15x16)deltaY / (SQ15x16)deltaX);

  Direction aimDirection = getAimDirection(deltaX, deltaY, heading);
  Direction curDirection = enemies[enemyIdx].getTurretDirection();

  if (aimDirection != Direction::None && aimDirection != curDirection) {
  
    if (aimDirection > curDirection) ++curDirection;
    if (aimDirection < curDirection) --curDirection;
  
  }
  
  return curDirection;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Get the direction to turn turret towards.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
Direction getAimDirection(const int8_t deltaX, const int8_t deltaY, const SQ15x16 heading) {

  if (deltaY <= 0) {

    if (deltaX > 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::East;
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::NorthEast;

    }
      
    if (deltaX < 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::West;
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::NorthWest;
  
    }

    if (heading > ROTATE_Q2_MIN && heading < ROTATE_Q2_MAX)        return Direction::North;
  
  }

  if (deltaY > 0) {
    
    if (deltaX > 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::East;      
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::SouthEast;
      
    }
  
    if (deltaX < 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::West;
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::SouthWest;
      
    }

    if (heading > ROTATE_Q2_MIN && heading <= ROTATE_Q2_MAX)       return Direction::South;

  }

  return Direction::None;
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player has run over an obstacle.  If so, process the action ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForObstacleCollision() {

  if (obstacle.getEnabled()) {

    if (arduboy.collide(player.getRect(), obstacle.getRect())) {

      switch (obstacle.getObstacleType()) {

        case ObstacleType::Fuel:
          player.setFuel(obstacle.getValue());
          break;

        case ObstacleType::Bullets:
          player.addBullets(obstacle.getValue().getInteger());
          break;

        case ObstacleType::Health:
          player.addHealth(obstacle.getValue());
          break;

        case ObstacleType::PowerUp:
          player.setPowerUp(true);
          player.addBullets(obstacle.getValue().getInteger());
          break;

        case ObstacleType::Count:
          break;

      }

      #ifdef USE_LEDS             
      arduboy.setRGBled(GREEN_LED, 8);
      ledCountdown = LED_COUNTDOWN;
      #endif

      obstacle.setEnabled(false);
      if (!sound.playing()) sound.tones(collect_obstacle);

    }

  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player has shot and enemy.  Reduce the enemy's health and if this kills the enemy, increase the score.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForEnemiesShot() {
  
  for (uint8_t i = 0; i < PLAYER_BULLETS_MAX; ++i) {

    if (playerBullets[i].getEnabled()) {
  
      Point bulletPoint = { playerBullets[i].getX(), playerBullets[i].getY() };
      
      for (uint8_t j = 0; j < NUMBER_OF_ENEMIES; ++j) {
  
        if (enemies[j].getEnabled() && enemies[j].getHealth() > 0) {
  
          if (arduboy.collide(bulletPoint, enemies[j].getRect())) {
  
            playerBullets[i].setEnabled(false);
            enemies[j].decHealth((SQ7x8)1.0);
  
            if (enemies[j].getHealth().getInteger() == 0) player.setScore(player.getScore() + 1);
            if (!sound.playing()) sound.tones(hit_by_bullets);

            break;
  
          }
  
        }
  
      }
  
    }
  
  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player has been shot by an enemy bullet.  If so, reduce the player's health ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForPlayerShot() {
  
  Rect playerRect = player.getRect();
  
  for (uint8_t i = 0; i < enemyBulletsMax[level]; ++i) {
  
    if (enemyBullets[i].getEnabled()) {
  
      Point bulletPoint = { enemyBullets[i].getX(), enemyBullets[i].getY() };
    
      if (arduboy.collide(bulletPoint, playerRect)) {
  
        enemyBullets[i].setEnabled(false);
        player.decHealth(playerHitBulletDec[static_cast<uint8_t>(enemyBullets[i].getEnemyType())]);

        #ifdef USE_LEDS             
        arduboy.setRGBled(RED_LED, 8);
        ledCountdown = LED_COUNTDOWN;
        #endif

        break;
  
      }
  
    }
  
  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player and an enemy have collided.  If so, reduce the player's health ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForPlayerEnemyCollision() {

  Rect playerRect = player.getRect(); 

  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

    if (enemies[i].getEnabled() && enemies[i].getDelayStart() == 0) {

      if (enemies[i].getEnemyType() != EnemyType::Boat && arduboy.collide(playerRect, enemies[i].getRect())) {

        player.decHealth(PLAYER_HIT_PLANE_DECREMENT);
        if (!sound.playing()) sound.tones(hit_by_plane);

        #ifdef USE_LEDS             
        arduboy.setRGBled(RED_LED, 8);
        ledCountdown = LED_COUNTDOWN;
        #endif

        break;

      }

    }

  }
      
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if an active enemy is point towards the player, if so fire a bullet ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkCanEnemyShoot() {
  
  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

    Rect enemyRect = enemies[i].getRect();
    
    if (enemies[i].getEnabled() && enemies[i].getDelayStart() == 0 && enemies[i].getNumberOfBulletsFired() > 0
                                && enemyRect.x + enemyRect.width > 0 && enemyRect.x < WIDTH 
                                && enemyRect.y + enemyRect.height >= 0 && enemyRect.y < HEIGHT ) {
  
      if (isAimingAtPlayer(i)) {

        enemies[i].decNumberOfBulletsFired();
  
        enemyBullets[enemyBulletIdx].setX(enemies[i].getX().getInteger() + (enemies[i].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_X : (enemies[i].getWidth() / 2)));
        enemyBullets[enemyBulletIdx].setY(enemies[i].getY().getInteger() + (enemies[i].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_CENTER_Y : (enemies[i].getHeight() / 2)));
        enemyBullets[enemyBulletIdx].setDirection(enemies[i].getEnemyType() == EnemyType::Boat ? enemies[i].getTurretDirection() : enemies[i].getDirection());
        enemyBullets[enemyBulletIdx].setEnabled(true);
        enemyBullets[enemyBulletIdx].setEnemyType(enemies[i].getEnemyType());

        ++enemyBulletIdx;
        if (enemyBulletIdx == enemyBulletsMax[level]) enemyBulletIdx = 0;

        if (!sound.playing()) sound.tones(machine_gun_enemy);

        break;
  
      }
  
    }
  
  }


  // Reset the countdown to induce a delay before the next shot is fired ..
  
  enemyShotCountdown = random(10, 20);

}



