#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

typedef struct {
    int mana;
    double movespeed;
    Uint32 lastHitTime;
    Uint32 lastShootTime;

    SDL_Rect playerRect;
    size_t ability_count;
    Health health;

    Projectile* projectiles;
    size_t projectile_count;
    size_t projectile_capacity;
} Player;

Player createPlayer();
void freePlayer(Player *player);

void update_player_movement(Player *player, const Uint8 *keystate, double deltaTime);
void playerFire(Player *player, int mouseX, int mouseY);
void playerProjUpdate(Player *player, double deltaTime);

#endif //PLAYER_H