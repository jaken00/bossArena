#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct Projectile{
    SDL_Rect projectileRect;
    double posX;
    double posY;
    double velocityX;
    double velocityY;
    bool active;
} Projectile;

typedef struct {
    int health;
    int mana;
    double movespeed;
    SDL_Rect playerRect;
    size_t ability_count;
} Player;

typedef struct {
    int health;
    int phase;
    double movespeed;    
    SDL_Rect enemyRect;
    size_t ability_count;
    
    Projectile* projectiles;
    size_t projectile_count;
    size_t projectile_capacity;
} Enemy;

Player createPlayer();
Enemy createEnemy();

void freePlayer(Player *player);

void monitorEnemyPhase(Enemy *enemy);
void fireProjectile(Enemy *enemy, int targetX, int targetY);
void updateProjectile(Enemy *enemy, double deltaTime);
void drawProjectiles(SDL_Renderer* renderer, Enemy *enemy);

void update_player_movement(Player *player, const Uint8 *keystate, double deltaTime);





#endif //GAME_H