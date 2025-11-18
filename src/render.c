#include "render.h"

void renderEntities(SDL_Renderer *renderer, SDL_Rect *player, SDL_Rect *enemy){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, player);
    SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
    SDL_RenderFillRect(renderer, enemy);
}