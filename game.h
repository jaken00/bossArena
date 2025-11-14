#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct Ability{
    int cooldown;
    int damage;
    char name[50];
    bool draw;
    SDL_Rect abilityRect;
    SDL_Scancode hotkey;
} Ability;

typedef struct Projectile{
    SDL_Rect projectileRect;
    double velocityX;
    double velocityY;
    bool active;
} Projectile;

typedef struct {
    int health;
    int mana;
    double movespeed;
    SDL_Rect playerRect;
    Ability** abilities;
    size_t ability_count;
} Player;

typedef struct {
    int health;
    int phase;
    double movespeed;    
    SDL_Rect enemyRect;
    Ability** abilities;
    size_t ability_count;
    
    Projectile* projectiles;
    size_t projectile_count;
    size_t projectile_capacity;
} Enemy;

Player createPlayer();
Enemy createEnemy();

void freePlayer(Player *player);

Ability* createAbility(char name[6], int damage, int cooldown ,SDL_Scancode hotkey, SDL_Rect abilityRect);

void monitorEnemyPhase(Enemy *enemy);
void fireProjectile(Enemy *enemy, int targetX, int targetY);
void updateProjectile(Enemy *enemy, double deltaTime);
void drawProjectiles(SDL_Renderer* renderer, Enemy *enemy);






#endif //GAME_H