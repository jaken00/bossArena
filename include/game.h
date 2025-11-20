#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"
#include "enemy.h"




void drawProjectiles(SDL_Renderer* renderer, Enemy *enemy, Player *player);

void enemy_attack_timer(double deltaTime, double *attackTimer, Enemy *enemy, Player player);
void move_enemy(Enemy *enemy, SDL_Rect *playerRect, double deltaTime);
bool projectileHit(SDL_Rect *target, SDL_Rect *source);
void enemyProjectileCollisionCall(Player *player, Enemy *enemy);
void playerProjectileCollisionCall(Player *player, Enemy *enemy);

void takeDamage(Health *hp, int damage);



#endif //GAME_H