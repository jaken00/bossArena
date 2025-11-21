#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "types.h"

void initRenderTextures(SDL_Renderer *renderer);
void cleanupRenderTextures();
void renderBackground(SDL_Renderer *renderer);
void renderEntities(SDL_Renderer *renderer, SDL_Rect *player, SDL_Rect *enemy);
void renderProjectiles(SDL_Renderer* renderer, Enemy *enemy, Player *player);
void draw_filled_circle(SDL_Renderer *renderer, int cx, int cy, int radius);
void draw_circle_outline(SDL_Renderer *renderer, int cx, int cy, int radius);