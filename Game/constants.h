#ifndef CONSTANTS_H
#define CONSTANTS_H
// Before playing, make sure the resolution is correct for your screen
// so the game will not be stretched
const int WINDOW_WIDTH = 2560;
const int WINDOW_HEIGHT = 1440;

// Settings you can change to make the game easier or harder
const float PLAYER_SPEED = 300.0f;
const float COLLISION_SPEED_FACTOR = 0.5f;
const float ENEMY_SPEED = 100.0f;
const int PL_ATTACK_DAMAGE = 80;
const int BOT_ATTACK_DAMAGE = 20;
const float ATTACK_COOLDOWN = 1.5f;
const float ENEMY_SPAWN_TIME = 3.0f;
const float ENEMY_ATTACK_COOLDOWN = 2.0f;
#endif // CONSTANTS_H
