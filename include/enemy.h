#ifndef ENEMY_H
#define ENEMY_H


#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"


Enemy createEnemy();

void monitorEnemyPhase(Enemy *enemy);
void fireProjectile(Enemy *enemy, int targetX, int targetY);
void updateProjectile(Enemy *enemy, double deltaTime);
void move_enemy(Enemy *enemy, SDL_Rect *playerRect, double deltaTime);



#endif //ENEMY_H