#ifndef ENEMY_H
#define ENEMY_H


#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

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

Enemy createEnemy();

void monitorEnemyPhase(Enemy *enemy);
void fireProjectile(Enemy *enemy, int targetX, int targetY);
void updateProjectile(Enemy *enemy, double deltaTime);
void move_enemy(Enemy *enemy, SDL_Rect *playerRect, double deltaTime);



#endif //ENEMY_H