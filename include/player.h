#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

Player createPlayer();
void freePlayer(Player *player);

void update_player_movement(Player *player, const Uint8 *keystate, SDL_Event *event, double deltaTime);
void playerFire(Player *player, int mouseX, int mouseY);
void playerProjUpdate(Player *player, double deltaTime);

#endif //PLAYER_H