#ifndef ENEMY_H
#define ENEMY_H


#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"


#define P2_ATTACK_COOLDOWN 5000;

Enemy createEnemy();

void monitorEnemyPhase(Enemy *enemy);
void fireProjectile(Enemy *enemy, int targetX, int targetY);
void updateProjectile(Enemy *enemy, double deltaTime);
void move_enemy(Enemy *enemy, SDL_Rect *playerRect, double deltaTime);
bool enemyP2Attack(Enemy *enemy);
void enemyP3Attack(Enemy *enemy);
void enemyAttackOrchestrator(Enemy *enemy);
int randomRadius();


#endif //ENEMY_H