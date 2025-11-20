// include/types.h
#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define ATTACK_INTERVAL 500

typedef struct {
    int hp;
    int maxHp;
} Health;

typedef struct {
    SDL_Rect projectileRect;
    double posX, posY;
    double velocityX, velocityY;
    int damageValue;
    bool active;
} Projectile;

struct Player;
struct Enemy;

typedef struct Player {
    Health health;

    Uint32 lastHitTime;
    Uint32 lastShootTime;

    int mana;
    double movespeed;

    SDL_Rect playerRect;
    int ability_count;

    Projectile *projectiles;
    int projectile_count;
    int projectile_capacity;
} Player;

typedef struct Enemy {
    Health health;

    Uint32 lastHitTime;

    int phase;
    double movespeed;

    SDL_Rect enemyRect;
    int ability_count;

    Projectile *projectiles;
    int projectile_count;
    int projectile_capacity;
} Enemy;

#endif // TYPES_H
