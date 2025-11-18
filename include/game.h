#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define ATTACK_INTERVAL 500

typedef struct {
    int hp;
    int maxHp;
} Health;

typedef struct Projectile{
    SDL_Rect projectileRect;
    double posX;
    double posY;
    double velocityX;
    double velocityY;
    bool active;
    int damageValue;
} Projectile;

typedef struct {
    int mana;
    double movespeed;
    Uint32 lastHitTime;
    SDL_Rect playerRect;
    size_t ability_count;
    Health health;

    Projectile* projectiles;
    size_t projectile_count;
    size_t projectile_capacity;
} Player;

typedef struct {
    int phase;
    double movespeed;    
    SDL_Rect enemyRect;
    size_t ability_count;
    Health health;

    Projectile* projectiles;
    size_t projectile_count;
    size_t projectile_capacity;
} Enemy;

Player createPlayer();
Enemy createEnemy();

void freePlayer(Player *player);

void monitorEnemyPhase(Enemy *enemy);
void fireProjectile(Enemy *enemy, int targetX, int targetY);
void updateProjectile(Enemy *enemy, double deltaTime); //move these into one (tkae both player and enemy proj)
void drawProjectiles(SDL_Renderer* renderer, Enemy *enemy, Player *player);

void update_player_movement(Player *player, const Uint8 *keystate, double deltaTime);
void enemy_attack_timer(double deltaTime, double *attackTimer, Enemy *enemy, Player player);
void move_enemy(Enemy *enemy, SDL_Rect *playerRect, double deltaTime);
bool projectileHit(SDL_Rect *target, SDL_Rect *source);
void enemyProjectileCollisionCall(Player *player, Enemy *enemy);

void playerFire(Player *player, int mouseX, int mouseY);
void playerProjUpdate(Player *player, double deltaTime);



void takeDamage(Health *hp, int damage);



#endif //GAME_H